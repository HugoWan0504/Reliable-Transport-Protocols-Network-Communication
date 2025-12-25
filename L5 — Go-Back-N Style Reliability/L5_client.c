#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
  int sock = 0;
  int server_port;
  struct sockaddr_in server_address;
  int addrlen = sizeof(server_address);
  char *server_ip = "127.0.0.1"; /* localhost loopback */

  int bytes_sent, bytes_recv;
  int pkt_num;

  /* Check argument */
  if (argc != 3) {
    printf("Usage: %s <server_port> <number of packets to receive>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  server_port = atoi(argv[1]);
  if (server_port < 1025 || server_port > 65535) {
    printf("Invalid port number (1025-65535)\n");
    exit(EXIT_FAILURE);
  }

  /* Number of packets */
  pkt_num = atoi(argv[2]);
  if (pkt_num <= 0) {
    printf("Invalid number of packets to receive");
    exit(EXIT_FAILURE);
  } else {
    printf("Requesting %d of packets\n", pkt_num);
  }

  /* Creating socket file descriptor */
  if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
    printf("Socket creation error\n");
    exit(EXIT_FAILURE);
  }

  /* Setting up the server address structure */
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(server_port);
  server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); /* local address */

  /* Sending an initial request to server */
  Packet pkt_send;
  pkt_send.flag = PKT_REQ;
  pkt_send.value = pkt_num;

  // const char *hello = "Client hello";
  bytes_sent = sendto(sock, &pkt_send, sizeof(pkt_send), 0,
                      (struct sockaddr *)&server_address, addrlen);
  printf("Sent initial request to server\n");

  /* Continuously receive and print messages from server */
  Packet pkt_recv;
  while (pkt_num > 0) {
    bytes_recv =
        recvfrom(sock, &pkt_recv, sizeof(pkt_recv), 0,
                 (struct sockaddr *)&server_address, (socklen_t *)&addrlen);
    if (bytes_recv < 0) {
      printf("Receive error\n");
      close(sock);
      exit(EXIT_FAILURE);
    }
    if (pkt_recv.flag == PKT_DATA) {
      printf("Recv: %d\n", pkt_recv.value);
      pkt_num--;
      /* Send ACK */
      Packet pkt_ack;
      pkt_ack.flag = PKT_ACK;
      pkt_ack.value = pkt_recv.value;

      // const char *ack = "ACK";
      // sendto(sock, &pkt_ack, sizeof(pkt_ack), 0, (struct sockaddr *)&server_address,
      //     addrlen);
    }
  }
  /* We are done here */
  printf("All packets received\n");

  close(sock);
  return EXIT_SUCCESS;
}
