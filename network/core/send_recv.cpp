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

void send_int_m(int sock, int value, struct sockaddr_in group_addr)
{
    int net_value = htonl(value);
    ssize_t sent = sendto(sock, &net_value, sizeof(net_value), 0,
                          (struct sockaddr *)&group_addr, sizeof(group_addr));
}

void send_float_m(int sock, float value, struct sockaddr_in group_addr)
{
    float net_value = htonl(value);
    ssize_t sent = sendto(sock, &net_value, sizeof(net_value), 0,
                          (struct sockaddr *)&group_addr, sizeof(group_addr));
}

void send_double_m(int sock, double value, struct sockaddr_in group_addr)
{
    double net_value = htonl(value);
    ssize_t sent = sendto(sock, &net_value, sizeof(net_value), 0,
                          (struct sockaddr *)&group_addr, sizeof(group_addr));
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

float recv_float(int fd, float &data)
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

double recv_double(int fd, double &data)
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

int recv_int_m(int sock, int &data)
{   
    int value{0};
    struct sockaddr_in sender_addr;
    socklen_t sender_len = sizeof(sender_addr);

    ssize_t received = recvfrom(sock, &data, sizeof(data), 0,
                                (struct sockaddr *)&sender_addr, &sender_len);
    if (received > 0)
    {
        // Преобразуем из сетевого порядка в хост-порядок
        value = ntohl(data);
        std::cout << "Получено: " << value
                  << " от " << inet_ntoa(sender_addr.sin_addr) << std::endl;
    }
    else
    {
        std::cerr << "Ошибка получения\n";
    }
    return value;
}

int recv_float_m(int sock, float &data)
{   
    float value{0};
    struct sockaddr_in sender_addr;
    socklen_t sender_len = sizeof(sender_addr);

    ssize_t received = recvfrom(sock, &data, sizeof(data), 0,
                                (struct sockaddr *)&sender_addr, &sender_len);
    if (received > 0)
    {
        // Преобразуем из сетевого порядка в хост-порядок
        value = ntohl(data);
        std::cout << "Получено: " << value
                  << " от " << inet_ntoa(sender_addr.sin_addr) << std::endl;
    }
    else
    {
        std::cerr << "Ошибка получения\n";
    }
    return value;
}

int recv_double_m(int sock, double &data)
{   
    double value{0};
    struct sockaddr_in sender_addr;
    socklen_t sender_len = sizeof(sender_addr);

    ssize_t received = recvfrom(sock, &data, sizeof(data), 0,
                                (struct sockaddr *)&sender_addr, &sender_len);
    if (received > 0)
    {
        // Преобразуем из сетевого порядка в хост-порядок
        value = ntohl(data);
        std::cout << "Получено: " << value
                  << " от " << inet_ntoa(sender_addr.sin_addr) << std::endl;
    }
    else
    {
        std::cerr << "Ошибка получения\n";
    }
    return value;
}