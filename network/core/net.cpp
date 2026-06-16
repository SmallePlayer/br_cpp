#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "net.h"
#include "signals.hpp"
#include "send_recv.h"
#include "core/json.hpp" 



int socket_id_global = -1;


// Создание сокета TCP
int create_tcp_socket()
{
    int socket_id = socket(AF_INET, SOCK_STREAM, 0);
    return socket_id;
}

// Создание сокета UDP
int create_udp_socket()
{
    int socket_id = socket(AF_INET, SOCK_DGRAM, 0);
    return socket_id;
}

// settings_send_multicast
void ssm(int sock){ 
    // Разрешаем повторное использование адреса (важно для нескольких слушателей)
    int reuse = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        std::cerr << "Ошибка SO_REUSEADDR\n";
        close(sock);
    }

    // Привязываемся к порту
    struct sockaddr_in local_addr;
    std::memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = INADDR_ANY; // Слушаем все интерфейсы
    local_addr.sin_port = htons(12345);

    if (bind(sock, (struct sockaddr*)&local_addr, sizeof(local_addr)) < 0) {
        std::cerr << "Ошибка bind\n";
        close(sock);
    }

    // Присоединяемся к multicast-группе
    struct ip_mreq group;
    group.imr_multiaddr.s_addr = inet_addr("239.255.0.1"); // Адрес группы
    group.imr_interface.s_addr = INADDR_ANY;               // Локальный интерфейс
    
    if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &group, sizeof(group)) < 0) {
        std::cerr << "Ошибка добавления в группу\n";
        close(sock);
    }
}

// settings_send_multicast
struct sockaddr_in spm(){  
    struct sockaddr_in group_addr;
    std::memset(&group_addr, 0, sizeof(group_addr));
    group_addr.sin_family = AF_INET;
    group_addr.sin_addr.s_addr = inet_addr("239.255.0.1"); // Адрес группы
    group_addr.sin_port = htons(12345);  
    return group_addr;                
}

// Настройки отправителя UDP
void settings_udp_sender(int fd)
{
    u_char ttl_val = static_cast<int>(2);
    if (setsockopt(fd, IPPROTO_IP, IP_MULTICAST_TTL, &ttl_val, sizeof(ttl_val)) < 0)
    {
        perror("setsockopt IP_MULTICAST_TTL failed");
    }

    u_char loop = 2;
    if (setsockopt(fd, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop)) < 0)
    {
        perror("setsockopt IP_MULTICAST_LOOP failed");
        // не фатально, можно продолжить
    }
}



sockaddr_in settings_server_Tsocket(int server_id, int PORT, int queue)
{
    uint8_t opt = 1;
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    setsockopt(server_id, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    bind(server_id, (sockaddr *)&address, sizeof(address));
    listen(server_id, queue);
    return address;
}

sockaddr_in settings_client_Tsocket(int server_id, char *HOST, int PORT)
{
    sockaddr_in server_addres;
    server_addres.sin_family = AF_INET;
    server_addres.sin_port = htons(PORT);
    inet_pton(AF_INET, HOST, &server_addres.sin_addr);
    return server_addres;
}

void connect_server(int socket_id, sockaddr_in server_addres)
{
    connect(socket_id, (sockaddr *)&server_addres, sizeof(server_addres));
}

int accept_client(int server_number)
{
    sockaddr_in client_addr{};
    socklen_t client_len = sizeof(client_addr);

    int client_fd = accept(server_number, (sockaddr *)&client_addr, &client_len);
    return client_fd;
}

void on_exit(int sig)
{
    std::cout << "\nЗакрываю соединение...\n";
    if (socket_id_global >= 0)
    {
        close(socket_id_global);
    }
    exit(0);
}


