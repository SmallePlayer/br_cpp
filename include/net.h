#ifndef NET_H
#define NET_H

#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int create_socket();
sockaddr_in settings_server_socket(int server_number, int PORT, int queue = 5);
sockaddr_in settings_client_socket(int server_number, char* HOST, int PORT);
void connect_server(int socket_id, sockaddr_in server_addres);
int accpet_client(int server_number);

template<typename T>
void send_data(int socket_id, const T& data) {
    send(socket_id, &data, sizeof(data), 0);
}

template<typename T>
void reciv_data(int socket_id, T& data) {
    read(socket_id, &data, sizeof(data));
}

#endif
