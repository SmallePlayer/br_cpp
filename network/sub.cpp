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



void close_dissconnect(int socket_id){
    char buf[1];  
        ssize_t n = recv(socket_id, buf, sizeof(buf), 0);

        if (n == 0) {
            std::cout << "Брокер отключился\n";
            close(socket_id);
            exit(0);
        }
}

int main(){
    const int PORT = 8080;
    const char* HOST = "127.0.0.1";
    
    Subscribe sub;
    sub.role = "sub";
    sub.topik = "info";

    signal(SIGINT, on_exit);

    int socket_id = create_socket();
    socket_id_global = socket_id;
    sockaddr_in server_addres = settings_client_socket(socket_id, (char*)HOST, PORT);
    connect_server(socket_id, server_addres);

    std::cout << "Connected to server!\n";
    send_data(socket_id, sub);
    
    int data{0};

    while (true){
        reciv_data(socket_id, data);
        std::cout << "data: ";
        std::cout << data << std::endl;

        close_dissconnect(socket_id);

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    close(socket_id);

    return 0;
}