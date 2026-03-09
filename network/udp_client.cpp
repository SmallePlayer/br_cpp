#include <iostream>
#include <cstring>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <opencv2/opencv.hpp>
#include "jpeg_work.h"
#include "frames.h"


int64_t now_ms() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now().time_since_epoch()
    ).count();
}

int main() {
    const int PORT = 8080;
    const char* HOST = "127.0.0.1";

    // SOCK_DGRAM — UDP
    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, HOST, &server_addr.sin_addr);

    const char* hello = "hello";
    sendto(sock, hello, strlen(hello), 0, (sockaddr*)&server_addr, sizeof(server_addr));
    std::cout << "Подключились к серверу\n";

    // connect() у UDP не устанавливает соединение!
    // просто запоминает адрес сервера чтобы не указывать его в каждом sendto
    connect(sock, (sockaddr*)&server_addr, sizeof(server_addr));

    std::cout << "UDP клиент запущен\n";

    
    while (true) {
        std::vector<uchar> packet(70000);  // буфер с запасом
        int bytes = recv(sock, packet.data(), packet.size(), 0);
        if (bytes <= 0) break;

        // разбираем пакет
        int64_t ts;
        uint32_t size;
        memcpy(&ts, packet.data(), sizeof(ts));
        memcpy(&size, packet.data() + sizeof(ts), sizeof(size));
        size = ntohl(size);

        int64_t latency = now_ms() - ts;
        std::cout << "Задержка: " << latency << " мс, размер: " << size << " байт\n";

        std::vector<uchar> buffer(
            packet.data() + sizeof(ts) + sizeof(uint32_t),
            packet.data() + sizeof(ts) + sizeof(uint32_t) + size
        );

        cv::Mat img = jpeg_decompress(buffer);
        if (img.empty()) continue;

        cv::imshow("UDP Frame", img);
        if (cv::waitKey(1) == 'q') break;
    }

    close(sock);
    return 0;
}