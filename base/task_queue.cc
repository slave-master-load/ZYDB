#include "base/task_queue.h"

namespace zydb::base {

void TaskQueue::Add(Task* task) {
    std::lock_guard lock(mutex_);
    task_queue_.push(task);
    cv_.notify_one();
}

Task* TaskQueue::Pop() {
    std::unique_lock lock(mutex_);
    cv_.wait(lock, [this] {
        return stop_.load(std::memory_order_acquire) || !task_queue_.empty();
    });

    if (task_queue_.empty()) return nullptr;

    auto task = task_queue_.front();
    task_queue_.pop();
    return task;
}

void TaskQueue::Stop() {
    std::lock_guard lock(mutex_);
    stop_.store(true, std::memory_order_release);
    cv_.notify_all();
}

bool TaskQueue::Empty() const {
    std::lock_guard lock(mutex_);
    return task_queue_.empty();
}

} // namespace zydb::base
