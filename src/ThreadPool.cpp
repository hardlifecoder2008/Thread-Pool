#include "ThreadPool.hpp"

explicit ThreadPool::ThreadPool(size_t n)
{
    if (n <= 0)
        std::invalid_argument("ThreadPool size must be n > 0");
    for (size_t i = 0; i < n; ++i)
    {
        m_workers_.emplace_back([this]
                                { worker_loop(); });
    }
}
ThreadPool::~ThreadPool()
{
    shutdown();
}

void ThreadPool::shutdown()
{
    {
        std::lock_guard<std::mutex> lk(m_mutex);
        if (m_stop)
        {
            return;
        }
        m_stop = true;
    }
    m_cv_.notify_all();
    for (auto &w : m_workers_)
    {
        if (w.joinable())
            w.join();
    }
    m_workers_.clear();
}

void ThreadPool::worker_loop()
{
    std::function<void()> task;
    {
        std::unique_lock<std::mutex> lk(m_mutex);
        m_cv_.wait(lk, [this]
                   { return m_stop || !m_tasks_.empty(); });

        if (m_stop && m_tasks_.empty())
        {
            return;
        }
        task = move(m_tasks_.front());
        m_tasks_.pop();
    }
    task();
}
template <class F>
void ThreadPool::submit(F &&f)
{
    {
        std::lock_guard<std::mutex> lk(m_mutex);
        if (m_stop)
            throw std::runtime_error("submit on stopped ThreadPool");
        m_tasks_.emplace(std::forward<F>(f));
    }
    m_cv_.notify_one();
}