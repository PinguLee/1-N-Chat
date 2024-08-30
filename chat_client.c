#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>

#define PORT 3288
#define BUFFER_SIZE 1024

#pragma comment(lib, "ws2_32.lib")

// 전역 변수
SOCKET clientSocket;

// 입력을 처리할 스레드 함수
DWORD WINAPI InputThread(LPVOID lpParam) {
    char buffer[BUFFER_SIZE];
    while (1) {
        printf("입력: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        send(clientSocket, buffer, strlen(buffer), 0);
        memset(buffer, 0, BUFFER_SIZE);
    }
    return 0;
}

// 수신을 처리할 스레드 함수
DWORD WINAPI ReceiveThread(LPVOID lpParam) {
    char buffer[BUFFER_SIZE];
    int recv_len;
    while (1) {
        recv_len = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
        if (recv_len == SOCKET_ERROR) {
            printf("수신 오류: %d\n", WSAGetLastError());
            break;
        } else if (recv_len == 0) {
            printf("서버와의 연결이 종료되었습니다.\n");
            break;
        }
        buffer[recv_len] = '\0';
        printf("서버로부터 받은 메시지: %s\n", buffer);
    }
    return 0;
}

int main() {
    WSADATA wsaData;
    HANDLE hInputThread, hReceiveThread;
    DWORD threadId;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("소켓 초기화 실패: %d\n", WSAGetLastError());
        exit(1);
    }
    printf("소켓 초기화 성공\n");

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        printf("소켓 생성 실패: %d\n", WSAGetLastError());
        WSACleanup();
        exit(1);
    }
    printf("소켓 생성 성공\n");

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("172.29.133.231");
    serverAddress.sin_port = htons(PORT);

    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        printf("서버 연결 실패: %d\n", WSAGetLastError());
        closesocket(clientSocket);
        WSACleanup();
        exit(1);
    }
    printf("서버 연결 성공\n");

    // 입력 스레드 생성
    hInputThread = CreateThread(NULL, 0, InputThread, NULL, 0, &threadId);
    if (hInputThread == NULL) {
        printf("입력 스레드 생성 실패: %d\n", GetLastError());
        closesocket(clientSocket);
        WSACleanup();
        exit(1);
    }

    // 수신 스레드 생성
    hReceiveThread = CreateThread(NULL, 0, ReceiveThread, NULL, 0, &threadId);
    if (hReceiveThread == NULL) {
        printf("수신 스레드 생성 실패: %d\n", GetLastError());
        closesocket(clientSocket);
        WSACleanup();
        exit(1);
    }

    // 스레드가 끝날 때까지 기다림
    WaitForSingleObject(hInputThread, INFINITE);
    WaitForSingleObject(hReceiveThread, INFINITE);

    // 자원 정리
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
