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
#include "br_time.h"
#include "net.h"
#include "signals.hpp"
#include "config.hpp"





int main()
{   
    auto publisher = create_publisher("info");

    int data{5};

    while (true){
        send_data(publisher, data);
        delay_seconds(1);
    }


    close(publisher);
    return 0;
}