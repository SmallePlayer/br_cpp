#include "core/net.h"



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