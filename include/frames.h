#ifndef FRAMES_H
#define FRAMES_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

void capture_camera(cv::VideoCapture& cap);
cv::Mat get_frame();

#endif