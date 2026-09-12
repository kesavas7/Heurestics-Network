#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#define PORT 8080
#define BUFFER_SIZE 1024
int main() {
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        std::cerr << "Failed to create socket\n";
        return 1;
    }
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; 
    server_addr.sin_port = htons(PORT);   
    if (bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Bind failed\n";
        close(listen_fd);
        return 1;
    }
    if (listen(listen_fd, 5) < 0) {
        std::cerr << "Listen failed\n";
        close(listen_fd);
        return 1;
    }
    std::cout << "Server listening on port " << PORT << "...\n";
    sockaddr_in client_addr{};
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd < 0) {
        std::cerr << "Accept failed\n";
        close(listen_fd);
        return 1;
    }
    std::cout << "Client connected!\n";
    char buffer[BUFFER_SIZE] = {0};
    ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        std::cout << "Received from client: " << buffer << std::endl;
    }
    close(client_fd);
    close(listen_fd);
    return 0;
}
