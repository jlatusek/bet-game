#include "config.h"
#include "socket.h"
#include "tools.h"

#include <arpa/inet.h>
#include <cstring>
#include <memory>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    auto socket = std::make_unique<io::socket>(AF_INET, SOCK_STREAM, 0);
    socket->bind(SERVER_PORT);
    socket->listen(BEGASEP_NUM_CLIENTS);
    auto client_socket = socket->accept();

    client_socket->send("Hejka naklejka");
    return 0;
}