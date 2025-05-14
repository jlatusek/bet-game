#pragma once
#include "event.h"

#include <map>
#include <sys/epoll.h>
#include <vector>

class Epoll
{
  public:
    Epoll();
    void add(const event::Event &ev);
    void del(const event::Event &ev);
    void modify(const event::Event &ev);
    std::vector<event::Event> wait();

  private:
    int epfd;
    std::map<int, event::Event> registered_events;
};
