#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <signal.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_client(int client_socket, int client_count) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        bytes_read = read(client_socket, buffer, sizeof(buffer));
        if (bytes_read <= 0) {
            break;
        }

        printf("Msg from client (%d): %s\n", client_count, buffer);
        write(client_socket, buffer, bytes_read);

        if (strncmp("exit", buffer, 4) == 0) {
            printf("Server Exit...\n");
            break;
        }
    }

    close(client_socket);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    pid_t childpid;
    int client_count = 0;

    signal(SIGCHLD, SIG_IGN); // Prevent zombie processes

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    printf("Socket creation successful.\n");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    printf("Socket bind successful.\n");

    if (listen(server_socket, 10) < 0) {
        perror("Listen failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d...\n", PORT);

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        printf("Connection accepted from %s:%d.\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        client_count++;

        if ((childpid = fork()) == 0) {
            close(server_socket);
            handle_client(client_socket, client_count);
            exit(0);
        }
        close(client_socket);
    }

    close(server_socket);
    return 0;
}