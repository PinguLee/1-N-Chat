#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
  int server_fd, client_fd;
  struct sockaddr_in address;
  char buffer[BUFFER_SIZE] = {0};
  int addrlen = sizeof(address);

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
    perror("소켓 생성 실패");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
  {
    perror("소켓 바인딩 실패");
    exit(EXIT_FAILURE);
  }

  if (listen(server_fd, 3) < 0)
  {
    perror("소켓 리스닝 실패");
    exit(EXIT_FAILURE);
  }

  printf("%d\n", PORT);

  if ((client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
  {
    perror("클라이언트 연결 수락 실패");
    exit(EXIT_FAILURE);
  }

  while (1)
  {
    int valread = read(client_fd, buffer, BUFFER_SIZE);
    if (valread > 0)
    {
      buffer[valread] = '\0';
      printf("%s 입력 잘 받았음\n", buffer);
      send(client_fd, buffer, strlen(buffer), 0);
    }
  }

  close(client_fd);
  close(server_fd);

  return 0;
}