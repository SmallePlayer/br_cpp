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
#include "net.h"


int64_t now_ms() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now().time_since_epoch()
    ).count();
}

void copy_data(){

}

int main(){
    const int PORT = 8080; // порт для подключения
    const char* HOST = "127.0.0.1"; // адрес для подключения

    int socket_id = create_socket(); // 1. Создаём сокет
    
    sockaddr_in server_addres = settings_client_socket(socket_id, (char*)HOST, PORT); // 2. Настраиваем адрес сервера

    connect_server(socket_id, server_addres); // 3. Подключаемся к серверу

    int flag = 1;
    setsockopt(socket_id, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag)); // отключаем Nagle's algorithm для уменьшения задержки

    int buf = 4 * 1024 * 1024;
    setsockopt(socket_id, SOL_SOCKET, SO_SNDBUF, &buf, sizeof(buf)); // увеличиваем размер буфера отправки для уменьшения вероятности блокировки при отправке больших кадров



    std::cout << "Connected to server!\n";

    cv::VideoCapture cap(0); // открываем камеру
    cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M','J','P','G')); // устанавливаем кодек MJPEG для получения сжатых кадров напрямую из камеры
    std::cout << cap.get(cv::CAP_PROP_FPS) << std::endl; // выводим FPS камеры для отладки
    if(!cap.isOpened()){
        std::cerr << "Не удалось открыть камеру!" << std::endl;
        return -1;
    }

    std::thread t(capture_camera, std::ref(cap)); // запускаем отдельный поток для захвата кадров с камеры
    t.detach(); // отсоединяем поток, так как он будет работать бесконечно и не требует синхронизации с основным потоком

    while (true){
        cv::Mat local = get_frame(); // получаем последний захваченный кадр из глобальной переменной, которую обновляет поток захвата камеры

        if (local.empty()) continue; // если кадр пустой, пропускаем итерацию 

        int64_t ts = now_ms(); // получаем текущее время в миллисекундах

        std::vector<uchar> buffer = jpeg_compress(local, 60); // сжимаем кадр в JPEG формат с качеством 60 для уменьшения размера данных и, соответственно, задержки при отправке
        uint32_t size = htonl(buffer.size()); // преобразуем размер кадра в сетевой порядок байтов (big-endian) для корректной передачи по сети

        std::vector<uchar> packet(sizeof(int64_t) + sizeof(uint32_t) + buffer.size());   // создаём буфер для отправки
        memcpy(packet.data(), &ts, sizeof(ts));                                          // копируем метку времени в начало пакета
        memcpy(packet.data() + sizeof(ts), &size, sizeof(size));                         // копируем размер кадра после метки времени
        memcpy(packet.data() + sizeof(ts) + sizeof(size), buffer.data(), buffer.size()); // копируем данные кадра после размера
        send(socket_id, packet.data(), packet.size(), 0); // отправляем весь пакет целиком
        
        // ждём подтверждения от receiver перед отправкой следующего кадра
        char ack = 0;
        recv(socket_id, &ack, sizeof(ack), 0);
    }

    // 6. Закрываем сокет
    close(socket_id);
    cap.release();
    return 0;
}