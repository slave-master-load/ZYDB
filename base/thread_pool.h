#pragma once

#include <thread>

#include "base/task_queue.h"

namespace zydb::base {

class ThreadPool {
public:
    ThreadPool(size_t thread_num = 16);
    ~ThreadPool() = default;

    void AddTask(Task* task);

    void JoinAll();
    void StopAll();

private:
    std::atomic<bool> stop_;

    TaskQueue task_queue_;
    std::vector<std::thread> threads_;
};

} // namespace zydb::base
