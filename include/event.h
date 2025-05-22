#pragma once
#include "io/socket.h"

#include <set>
#include <sys/epoll.h>

class Epoll;

namespace event
{

class Event
{
  public:
    explicit Event(io::FileDescriptor::sptr fd, std::set<EPOLL_EVENTS> events);
    Event() = default;
    [[nodiscard]] unsigned int get_posix_event() const;
    [[nodiscard]] io::FileDescriptor::sptr get_fd() const;
    friend Epoll;

  private:
    std::shared_ptr<io::FileDescriptor> fd;
    std::set<EPOLL_EVENTS> listen_events;
};

} // namespace event
