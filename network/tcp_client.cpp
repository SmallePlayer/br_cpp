#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct __attribute__((packed)) Robot {
    int x;
    int y;
};

int main(){
    const int PORT = 8080;
    const char* HOST = "127.0.0.1";

    int sock_number = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_addres;
    server_addres.sin_family = AF_INET;
    server_addres.sin_port = htons(PORT);

    inet_pton(AF_INET, HOST, &server_addres.sin_addr);

    connect(sock_number, (sockaddr*)&server_addres, sizeof(server_addres));

    std::cout << "Connected to server!\n";

    Robot robot{};
    while (true){
        
        robot.x = htonl(1);
        robot.y = htonl(2);
        
        send(sock_number, &robot, sizeof(robot), 0);

        char buffer[1024]{};
        read(sock_number, buffer, sizeof(buffer) - 1);
        std::cout << "Ответ сервера: " << buffer << "\n";

        std::this_thread::sleep_for(std::chrono::seconds(1)); // пауза 1 секунда
    }

    // 6. Закрываем сокет
    close(sock_number);

    return 0;
}