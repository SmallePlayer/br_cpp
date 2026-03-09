#include <opencv2/opencv.hpp>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include "jpeg_work.h"


std::vector<uchar>  jpeg_compress(const cv::Mat& img, int quality){
    std::vector<uchar> jpeg_buffer;
    std::vector<int> params = {cv::IMWRITE_JPEG_QUALITY, quality};
    cv::imencode(".jpg", img, jpeg_buffer, params);
    return jpeg_buffer;
}

cv::Mat jpeg_decompress(const std::vector<uchar>& buffer) {
    return cv::imdecode(buffer, cv::IMREAD_COLOR);
}