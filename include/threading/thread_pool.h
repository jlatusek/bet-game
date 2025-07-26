#pragma once

#include "epoll.h"
#include "event/event.h"
#include "event/event_handler.h"
#include "io/socket.h"
#include "queue.h"

#include <atomic>
#include <functional>
#include <sys/types.h>

namespace threading
{

class thread_pool
{
  public:
    thread_pool(unsigned int workers);
    thread_pool(const thread_pool &) = delete;
    thread_pool &operator=(const thread_pool &) = delete;
    thread_pool(thread_pool &&) = delete;

    void start();
    void stop();
    void register_event_handler(std::shared_ptr<event::event_handler> eh);
    void rearm(std::shared_ptr<event::event_handler> eh);
    void enqueue();

  private:
    std::atomic_bool run_;
    unsigned int workers_;
    queue<std::shared_ptr<event::Event>> task_to_process;
    std::unordered_map<int, std::shared_ptr<event::event_handler>> event_handlers;
    Epoll epoll_;
};

} // namespace threading
