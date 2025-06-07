#pragma once

#include <queue>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <functional>

namespace zydb::base {

struct Task {
public:
    using Func = std::function<void()>;
    Task(Func&& t, bool own = true) : own(own), task(std::move(t)) {}
    ~Task() = default;

    void Run() {
        task();
        if (own) delete this;
    }

private:
    bool own;
    Func task;

    Task(const Task&) = delete;
    Task& operator=(const Task&) = delete;
};

class TaskQueue {
public:
    TaskQueue() = default;
    ~TaskQueue() = default;

    void Add(Task* task);
    Task* Pop();

    bool Empty() const;

    void Stop();

private:
    std::queue<Task*> task_queue_;

    std::atomic<bool> stop_ = false;

    mutable std::mutex mutex_;
    std::condition_variable cv_;

    TaskQueue(const TaskQueue&) = delete;
    TaskQueue& operator=(const TaskQueue&) = delete;
};

} // namespace zydb::base
