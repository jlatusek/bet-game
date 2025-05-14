#include <gtest/gtest.h>

#include <thread>
#include <vector>

#include "queue.h"

class QtestClass
{
  public:
    explicit QtestClass(const unsigned int id) : id(id) {};
    [[nodiscard]] unsigned int getId() const
    {
        return id;
    };

  private:
    unsigned int id;
};

TEST(Queue, add)
{
    queue<int> q;
    for (auto i = 0; i < 10; ++i)
    {
        q.push(i);
    }

    for (auto i = 0; i < 10; ++i)
    {
        int poped = q.pop();
        ASSERT_EQ(i, poped);
    }
}

TEST(Queue, add_no_trival)
{
    queue<QtestClass> q;
    for (auto i = 0; i < 10; ++i)
    {
        q.push(QtestClass(i));
    }

    for (auto i = 0; i < 10; ++i)
    {
        auto poped = q.pop();
        ASSERT_EQ(i, poped.getId());
    }
}

TEST(Queue, ConcurrentAccess)
{
    queue<int> q;
    constexpr int producer_cnt = 4;
    constexpr int consumer_cnt = 4;
    constexpr int items_per_producer = 25;
    std::vector<std::jthread> producers, consumers;
    std::vector<int> results;
    std::mutex results_mutex;

    for (int p = 0; p < producer_cnt; ++p)
    {
        producers.emplace_back([&q, p, items_per_producer]() {
            for (int i = 0; i < items_per_producer; ++i)
            {
                q.push(p * items_per_producer + i);
            }
        });
    }

    for (int c = 0; c < consumer_cnt; ++c)
    {
        consumers.emplace_back([&q, &results, &results_mutex, producer_cnt, items_per_producer]() {
            for (int i = 0; i < (producer_cnt * items_per_producer) / consumer_cnt; ++i)
            {
                int value = q.pop();
                std::lock_guard<std::mutex> lock(results_mutex);
                results.push_back(value);
            }
        });
    }

    for (auto &producer : producers)
    {
        producer.join();
    }
    for (auto &consumer : consumers)
    {
        consumer.join();
    }

    ASSERT_EQ(results.size(), producer_cnt * items_per_producer);

    std::sort(results.begin(), results.end());
    for (size_t i = 0; i < results.size(); ++i)
    {
        ASSERT_EQ(results[i], i);
    }
}