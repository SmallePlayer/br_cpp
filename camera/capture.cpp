#include <opencv2/opencv.hpp>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include "jpeg_work.h"



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



void frame_score(double start_time){
    double current_time = cv::getTickCount();
    double difference = (current_time - start_time) / cv::getTickFrequency();
    std::cout << "Difference: " << difference << std::endl;
    start_time = current_time;
}


int main() {
    

    cv::VideoCapture cap(0);
    if(!cap.isOpened()){
        std::cerr << "Не удалось открыть камеру!" << std::endl;
        return -1;
    }
    
    std::thread t(capture_camera, std::ref(cap));
    t.detach();

    
    while(true){
        double start_time = cv::getTickCount();
        cv::Mat local = get_frame();

        if (frame.empty()) {
            std::cerr << "Пустой кадр!" << std::endl;
            continue;
        }
        
        std::vector<uchar> jpeg_buffer = jpeg_compress(local);
        //cv::Mat decode_frame = jpeg_decompress(jpeg_buffer);
        
        frame_score(start_time);
    }

    cap.release();
    return 0;
}