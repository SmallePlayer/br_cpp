#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <opencv2/opencv.hpp>
#include "frames.h"
#include "jpeg_work.h"

int64_t now_ms() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now().time_since_epoch()
    ).count();
}

int main(){
    const int PORT = 8080; // порт для прослушивания
    const char* HOST = "127.0.0.1"; // адрес для прослушивания (локальный хост)

    int server_number = socket(AF_INET, SOCK_STREAM, 0); // создание сокета

    uint8_t opt = 1; // разрешить повторное использование адреса
    setsockopt(server_number, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); // установка опции для сокета

    sockaddr_in address{}; // структура для хранения адреса сервера
    address.sin_family = AF_INET; // семейство протоколов (IPv4)
    address.sin_addr.s_addr = INADDR_ANY; // принимать соединения на всех интерфейсах
    address.sin_port = htons(PORT); // преобразование порта в сетевой порядок байтов

    bind(server_number, (sockaddr*)&address, sizeof(address)); // привязка сокета к адресу и порту

    listen(server_number, 5); // начало прослушивания входящих соединений (макс. 5 в очереди)

    sockaddr_in client_addr{}; // структура для хранения адреса клиента
    socklen_t client_len = sizeof(client_addr);

    int client_fd = accept(server_number, (sockaddr*)&client_addr, &client_len);

    int buf = 4 * 1024 * 1024;
    setsockopt(client_fd, SOL_SOCKET, SO_RCVBUF, &buf, sizeof(buf));

    
    while (true){

        int64_t ts = now_ms();
        int bytes = read(client_fd, &ts, sizeof(ts));
        if (bytes <= 0) { std::cout << "Клиент отключился\n"; break; }

        int64_t latency = now_ms() - ts;  // задержка в мс
        std::cout << "Задержка: " << latency << " мс\n";

        uint32_t size = 0;
        read(client_fd, &size, sizeof(size));
        size = ntohl(size); //получаем размер кадра в байтах
        
        std::vector<uchar> buffer(size); // буфер для приёма кадра
        // read(client_fd, buffer.data(), buffer.size());
        int total = 0;
        while (total < (int)size) { 
            int r = read(client_fd, buffer.data() + total, size - total); //читаем данные и вкладываем их в buffer. r - количество прочитанных байт
            total += r;
        }   

        cv::Mat img = jpeg_decompress(buffer);
        cv::imshow("Received Frame", img);
        if (cv::waitKey(1) == 'q') { 
            break;
        }

        // сигналим sender что готовы к следующему кадру
        char ack = 1;
        send(client_fd, &ack, sizeof(ack), 0);


    }

    close(client_fd);  // закрыть соединение с клиентом
    close(server_number);

    cv::destroyAllWindows();
    return 0;
}