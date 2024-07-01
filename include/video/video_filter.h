#ifndef __VIDEO_OPTIONAL_H__
#define __VIDEO_OPTIONAL_H__

#include <iostream>
#include <cstdint>
#include <memory>

extern "C" 
{
    #include <libavutil/avutil.h>
    #include <libavfilter/avfilter.h>
    #include <libavfilter/buffersink.h>
    #include <libavfilter/buffersrc.h>
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavutil/imgutils.h>
    #include <libswscale/swscale.h>
}

#include <common/common_type.h>
#include <video/filter_processor/filter_processor_base.h>

class VideoFilter
{
public:
    VideoFilter(int width, int height, enum FilterType filter_type, const std::string& filter_args = "");
    ~VideoFilter();

public:
    uint8_t* Filter(const uint8_t* data);

private:
    int                                  width_;
    int                                  height_;
    bool                                 none_filter_;
    AVFilterGraph*                       filter_graph_;     // 滤波器节点
    AVFilterContext*                     buffersrc_ctx_;    // 数据源滤波器上下文
    AVFilterContext*                     buffersink_ctx_;   // 数据目的滤波器上下文
    AVFilterContext*                     filter_ctx_;       // 滤波器上下文
    AVFrame*                             filter_frame_;     // 滤波器帧
    std::shared_ptr<FilterProcessorBase> filter_processor_; // 滤波器处理器

private:
    void FreeFilterGraph(); 
};

#endif  // __VIDEO_OPTIONAL_H__