#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "settings_socket.h"


const int PORT = 8080; // порт для прослушивания
const char* HOST = "127.0.0.1"; // адрес для прослушивания (локальный хост)



int main(){
    int server_id = create_socket();
    settings_server_socket(server_id, PORT, 5);
    int client_id = accpet_client(server_id);
    
    int answer = 0;

    int data = 0;

    while (true){
        answer = read(client_id, &data, sizeof(data));
        
        if(answer == 0){
            std::cout << "Клиент отключился\n";
            break;
        }

        if (answer < 0) {
            perror("read error");
            break;
        }

        std::cout << "Получено (" << answer << " байт): data=" << data << "\n";

        std::string response = "Сервер получил: data=" + std::to_string(data);
        send(client_id, response.c_str(), response.size(), 0);


    }

    close(client_id);  // закрыть соединение с клиентом
    close(server_id);

    return 0;
}