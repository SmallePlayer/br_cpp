#include "core/net.h"

int main()
{
    std::string topik = "info";

    int sub_h = create_sub();
    settings_multicast_sub(sub_h);

    int sub = create_sub();
    settings_udp_sub(sub);

    std::string message = " "; 
    ssize_t bytes = recv_hello(sub_h, message);
    std::cout << "[" << message << "]" << std::endl;
    
    if(bytes > 0 && message == topik)
    {
        int data;
        while (true)
        {
            int bytes = recv_int(sub, data);
            if (bytes > 0)
            {
                std::cout << "Received: " << data << std::endl;
            }
            else if (bytes < 0)
            {
                perror("recvfrom error");
                break;
            }
        }
    }
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