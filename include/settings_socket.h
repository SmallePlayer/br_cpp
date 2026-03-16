#ifndef SETTINGS_SOCKET_H
#define SETTINGS_SOCKET_H

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



#endif