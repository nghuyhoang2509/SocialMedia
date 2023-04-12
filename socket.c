#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

int main()
{
    WSADATA wsaData;
    SOCKET socket_client;
    struct sockaddr_in server;
    char *message;
    char server_reply[2000];
    int recv_size;

    // Khởi tạo Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("Khong the khoi tao Winsock\n");
        return 1;
    }

    // Tạo socket
    if ((socket_client = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        printf("Khong the tao socket\n");
        return 1;
    }

    // Cấu hình server address và port
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("ws://localhost");
    server.sin_port = htons(9001);

    // Kết nối đến server
    if (connect(socket_client, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        printf("Khong the ket noi den server\n");
        return 1;
    }

    printf("Ket noi thanh cong\n");

    // Gửi message đến server
    message = "GET / HTTP/1.1\r\n\r\n";
    if (send(socket_client, message, strlen(message), 0) < 0)
    {
        printf("Gui message khong thanh cong\n");
        return 1;
    }

    // Nhận phản hồi từ server
    if ((recv_size = recv(socket_client, server_reply, 2000, 0)) == SOCKET_ERROR)
    {
        printf("Nhan phan hoi khong thanh cong\n");
        return 1;
    }

    printf("Phan hoi tu server:\n");
    printf("%s\n", server_reply);

    closesocket(socket_client);
    WSACleanup();
    getchar();
    return 0;
}
