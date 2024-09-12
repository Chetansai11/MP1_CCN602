#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024
#define PORT 8080

void communicate_with_server(int server_socket) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        printf("Msg to server: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        write(server_socket, buffer, strlen(buffer));

        memset(buffer, 0, BUFFER_SIZE);
        bytes_read = read(server_socket, buffer, sizeof(buffer));
        if (bytes_read <= 0) {
            break;
        }

        printf("Msg from server: %s", buffer);

        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }
}

int main() {
    int server_socket;
    struct sockaddr_in server_addr;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    printf("Socket creation successful.\n");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0) {
        perror("Connection to the server failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    printf("Connected to the server.\n");

    communicate_with_server(server_socket);

    close(server_socket);
    return 0;
}