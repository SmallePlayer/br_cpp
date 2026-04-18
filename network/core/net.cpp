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
#include "signals.hpp"
#include "config.hpp"
#include "send_recv.h"

int socket_id_global = -1;

int create_tcp_socket()
{
    int socket_id = socket(AF_INET, SOCK_STREAM, 0);
    return socket_id;
}

int create_udp_socket()
{
    int socket_id = socket(AF_INET, SOCK_DGRAM, 0);
    return socket_id;
}

void settings_udp_sender(int fd)
{
    u_char ttl_val = static_cast<int>(2);
    if (setsockopt(fd, IPPROTO_IP, IP_MULTICAST_TTL, &ttl_val, sizeof(ttl_val)) < 0)
    {
        perror("setsockopt IP_MULTICAST_TTL failed");
    }

    u_char loop = 0;
    if (setsockopt(fd, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop)) < 0)
    {
        perror("setsockopt IP_MULTICAST_LOOP failed");
        // не фатально, можно продолжить
    }
}

// net.cpp (добавить после существующих функций)

void setup_multicast_sender(int sock, int ttl)
{
    //create_topik

    /*
    Создание и отправка первого сообщения топика.
    потом sub получается первое сообщение проверяет его размер если он совпадает с тем что пришло впервый 
    раз и читает и получает подтверждение топиков.
    */


    // Устанавливаем TTL (время жизни) для multicast пакетов
    // ttl = 1 - только локальная сеть, >1 - могут выходить за роутер
    u_char ttl_val = static_cast<u_char>(ttl);
    if (setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, &ttl_val, sizeof(ttl_val)) < 0)
    {
        perror("setsockopt IP_MULTICAST_TTL failed");
    }

    // Отключаем loopback, чтобы не получать свои же пакеты (опционально)
    // Если закомментировать, то отправитель будет слышать сам себя.
    u_char loop = 0;
    if (setsockopt(sock, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop)) < 0)
    {
        perror("setsockopt IP_MULTICAST_LOOP failed");
        // не фатально, можно продолжить
    }

}

void setup_multicast_receiver(int sock, const char *multicast_addr, int port)
{
    // 1. Разрешаем переиспользование адреса (SO_REUSEADDR)
    int reuse = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
    {
        perror("setsockopt SO_REUSEADDR failed");
    }

    // 2. Привязываем сокет к порту
    struct sockaddr_in local_addr{};
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(port);
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0)
    {
        perror("bind multicast receiver failed");
    }

    // 3. Подписываемся на мультикаст-группу
    struct ip_mreq mreq{};
    mreq.imr_multiaddr.s_addr = inet_addr(multicast_addr);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY); // слушаем на всех интерфейсах

    if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)
    {
        perror("setsockopt IP_ADD_MEMBERSHIP failed");
    }
}

void create_topik(std::string topik){
    std::cout << "Create topik: " << topik << std::endl;
}

int create_pub(std::string topik)
{
    int fd = create_udp_socket();
    create_topik(topik);
    return fd;
}

int create_sub()
{
    int fd = create_udp_socket();
    return fd;
}



sockaddr_in settings_server_socket(int server_id, int PORT, int queue)
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

sockaddr_in settings_client_socket(int server_id, char *HOST, int PORT)
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

int accpet_client(int server_number)
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

int create_publisher(std::string current_topik)
{
    setup_signal_handlers();
    ClientHello sub;
    sub.role = "pub";
    sub.topik = current_topik;
    auto socket_id = create_tcp_socket();
    sockaddr_in server_addres = settings_client_socket(socket_id, (char *)HOST, PORT);
    connect_server(socket_id, server_addres);

    std::cout << "Connected to server!\n";
    send_data(socket_id, sub);
    return socket_id;
}

int create_subscriber(std::string current_topik)
{
    ClientHello sub;
    sub.role = "sub";
    sub.topik = current_topik;

    int socket_id = create_tcp_socket();
    sockaddr_in server_addres = settings_client_socket(socket_id, (char *)HOST, PORT);
    connect_server(socket_id, server_addres);

    std::cout << "Connected to server!\n";
    send_data(socket_id, sub);
    return socket_id;
}

int check_disconnect(RecvStatus status, int sub)
{
    if (status == RecvStatus::DISCONNECTED)
    {
        std::cout << "Брокер отключился\n";
        close(sub);
        return -1;
    }
    if (status == RecvStatus::ERROR)
    {
        std::cerr << "Ошибка приёма данных\n";
        return -1;
    }
    return 0;
}