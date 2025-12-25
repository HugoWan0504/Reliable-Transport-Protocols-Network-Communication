/* Lab 3: Server/client using TCP protocol */
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define MAX_CLIENT 5

int main(int argc, char const *argv[]) {
    int server_sock;
    int client_sock;
    int listening_port;
    struct sockaddr_in server_address;
    int addrlen = sizeof(server_address);

    int bytes_sent;
    char send_buffer[BUFFER_SIZE] = {0};

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
    if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
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

    /* Listen for incoming connections */
    if (listen(server_sock, MAX_CLIENT) < 0) {
        printf("Listen failed\n");
        close(server_sock);
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d\n", listening_port);

    /* Accept client connection */
    if ((client_sock = accept(server_sock, (struct sockaddr *)&server_address, (socklen_t*)&addrlen)) < 0) {
        printf("Accept failed\n");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    printf("Connected to client\n");

    // Continuously prompt server user for input and send to client
    while (1) {
        printf("Enter message to send to client: ");
        fgets(send_buffer, BUFFER_SIZE, stdin);

        /* Send data to client */
        bytes_sent = send(client_sock, send_buffer, strlen(send_buffer), 0);

        /* Check if send failed or client has closed the connection */
        if (bytes_sent <= 0) {
            printf("Client closed the socket\n");
            break;
        }
    }

    close(client_sock);
    close(server_sock);
    return 0;
}
