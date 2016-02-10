#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <errno.h>
#include <unistd.h>
#include <semaphore.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define BUF_SIZE 256
#define MAX_PENDING 256

void Die(char *str) {
  char *errno_str = strerror(errno);
  
  printf(str);
  printf("errno(%d):\n", errno);
  printf(errno_str);

  //free(errno_str);
  exit(-1);
}

int main(int argc, char **argv) {
  int result;
  char buf[BUF_SIZE];
  struct sockaddr_in server_sock, client_sock;
  int client_sock_fd = -1;
  int server_sock_fd = -1;

  unsigned int clientlen = sizeof(client_sock);
  int received = 0;
  char recv_buf[BUF_SIZE];
  int sent_total_len = 0;
  int sent_cur_len = 0;

  FILE *fp = NULL;
  char *test_file_buf = NULL;
  long test_file_size = 0;

  //server listens on port 3000
  char *local_host_ip = "127.0.0.1";
  uint32_t port = 3000;

  //open the test file
  if((fp = fopen("test_file.html", "r")) != 0) {
    fseek(fp, 0, SEEK_END);
    test_file_size = ftell(fp) + 1;
    test_file_buf = malloc(test_file_size);
    fseek(fp, 0, SEEK_SET);

    fread(test_file_buf, test_file_size, 1, fp);
    fclose(fp);
  }
  else {
    Die("failed to open test file");
  }

  //create a socket to listen for connections
  memset(&server_sock, 0, sizeof(client_sock));
  server_sock.sin_family = AF_INET;
  server_sock.sin_addr.s_addr = inet_addr(local_host_ip);
  server_sock.sin_port = htons(port);

  if((server_sock_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    Die("failed to create socket fd");
  }

  if((result = bind(server_sock_fd, (struct sockaddr *)&server_sock, sizeof(server_sock))) < 0) {
    Die("failed to bind server socket");
  }

  if((result = listen(server_sock_fd, MAX_PENDING)) < 0) {
    Die("Failed to listen on server socket");
  }


  //listen for new connections
  while(1) {
    if((client_sock_fd = accept(server_sock_fd, (struct sockaddr *) &client_sock, &clientlen)) < 0) {
    } else {
      while(received < BUF_SIZE) {
        if((result = recv(client_sock_fd, recv_buf, BUF_SIZE-1,0)) < 0) {
          Die("failed to receive bytes from client");
        }

        received += result;
        recv_buf[result] = '\0';
        if(strcmp(recv_buf, "fetch") == 0) {
          
        } else {
          printf("wrong input from client");
        }

        printf(recv_buf);

        //send back file data to the client
        while(1) {
          if((sent_cur_len = send(client_sock_fd, test_file_buf, test_file_size - sent_total_len, 0)) > 0) {
            sent_total_len += sent_cur_len;
            if(sent_total_len == test_file_size) {
              printf("file transmitted");
              break;
            }
          } else if(sent_cur_len == 0) {
            Die("zero bytes transmitted to client");
          } else {
            printf("file transmitted completed");
            break;
          }
        }

        close(client_sock_fd);
        close(server_sock_fd);
        return 0;
      }
    }
  }
}
