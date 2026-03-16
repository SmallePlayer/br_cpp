#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "settings_socket.h"




int create_socket(){
    int socket_id = socket(AF_INET, SOCK_STREAM, 0);
    return socket_id;
}

sockaddr_in settings_server_socket(int server_id, int PORT, int queue){
    uint8_t opt = 1;
    sockaddr_in address{}; 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons(PORT); 
    setsockopt(server_id, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); // установка опции для сокета
    
    bind(server_id, (sockaddr*)&address, sizeof(address));
    listen(server_id, queue);
    return address;
}

sockaddr_in settings_client_socket(int server_id, char* HOST, int PORT){
    sockaddr_in server_addres;
    server_addres.sin_family = AF_INET;
    server_addres.sin_port = htons(PORT);
    inet_pton(AF_INET, HOST, &server_addres.sin_addr);
    return server_addres;
}

void connect_server(int socket_id, sockaddr_in server_addres){
        connect(socket_id, (sockaddr*)&server_addres, sizeof(server_addres));
}

int accpet_client(int server_number){
    sockaddr_in client_addr{};
    socklen_t client_len = sizeof(client_addr);

    int client_fd = accept(server_number, (sockaddr*)&client_addr, &client_len);
    return client_fd;
}