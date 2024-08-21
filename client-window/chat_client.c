#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server;
    char message[BUFFER_SIZE], server_reply[BUFFER_SIZE];
    int recv_size;

    // Winsock 초기화
    WSAStartup(MAKEWORD(2, 2), &wsa);

    // 소켓 생성
    sock = socket(AF_INET, SOCK_STREAM, 0);

    // 서버 주소와 포트 설정
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);

    // 서버에 연결
    connect(sock, (struct sockaddr *)&server, sizeof(server));

    while (1)
    {
        // 사용자로부터 메시지 입력받기
        printf("Enter message: ");
        fgets(message, BUFFER_SIZE, stdin);

        // 서버로 메시지 전송
        send(sock, message, strlen(message), 0);

        // 서버로부터 응답 받기
        recv_size = recv(sock, server_reply, BUFFER_SIZE, 0);
        server_reply[recv_size] = '\0';
        printf("Server reply: %s\n", server_reply);
    }

    // 소켓 종료
    closesocket(sock);
    WSACleanup();

    return 0;
}

