#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <vector>
#include <mutex>
#include "core/net.h"


void thread_client(int client_id)
{                  // функция для работы с каждым отдельным потоком
    int answer{0}; // переменная с данными

    while (true)
    {                                  // цикл работы с клиентом
        reciv_data(client_id, answer); // получение данных от отправителя

        if (answer == 0)
        {
            std::cout << "Клиент отключился\n";
            break;
        }

        if (answer < 0)
        {
            perror("read error");
            break;
        }

        std::cout << "Получено (" << sizeof(answer) << " байт): data=" << answer << "\n";
    }
}

int main()
{
    int server_id = create_socket();            // создание сокета
    settings_server_socket(server_id, PORT, 5); // настройка сокета

    std::vector<std::thread> threads; // массив подключений

    while (true)
    {                                             // цикл с работой с подключениями
        int client_id = accpet_client(server_id); // определение подключения

        threads.emplace_back(std::thread(thread_client, client_id)); // создание потока для каждого подключения
        threads.back().detach();                                     // пустить поток в работу и откоючиться от него
    }

    close(server_id);

    return 0;
}