#include "gtest/gtest.h"
#include "base/file_util.h"

using namespace zydb::base;

class FileUtilTest : public testing::Test {
public:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
};

TEST_F(FileUtilTest, BasicDemo) {
    std::string_view filename = "/tmp/test.txt";
    EXPECT_EQ(FileUtil::Exists(filename), false);
    FileUtil::CreateFile(filename);
    EXPECT_EQ(FileUtil::Exists(filename), true);
    FileUtil::DeleteFile(filename);
    EXPECT_EQ(FileUtil::Exists(filename), false);
    EXPECT_EQ(FileUtil::GetParentPath(filename), "/tmp");

    {
        ScopeTempFile file;
        EXPECT_EQ(FileUtil::Exists(file.GetName()), true);
    }
}
