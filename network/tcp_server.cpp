#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "net.h"


const int PORT = 8080; // порт для прослушивания
const char* HOST = "127.0.0.1"; // адрес для прослушивания (локальный хост)



int main(){
    int server_id = create_socket();
    settings_server_socket(server_id, PORT, 5);
    int client_id = accpet_client(server_id);
    
    float answer = 0.0f;

    

    while (true){
        reciv_data(client_id, answer);
        
        if(answer == 0){
            std::cout << "Клиент отключился\n";
            break;
        }

        if (answer < 0) {
            perror("read error");
            break;
        }

        std::cout << "Получено (" << answer << " байт): data=" << answer << "\n";

        std::string response = "Сервер получил: data=" + std::to_string(answer);
        send(client_id, response.c_str(), response.size(), 0);


    }

    close(client_id);  // закрыть соединение с клиентом
    close(server_id);

    return 0;
}