#include <stdio.h>
#include <string.h>

#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <errno.h>
#include <unistd.h>
#include <semaphore.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/in.h>

#define BUF_SIZE 256

int main(int argc, char **argv) {
  int result;
  char buf[BUF_SIZE];
  struct sockaddr_in server_sock, client_sock;
  int client_sock_fd = -1;
  int server_sock_fd = -1;

  //server listens on port 3000
  char *local_host_ip = "127.0.0.1";
  uint32_t port = 3000;

  if((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    Die("failed to create socket");
  }
  

  //create a socket to listen for connections
  memset(&echoserver, 0, sizeof(echoserver));
  echoserver.sin_family = AF_INET;
  echoserver.sin_addr.s_addr = inet_addr(local_host_ip);
  echoserver.sin_port = htons(port);

  if(result = connect(sock, (struct sockaddr *)&echoserver, sizeof(echoserver)) < 0) {
    Die("failed to create listening socket");
  }


  //listen for new connections
  while(1) {
    unsigned int clientlen = sizeof(client_sock);
  }
}
