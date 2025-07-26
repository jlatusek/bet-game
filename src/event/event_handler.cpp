#include "event/event_handler.h"

#include "threading/thread_pool.h"

#include <csignal>
#include <cstring>
#include <iostream>
#include <sys/signalfd.h>

event::event_handler::event_handler(const std::shared_ptr<event::Event> &ev) : event_(ev)
{
}

event::server_socket_ev_handler::server_socket_ev_handler(std::shared_ptr<io::Socket> &socket,
                                                          const std::shared_ptr<event::Event> &ev)
    : event_handler(ev), socket_(socket)
{
}
void event::server_socket_ev_handler::handle_event(threading::thread_pool &tp)
{
    auto client = socket_->accept();
}
event::signal_ev_handler::signal_ev_handler(const std::shared_ptr<Event> &ev) : event_handler(ev)
{
}
void event::signal_ev_handler::handle_event(threading::thread_pool &tp)
{
    std::cout << "Signal event occurred" << std::endl;
    signalfd_siginfo tmp_sig[10];
    std::vector<signalfd_siginfo> signals(10);
    while (const int r = read(event_->get_fd()->get_posix_descriptor(), tmp_sig, sizeof(tmp_sig)) != -1)
    {
        for (auto i = 0; i < r; ++i)
        {
            signals.push_back(tmp_sig[i]);
        }
    }
    if (errno == EAGAIN)
    {
        std::cout << "All signal read" << std::endl;
    }
    else
    {
        std::cerr << "Error occurred during processing signals error: " << std::strerror(errno) << std::endl;
    }

    for (const auto &sig : signals)
    {
        if (sig.ssi_signo == SIGTERM || sig.ssi_signo == SIGINT || sig.ssi_signo == SIGUSR1 || sig.ssi_signo == SIGUSR2)
        {
            std::cout << "Event loop stoped" << std::endl;
            tp.stop();
        }
    }
    tp.rearm(*this);
}
