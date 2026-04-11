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
#include "core/net.h"
#include "core/br_time.h"


int main()
{
    signal(SIGINT, on_exit);

    int socket_id = create_tcp_socket();
    socket_id_global = socket_id;
    sockaddr_in server_addres = settings_client_socket(socket_id, (char *)HOST, PORT);
    connect_server(socket_id, server_addres);

    std::cout << "Connected to server!\n";

    int data{5};

    while (true)
    {
        send_data(socket_id, data);


        delay_seconds(1);
    }

    close(socket_id);

    return 0;
}