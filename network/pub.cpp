#include "core/br_time.h"
#include "core/net.h"

int main()
{
    int pub = create_pub();

    sockaddr_in client_addr{};
    socklen_t addr_len = sizeof(client_addr);
    char buffer[1024];
    
    while (true)
    { // 4. Приём данных от клиента
        int n = recvfrom(pub, buffer, sizeof(buffer) - 1, 0,
                         (struct sockaddr *)&client_addr, &addr_len);
        if (n < 0)
        {
            std::cerr << "Ошибка приёма\n";
            continue;
        }
        buffer[n] = '\0';
        std::cout << "Получено от " << inet_ntoa(client_addr.sin_addr)
                  << ":" << ntohs(client_addr.sin_port) << " -> " << buffer << std::endl;
    }
}

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