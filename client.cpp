#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define PORT 8080
#define SERVER_IP "127.0.0.1"
int main() {
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        std::cerr << "Socket creation failed\n";
        return 1;
    }
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address or address not supported\n";
        close(sock_fd);
        return 1;
    }
    if (connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection failed\n";
        close(sock_fd);
        return 1;
    }
    std::cout << "Connected to server successfully!\n";
    const char* message = "Hello Server";
    ssize_t bytes_sent = send(sock_fd, message, strlen(message), 0);
    if (bytes_sent < 0) {
        std::cerr << "Failed to send message\n";
    } else {
        std::cout << "Sent " << bytes_sent << " bytes to server: " << message << std::endl;
    }
    close(sock_fd);
    return 0;
}
