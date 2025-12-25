#include "netinet/in.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char const *argv[]) {
  int server_fd, client_socket;
  struct sockaddr_in address;
  int addrlen = sizeof(address);

  // Check argument
  if (argc != 2) {
    printf("Usage: %s <listen_port>\n", argv[0]);
    return -1;
  }

  // Check user input
  int listening_port = atoi(argv[1]);
  if (listening_port < 0) {
    printf("Invalid port number (1-65535)");
    return -1;
  }

  // Creating socket file descriptor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    printf("Socket failed\n");
    exit(EXIT_FAILURE);
  }

  // Setting up the address structure
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(listening_port);

  // Binding socket to the specified IP and port
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    printf("Bind failed\n");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  // Listening for incoming connections
  if (listen(server_fd, 1) < 0) {
    printf("Listen failed\n");
    close(server_fd);
    exit(EXIT_FAILURE);
  }
  printf("Server listening on port %d\n", listening_port);

  // Accept incoming connection
  if ((client_socket = accept(server_fd, (struct sockaddr *)&address,
                              (socklen_t *)&addrlen)) < 0) {
    printf("Accept failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  // Read message from client
  char recv_buffer[BUFFER_SIZE] = {0};
  read(client_socket, recv_buffer, BUFFER_SIZE);
  printf("Message recv from client\n");

  // Send to client
  const char *message = "Hello from server!";

  send(client_socket, message, strlen(message), 0);
  printf("Message sent to client\n");

  close(client_socket);
  close(server_fd);
  return 0;
}
