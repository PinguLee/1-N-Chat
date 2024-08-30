#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>

#define PORT 3288
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 16

typedef struct {
    int socket;
    char ip[INET_ADDRSTRLEN];
} Client;

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    fd_set readfds;
    int max_sd, sd, activity;
    char buffer[BUFFER_SIZE];
    Client clients[MAX_CLIENTS];
    int client_count = 0;

    // 클라이언트 배열 초기화
    for (int i = 0; i < MAX_CLIENTS; i++) {
        clients[i].socket = 0;
    }

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("소켓 생성 실패");
        exit(1);
    }
    printf("소켓 생성 성공\n");

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("소켓 바인딩 실패");
        close(server_fd);
        exit(1);
    }
    printf("소켓 바인딩 성공\n");

    if (listen(server_fd, 16) < 0) {
        perror("소켓 리스닝 실패");
        close(server_fd);
        exit(1);
    }
    printf("소켓 리스닝 성공\n");

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);
        max_sd = server_fd;

        for (int i = 0; i < MAX_CLIENTS; i++) {
            sd = clients[i].socket;
            if (sd > 0) {
                FD_SET(sd, &readfds);
            }
            if (sd > max_sd) {
                max_sd = sd;
            }
        }

        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if (activity < 0) {
            perror("select 오류");
            continue;
        }

        if (FD_ISSET(server_fd, &readfds)) {
            new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
            if (new_socket < 0) {
                perror("클라이언트 연결 실패");
                continue;
            }

            // 클라이언트 IP 주소 가져오기
            char client_ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &address.sin_addr, client_ip, sizeof(client_ip));

            printf("클라이언트 연결 성공: %s\n", client_ip);

            if (client_count < MAX_CLIENTS) {
                clients[client_count].socket = new_socket;
                strncpy(clients[client_count].ip, client_ip, sizeof(clients[client_count].ip));
                client_count++;
            } else {
                printf("클라이언트 수 초과\n");
                close(new_socket);
            }
        }

        for (int i = 0; i < MAX_CLIENTS; i++) {
            sd = clients[i].socket;
            if (FD_ISSET(sd, &readfds)) {
                int read_data = read(sd, buffer, BUFFER_SIZE);
                if (read_data <= 0) {
                    close(sd);
                    clients[i].socket = 0;
                } else {
                    buffer[read_data] = '\0';
                    printf("%s: %s\n", clients[i].ip, buffer);

                    for (int j = 0; j < MAX_CLIENTS; j++) {
                        if (clients[j].socket != 0 && clients[j].socket != sd) {
                            send(clients[j].socket, buffer, read_data, 0);
                        }
                    }
                }
            }
        }
    }

    close(server_fd);
    return 0;
}