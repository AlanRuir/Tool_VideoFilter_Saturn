#include <iostream>
#include <cstring>

#include <video/video_filter.h>
#include <video/filter_processor/edgedetect_filter_processor.h>
#include <video/filter_processor/crop_filter_processor.h>
#include <video/filter_processor/uspp_filter_processor.h>
#include <video/filter_processor/drawtext_filter_processor.h>
#include <video/filter_processor/scale_filter_processor.h>
#include <video/filter_processor/hflip_filter_processor.h>
#include <video/filter_processor/vflip_filter_processor.h>

VideoFilter::VideoFilter(int width, int height, enum FilterType filter_type, const std::string& filter_args)
    : width_(width)
    , height_(height)
    , none_filter_(false)
    , filter_graph_(nullptr)
    , buffersrc_ctx_(nullptr)
    , buffersink_ctx_(nullptr)
    , filter_ctx_(nullptr)
    , filter_frame_(nullptr)
    , filter_processor_(nullptr)
{
#if LIBAVFILTER_VERSION_MAJOR < 7
    avfilter_register_all();
#endif

    std::string filter_name = "";

    switch (filter_type)
    {
        case FilterType::EDGEDETECT:
            filter_processor_ = std::make_shared<EdgeDetectFilterProcessor>();
            filter_name = "edgedetect";
            break;
        case FilterType::CROP:
            filter_processor_ = std::make_shared<CropFilterProcessor>();
            filter_name = "crop";
            break;
        case FilterType::USPP:
            filter_processor_ = std::make_shared<UsppFilterProcessor>();
            filter_name = "uspp";
            break;
        case FilterType::DRAWTEXT:
            filter_processor_ = std::make_shared<DrawTextFilterProcessor>();
            filter_name = "drawtext";
            break;
        case FilterType::SCALE:
            filter_processor_ = std::make_shared<ScaleFilterProcessor>();
            filter_name = "scale";
            break;
        case FilterType::HFLIP:
            filter_processor_ = std::make_shared<HflipFilterProcessor>();
            filter_name = "hflip";
            break;
        case FilterType::VFLIP:
            filter_processor_ = std::make_shared<VflipFilterProcessor>();
            filter_name = "vflip";
            break;
        case FilterType::NONE:
            none_filter_ = true;
            filter_name = "null";
            break;
        default:
            std::string error = "Unsupported filter: " + std::to_string((int)filter_type);
            throw std::runtime_error(error);
            break;
    }

    if (!filter_processor_ && !none_filter_)
    {
        std::string error = "Unsupported filter: " + std::to_string((int)filter_type);
        throw std::runtime_error(error);
    }

    char args[512] = {0};
    int result = 0;

    const AVFilter* buffersrc  = avfilter_get_by_name("buffer");            // 数据源滤波器
    const AVFilter* buffersink = avfilter_get_by_name("buffersink");        // 数据目的滤波器
    const AVFilter* filter = avfilter_get_by_name(filter_name.c_str());     // 滤波器

    AVFilterInOut* inputs  = avfilter_inout_alloc();        // 滤波器输入
    AVFilterInOut* outputs = avfilter_inout_alloc();        // 滤波器输出

    enum AVPixelFormat pix_fmts[] = {AV_PIX_FMT_YUV420P, AV_PIX_FMT_NONE};      // 滤波器支持的像素格式, AV_PIX_FMT_NONE 表示支持所有像素格式

    filter_graph_ = avfilter_graph_alloc();
    if (!inputs || !outputs || !filter_graph_)
    {
        result = AVERROR(ENOMEM);
        avfilter_inout_free(&inputs);
        avfilter_inout_free(&outputs);

        std::string error = "Cannot alloc filter graph: " + std::to_string(result);
        throw std::runtime_error(error);
    }

    snprintf(args, sizeof(args), "video_size=%dx%d:pix_fmt=%d:time_base=1/25", width, height, AV_PIX_FMT_YUV420P);
    result = avfilter_graph_create_filter(&buffersrc_ctx_, buffersrc, "in", args, nullptr, filter_graph_);      // 创建数据源滤波器

    if (result < 0)
    {
        std::cerr << "Cannot create buffer source: " << result << std::endl;
        avfilter_inout_free(&inputs);
        avfilter_inout_free(&outputs);
        
        std::string error = "Cannot create buffer source: " + std::to_string(result);
        throw std::runtime_error(error);
    }

    result = avfilter_graph_create_filter(&buffersink_ctx_, buffersink, "out", nullptr, nullptr, filter_graph_);    // 创建数据目的滤波器
    if (result < 0)
    {
        std::cerr << "Cannot create buffer sink: " << result << std::endl;
        avfilter_inout_free(&inputs);
        avfilter_inout_free(&outputs);
        std::string error = "Cannot create buffer sink: " + std::to_string(result);
        throw std::runtime_error(error);
    }

    result = avfilter_graph_create_filter(&filter_ctx_, filter, filter_name.c_str(), filter_args.c_str(), nullptr, filter_graph_);     // 创建滤波器
    if (result < 0)
    {
        std::cerr << "Cannot create buffer sink: " << result << std::endl;
        avfilter_inout_free(&inputs);
        avfilter_inout_free(&outputs);

        std::string error = "Cannot create buffer sink: " + std::to_string(result);
        throw std::runtime_error(error);
    }

    result = avfilter_link(buffersrc_ctx_, 0, filter_ctx_, 0);        // 链接滤波器, 0 表示输入通道, 0 表示输出通道
    if (result >= 0)
    {
        result = avfilter_link(filter_ctx_, 0, buffersink_ctx_, 0);    // 链接滤波器, 0 表示输入通道, 0 表示输出通道
    }
    if (result < 0)
    {
        std::cerr << "Cannot link buffer source: " << result << std::endl;
        avfilter_inout_free(&inputs);
        avfilter_inout_free(&outputs);

        std::string error = "Cannot link buffer source: " + std::to_string(result);
        throw std::runtime_error(error);
    }

    result = avfilter_graph_config(filter_graph_, nullptr);        // 配置滤波器, 生成滤波器节点
    if (result < 0)
    {
        std::cerr << "Cannot configure filter: " << result << std::endl;
        avfilter_inout_free(&inputs);
        avfilter_inout_free(&outputs);

        std::string error = "Cannot configure filter: " + std::to_string(result);
        throw std::runtime_error(error);
    }

    filter_frame_ = av_frame_alloc();
    if (!filter_frame_)
    {
        result = AVERROR(ENOMEM);
        avfilter_inout_free(&inputs);
        avfilter_inout_free(&outputs);

        std::string error = "Cannot alloc filter frame: " + std::to_string(result);
        throw std::runtime_error(error);
    }
}

VideoFilter::~VideoFilter()
{
    FreeFilterGraph();
}

void VideoFilter::FreeFilterGraph()
{
    if (filter_frame_)
    {
        av_frame_free(&filter_frame_);
        filter_frame_ = nullptr;
    }

    if (filter_graph_)
    {
        avfilter_graph_free(&filter_graph_);
        filter_graph_ = nullptr;
    }

    buffersink_ctx_ = nullptr;
    buffersrc_ctx_  = nullptr;
    filter_ctx_     = nullptr;
}

uint8_t* VideoFilter::Filter(const uint8_t* data)
{
    if (none_filter_)
    {
        return const_cast<uint8_t*>(data);        // 如果没有滤波器, 直接返回
    }

    AVFrame* frame = av_frame_alloc();
    if (!frame)
    {
        std::cerr << "Cannot alloc frame" << std::endl;
        return nullptr;
    }

    frame->format = AV_PIX_FMT_YUV420P;
    frame->width  = width_;
    frame->height = height_;

    int result = av_image_fill_arrays(frame->data, frame->linesize, data, AV_PIX_FMT_YUV420P, width_, height_, 1);        // 填充数据
    if (result < 0)
    {
        std::cerr << "Cannot fill frame data: " << result << std::endl;
        av_frame_free(&frame);
        return nullptr;
    }

    result = av_buffersrc_add_frame(buffersrc_ctx_, frame);        // 将数据添加到滤波器上
    if (result < 0)
    {
        std::cerr << "Cannot add frame: " << result << std::endl;
        av_frame_free(&frame);
        return nullptr;
    }

    uint8_t* output_data = filter_processor_->ProcessFrame(buffersink_ctx_, width_, height_);
    av_frame_free(&frame);
    return output_data;
}