#include "core/net.h"
#include <netinet/tcp.h>
#include <opencv2/opencv.hpp>
#include "frames.h"
#include "jpeg_work.h"



int main(){
    auto pub =  create_publisher("/camera");

    cv::VideoCapture cap(0);                                                   // открываем камеру
    cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G')); // устанавливаем кодек MJPEG для получения сжатых кадров напрямую из камеры
    if (!cap.isOpened())
    {
        std::cerr << "Не удалось открыть камеру!" << std::endl;
        return -1;
    }

    std::thread t(capture_camera, std::ref(cap)); // запускаем отдельный поток для захвата кадров с камеры
    t.detach();  
    
    while(true){
        cv::Mat local = get_frame();

        if (local.empty())
            continue;

        std::vector<uchar> buffer = jpeg_compress(local, 60);
        uint32_t size = htonl(buffer.size());

        std::vector<uchar> packet(sizeof(uint32_t) + buffer.size());  
        memcpy(packet.data(), &size, sizeof(size));                                      
        memcpy(packet.data() + sizeof(size), buffer.data(), buffer.size());              
        send(pub, packet.data(), packet.size(), 0); 

        char ack = 0;
        recv(pub, &ack, sizeof(ack), 0);
    }
}