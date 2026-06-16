#include <cerrno>
#include <cstring>
#include "core/net.h"
#include "core/json.hpp" 
#include "core/delay.h"

using json = nlohmann::json;

char *host = "127.0.0.1";
int port = 50020;

int main(){
    
    int sock = create_tcp_socket();
    settings_server_Tsocket(sock, port);
    std::cout << "server start" << '\n';
    
    int data{5};

    int sock_client = accept_client(sock);
    
    while(true){
        send_tcp_int(sock_client, data);
        std::cout << "send: " << data << '\n';
        delay_seconds(1);
    }
}




// int main()
// {
//     int sock = create_tcp_socket();            // создание сокета
//     settings_server_Tsocket(sock, PORT, 5);

//     int x_self, y_self, x_target, y_target;
//     float angle;


//     json msg;
//     msg["x_self"] = x_self;
//     msg["y_self"] = y_self;
//     msg["angle"] = angle;
//     msg["x_target"] = x_self;
//     msg["x_target"] = x_self;
    


//     int value{0};
//     while (true) {
//         msg = recv_json(sock);
//     }

//     close(sock);
//     return 0;
// }

/*
int main()
{
    int sub = create_udp_socket();
    settings_udp_sub(sub);

    int data;

    while (true)
    {
        recv_int(sub, data);

        std::cout << "Received: " << data << std::endl; // <-- ВЫВОДИМ!
    }
}
*/

// int main()
// {
//    auto sub = create_subscriber("info");

//     int data{0};

//     while (true)
//     {
//         RecvStatus status = reciv_data(sub, data);

//         if(check_disconnect(status, sub) < 0) {break;}

//         std::cout << "data: ";
//         std::cout << data << std::endl;
//     }

//     return 0;
// }