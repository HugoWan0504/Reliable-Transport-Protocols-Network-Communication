#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char const *argv[]) {
  int sock = 0;
  struct sockaddr_in serv_addr;
  char recv_buffer[BUFFER_SIZE] = {0};

  const char *message = "Hello from client";

  // Check argument
  if (argc != 2) {
    printf("Usage: %s <server_port>\n", argv[0]);
    return -1;
  }

  // Check user input
  int server_port = atoi(argv[1]);
  if (server_port < 0) {
    printf("Invalid port number (1-65535)");
    return -1;
  }

  // Creating socket file descriptor
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("Socket creation error\n");
    return -1;
  }

  // Setting up the server address structure
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(server_port);
  // Convert IPv4 and IPv6 addresses from text to binary form
  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
    printf("Invalid address/ Address not supported\n");
    return -1;
  }

  // Connect to the server
  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    printf("Connection failed\n");
    return -1;
  }

  // Send message to the server
  send(sock, message, strlen(message), 0);
  printf("Message sent from client: %s\n", message);

  // Read response from server
  read(sock, recv_buffer, BUFFER_SIZE);
  printf("Message from server: %s\n", recv_buffer);

  // Lab2: Client read from user input
  printf("Begin\n");
  char send_buffer[BUFFER_SIZE] = {0};
  while (1) {
    printf("Enter message to send: ");
    // TODO your code here
    fgets(send_buffer, BUFFER_SIZE, stdin); // Read user input
    send(sock, send_buffer, strlen(send_buffer), 0); // Send input to server

    // Receive response from server
    memset(recv_buffer, 0, BUFFER_SIZE); // Clear the buffer
    read(sock, recv_buffer, BUFFER_SIZE);
    printf("Message from server: %s\n", recv_buffer);
    // break;
  }
  close(sock);
  return 0;
}
