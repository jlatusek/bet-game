#include "../../include/io/socket.h"
#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <sstream>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

io::socket::socket(const int domain, const int type, const int protocol) : fd(::socket(domain, type, protocol))
{
    if (fd < 0)
    {
        throw std::runtime_error(std::format("Cannot create socket: {}", strerror(errno)));
    }
}

io::socket::socket(const int fd) : fd(fd)
{
}

io::socket::~socket()
{
    ::close(fd);
}
io::socket::descriptor io::socket::get_descriptor() const
{
    return fd;
}
void io::socket::set_non_blocking() const
{
    throw std::runtime_error("Not implemented yet");
}

void io::socket::bind(const int port)
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

void io::socket::listen(const int backlog) const
{
    if (::listen(fd, backlog) != 0)
    {
        throw std::runtime_error(std::format("Cannot listen acceptor socket: {}", strerror(errno)));
    }
}

io::socket::uptr io::socket::accept() const
{
    const int cli_fd = ::accept(fd, nullptr, 0);
    if (cli_fd < 0)
    {
        throw std::runtime_error(std::format("Cannot accept incoming connection: {}", strerror(errno)));
    }

    return std::make_unique<io::socket>(cli_fd);
}
void io::socket::connect(const int port, const std::string &cp)
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
void io::socket::send(const std::string &msg) const
{
    write(msg.c_str(), msg.length());
}
std::string io::socket::receive() const
{
    std::string msg;
    msg.clear();
    char buff[READ_BUFF_SIZE];
    size_t r = 0;
    ::memset(buff, 0, READ_BUFF_SIZE);
    r = read(buff, READ_BUFF_SIZE);
    msg.append(buff, buff + r);
    return msg;
}

size_t io::socket::read(void *buff, const size_t len) const
{
    const ssize_t r = ::read(fd, buff, len);
    if (r < 0)
    {
        throw std::runtime_error(std::format("Cannot read from socket: {}", strerror(errno)));
    }
    else if (0 == r)
    {
        // TODO maybe we should indicate this error in other way?
        throw std::runtime_error("Connection closed by host");
    }

    return r;
}

void io::socket::write(const void *buff, const size_t len) const
{
    if (const size_t w = ::write(fd, buff, len); w < len)
    {
        throw std::runtime_error(std::format("Cannot write to socket: {}", strerror(errno)));
    }
}
