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
  // 소켓 생성
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (socket_fd) {
    perror("서버 소켓 생성 실패");
    exit(1);  // 문제가 있을시 1을 반환하고 종료
  }

  return 0;
}