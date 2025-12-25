/* Lab 3: Server/client using UDP protocol */
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
    int addrlen = sizeof(server_address);

    char recv_buffer[BUFFER_SIZE] = {0};
    char send_buffer[BUFFER_SIZE] = "hello from client";

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
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
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

    /* Send initial hello message to the server */
    sendto(sock, send_buffer, strlen(send_buffer), 0, (struct sockaddr *)&server_address, addrlen);
    printf("Hello message sent to server\n");

    /* Continuously receive and print messages from server */
    while (1) {
        memset(recv_buffer, 0, BUFFER_SIZE);

        /* Receive data from server */
        recvfrom(sock, recv_buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server_address, &addrlen);

        /* Print received message */
        printf("From server: %s", recv_buffer);
    }

    close(sock);
    return 0;
}
