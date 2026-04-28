#include "net.h"

//------settings------

sockaddr_in server_addr{};
static sockaddr_in multicast_addr{};
sockaddr_in client_addr{};
socklen_t client_len = sizeof(client_addr);

void settings_udp_pub()
{
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, HOST, &server_addr.sin_addr);
}

void settings_udp_sub(int sub)
{
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sub, (sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        std::cerr << "Ошибка bind\n";
        close(sub);
    }
}



//------send------


void send_int(int pub, int &data)
{
    ssize_t byte_sent = sendto(
        pub,
        &data,
        sizeof(data),
        0,
        (sockaddr *)&server_addr,
        sizeof(server_addr));
}

void send_float(int pub, float &data)
{
    ssize_t byte_sent = sendto(
        pub,
        &data,
        sizeof(data),
        0,
        (sockaddr *)&server_addr,
        sizeof(server_addr));
}

void send_double(int pub, double &data)
{
    ssize_t byte_sent = sendto(
        pub,
        &data,
        sizeof(data),
        0,
        (sockaddr *)&server_addr,
        sizeof(server_addr));
}



//-----recv------




int recv_int(int fd, int &data)
{
    ssize_t bytes_received = recvfrom(
        fd,
        &data,
        sizeof(data),
        0,
        (sockaddr *)&client_addr,
        &client_len);
    return bytes_received;
}

int recv_float(int fd, float &data)
{
    ssize_t bytes_received = recvfrom(
        fd,
        &data,
        sizeof(data),
        0,
        (sockaddr *)&client_addr,
        &client_len);
    return bytes_received;
}

int recv_double(int fd, double &data)
{
    ssize_t bytes_received = recvfrom(
        fd,
        &data,
        sizeof(data),
        0,
        (sockaddr *)&client_addr,
        &client_len);
    return bytes_received;
}
