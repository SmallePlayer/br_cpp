#ifndef NET_H
#define NET_H

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "signals.hpp"
#include "send_recv.h"

constexpr const char* HOST = "127.0.0.1";
constexpr int PORT = 8080; 

constexpr const char* MULTICAST_ADDR = "239.1.2.3";
constexpr int MULTICAST_PORT = 50001;

extern int socket_id_global;

struct ClientHello
{
    std::string role;
    std::string topik;
};



int create_tcp_socket();
int create_udp_socket();
struct sockaddr_in spm();
void ssm(int sock);
sockaddr_in settings_server_Tsocket(int server_number, int PORT, int queue = 5);
sockaddr_in settings_client_Tsocket(int server_number, char *HOST, int PORT);
void connect_server(int socket_id, sockaddr_in server_addres);
int accept_client(int server_number);
void on_exit(int sig);




#endif
