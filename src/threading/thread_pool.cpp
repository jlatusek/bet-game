#include "threading/thread_pool.h"

#include "event/event_handler.h"

#include <functional>
#include <thread>
#include <utility>

using namespace threading;

thread_pool::thread_pool(const unsigned int workers) : workers_(workers)
{
}

void thread_pool::start()
{
    for (auto i = 0; i < workers_; ++i)
    {
        std::jthread([this]() {
            while (this->run_)
            {
                const auto event = task_to_process.pop();
                const auto handler = event_handlers[event->get_fd()->get_posix_descriptor()];
                handler->handle_event(*this);
            }
        });
    }

    while (run_)
    {
        for (auto captured_events = epoll_.wait(); auto &event : captured_events)
        {
            task_to_process.push(std::make_shared<event::Event>(event));
        }
    }
}
void thread_pool::stop()
{
    run_ = false;
}

void thread_pool::register_event_handler(std::shared_ptr<event::event_handler> eh)
{
    auto ev = eh->get_event();
    epoll_.add(*ev);
    event_handlers[ev->get_fd()->get_posix_descriptor()] = std::move(eh);
}

void thread_pool::rearm(std::shared_ptr<event::event_handler> eh)
{
    auto ev = eh->get_event();
    epoll_.modify(*ev);
    event_handlers[ev->get_fd()->get_posix_descriptor()] = std::make_shared<event::event_handler>(eh);
}
