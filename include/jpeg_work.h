#pragma once
#ifndef JPEG_WORK
#define JPEG_WORK

#include <opencv2/opencv.hpp>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

std::vector<uchar>  jpeg_compress(const cv::Mat& img, int quality = 80);
cv::Mat jpeg_decompress(const std::vector<uchar>& buffer);

#endif