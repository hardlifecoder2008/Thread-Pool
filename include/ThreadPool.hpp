#pragma once
#include <vector>
#include <thread>
#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>


class ThreadPool
{
public:
    explicit ThreadPool(size_t n);

    ThreadPool(const ThreadPool &) = delete;

    ThreadPool &operator=(const ThreadPool &) = delete;

    ThreadPool(ThreadPool &&) = delete;

    ThreadPool &operator=(ThreadPool &&) = delete;

    ~ThreadPool();

    template <class F>
    void submit(F &&f);

    void shutdown();
private:
    void worker_loop() noexcept;
  

    std::mutex m_mutex;
    std::condition_variable m_cv_;
    std::queue<std::function<void()>> m_tasks_;
    bool m_stop = false;
    std::vector<std::thread> m_workers_;
};
#include "ThreadPool.tpp"