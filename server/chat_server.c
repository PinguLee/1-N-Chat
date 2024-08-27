#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 3288
#define BUFFER_SIZE 1024

/*
  socket() : socket handler 생성
  close() : socket handle 해제
  bind() : IP 및 Port 할당
  listen() : accept에서 사용될 connection 대기열 설정
  accept() : connection 대기열에서 connection을 가져움
  connect() : 서버로 연결
*/

int main() {
  struct sockaddr_in address;

  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (socket_fd < 0) {
    perror("소켓 생성 실패");
    exit(1);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  if (bind(socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("소켓 바인딩 실패");
    close(socket_fd);
    exit(1);
  }

  if (listen(socket_fd, 3) < 0) {
    perror("소켓 리스닝 실패");
    close(socket_fd);
    exit(EXIT_FAILURE);
  }

  // 클라이언트 연결

  close(socket_fd);
  return 0;
}