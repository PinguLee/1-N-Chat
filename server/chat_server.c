#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

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

int main()
{
  int server_socket = socket(PF_INET, SOCK_STREAM, 0);

  return 0;
}