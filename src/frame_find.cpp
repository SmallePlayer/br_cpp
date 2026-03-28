#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <set>
#include <chrono>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>

#include "brezenhem.h"
#include "a_star.h"
#include "net.h"
#include "frames.h"
#include "jpeg_work.h"


void create_aruco(){
    int id = 23;
    int markerSize = 400;
    int padding = 50; // белое поле вокруг маркера

    auto dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);

    cv::Mat marker;
    cv::aruco::generateImageMarker(dictionary, id, markerSize, marker, 1);

    // добавляем белое поле со всех сторон
    cv::Mat markerWithBorder;
    cv::copyMakeBorder(marker, markerWithBorder,
                       padding, padding, padding, padding,
                       cv::BORDER_CONSTANT, cv::Scalar(255));

    cv::imwrite("aruco_6x6_250_" + std::to_string(id) + ".png", markerWithBorder);
}

void detect_aruco(){
    cv::Mat frame_arcuo = cv::imread("aruco_6x6_250_23.png", cv::IMREAD_COLOR);
    if (frame_arcuo.empty()) {
        std::cerr << "Cannot read image\n";
        return;
    }

    cv::aruco::Dictionary dict = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
    cv::aruco::DetectorParameters params;
    cv::aruco::ArucoDetector detector(dict, params);  // принимает по значению, не Ptr

    std::vector<int> ids;
    std::vector<std::vector<cv::Point2f>> corners;
    detector.detectMarkers(frame_arcuo, corners, ids);

    if (ids.empty()) std::cout << "No markers found\n";
    for (int id : ids) std::cout << "Found ID: " << id << std::endl;
}


int main(){
    cv::Mat img(1080, 1920, CV_8UC3, cv::Scalar(169, 220, 201)); // создаем черное изображение

    create_aruco();
    detect_aruco();

    cv::imshow("Image", img);
    cv::waitKey(0);


    return 0;
}