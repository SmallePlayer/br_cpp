#ifndef TOPIC_SERVER_H
#define TOPIC_SERVER_H

#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include <atomic>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

class TopicServer {
public:
    using DataCallback = std::function<void(const std::vector<uint8_t>&)>;

    TopicServer(int port) : port_(port), running_(false) {}
    ~TopicServer() { stop(); }

    void start() {
        running_ = true;
        server_thread_ = std::thread(&TopicServer::run, this);
    }

    void stop() {
        running_ = false;
        close(server_fd_);
        for (auto& t : client_threads_) {
            if (t.joinable()) t.join();
        }
    }

    void publish(const uint8_t* data, size_t size) {
        std::lock_guard<std::mutex> lock(mutex_);
        int32_t net_size = htonl(static_cast<int32_t>(size));
        for (int client_fd : clients_) {
            send(client_fd, &net_size, sizeof(net_size), 0);
            send(client_fd, data, size, 0);
        }
    }

private:
    void run() {
        server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
        int opt = 1;
        setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port_);

        bind(server_fd_, (sockaddr*)&addr, sizeof(addr));
        listen(server_fd_, 10);

        while (running_) {
            sockaddr_in client_addr{};
            socklen_t client_len = sizeof(client_addr);
            int client_fd = accept(server_fd_, (sockaddr*)&client_addr, &client_len);

            if (client_fd < 0) continue;

            std::lock_guard<std::mutex> lock(mutex_);
            clients_.push_back(client_fd);
            client_threads_.emplace_back(&TopicServer::handle_client, this, client_fd);
        }
    }

    void handle_client(int client_fd) {
        while (running_) {
            int32_t size_net;
            int bytes = recv(client_fd, &size_net, sizeof(size_net), MSG_WAITALL);
            if (bytes <= 0) break;

            int32_t size = ntohl(size_net);
            if (size <= 0 || size > 10 * 1024 * 1024) break;

            std::vector<uint8_t> data(size);
            int received = recv(client_fd, data.data(), size, MSG_WAITALL);
            if (received != size) break;
        }

        std::lock_guard<std::mutex> lock(mutex_);
        clients_.erase(
            std::remove(clients_.begin(), clients_.end(), client_fd),
            clients_.end()
        );
        close(client_fd);
    }

    int port_;
    int server_fd_;
    std::atomic<bool> running_;
    std::vector<int> clients_;
    std::vector<std::thread> client_threads_;
    std::thread server_thread_;
    std::mutex mutex_;
};

#endif
