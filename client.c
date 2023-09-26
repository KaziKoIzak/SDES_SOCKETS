#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>


int main() {
    // Create a socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Define server address
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);  // Port number
    server_addr.sin_addr.s_addr = inet_addr("0.0.0.0"); // Server IP address

    // Connect to server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error connecting");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // Communication
    const char *message = "Hello, server!";
    send(client_socket, message, strlen(message), 0);

    char buffer[1024];
    recv(client_socket, buffer, sizeof(buffer), 0);
    printf("Received: %s\n", buffer);

    // Clean up
    close(client_socket);

    return 0;
}
