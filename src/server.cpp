#include "config.h"
#include "epoll.h"
#include "io/socket.h"
#include "threading/thread_pool.h"
#include "tools.h"

#include <arpa/inet.h>
#include <cstring>
#include <memory>
#include <sys/socket.h>
#include <unistd.h>

#include <csignal>
#include <fcntl.h>
#include <memory>
#include <sys/epoll.h>
#include <sys/signalfd.h>

std::shared_ptr<io::FileDescriptor> listen_signal_events(Epoll &);

int main(int argc, char *argv[])
{
    auto socket = std::make_shared<io::Socket>(AF_INET, SOCK_STREAM, 0);
    socket->bind(SERVER_PORT);
    socket->listen(BEGASEP_NUM_CLIENTS);

#define MAX_EVENTS 10
    int listen_sock = socket->get_posix_descriptor();

    Epoll epoll{};

    auto signal_fd = listen_signal_events(epoll);
    threading::thread_pool tp(WORKERS);

    tp.register_event_handler(std::make_shared<event::server_socket_ev_handler>(
        socket, std::make_shared<event::Event>(socket, std::set({EPOLL_EVENTS::EPOLLIN}))));

    tp.register_event_handler(std::make_shared<event::signal_ev_handler>(
        std::make_shared<event::Event>(socket, std::set({EPOLL_EVENTS::EPOLLIN}))));

    for (;;)
    {
        auto captured_events = epoll.wait();
        for (auto &event : captured_events)
        {
            if (event.get_fd()->get_posix_descriptor() == listen_sock)
            {
                auto conn_sock = socket->accept();
                conn_sock->set_non_blocking();
                epoll.add(event::Event(conn_sock, {
                                                      EPOLL_EVENTS::EPOLLIN,
                                                      EPOLL_EVENTS::EPOLLET,
                                                      EPOLL_EVENTS::EPOLLONESHOT,
                                                  }));
            }
            else if (event.get_fd()->get_posix_descriptor() == signal_fd->get_posix_descriptor())
            {
                std::cout << "Keyboard event occurred" << std::endl;
                signalfd_siginfo buff[10];
                while (int r = read(signal_fd->get_posix_descriptor(), buff, sizeof(buff)) != -1)
                {
                    std::cout << buff[0].ssi_signo << " " << std::endl;
                }
                if (errno == EAGAIN)
                {
                    std::cout << "Ales gut" << std::endl;
                }
                else
                {
                    std::cout << "other error" << strerror(errno) << std::endl;
                }
                epoll.modify(event::Event(signal_fd, {
                                                         EPOLL_EVENTS::EPOLLIN,
                                                         EPOLL_EVENTS::EPOLLET,
                                                         EPOLL_EVENTS::EPOLLONESHOT,
                                                     }));
                return 0;
            }
            else
            {
                std::cout << "Event fd:" << event.get_fd()->get_posix_descriptor() << std::endl;
            }
        }
    }

    return 0;
}

io::FileDescriptor::sptr listen_signal_events(Epoll &epoll)
{
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGTERM);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);
    int r = sigprocmask(SIG_BLOCK, &mask, 0);

    auto signal_fd = std::make_shared<io::FileDescriptor>(signalfd(-1, &mask, O_NONBLOCK));

    epoll.add(event::Event(signal_fd, {
                                          EPOLL_EVENTS::EPOLLIN,
                                          EPOLL_EVENTS::EPOLLET,
                                          EPOLL_EVENTS::EPOLLONESHOT,
                                      }));
    return signal_fd;
}
