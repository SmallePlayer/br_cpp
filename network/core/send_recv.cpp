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


void settings_multicast_pub()
{
    multicast_addr.sin_family = AF_INET;
    multicast_addr.sin_port = htons(MULTICAST_PORT);
    inet_pton(AF_INET, MULTICAST_ADDR, &multicast_addr.sin_addr);
}

void settings_multicast_sub(int sub)
{
    setup_multicast_receiver(sub, MULTICAST_ADDR, MULTICAST_PORT);
}

//------send------

void send_hello(int fd, std::string topik)
{   
    std::cout << "SENDING HELLO: " << topik << " to " 
              << MULTICAST_ADDR << ":" << MULTICAST_PORT << std::endl;
    sendto(fd, topik.data(), topik.size(), 0,
           (struct sockaddr *)&multicast_addr, sizeof(multicast_addr));
}

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

void send_multicast_int(int pub, int data)
{
    sendto(pub, &data, sizeof(data), 0,
           (sockaddr *)&multicast_addr, sizeof(multicast_addr));
}

void send_multicast_float(int pub, float data)
{
    sendto(pub, &data, sizeof(data), 0,
           (sockaddr *)&multicast_addr, sizeof(multicast_addr));
}

void send_multicast_double(int pub, double data)
{
    sendto(pub, &data, sizeof(data), 0,
           (sockaddr *)&multicast_addr, sizeof(multicast_addr));
}

//-----recv------




ssize_t recv_hello(int fd, std::string &msg)
{   
    std::cout << "WAITING FOR HELLO on fd=" << fd << std::endl;
    char buffer[256];
    ssize_t received = recvfrom(fd, buffer, sizeof(buffer), 0,
                                (struct sockaddr *)&client_addr, &client_len);
    std::cout << "RECEIVED: " << received << " bytes" << std::endl;

    if (received > 0)
    {
        msg.assign(buffer, received); // ← пишем в ссылку
    }

    return received;
}

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

int recv_multicast_int(int fd, int &data)
{
    return recvfrom(fd,
                    &data,
                    sizeof(data),
                    0,
                    (sockaddr *)&client_addr,
                    &client_len);
}

int recv_multicast_float(int fd, float &data)
{
    return recvfrom(fd,
                    &data,
                    sizeof(data),
                    0,
                    (sockaddr *)&client_addr,
                    &client_len);
}

int recv_multicast_double(int fd, double &data)
{
    return recvfrom(fd,
                    &data,
                    sizeof(data),
                    0,
                    (sockaddr *)&client_addr,
                    &client_len);
}