#include <memory>
#include <fstream>
#include <string>
#include <cstdint>
#include <iostream>
#include <chrono>
#include <thread>

#include <video/video_filter.h>

// FilterType filter_type = FilterType::NONE;
// std::string filter_args = "";
// FilterType filter_type = FilterType::CROP;
// std::string filter_args = "w=640:h=360:x=500:y=500";
// FilterType filter_type = FilterType::USPP;
// std::string filter_args = "";
// FilterType filter_type = FilterType::EDGEDETECT;
// std::string filter_args = "";
// FilterType filter_type = FilterType::DRAWTEXT;
// std::string filter_args = "text='timestamp:%{localtime}':x=10:y=10:fontsize=32: box=1: boxcolor=black@0.6";
// FilterType filter_type = FilterType::SCALE;
// std::string filter_args = "640:360";
// FilterType filter_type = FilterType::HFLIP;
// std::string filter_args = "";
FilterType filter_type = FilterType::VFLIP;
std::string filter_args = "";


uint64_t GetFileSize(const std::string& filePath)       // 获取文件大小
{
    std::ifstream file(filePath, std::ifstream::ate | std::ifstream::binary);
    if (!file) 
    {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return 0;
    }

    return static_cast<uint64_t>(file.tellg());
}

void DisplayProgress(int current, int total)        // 显示进度
{
    const int barWidth = 70;
    float progress = static_cast<float>(current) / total;
    int barLength = static_cast<int>(progress * barWidth);

    std::string progressBar = "[";
    for (int i = 0; i < barLength; ++i) 
    {
        progressBar += "=";
    }
    for (int i = barLength; i < barWidth; ++i) 
    {
        progressBar += " ";
    }
    progressBar += "]";

    std::string progressLog = "Progress: " + std::to_string(current) + "/" + std::to_string(total);

    std::cout << "\r" << progressBar << " " << progressLog << std::flush;
}

int main(int argc, char* argv[])
{
    uint32_t frame_size = 1920 * 1080 * 3 / 2;
    std::shared_ptr<uint8_t> frame_buffer(new uint8_t[frame_size](), std::default_delete<uint8_t[]>());
    std::shared_ptr<VideoFilter> video_filter = std::make_shared<VideoFilter>(1920, 1080, filter_type, filter_args);     // 创建编码器
    FILE* file = nullptr;
    FILE* file2 = fopen("./output.yuv", "ab+");    // 打开文件
    
    std::cout << "开始预处理" << std::endl;
    for (size_t i = 1; i < 513; ++i)
    {
        std::string file_path = std::string("../output/") + std::to_string(i) + ".yuv";     // 换成自己的文件路径
        file = fopen(file_path.c_str(), "rb");                  // 打开文件
        if (!file)
        {
            std::cerr << "Failed to open file: " << file_path << std::endl;
            return -1;
        }
        fread(frame_buffer.get(), 1, frame_size, file);         // 从文件中读取图像
        uint8_t* data = video_filter->Filter(frame_buffer.get());
        // std::this_thread::sleep_for(std::chrono::milliseconds(40));
        fwrite(data, 1, 1920 * 1080 * 3 / 2, file2);
        fclose(file);                                           // 关闭文件
        DisplayProgress(i, 513);                                // 显示进度
    }
    std::cout << std::endl;
    std::cout << "预处理完成" << std::endl;

    fclose(file2);

    return 0;
}