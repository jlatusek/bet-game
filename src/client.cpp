#include "config.h"
#include "tools.h"

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

int bet_game(int sockfd)
{
    char buff[32];
    for (;;)
    {
        bzero(buff, sizeof(buff));
        printf("Enter the string : ");
        int n = 0;
        while ((buff[n++] = getchar()) != '\n')
            ;
        write(sockfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("From Server : %s", buff);
        if ((strncmp(buff, "exit", 4)) == 0)
        {
            printf("Client Exit...\n");
            break;
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    struct sockaddr_in server_addr = {};
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    VerifyOrReturnLogErrorMsg(sock_fd != 1, sock_fd, "Socket creation failed");
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(SERVER_PORT);

    int ret = connect(sock_fd, reinterpret_cast<struct sockaddr *>(&server_addr), sizeof(server_addr));
    VerifyOrReturnLogErrorMsg(ret != 0, ret, "Connection to server failed");
}