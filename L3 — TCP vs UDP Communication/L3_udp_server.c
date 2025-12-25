/* Lab 3: Server/client using UDP protocol */
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char const *argv[]) {
    int server_sock;
    int listening_port;
    struct sockaddr_in server_address, client_address;
    int addrlen = sizeof(client_address);

    int bytes_sent;
    char send_buffer[BUFFER_SIZE] = {0};
    char recv_buffer[BUFFER_SIZE] = {0};

    /* Check argument */
    if (argc != 2) {
        printf("Usage: %s <listen_port>\n", argv[0]);
        return -1;
    }

    /* Check user input port number */
    listening_port = atoi(argv[1]);
    if (listening_port < 0) {
        printf("Invalid port number (1024-65535)\n");
        return -1;
    }

    /* Creating socket file descriptor */
    if ((server_sock = socket(AF_INET, SOCK_DGRAM, 0)) == 0) {
        printf("Socket failed\n");
        exit(EXIT_FAILURE);
    }

    /* Setting up the server address structure */
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY; // Allow client from any address
    server_address.sin_port = htons(listening_port);

    /* Bind server socket to listen on an address */
    if (bind(server_sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        printf("Bind failed\n");
        close(server_sock);
        exit(EXIT_FAILURE);
    }
    printf("UDP server listening on port %d\n", listening_port);

    // Wait for the initial "hello" message from client
    printf("Waiting for hello message from client...\n");
    recvfrom(server_sock, recv_buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_address, &addrlen);
    printf("Received from client: %s\n", recv_buffer);

    // Continuously prompt server user for input and send to client
    while (1) {
        printf("Enter message to send to client: ");
        fgets(send_buffer, BUFFER_SIZE, stdin);

        /* Send data to client */
        bytes_sent = sendto(server_sock, send_buffer, strlen(send_buffer), 0, (struct sockaddr *)&client_address, addrlen);

        /* Check if send failed */
        if (bytes_sent <= 0) {
            printf("Failed to send message\n");
            break;
        }
    }

    close(server_sock);
    return 0;
}
