#include <thread>
#include "core/br_time.h"
#include "core/net.h"

void create_topik_and_connect(int pub_h)
{

    while (true)
    {
        send_hello(pub_h, "info");
        delay_seconds(3);
    }
}

int main()
{
    int pub_h = create_pub();
    setup_multicast_sender(pub_h, 2);
    struct sockaddr_in local_addr{};
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    local_addr.sin_port = htons(0); // любой порт
    bind(pub_h, (struct sockaddr *)&local_addr, sizeof(local_addr));

    settings_multicast_pub();

    std::vector<std::thread> threads;
    threads.emplace_back(std::thread(create_topik_and_connect, pub_h)); // создание потока для каждого подключения
    threads.back().detach();
    int pub = create_pub();
    settings_udp_pub();

    int counter = 0;
    while (true)
    {

        send_int(pub, counter);
        std::cout << "Sent: " << counter << std::endl;
        counter++;
        delay_seconds(1);
    }
}

/*
int main()
{
    int pub = create_pub();
    settings_udp_pub();

    int counter{0};

    while (true)
    {
        send_int(pub, counter);
        std::cout << "Send: " << counter << std::endl;
        counter++;
        delay_microseconds(10);
    }
}
*/

/*
int main()
{
    auto publisher = create_publisher("info");

    int data{5};

    while (true){
        send_data(publisher, data);
        delay_seconds(1);
    }

    return 0;
}*/