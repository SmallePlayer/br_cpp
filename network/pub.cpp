#include "core/br_time.h"
#include "core/net.h"



int main(){
    int pub = create_pub();
    setup_multicast_sender(pub, 2);
    settings_multicast_pub();

    int counter = 0;
    while (true) {
        send_multicast_int(pub, counter);
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