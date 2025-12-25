/* Lab 3: Server/client using TCP protocol */
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char const *argv[]) {
    int sock = 0;
    struct sockaddr_in server_address;
    int server_port;

    int bytes_recv;
    char recv_buffer[BUFFER_SIZE] = {0};
    char send_buffer[BUFFER_SIZE] = {0};

    /* Check argument */
    if (argc != 2) {
        printf("Usage: %s <server_port>\n", argv[0]);
        return -1;
    }

    /* Check user input port number */
    server_port = atoi(argv[1]);
    if (server_port < 0) {
        printf("Invalid port number (1-65535)\n");
        return -1;
    }

    /* Creating socket file descriptor */
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation error\n");
        return -1;
    }

    /* Setting up the server address structure */
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    /* Convert IPv4 and IPv6 addresses from text to binary form */
    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0) {
        printf("Invalid address/ Address not supported\n");
        return -1;
    }

    /* Connect to the server */
    if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        printf("Connection failed\n");
        return -1;
    }

    printf("Connected to server\n");

    /* Continuously receive and print messages from server */
    while (1) {
        memset(recv_buffer, 0, BUFFER_SIZE);

        /* Recv data from server */
        bytes_recv = recv(sock, recv_buffer, BUFFER_SIZE, 0);

        /* Check if client had closed the connection */
        if (bytes_recv > 0) {
            printf("From server: %s", recv_buffer);
        } else {
            printf("Server closed the socket\n");
            break;
        }
    }

    close(sock);
    return 0;
}
