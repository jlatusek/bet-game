#include "config.h"
#include "socket.h"
#include "tools.h"

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <memory>
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
    auto socket = std::make_unique<io::socket>(AF_INET, SOCK_STREAM, 0);
    socket->connect(SERVER_PORT, "127.0.0.1");

    auto msg = socket->receive();
    std::cout << "Received message: " << msg << std::endl;

}