#ifndef CAPTURE_H
#define CAPTURE_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include "jpeg_work.h"

void capture_camera(cv::VideoCapture& cap);
cv::Mat get_frame();
void frame_score(double start_time);

#endif