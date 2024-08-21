#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h> // 인터넷 프로토콜 관련 라이브러리

#define PORT 8080        // 포트 번호 : 8080
#define BUFFER_SIZE 1024 // 버퍼 크기 : 1024 바이트

int main()
{
  int server_fd, client_fd;       // 서버와 클라이언트를 위한 소켓 파일 디스크립터
  struct sockaddr_in address;     // IPv4 주소 구조체
  char buffer[BUFFER_SIZE] = {0}; // 데이터를 저장할 버퍼, 0으로 초기화
  int addrlen = sizeof(address);  // 주소 길이를 저장할 변수, 나중에 사용

  /* 소켓 생성 */
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  { // AF_INET: IPv4 사용, SOCK_STREAM: TCP 사용, 0: TCP 기본 값 사용
    perror("소켓 생성 실패");
    exit(EXIT_FAILURE);
  }

  /* 주소, 포트 설정 */
  address.sin_family = AF_INET;         // IPv4로 설정
  address.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0과 같은 의미
  address.sin_port = htons(PORT);       // 포트 설정, htons : 호스트 바이트 순서에서 네트워크 바이트 순서로 변환

  // 소켓 바인딩 */

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
  { // 서버 소켓을 특정 IP 주소와 포트에 바인딩
    perror("소켓 바인딩 실패");
    exit(EXIT_FAILURE);
  }

  /* 소켓 리스닝 */
  if (listen(server_fd, 3) < 0)
  { // 클라이언트 연결을 대기 상태로 전환, 백로그 큐의 크기를 3으로 설정 (최대 3개 연결 대기)
    perror("소켓 리스닝 실패");
    exit(EXIT_FAILURE);
  }

  printf("%d\n", PORT); // 서버가 리스닝 중인 포트 번호 출력

  /* 클라이언트 연결 수락 */
  // 클라이언트의 연결 요청을 받아들이고, 새로운 소켓 디스크립터 반환
  // 이 새로운 소켓은 클라이언트와의 데이터 송수신에 사용됨
  if ((client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
  {
    perror("클라이언트 연결 수락 실패");
    exit(EXIT_FAILURE);
  }

  /* 데이터 송수신 루프 */
  while (1)
  {
    int valread = read(client_fd, buffer, BUFFER_SIZE); // 클라이언트로부터 데이터 읽기
    if (valread > 0)
    {                         // 데이터가 있으면
      buffer[valread] = '\0'; // 문자열 끝을 null로 구분
      printf("%s 입력 잘 받았음\n", buffer);
      send(client_fd, buffer, strlen(buffer), 0); // 클라이언트에게 받은 데이터를 다시 전송
    }
  }

  /* 소켓 종료 */
  close(client_fd);
  close(server_fd);

  return 0;
}