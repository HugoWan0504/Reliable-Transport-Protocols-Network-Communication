#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUFFERSIZE 1024

/* Value for Packet.flag */
#define PKT_DATA 0 /* This packet contains data */
#define PKT_ACK 1  /* This packet contains acknowledgement */
#define PKT_REQ 2  /* This packet contains client request */

/* Packet data structure */
typedef struct {
  int flag;
  int value;
} Packet;

int main(int argc, char const *argv[]) {
  int sock;
  int listening_port;
  struct sockaddr_in server_address, client_address;
  int addrlen = sizeof(client_address);

  int bytes_sent, bytes_recv;

  /* Check argument */
  if (argc != 2) {
    printf("Usage: %s <listen_port>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  listening_port = atoi(argv[1]);
  if (listening_port < 1025 || listening_port > 65535) {
    printf("Invalid port number (1025-65535)\n");
    exit(EXIT_FAILURE);
  }

  /* Creating socket file descriptor */
  if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
    printf("Socket creation error\n");
    exit(EXIT_FAILURE);
  }

  /* Setting up the server address structure */
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY;     /* Local address */
  server_address.sin_port = htons(listening_port); /* Port */

  /* Binding the socket */
  if (bind(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
    printf("Bind failed\n");
    close(sock);
    exit(EXIT_FAILURE);
  }

  printf("Server listening on port %d\n", listening_port);

  Packet pkt_recv;
  bytes_recv = recvfrom(sock, &pkt_recv, sizeof(pkt_recv), 0, (struct sockaddr *)&client_address, (socklen_t *)&addrlen);
  if (bytes_recv < 0) {
    printf("Receive error");
    close(sock);
    exit(EXIT_FAILURE);
  }
  printf("Received %d\n", pkt_recv.value);
  int pkt_num = pkt_recv.value;

  /* Start sending to client */
  int cur_seq = 0;
  Packet pkt_tosend;
  pkt_tosend.flag = PKT_DATA;

  while (pkt_num > 0) {
    pkt_tosend.value = cur_seq;
    bytes_sent = sendto(sock, &pkt_tosend, sizeof(pkt_tosend), 0, (struct sockaddr *)&client_address, addrlen);
    if (bytes_sent < 0) {
      printf("Send error");
      close(sock);
      exit(EXIT_FAILURE);
    }
    printf("Sent: %d\n", pkt_tosend.value);

    // Set up select() for non-blocking ACK reception with timeout
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(sock, &read_fds);

    struct timeval timer;
    timer.tv_sec = 2;
    timer.tv_usec = 0;

    int ret = select(sock + 1, &read_fds, NULL, NULL, &timer);
    if (ret == 0) {
      // Timeout occurred
      printf("Timeout occurred, no ACK received for packet %d.\n", cur_seq);
      continue;
    } else if (ret > 0) {
      if (FD_ISSET(sock, &read_fds)) {
        Packet pkt_ack;
        bytes_recv = recvfrom(sock, &pkt_ack, sizeof(pkt_ack), 0, (struct sockaddr *)&client_address, (socklen_t *)&addrlen);
        if (bytes_recv < 0) {
          printf("Receive error\n");
          close(sock);
          exit(EXIT_FAILURE);
        }

        if (pkt_ack.flag == PKT_ACK && pkt_ack.value == cur_seq) {
          printf("Received ACK for packet %d\n", cur_seq);
          cur_seq++;
          pkt_num--;
        } else {
          printf("Invalid ACK received for packet %d\n", cur_seq);
        }
      }
    }
    
    sleep(1); // Simulate some delay if needed
  }

  close(sock);
  return EXIT_SUCCESS;
}
