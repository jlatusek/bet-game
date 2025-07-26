#pragma once

#include "event.h"

#include "io/socket.h"

namespace threading
{
class thread_pool;
}

namespace event
{

class event_handler
{
  public:
    virtual ~event_handler() = default;
    virtual void handle_event(threading::thread_pool &tp) = 0;
    std::shared_ptr<event::Event> get_event()
    {
        return event_;
    }

  protected:
    event_handler(const std::shared_ptr<event::Event> &ev);
    std::shared_ptr<event::Event> event_;
};

} // namespace event

namespace event
{
class server_socket_ev_handler : public event_handler
{
  public:
    server_socket_ev_handler(std::shared_ptr<io::Socket> &socket, const std::shared_ptr<event::Event> &ev);
    void handle_event(threading::thread_pool &tp) override;

  private:
    std::shared_ptr<io::Socket> socket_;
};

class signal_ev_handler : public event_handler
{
  public:
    explicit signal_ev_handler(const std::shared_ptr<Event> &ev);
    void handle_event(threading::thread_pool &tp) override;

  private:
    std::shared_ptr<io::Socket> socket_;
};
} // namespace event
