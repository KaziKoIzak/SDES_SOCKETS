#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
    // Create a socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Define server address
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);  // Port number
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the specified address and port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 10) == -1) {
        perror("Error listening");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening...\n");

    // Accept connections
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
    if (client_socket == -1) {
        perror("Error accepting connection");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Communication
    char buffer[1024];
    recv(client_socket, buffer, sizeof(buffer), 0);
    printf("Received: %s\n", buffer);

    const char *response = "Hello, client!";
    send(client_socket, response, strlen(response), 0);

    // Clean up
    close(client_socket);
    close(server_socket);

    return 0;
}