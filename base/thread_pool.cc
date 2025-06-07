#include "base/thread_pool.h"

namespace zydb::base {

ThreadPool::ThreadPool(size_t thread_num) : stop_(false) {
    for (int i = 0; i < thread_num; ++i) {
        threads_.emplace_back([this] {
            while (!stop_.load(std::memory_order_acquire) || !task_queue_.Empty()) {
                auto task = task_queue_.Pop();
                if (task) task->Run();
            }
        });
    }
}

void ThreadPool::AddTask(Task* task) {
    if (!stop_.load(std::memory_order_acquire))
    {
        task_queue_.Add(task);
    }
    else
    {
        delete task;
    }
}

void ThreadPool::JoinAll() {
    for (auto& thread : threads_) {
        if (thread.joinable()) thread.join();
    }
}

void ThreadPool::StopAll() {
    stop_.store(true, std::memory_order_release);
    task_queue_.Stop();
    JoinAll();
}

} // namespace zydb::base
