#include <thread>
#include <cerrno>
#include <cstring>
#include "core/delay.h"
#include "core/net.h"
#include "core/json.hpp" 

using json = nlohmann::json;

char *host = "127.0.0.1";
int port = 50020;

int main(){
    int sock = create_tcp_socket();
    sockaddr_in server_addres = settings_client_Tsocket(sock, host, port);
    std::cout << "start" << '\n';
    connect_server(sock, server_addres);

    int data{0};

    while(true){
        recv_tcp_int(sock, data);
        std::cout << data << '\n';
        delay_seconds(1);
    }


}




// int main()
// {
//     int sock = create_udp_socket();
//     if (sock < 0) {
//         std::cerr << "Ошибка создания сокета\n";
//         return 1;
//     }

//     // Настройка адреса multicast-группы
//     struct sockaddr_in group_addr = spm();                    // Порт

//     int value{0};
//     while (true) {
//         // Отправляем int в сетевом порядке байт
//         send_int_m(sock, value, group_addr);

//         std::cout << value << std::endl;
        
//         value++;
//         delay_seconds(1);
//     }

//     close(sock);
//     return 0;
// }

/*
int main()
{
    int pub = create_udp_socket();
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