#include <iostream>
#include <cstring>
#include <unistd.h>
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

    // SOCK_DGRAM вместо SOCK_STREAM — это и есть UDP
    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(sock, (sockaddr*)&address, sizeof(address));

    char hello[16]{};
    sockaddr_in client_addr{};
    socklen_t client_len = sizeof(client_addr);
    recvfrom(sock, hello, sizeof(hello), 0, (sockaddr*)&client_addr, &client_len);
    std::cout << "Клиент подключился\n";

    std::cout << "UDP сервер слушает порт " << PORT << "\n";

    cv::VideoCapture cap(0);
    if(!cap.isOpened()){
        std::cerr << "Не удалось открыть камеру!" << std::endl;
        return -1;
    }

    std::thread t(capture_camera, std::ref(cap));
    t.detach();

    while (true) {
        cv::Mat local = get_frame();
        

        if (local.empty()) continue;
        cv::resize(local, local, cv::Size(320, 240));

        int64_t ts = now_ms();

        std::vector<uchar> buffer = jpeg_compress(local, 80);
        uint32_t size = htonl(buffer.size());

        if (buffer.size() > 60000) {
            std::cerr << "Кадр слишком большой: " << buffer.size() << " байт\n";
            continue;
        }

        std::vector<uchar> packet(sizeof(int64_t) + sizeof(uint32_t) + buffer.size());
        memcpy(packet.data(), &ts, sizeof(ts));
        memcpy(packet.data() + sizeof(ts), &size, sizeof(size));
        memcpy(packet.data() + sizeof(ts) + sizeof(size), buffer.data(), buffer.size());

        sendto(sock, packet.data(), packet.size(), 0,
               (sockaddr*)&client_addr, client_len);
        
    }

    close(sock);
    return 0;
}