#include <thread>
#include "core/br_time.h"
#include "core/net.h"



int main()
{
    int sock = create_pub();
    if (sock < 0) {
        std::cerr << "Ошибка создания сокета\n";
        return 1;
    }

    // Настройка адреса multicast-группы
    struct sockaddr_in group_addr = spm();                    // Порт

    int value{0};
    while (true) {
        // Отправляем int в сетевом порядке байт
        send_int_m(sock, value, group_addr);

        std::cout << value << std::endl;
        
        value++;
        delay_seconds(1);
    }

    close(sock);
    return 0;
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