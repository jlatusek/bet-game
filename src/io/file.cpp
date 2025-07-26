#include "io/file.h"

#include <cstring>
#include <fcntl.h>
#include <format>
#include <unistd.h>

io::FileDescriptor::FileDescriptor(const int fd) : fd(fd)
{
}

void io::FileDescriptor::send(const std::string &msg)
{
    write(msg.c_str(), msg.length());
}

std::string io::FileDescriptor::receive()
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

int io::FileDescriptor::get_posix_descriptor() const
{
    return fd;
}

void io::FileDescriptor::set_non_blocking()
{
    const int flags = fcntl(fd, F_GETFL, 0);
    if (::fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        throw std::runtime_error(std::format("fcntl error: {}", strerror(errno)));
    }
}
void io::FileDescriptor::write(const void *buff, const size_t len)
{
    if (const size_t w = ::write(fd, buff, len); w < len)
    {
        throw std::runtime_error(std::format("Cannot write to socket: {}", strerror(errno)));
    }
}

size_t io::FileDescriptor::read(void *buff, const size_t len)
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
