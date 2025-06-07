#include "gtest/gtest.h"
#include "base/file_util.h"
#include "base/skiplist.h"
#include "base/random.h"

using namespace zydb::base;

class SkiplistTest : public testing::Test {
public:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
};

TEST_F(SkiplistTest, BasicDemo) {
    ScopeTempFile tmp_file(false);
    SkipList skiplist(tmp_file.GetName(), tmp_file.GetName(), tmp_file.GetName());
    EXPECT_TRUE(!skiplist.Find("aaa").Valid());
    EXPECT_TRUE(!skiplist.Find("bbb").Valid());
    EXPECT_TRUE(!skiplist.Find("ccc").Valid());

    // Insert
    skiplist.Insert("aaa", "111");
    EXPECT_EQ(skiplist.GetSize(), 1);
    skiplist.Insert("bbb", "222");
    skiplist.Insert("bbb", "22");
    EXPECT_EQ(skiplist.GetSize(), 2);
    skiplist.Insert("ccc", "333");
    EXPECT_EQ(skiplist.GetSize(), 3);
    EXPECT_EQ(skiplist.Find("aaa").GetKey(), "aaa");
    EXPECT_EQ(skiplist.Find("aaa").GetValue(), "111");
    EXPECT_EQ(skiplist.Find("bbb").GetKey(), "bbb");
    EXPECT_EQ(skiplist.Find("bbb").GetValue(), "222");
    EXPECT_EQ(skiplist.Find("ccc").GetKey(), "ccc");
    EXPECT_EQ(skiplist.Find("ccc").GetValue(), "333");

    // Delete
    skiplist.Remove("bbb");
    EXPECT_EQ(skiplist.GetSize(), 2);
    EXPECT_TRUE(!skiplist.Find("bbb").Valid());
    skiplist.Remove("ccc");
    EXPECT_EQ(skiplist.GetSize(), 1);
    EXPECT_TRUE(!skiplist.Find("ccc").Valid());
    skiplist.Remove("aaa");
    EXPECT_EQ(skiplist.GetSize(), 0);
    EXPECT_TRUE(!skiplist.Find("aaa").Valid());
}

TEST_F(SkiplistTest, MoreData) {
    ScopeTempFile tmp_file(false);
    SkipList skiplist(tmp_file.GetName(), tmp_file.GetName(), tmp_file.GetName());
    std::map<std::string, std::string> str2str;
    Random random;
    for (int i = 0; i < 10000; ++i) {
        auto data = random.GetString(random.GetInt(10, 50));
        str2str.insert({data, data});
        skiplist.Insert(data, data);
        skiplist.Insert(data, data);
    }
    EXPECT_EQ(skiplist.GetSize(), str2str.size());
    for (auto& [key, value] : str2str) {
        auto entry = skiplist.Find(key);
        EXPECT_EQ(entry.GetKey(), value);
    }
    for (int i = 0; i < 10000; ++i) {
        auto key = random.GetString(random.GetInt(30, 50));
        EXPECT_TRUE(!skiplist.Find(key).Valid());
        skiplist.Remove(key);
    }
    for (auto& [key, value] : str2str) skiplist.Remove(key);
    for (auto& [key, value] : str2str) {
        EXPECT_TRUE(!skiplist.Find(key).Valid());
    }
    EXPECT_EQ(skiplist.GetSize(), 0);
}
