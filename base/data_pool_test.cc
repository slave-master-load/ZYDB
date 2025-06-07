#include "gtest/gtest.h"
#include "base/file_util.h"
#include "base/data_pool.h"
#include "base/random.h"

using namespace zydb::base;

class DataPoolTest : public testing::Test {
public:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
};

TEST_F(DataPoolTest, BasicDemo) {
    ScopeTempFile tmp_file(false);
    Random random;
    DataPool data_pool(tmp_file.GetName());
    auto data1 = random.GetString(random.GetInt(15, 30));
    auto [offset1, buffer1] = data_pool.Alloc(data1.length());
    memcpy(buffer1, data1.data(), data1.length());
    auto ret_data1 = data_pool.GetData(offset1);
    EXPECT_EQ(data1, ret_data1);
    auto data2 = random.GetString(random.GetInt(15, 30));
    auto [offset2, buffer2] = data_pool.Alloc(data2.length());
    memcpy(buffer2, data2.data(), data2.length());
    auto ret_data2 = data_pool.GetData(offset2);
    EXPECT_EQ(data2, ret_data2);
}
