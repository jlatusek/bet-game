#include "io/socket.h"

#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <sstream>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>


io::Socket::Socket(const int domain, const int type, const int protocol)
    : FileDescriptor(::socket(domain, type, protocol))
{
    if (fd < 0)
    {
        throw std::runtime_error(std::format("Cannot create socket: {}", strerror(errno)));
    }
}

io::Socket::Socket(const int fd) : FileDescriptor(fd)
{
}

io::Socket::~Socket()
{
    ::close(fd);
}

void io::Socket::bind(const int port)
{
    sockaddr_in addr{};

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (::bind(fd, reinterpret_cast<const sockaddr *>(&addr), sizeof(addr)) < 0)
    {
        std::stringstream err;
        err << "Cannot bind acceptor socket: " << strerror(errno);
        throw std::runtime_error(err.str());
    }
}

void io::Socket::listen(const int backlog)
{
    if (::listen(fd, backlog) != 0)
    {
        throw std::runtime_error(std::format("Cannot listen acceptor socket: {}", strerror(errno)));
    }
}

io::Socket::sptr io::Socket::accept()
{
    const int cli_fd = ::accept(fd, nullptr, 0);
    if (cli_fd < 0)
    {
        throw std::runtime_error(std::format("Cannot accept incoming connection: {}", strerror(errno)));
    }

    return std::make_unique<io::Socket>(cli_fd);
}

void io::Socket::connect(const int port, const std::string &cp)
{
    sockaddr_in server_addr{};

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(cp.c_str());
    server_addr.sin_port = htons(port);

    if (::connect(fd, reinterpret_cast<struct sockaddr *>(&server_addr), sizeof(server_addr)) != 0)
    {
        throw std::runtime_error(std::format("Cannot connect acceptor socket: {}", strerror(errno)));
    };
}
