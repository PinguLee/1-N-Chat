#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>

#define PORT 3288
#define BUFFER_SIZE 1024

#pragma comment(lib, "ws2_32.lib") 

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    struct sockaddr_in serverAddress;
    char buffer[BUFFER_SIZE] = {0};

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("���� �ʱ�ȭ ����\n");
        exit(1);
    }
    printf("���� �ʱ�ȭ ����\n");

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        printf("���� ���� ����: %d\n", WSAGetLastError());
        WSACleanup();
        exit(1);
    }
    printf("���� ���� ����\n");

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddress.sin_port = htons(PORT);

    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        printf("���� ���� ����: %d\n", WSAGetLastError());
        closesocket(clientSocket);
        WSACleanup();
        exit(1);
    }

    printf("���� ���� ����\n");

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
