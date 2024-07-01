#ifndef __VIDEO_FILTER_PROCESSOR_BASE_H__
#define __VIDEO_FILTER_PROCESSOR_BASE_H__

#include <iostream>

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

class FilterProcessorBase
{
public:
    virtual ~FilterProcessorBase() = default;
    virtual uint8_t* ProcessFrame(AVFilterContext* buffersink_ctx, int width, int height) = 0;
};

#endif  // __VIDEO_FILTER_PROCESSOR_BASE_H__