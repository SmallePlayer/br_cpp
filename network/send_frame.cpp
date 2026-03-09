#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h> 
#include <opencv2/opencv.hpp>
#include "jpeg_work.h"
#include "frames.h"


int64_t now_ms() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now().time_since_epoch()
    ).count();
}

int main(){
    const int PORT = 8080;
    const char* HOST = "127.0.0.1";

    int sock_number = socket(AF_INET, SOCK_STREAM, 0);
    
    sockaddr_in server_addres;
    server_addres.sin_family = AF_INET;
    server_addres.sin_port = htons(PORT);

    inet_pton(AF_INET, HOST, &server_addres.sin_addr);

    connect(sock_number, (sockaddr*)&server_addres, sizeof(server_addres));

    int flag = 1;
    setsockopt(sock_number, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));

    int buf = 4 * 1024 * 1024;
    setsockopt(sock_number, SOL_SOCKET, SO_SNDBUF, &buf, sizeof(buf));



    std::cout << "Connected to server!\n";

    cv::VideoCapture cap(0);
    if(!cap.isOpened()){
        std::cerr << "Не удалось открыть камеру!" << std::endl;
        return -1;
    }

    std::thread t(capture_camera, std::ref(cap));
    t.detach();

    while (true){
        cv::Mat local = get_frame();

        if (local.empty()) continue;

        int64_t ts = now_ms();

        std::vector<uchar> buffer = jpeg_compress(local);
        uint32_t size = htonl(buffer.size());
        // send(sock_number, &ts, sizeof(ts), 0);
        // send(sock_number, &size, sizeof(size), 0);
        // send(sock_number, buffer.data(), buffer.size(), 0);

        std::vector<uchar> packet(sizeof(int64_t) + sizeof(uint32_t) + buffer.size());
        memcpy(packet.data(), &ts, sizeof(ts));
        memcpy(packet.data() + sizeof(ts), &size, sizeof(size));
        memcpy(packet.data() + sizeof(ts) + sizeof(size), buffer.data(), buffer.size());
        send(sock_number, packet.data(), packet.size(), 0);

        // ждём подтверждения от receiver перед отправкой следующего кадра
        char ack = 0;
        recv(sock_number, &ack, sizeof(ack), 0);
    }

    // 6. Закрываем сокет
    close(sock_number);
    cap.release();
    return 0;
}