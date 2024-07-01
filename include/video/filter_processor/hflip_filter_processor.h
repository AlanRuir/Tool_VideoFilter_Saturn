#ifndef __VIDEO_FILTER_PROCESSOR_HFLIP_FILTER_PROCESSOR_H__
#define __VIDEO_FILTER_PROCESSOR_HFLIP_FILTER_PROCESSOR_H__

#include <iostream>

#include <video/filter_processor/filter_processor_base.h>

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

class HflipFilterProcessor : public FilterProcessorBase
{
public:
    uint8_t* ProcessFrame(AVFilterContext* buffersink_ctx, int width, int height) override;
};

#endif  // __VIDEO_FILTER_PROCESSOR_HFLIP_FILTER_PROCESSOR_H__