#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "net.h"



int main(){
    const int PORT = 8080;
    const char* HOST = "127.0.0.1";

    int socket_id = create_socket();
    sockaddr_in server_addres = settings_client_socket(socket_id, (char*)HOST, PORT);
    connect_server(socket_id, server_addres);

    std::cout << "Connected to server!\n";

    
    float data{5.2f};

    while (true){
        send_data(socket_id, data);

        char buffer[1024]{};
        read(socket_id, buffer, sizeof(buffer) - 1);
        std::cout << "Ответ сервера: " << buffer << "\n";

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    close(socket_id);

    return 0;
}