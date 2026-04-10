#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <csignal>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "net.h"



int main()
{
    const int PORT = 8080;
    const char *HOST = "127.0.0.1";

    ClientHello sub;
    sub.role = "sub";
    sub.topik = "info";

    signal(SIGINT, on_exit);

    int socket_id = create_socket();
    sockaddr_in server_addres = settings_client_socket(socket_id, (char *)HOST, PORT);
    connect_server(socket_id, server_addres);

    std::cout << "Connected to server!\n";
    send_data(socket_id, sub);

    int data{0};

    while (true)
    {
        RecvStatus status = reciv_data(socket_id, data);
              
        if (status == RecvStatus::DISCONNECTED) {
            std::cout << "Брокер отключился\n";
            close(socket_id);
            break;
        }
        if (status == RecvStatus::ERROR) {
            std::cerr << "Ошибка приёма данных\n";
            break;
        }

        std::cout << "data: ";
        std::cout << data << std::endl;

        

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    close(socket_id);

    return 0;
}