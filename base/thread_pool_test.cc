#include "gtest/gtest.h"
#include "base/thread_pool.h"

using namespace zydb::base;

class ThreadPoolTest : public testing::Test {
public:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
};

TEST_F(ThreadPoolTest, BasicDemo) {
    {
        ThreadPool thread_pool;
        thread_pool.StopAll();
    }

    {
        ThreadPool thread_pool;
        int sum = 0;
        std::mutex mutex;
        for (int i = 0; i < 1000; ++i) {
            auto func = [&mutex, &sum] {
                std::lock_guard lock(mutex);
                sum += 1;
            };
            thread_pool.AddTask(new Task(func));
        }
        thread_pool.StopAll();
        EXPECT_EQ(sum, 1000);
    }
}
