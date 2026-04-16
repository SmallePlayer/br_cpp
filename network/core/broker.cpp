/*
Брокер подругому опистаь данный код можно как сервер переадрисовщик.
Задача данного код подключать к себе  все созданые ноды которые создают топики.
Брокер получает соединение и определяет к нему подключился publisher или subscriber
и дальше решает ему получать от ноды соообщения или отправлять ноду сообщение.
*/
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
#include "net.h"

// глобальная переменная для блокирования поток для работы с вектором.
std::mutex clients_mutex;

// структура отвечающая за данные которые получает от клиента
struct Client
{
    int sock_client;
    std::string role;
    std::string topik;
};

// вектор подключившихся клиентов
std::vector<Client> clients;

// функция отвечающая за удаления клиента из вектора clients
void delete_client(int client_id)
{
    for (auto it = clients.begin(); it != clients.end();)
    {
        if (it->sock_client == client_id)
        {
            it = clients.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

// дебаг функция для контроля подключения нод
void print_client()
{
    for (const auto &c : clients)
    {
        std::cout << "sock_client: " << c.sock_client << ", role: " << c.role << ", topik: " << c.topik << "\n";
    }
}

// функция отвечающая за обработку подключенного клиента
void thread_client(int client_id)
{
    Client c; // структура подключившегося клиента
    c.sock_client = client_id;

    int answer{0}; // переменная с данными

    ClientHello msg_hello;            // структура первого сообщения
    reciv_data(client_id, msg_hello); // получения первого сообщения для создания или подкписки на топик

    if (msg_hello.role == "sub")
    {
        std::cout << "sub" << std::endl;
        c.sock_client = client_id;
        c.role = "sub";
        c.topik = msg_hello.topik;
    }
    else if (msg_hello.role == "pub")
    {
        std::cout << "pub" << std::endl;
        c.role = "pub";
        c.topik = msg_hello.topik;
    }

    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients.emplace_back(c);
    }

    print_client();

    while (true)
    {
        RecvStatus status = reciv_data(client_id, answer); // получение данных от отправителя

        if (status == RecvStatus::DISCONNECTED || status == RecvStatus::ERROR)
        {
            std::cout << "Клиент " << client_id << " отключился или ошибка!\n";
            close(client_id);
            std::lock_guard<std::mutex> lock(clients_mutex);
            delete_client(client_id);
            break;
        }

        std::cout << "Client: " << client_id << " Получено: " << answer << "\n";

        if (c.role == "sub")
        {
            continue; // Подписчик только слушает, не отправляет данные в брокер
        }

        auto data = answer;

        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            for (const auto &client : clients)
            {
                if (client.role == "sub" && client.topik == c.topik)
                {
                    send_data(client.sock_client, data); // Отправляем подписчику (client), а не издателю (c)
                    std::cout << "Отправлено Client " << client.sock_client << " Данные: " << data << std::endl;
                }
            }
        }
    }
}

// главная функция отвечающая за создание сокета и создание потоков подключений клиентов
int main()
{
    int server_id = create_tcp_socket();        // создание сокета
    settings_server_socket(server_id, PORT, 5); // настройка сокета
    std::cout << "Server wait connection" << std::endl;

    std::vector<std::thread> threads; // массив подключений

    while (true)
    {                                             // цикл с работой с подключениями
        int client_id = accpet_client(server_id); // определение подключения
        std::cout << "connect client: ";
        std::cout << client_id << std::endl;

        threads.emplace_back(std::thread(thread_client, client_id)); // создание потока для каждого подключения
        threads.back().detach();                                     // пустить поток в работу и откоючиться от него
    }

    close(server_id);

    return 0;
}