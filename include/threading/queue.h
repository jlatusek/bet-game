#pragma once
#include <condition_variable>
#include <mutex>
#include <queue>

namespace threading
{

template <typename T> class queue
{
  public:
    queue() = default;

    void push(T &obj)
    {
        std::unique_lock guard(mutex_);
        queue_.push(obj);
        cv.notify_one();
    }

    void push(T &&obj)
    {
        std::unique_lock guard(mutex_);
        queue_.push(obj);
        cv.notify_one();
    }

    T pop()
    {
        std::unique_lock guard(mutex_);
        while (queue_.empty())
        {
            cv.wait(guard);
        }
        T t = queue_.front();
        queue_.pop();
        return t;
    }

  private:
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable cv;
};

} // namespace threading
