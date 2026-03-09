#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct __attribute__((packed)) Robot {
    int x;
    int y;
};

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

    int bytes = 0;

    
    Robot robot{};
    while (true){
        

        bytes = read(client_fd, &robot, sizeof(robot));
        robot.x = ntohl(robot.x);
        robot.y = ntohl(robot.y);
        
        if(bytes == 0){
            std::cout << "Клиент отключился\n";
            break;
        }

        if (bytes < 0) {
            // < 0 = ошибка
            perror("read error");
            break;
        }

        std::cout << "Получено (" << bytes << " байт): x=" << robot.x << ", y=" << robot.y << "\n";

        std::string response = "Сервер получил: x=" + std::to_string(robot.x) + ", y=" + std::to_string(robot.y);
        send(client_fd, response.c_str(), response.size(), 0);


    }

    close(client_fd);  // закрыть соединение с клиентом
    close(server_number);

    return 0;
}