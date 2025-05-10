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
    static constexpr uint READ_BUFF_SIZE = 512;

    virtual ~socket();

    virtual descriptor get_descriptor() const;
    virtual void set_non_blocking() const;
    virtual void bind(int);
    virtual void listen(int) const;
    virtual socket::uptr accept() const;
    virtual void connect(int port, const std::string &cp);

    socket(int, int, int);
    explicit socket(int);

    virtual void send(const std::string &) const;
    virtual std::string receive() const;

    socket(const socket &) = delete;
    socket &operator=(const socket &) = delete;
    socket(socket &&) = delete;
    socket &operator=(socket &&) = delete;

  protected:
    virtual void write(const void *, size_t) const;
    virtual size_t read(void *, size_t) const;

  private:
    int fd;
};

} // namespace io
