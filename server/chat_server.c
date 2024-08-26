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
  /*
    socket(도메인, 소켓 타입, 프로토콜)

    도메인 {
      AF_INET: IPv4
      AF_INET6: IPv6
      AF_LOCAL: 로컬 소켓
    }

    소켓 타입 {
      SOCK_STREAM: TCP
      SOCK_DGRAM: UDP
      SOCK_RAW: 원시 소켓 (직접 프로토콜 접근)
    }
  */
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (socket_fd) {
    perror("서버 소켓 생성 실패");
    exit(1);  // 문제가 있을시 1을 반환하고 종료
  }

  printf("소켓 생성 완료");

  // 보류

  close(socket_fd);
  return 0;
}