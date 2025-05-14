#pragma once
#include <memory>

namespace io
{
class FileDescriptor
{
  public:
    explicit FileDescriptor(int fd);
    virtual ~FileDescriptor() = default;
    typedef std::shared_ptr<FileDescriptor> sptr;
    typedef std::unique_ptr<FileDescriptor> uptr;

    virtual void send(const std::string &);
    [[nodiscard]] virtual std::string receive();
    [[nodiscard]] virtual int get_posix_descriptor() const;
    virtual void set_non_blocking();

    FileDescriptor(const FileDescriptor &) = delete;
    FileDescriptor(FileDescriptor &&) = delete;
    FileDescriptor operator=(const FileDescriptor &) = delete;
    FileDescriptor operator=(FileDescriptor &&) = delete;

  protected:
    static constexpr uint READ_BUFF_SIZE = 512;
    virtual void write(const void *, size_t);
    virtual size_t read(void *, size_t);
    int fd;
};

} // namespace io