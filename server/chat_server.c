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
    socket(도메인, 소켓 타입, 프로토콜) => 

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

    프로토콜 {
      0: 기본 프로토콜 (TCP/UDP에 따라 자동 선택)
    }
  */
  struct sockaddr_in address;  // IPv4를 사용하기 위한 구조체

  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (socket_fd) {
    perror("소켓 생성 실패"); /* printf와 달리 errno 값을 참조하여 오류에
                                    대한 설명 자동으로 생성 (errno 는 C에서
                                    시스템 호출, 라이브러리 함수가 실패했을때
                                    오류 정보를 정수로 제공해주는 전역변수)*/
    exit(1);                  /* 문제가 있을시 1을 반환하고 종료*/
  }

  // 주소 구조체 정의
  address.sin_family = AF_INET;  // IPv4로 설정
  address.sin_addr.s_addr =
      INADDR_ANY; /* 모든 네트워크 인터페이스 사용 -> 서버가 여러 네트워크
                  인터페이스를 가지고 있더라도 모든 인터페이스에서 들어오는
                  요청을 받기 위해 (예: 서버가 여러 IP 주소를 가질 때) */
  address.sin_port =
      htons(PORT); /* 호스트 바이트 순서를 네트워크 바이트 순서로 변환 */

  /* bind() 함수는 소켓을 특정 네트워크 주소로 연결한다. (ip, port)
    리턴 (0: 성공, 1: 실패)
    bind(socket_fd, (struct sockaddr *)&address, sizeof(address)):

socket_fd: 소켓 파일 디스크립터로, socket() 호출로 생성된 소켓을 식별합니다.
(struct sockaddr *)&address: 소켓에 바인딩할 주소 정보를 담고 있는 구조체입니다.
address는 struct sockaddr_in 타입의 구조체로, 소켓 주소와 포트를 설정하고
있습니다. struct sockaddr * 타입으로 캐스팅하여 bind() 함수에 전달합니다.
sizeof(address): address 구조체의 크기를 바이트 단위로 제공합니다.

  */
  if (bind(socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("소켓 바인딩 실패");
    close(socket_fd);
    exit(1);
  }

  printf("소켓이 포트 %d에 바인딩 완료\n", PORT);

  close(socket_fd);
  return 0;
}