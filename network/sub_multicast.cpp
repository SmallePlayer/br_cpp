#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        std::cerr << "Ошибка создания сокета\n";
        return 1;
    }

    // Разрешаем повторное использование адреса (важно для нескольких слушателей)
    int reuse = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        std::cerr << "Ошибка SO_REUSEADDR\n";
        close(sock);
        return 1;
    }

    // Привязываемся к порту
    struct sockaddr_in local_addr;
    std::memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = INADDR_ANY; // Слушаем все интерфейсы
    local_addr.sin_port = htons(12345);

    if (bind(sock, (struct sockaddr*)&local_addr, sizeof(local_addr)) < 0) {
        std::cerr << "Ошибка bind\n";
        close(sock);
        return 1;
    }

    // Присоединяемся к multicast-группе
    struct ip_mreq group;
    group.imr_multiaddr.s_addr = inet_addr("239.255.0.1"); // Адрес группы
    group.imr_interface.s_addr = INADDR_ANY;               // Локальный интерфейс
    
    if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &group, sizeof(group)) < 0) {
        std::cerr << "Ошибка добавления в группу\n";
        close(sock);
        return 1;
    }

    std::cout << "Слушаем multicast на 239.255.0.1:12345...\n";

    while (true) {
        int net_value;
        struct sockaddr_in sender_addr;
        socklen_t sender_len = sizeof(sender_addr);
        
        ssize_t received = recvfrom(sock, &net_value, sizeof(net_value), 0,
                                    (struct sockaddr*)&sender_addr, &sender_len);
        if (received > 0) {
            // Преобразуем из сетевого порядка в хост-порядок
            int value = ntohl(net_value);
            std::cout << "Получено: " << value 
                      << " от " << inet_ntoa(sender_addr.sin_addr) << std::endl;
        } else {
            std::cerr << "Ошибка получения\n";
        }
    }

    close(sock);
    return 0;
}