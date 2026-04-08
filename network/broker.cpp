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
#include "frames.h"
#include "jpeg_work.h"

struct Client{
    int sock_client;
    std::string role;
};

std::vector<Client> clients;

const int PORT = 8080; // порт для прослушивания
const char* HOST = "127.0.0.1"; // адрес для прослушивания (локальный хост)

void delete_client(int client_id){
    for (auto it = clients.begin(); it != clients.end();){
        if (it->sock_client == client_id){
            it = clients.erase(it);
        } else{
            ++it;
        }
    }
}

void print_client(){
    for (const auto& c : clients) {
        std::cout << "sock_client: " << c.sock_client << ", role: " << c.role << "\n";
    }
}

void thread_client(int client_id){ // функция для работы с каждым отдельным потоком 
    Client c;
    c.sock_client = client_id;

    int answer{0}; // переменная с данными
    std::string role_client = " ";
    reciv_data(client_id, role_client);

    if(role_client == "sub") {
        std::cout << "sub" << std::endl;
        c.role = "sub";
    }
    else if(role_client == "pub") {
        std::cout << "pub" << std::endl;
        c.role = "pub";
    }

    clients.emplace_back(c);

    print_client();

    while (true){ // цикл работы с клиентом
        RecvStatus status = reciv_data(client_id, answer); // получение данных от отправителя

        if (reciv_data(client_id, answer) == RecvStatus::DISCONNECTED) {
            std::cout << "Клиент " << client_id <<" отключился!\n";
            close(client_id);
            delete_client(client_id);
            break;
        }

        std::cout << "Client: " << client_id << " Получено: " << answer << "\n";

        int data = answer;

        for (const auto& c : clients) {
            //std::cout << "sock_client: " << c.sock_client << ", role: " << c.role << "\n";
            if(c.role == "sub"){
                send_data(c.sock_client, data);
                std::cout << "Отправлено Client " << c.sock_client << " Данные: " << data << std::endl;
            }
        }
        

        // std::string response = "Сервер получил: data=" + std::to_string(answer);
        // send(client_id, response.c_str(), response.size(), 0);

    }
}


int main(){
    int server_id = create_socket();  //создание сокета
    settings_server_socket(server_id, PORT, 5); // настройка сокета
    std::cout << "Server wait connection" << std::endl;
    
    std::vector<std::thread> threads; // массив подключений

    while (true){ // цикл с работой с подключениями
        int client_id = accpet_client(server_id); // определение подключения
        std::cout << "connect client: ";
        std::cout << client_id << std::endl;
        
        threads.emplace_back(std::thread(thread_client, client_id)); // создание потока для каждого подключения
        threads.back().detach(); // пустить поток в работу и откоючиться от него
    }

    close(server_id);

    return 0;
}