#include "epoll.h"

#include <cstring>
#include <format>
#include <iostream>
#include <sys/epoll.h>

epoll_event get_epoll_posix_from_event(const event::Event &ev)
{
    epoll_event epoll_ev{};
    epoll_ev.data.fd = ev.getSocket()->get_posix_descriptor();
    epoll_ev.events = ev.get_posix_event();
    return epoll_ev;
}

Epoll::Epoll() : epfd(epoll_create1(0))
{
    if (epfd == -1)
    {
        throw std::runtime_error(std::format("Failed to create epoll: {}", strerror(errno)));
    }
}

void Epoll::add(const event::Event &ev)
{
    const int fd = ev.getSocket()->get_posix_descriptor();
    if (const auto search = registered_events.find(fd); search != registered_events.end())
    {
        throw std::runtime_error(std::format("Events for specified fd: {}, already registered", fd));
    }

    epoll_event event = get_epoll_posix_from_event(ev);
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event) != 0)
    {
        throw std::runtime_error(std::format("epoll_ctl failed during adding entry {}", strerror(errno)));
    }

    registered_events.emplace(fd, ev);
}

void Epoll::modify(const event::Event &ev)
{
    const int fd = ev.getSocket()->get_posix_descriptor();
    if (const auto search = registered_events.find(fd); search == registered_events.end())
    {
        throw std::runtime_error(std::format("Events for specified fd: {}, do not exists", fd));
    }

    epoll_event event_posix = get_epoll_posix_from_event(ev);
    if (epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &event_posix) != 0)
    {
        throw std::runtime_error(std::format("epoll_ctl failed during modifying entry {}", strerror(errno)));
    }

    registered_events[fd].listen_events = ev.listen_events;
}

void Epoll::del(const event::Event &ev)
{
    const int fd = ev.getSocket()->get_posix_descriptor();
    if (const auto search = registered_events.find(fd); search == registered_events.end())
    {
        throw std::runtime_error(std::format("Events for specified fd: {}, do not exists", fd));
    }

    epoll_event event = get_epoll_posix_from_event(ev);
    if (epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &event) != 0)
    {
        throw std::runtime_error(std::format("epoll_ctl failed during deleting entry {}", strerror(errno)));
    }

    registered_events.erase(fd);
}

std::vector<event::Event> Epoll::wait()
{
    std::vector<event::Event> captured_events;
    constexpr int MAX_EVENTS = 1024;
    epoll_event events[MAX_EVENTS] = {};
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);

    if (nfds == -1)
    {
        throw ::std::runtime_error(std::format("epoll_wait failed with error: {}", strerror(errno)));
    }
    for (auto i = 0; i < nfds; ++i)
    {
        captured_events.push_back(registered_events[events[i].data.fd]);
    }
    return captured_events;
}
