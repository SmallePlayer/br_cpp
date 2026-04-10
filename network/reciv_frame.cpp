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
#include "net.h"

int64_t now_ms()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::steady_clock::now().time_since_epoch())
        .count();
}

int main()
{
    const int PORT = 8080;          // порт для прослушивания
    const char *HOST = "127.0.0.1"; // адрес для прослушивания (локальный хост)

    int server_id = create_socket();         // Создаём сокет для сервера
    settings_server_socket(server_id, PORT); // Настраиваем адрес сервера и начинаем прослушивание входящих соединений

    int client_id = accpet_client(server_id); // ожидание и принятие входящего соединения от клиента

    int buf = 4 * 1024 * 1024;
    setsockopt(client_id, SOL_SOCKET, SO_RCVBUF, &buf, sizeof(buf)); // увеличиваем размер буфера приёма для уменьшения вероятности блокировки при приёме больших кадров

    while (true)
    { //

        int64_t ts = now_ms();
        int bytes = read(client_id, &ts, sizeof(ts)); // читаем метку времени, отправленную клиентом
        if (bytes <= 0)
        {
            std::cout << "Клиент отключился\n";
            break;
        }

        int64_t latency = now_ms() - ts; // задержка в мс
        std::cout << "Задержка: " << latency << " мс\n";

        uint32_t size = 0;
        read(client_id, &size, sizeof(size)); // читаем размер кадра, отправленный клиентом
        size = ntohl(size);                   // получаем размер кадра в байтах

        std::vector<uchar> buffer(size); // буфер для приёма кадра

        int total = 0; // количество уже прочитанных байт
        while (total < (int)size)
        {                                                                 // читаем кадр по частям, так как он может быть слишком большим для одного вызова read
            int r = read(client_id, buffer.data() + total, size - total); // читаем данные и вкладываем их в buffer. r - количество прочитанных байт
            total += r;                                                   // обновляем количество прочитанных байт
        }

        cv::Mat img = jpeg_decompress(buffer); // декомпрессируем полученный буфер в изображение OpenCV
        cv::imshow("Received Frame", img);
        if (cv::waitKey(1) == 'q')
        {
            break;
        }

        // сигналим sender что готовы к следующему кадру
        char ack = 1;
        send(client_id, &ack, sizeof(ack), 0);
    }

    close(client_id); // закрыть соединение с клиентом
    close(server_id);

    cv::destroyAllWindows();
    return 0;
}