#include "core/br_time.h"
#include "core/net.h"

int main()
{   
    auto publisher = create_publisher("info");

    int data{5};

    while (true){
        send_data(publisher, data);
        delay_seconds(1);
    }

    return 0;
}