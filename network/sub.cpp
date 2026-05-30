#include "core/net.h"



int main()
{
    int sock = create_sub();
    if (sock < 0) {
        std::cerr << "Ошибка создания сокета\n";
        return 1;
    }

    ssm(sock);

    int value{0};
    while (true) {
        recv_int_m(sock, value);
    }

    close(sock);
    return 0;
}

/*
int main()
{
    int sub = create_sub();
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
//     auto sub = create_subscriber("info");

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