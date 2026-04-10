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
#include "net.h"
#include "config.hpp"



int main()
{
    auto sub = create_subscriber();

    int data{0};

    while (true)
    {
        RecvStatus status = reciv_data(sub, data);

        if (status == RecvStatus::DISCONNECTED)
        {
            std::cout << "Брокер отключился\n";
            close(sub);
            break;
        }
        if (status == RecvStatus::ERROR)
        {
            std::cerr << "Ошибка приёма данных\n";
            break;
        }

        std::cout << "data: ";
        std::cout << data << std::endl;
    }

    close(sub);

    return 0;
}