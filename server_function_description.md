## 서버에 사용된 함수 설명

### socket : 소켓을 생성하여 소켓 핸들 반환
* **파라미터**
    * `domain`: 주소 체계 (예: AF_INET, AF_INET6)
    * `type`: 소켓 타입 (예: SOCK_STREAM, SOCK_DGRAM)
    * `protocol`: 프로토콜 (일반적으로 0으로 설정하여 시스템에 맡김)
* **리턴**
    * 성공: 소켓 파일 디스크립터 (정수)
    * 실패: -1
* **예제:**
    ```c
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ```
