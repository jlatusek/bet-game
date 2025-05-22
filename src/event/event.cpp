#include "../../include/event/event.h"

event::Event::Event(io::FileDescriptor::sptr fd, std::set<EPOLL_EVENTS> events)
    : fd(std::move(fd)), listen_events(std::move(events))
{
}

unsigned int event::Event::get_posix_event() const
{
    unsigned int posix_ev = 0;
    for (const EPOLL_EVENTS ev : listen_events)
    {
        posix_ev |= ev;
    }
    return posix_ev;
}

io::FileDescriptor::sptr event::Event::get_fd() const
{
    return fd;
}
