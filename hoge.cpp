#include <gtest/gtest.h>

int add(int x, int y)
{
    return x + y;
}

TEST(AddTest, Test1)
{
    ASSERT_EQ(2, add(1, 1));
}

TEST(AddTest, Test2)
{
    ASSERT_EQ(3, add(1, 2));
}

TEST(AddTest, Test3)
{
    ASSERT_EQ(4, add(2, 2));
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);

	int ret = RUN_ALL_TESTS();

//	while(1);

	return ret;
}
