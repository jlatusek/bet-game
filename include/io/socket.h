#pragma once
#include <io/file.h>

#include <memory>

namespace io
{

class Socket : public FileDescriptor
{
  public:
    typedef std::shared_ptr<Socket> sptr;
    typedef std::unique_ptr<Socket> uptr;

    ~Socket() override;

    virtual void bind(int);
    virtual void listen(int);
    [[nodiscard]] virtual sptr accept();
    virtual void connect(int port, const std::string &cp);

    Socket(int, int, int);
    explicit Socket(int);

    Socket(const Socket &) = delete;
    Socket &operator=(const Socket &) = delete;
    Socket(Socket &&) = delete;
    Socket &operator=(Socket &&) = delete;
};

} // namespace io
