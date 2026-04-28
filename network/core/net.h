#ifndef NET_H
#define NET_H

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "signals.hpp"
#include "config.hpp"
#include "send_recv.h"

extern int socket_id_global;

struct ClientHello
{
    std::string role;
    std::string topik;
};

enum class RecvStatus
{
    OK,           // данные получены
    DISCONNECTED, // клиент закрыл соединение (recv == 0)
    ERROR         // ошибка сокета
};

int create_tcp_socket();
int create_udp_socket();
int create_pub();
int create_sub();
sockaddr_in settings_server_socket(int server_number, int PORT, int queue = 5);
sockaddr_in settings_client_socket(int server_number, char *HOST, int PORT);
void connect_server(int socket_id, sockaddr_in server_addres);
int accpet_client(int server_number);
void on_exit(int sig);
int create_publisher(std::string current_topik);
int create_subscriber(std::string current_topik);
int check_disconnect(RecvStatus status, int sub);

template <typename T>
void send_data(int socket_id, const T &data)
{
    send(socket_id, &data, sizeof(data), 0);
}

template <typename T>
RecvStatus reciv_data(int socket_id, T &data)
{
    ssize_t n = read(socket_id, &data, sizeof(data));

    if (n == 0)
    {
        return RecvStatus::DISCONNECTED; // ⭐ клиент отключился
    }
    if (n < 0)
    {
        return RecvStatus::ERROR;
    }
    if (n != sizeof(data))
    {
        // неполные данные — тоже ошибка
        return RecvStatus::ERROR;
    }
    return RecvStatus::OK;
}

#endif
