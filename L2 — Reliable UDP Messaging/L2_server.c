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
    printf("Invalid port number (1024-65535)");
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

  // Get client's IP address and port number
  char client_ip[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &address.sin_addr, client_ip, INET_ADDRSTRLEN);
  int client_port = ntohs(address.sin_port);
  printf("Client info: IP %s, Port %d\n", client_ip, client_port);

  // Read message from client
  char recv_buffer[BUFFER_SIZE] = {0};
  read(client_socket, recv_buffer, BUFFER_SIZE);
  printf("Message from client: %s\n", recv_buffer);

  // Send to client
  char send_buffer[BUFFER_SIZE];
  snprintf(send_buffer, BUFFER_SIZE,
           "Hello from server! Your port number is %d", client_port);

  send(client_socket, send_buffer, strlen(send_buffer), 0);
  printf("Message sent to client: %s\n", send_buffer);

  // Lab 2: Server keep receiving messages from client
  char *acknowledge = "ACK!";
  int bytes_read;
  printf("Begin\n");
  while (1) {
    // TODO your code here
    // Receive message from client
    bytes_read = read(client_socket, recv_buffer, BUFFER_SIZE);
    if (bytes_read <= 0) {
        printf("Client closed the socket\n");
        break;
    }

    printf("Message received from client: %s", recv_buffer);

    // Send acknowledgment to client
    send(client_socket, acknowledge, strlen(acknowledge), 0);
    memset(recv_buffer, 0, BUFFER_SIZE); // Clear the buffer for the next message
    // break;
  }
  close(client_socket);
  close(server_fd);
  return 0;
}
