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
#include "settings_socket.h"


int64_t now_ms() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now().time_since_epoch()
    ).count();
}

int main(){
    const int PORT = 8080;
    const char* HOST = "127.0.0.1";

    int socket_id = create_socket();
    
    sockaddr_in server_addres = settings_client_socket(socket_id, (char*)HOST, PORT);

    connect_server(socket_id, server_addres);

    int flag = 1;
    setsockopt(socket_id, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));

    int buf = 4 * 1024 * 1024;
    setsockopt(socket_id, SOL_SOCKET, SO_SNDBUF, &buf, sizeof(buf));



    std::cout << "Connected to server!\n";

    cv::VideoCapture cap(0);
    cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M','J','P','G'));
    std::cout << cap.get(cv::CAP_PROP_FPS);
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

        std::vector<uchar> buffer = jpeg_compress(local, 60);
        uint32_t size = htonl(buffer.size());
        // send(sock_number, &ts, sizeof(ts), 0);
        // send(sock_number, &size, sizeof(size), 0);
        // send(sock_number, buffer.data(), buffer.size(), 0);

        std::vector<uchar> packet(sizeof(int64_t) + sizeof(uint32_t) + buffer.size());
        memcpy(packet.data(), &ts, sizeof(ts));
        memcpy(packet.data() + sizeof(ts), &size, sizeof(size));
        memcpy(packet.data() + sizeof(ts) + sizeof(size), buffer.data(), buffer.size());
        send(socket_id, packet.data(), packet.size(), 0);

        // ждём подтверждения от receiver перед отправкой следующего кадра
        char ack = 0;
        recv(socket_id, &ack, sizeof(ack), 0);
    }

    // 6. Закрываем сокет
    close(socket_id);
    cap.release();
    return 0;
}