#include <video/filter_processor/vflip_filter_processor.h>

uint8_t* VflipFilterProcessor::ProcessFrame(AVFilterContext* buffersink_ctx, int width, int height)
{
    (void)width;
    (void)height;
    
    AVFrame* filt_frame = av_frame_alloc();
    if (av_buffersink_get_frame(buffersink_ctx, filt_frame) < 0) 
    {
        std::cerr << "Cannot get frame from buffersink" << std::endl;
        av_frame_free(&filt_frame);
        return nullptr;
    }

    int num_bytes = av_image_get_buffer_size((AVPixelFormat)filt_frame->format, filt_frame->width, filt_frame->height, 1);
    uint8_t* output_data = (uint8_t*)av_malloc(num_bytes);
    if (!output_data) 
    {
        std::cerr << "Cannot alloc buffer" << std::endl;
        av_frame_free(&filt_frame);
        return nullptr;
    }

    if (av_image_copy_to_buffer(output_data, num_bytes, (const uint8_t* const*)filt_frame->data, (const int*)filt_frame->linesize, (AVPixelFormat)filt_frame->format, filt_frame->width, filt_frame->height, 1) < 0) 
    {
        std::cerr << "Cannot copy frame data" << std::endl;
        av_free(output_data);
        av_frame_free(&filt_frame);
        return nullptr;
    }

    av_frame_free(&filt_frame);
    return output_data;
}