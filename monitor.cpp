#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define HOST "127.0.0.1"

int main() {
    // Get the host and port from environment variables, or use default values
    const char* host_str = getenv("SERVER_HOST");
    const char* port_str = getenv("SERVER_PORT");
    const char* host = (host_str != NULL) ? host_str : HOST;
    int port = (port_str != NULL) ? atoi(port_str) : PORT;

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    // Set up the server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);

    // Convert IP address from text to binary form
    if (inet_pton(AF_INET, host, &server_address.sin_addr) <= 0) {
        std::cerr << "Invalid address" << std::endl;
        close(client_socket);
        return 1;
    }

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        std::cerr << "Connection failed" << std::endl;
        close(client_socket);
        return 1;
    }
    std::cout << "Connected to server" << std::endl;

    char buffer[1024] = {0};
    while (true) {
        int query;
        do {
        std::cout << "Enter the elevator number to be queried or type 0 to closed connection: ";
        std::cin >> query;
        } while(query < 0);

        send(client_socket , std::to_string(query).c_str() , std::to_string(query).length() , 0);
        recv(client_socket , buffer, 1024, 0);
        if(query == 0)
            break;
        std::cout << buffer << std::endl;
        memset(buffer, 0, sizeof(buffer));
    }

    close(client_socket);

    return 0;
}
