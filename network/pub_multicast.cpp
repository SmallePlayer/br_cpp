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

    // Настройка адреса multicast-группы
    struct sockaddr_in group_addr;
    std::memset(&group_addr, 0, sizeof(group_addr));
    group_addr.sin_family = AF_INET;
    group_addr.sin_addr.s_addr = inet_addr("239.255.0.1"); // Адрес группы
    group_addr.sin_port = htons(12345);                     // Порт

    int value = 0;
    while (true) {
        // Отправляем int в сетевом порядке байт
        int net_value = htonl(value);
        ssize_t sent = sendto(sock, &net_value, sizeof(net_value), 0,
                              (struct sockaddr*)&group_addr, sizeof(group_addr));
        
        if (sent > 0) {
            std::cout << "Отправлено: " << value << std::endl;
        } else {
            std::cerr << "Ошибка отправки\n";
        }
        
        value++;
        sleep(1);
    }

    close(sock);
    return 0;
}