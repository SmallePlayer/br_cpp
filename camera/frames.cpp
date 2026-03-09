#include <opencv2/opencv.hpp>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include "jpeg_work.h"
#include "capture.h"
#include "frames.h"

cv::Mat frame;
std::mutex frame_global;

void capture_camera(cv::VideoCapture& cap){
    cv::Mat local;
    while(true){
        cap >> local;
        if (local.empty()) continue;

        std::lock_guard<std::mutex> lock(frame_global);
        frame = local;
    }
}

cv::Mat get_frame(){
    std::lock_guard<std::mutex> lock(frame_global);
    return frame.clone();
}