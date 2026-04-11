#include "core/net.h"

int main()
{
    auto sub = create_subscriber();

    int data{0};

    while (true)
    {
        RecvStatus status = reciv_data(sub, data);

        if(check_disconnect(status, sub) < 0) {break;}
        
        std::cout << "data: ";
        std::cout << data << std::endl;
    }

    return 0;
}