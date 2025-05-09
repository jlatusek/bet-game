#include "config.h"
#include "tools.h"

#include <arpa/inet.h>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>

int main(void)
{
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    VerifyOrReturnLogErrorMsg(server_socket != -1, server_socket, "Failed to create socket");
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    address.sin_addr.s_addr = INADDR_ANY;

    int ret = bind(server_socket, reinterpret_cast<struct sockaddr *>(&address), sizeof(address));
    VerifyOrReturnLogErrorMsg(ret != -1, ret, "Failed to bind socket");

    listen(server_socket, BEGASEP_NUM_CLIENTS);

    int client_socket = accept(server_socket, NULL, NULL);

    char message[] = "Hello, World!";

    send(client_socket, message, strlen(message), 0);

    // Close the client socket
    close(client_socket);

    return 0;
}