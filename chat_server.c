#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 3288
#define BUFFER_SIZE 1024

int main() {
    int server_fd;
    int new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    char buffer[BUFFER_SIZE] = {0};
    const char *message = "나는 서버야";

    // 소켓 생성
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("소켓 생성 실패");
        exit(1);
    }
    printf("소켓 생성 성공\n");

    // 주소 설정
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 소켓 바인딩
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("소켓 바인딩 실패");
        close(server_fd);
        exit(1);
    }
    printf("소켓 바인딩 성공\n");

    // 클라이언트의 연결을 기다림
    if (listen(server_fd, 3) < 0) {
        perror("소켓 리스닝 실패");
        close(server_fd);
        exit(1);
    }
    printf("소켓 리스닝 성공\n");

    // 클라이언트 연결 수락
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
    if (new_socket < 0) {
        perror("클라이언트 연결 실패");
        close(server_fd);
        exit(1);
    }
    printf("클라이언트 연결 성공\n");

    // 클라이언트와의 연결 종료
    close(new_socket);
    close(server_fd);
    return 0;
}
