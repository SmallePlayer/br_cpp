#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    cv::VideoCapture cap(0);

    if(!cap.isOpened()){
        std::cerr << "Не удалось открыть камеру!" << std::endl;
        return -1;
    }

    cv::Mat framel

    while(true){
        cap >> frame;

        if (frame.empty()) {
            std::cerr << "Пустой кадр!" << std::endl;
            break;
        }

        cv::imshow("Камера", frame);

        if (cv::waitKey(1) == 'q') {
            break;
        }
    }

    cap.release();

    // Закрываем все окна opencv
    cv::destroyAllWindows();

    return 0;
}