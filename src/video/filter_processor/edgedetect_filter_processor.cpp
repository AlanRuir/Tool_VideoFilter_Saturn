#include <video/filter_processor/edgedetect_filter_processor.h>

uint8_t* EdgeDetectFilterProcessor::ProcessFrame(AVFilterContext* buffersink_ctx, int width, int height)
{
    AVFrame* filt_frame = av_frame_alloc();

    if (av_buffersink_get_frame(buffersink_ctx, filt_frame) < 0) 
    {
        std::cerr << "Cannot get frame from buffersink" << std::endl;
        av_frame_free(&filt_frame);
        return nullptr;
    }

    AVFrame* yuv_frame = av_frame_alloc();
    yuv_frame->format = AV_PIX_FMT_YUV420P;
    yuv_frame->width = width;
    yuv_frame->height = height;
    if (av_image_alloc(yuv_frame->data, yuv_frame->linesize, width, height, AV_PIX_FMT_YUV420P, 1) < 0) 
    {
        std::cerr << "Could not allocate YUV420P frame" << std::endl;
        av_frame_free(&filt_frame);
        av_frame_free(&yuv_frame);
        return nullptr;
    }

    memcpy(yuv_frame->data[0], filt_frame->data[0], width * height);
    memset(yuv_frame->data[1], 128, width * height / 4);
    memset(yuv_frame->data[2], 128, width * height / 4);

    int num_bytes = av_image_get_buffer_size(AV_PIX_FMT_YUV420P, width, height, 1);
    uint8_t* output_data = (uint8_t*)av_malloc(num_bytes);
    if (!output_data) 
    {
        std::cerr << "Cannot alloc buffer" << std::endl;
        av_frame_free(&filt_frame);
        av_frame_free(&yuv_frame);
        return nullptr;
    }

    if (av_image_copy_to_buffer(output_data, num_bytes, (const uint8_t* const*)yuv_frame->data, (const int*)yuv_frame->linesize, AV_PIX_FMT_YUV420P, width, height, 1) < 0) 
    {
        std::cerr << "Cannot copy frame data" << std::endl;
        av_free(output_data);
        av_frame_free(&filt_frame);
        av_frame_free(&yuv_frame);
        return nullptr;
    }

    av_frame_free(&filt_frame);
    av_frame_free(&yuv_frame);
    return output_data;
}