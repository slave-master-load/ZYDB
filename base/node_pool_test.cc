#include "gtest/gtest.h"
#include "base/file_util.h"
#include "base/node_pool.h"
#include "base/random.h"

using namespace zydb::base;

class NodePoolTest : public testing::Test {
public:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
};

TEST_F(NodePoolTest, BasicDemo) {
    ScopeTempFile tmp_file(false);
    Random random;
    NodePool node_pool(tmp_file.GetName());

    EXPECT_EQ(node_pool.GetNode(0), nullptr);

    for (int i = 0; i < 10; ++i) {
        auto offset = random.GetInt(0, 100000);
        auto [node_id, node] = node_pool.Alloc();
        EXPECT_EQ(node_id, i);
    }
    EXPECT_EQ(node_pool.GetNode(11), nullptr);

    for (int i = 7; i >= 2; --i) node_pool.Dealloc(i);
    for (int i = 2; i <= 7; ++i) {
        auto [node_id, node] = node_pool.Alloc();
        EXPECT_EQ(node_id, i);
    }
}
