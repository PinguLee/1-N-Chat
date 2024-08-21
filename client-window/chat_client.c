#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define SERVER_IP "127.0.0.1" // 서버 IP 주소
#define SERVER_PORT 8080      // 서버 포트
#define BUFFER_SIZE 1024      // 서버 버퍼 크기

int main()
{
    WSADATA wsa;                                          // 윈속 데이터를 저장할 구조체
    SOCKET sock;                                          // 소켓 핸들
    struct sockaddr_in server;                            // 서버의 주소 정보를 저장할 구조체
    char message[BUFFER_SIZE], server_reply[BUFFER_SIZE]; // 메시지와 서버 응답을 저장할 버퍼
    int recv_size;                                        // 수신된 데이터 크기를 저장할 변수

    /* Winsock 초기화 */
    WSAStartup(MAKEWORD(2, 2), &wsa); // 버전 2.2 사용, 초기화 성공 시 0 리턴

    /* 소켓 생성 */
    sock = socket(AF_INET, SOCK_STREAM, 0);

    /* 서버 주소와 포트 설정 */
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);

    /* 서버에 연결 */
    connect(sock, (struct sockaddr *)&server, sizeof(server)); // 클라이언트 소켓을 서버의 주소에 연결

    while (1)
    {
        /* 사용자로부터 메시지 입력받기 */
        printf("입력 : ");                  // 메시지 입력 요청
        fgets(message, BUFFER_SIZE, stdin); // 입력된 메시지를 버퍼에 저장

        /* 서버로 메시지 전송 */
        // send: 서버에 메시지를 전송, 메시지 길이를 지정
        send(sock, message, strlen(message), 0);

        /* 서버로부터 응답 받기 */
        recv_size = recv(sock, server_reply, BUFFER_SIZE, 0); // 서버로부터 데이터 수신
        server_reply[recv_size] = '\0';                       // 수신된 데이터의 끝에 null 문자 추가하여 문자열로 처리
        printf("서버 : %s\n", server_reply);                  // 서버의 응답을 출력
    }

    /* 소켓 종료 */
    closesocket(sock);
    WSACleanup();

    return 0;
}
