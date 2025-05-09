#pragma once
#include <memory>

namespace io
{

class socket
{
  public:
    typedef std::shared_ptr<socket> sptr;
    typedef std::unique_ptr<socket> uptr;
    typedef int descriptor;

    virtual ~socket();

    virtual descriptor get_descriptor() const;
    virtual void set_non_blocking() const;
    virtual void bind(int);
    virtual void listen(int) const;
    virtual socket::sptr accept() const;

    virtual void send(const std::string &) const = 0;
    virtual void receive(std::string &) const = 0;

  protected:
    socket(int, int, int);
    explicit socket(int);

    virtual void write(const void *, size_t) const;
    virtual size_t read(void *, size_t) const;

  private:
    descriptor fd;
    virtual socket::sptr create_accepted(descriptor) const = 0;

    socket(const socket &) = delete;
    socket &operator=(const socket &) = delete;
    socket(socket &&) = delete;
    socket &operator=(socket &&) = delete;
};

} // namespace io
