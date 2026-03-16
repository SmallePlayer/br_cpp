#include <opencv2/opencv.hpp>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include "jpeg_work.h"
#include "frames.h"




void frame_score(double start_time){
    double current_time = cv::getTickCount();
    double difference = (current_time - start_time) / cv::getTickFrequency();
    std::cout << "Difference: " << difference << std::endl;
    start_time = current_time;
}


int main() {
    

    cv::VideoCapture cap(0);
    cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M','J','P','G'));
    if(!cap.isOpened()){
        std::cerr << "Не удалось открыть камеру!" << std::endl;
        return -1;
    }
    
    std::thread t(capture_camera, std::ref(cap));
    t.detach();

    
    while(true){
        double start_time = cv::getTickCount();
        cv::Mat local = get_frame();
        
        std::vector<uchar> buffer = jpeg_compress(local);

        if (local.empty()) {
            std::cerr << "Пустой кадр!" << std::endl;
            continue;
        }
        
        //std::vector<uchar> jpeg_buffer = jpeg_compress(local);
        //cv::Mat decode_frame = jpeg_decompress(jpeg_buffer);
        
        frame_score(start_time);
    }

    cap.release();
    return 0;
}