#include <gtest/gtest.h>


using namespace std;


TEST(TestHarris, test_me)
{
  EXPECT_TRUE(false);
}


int main(int argc, char *argv[])
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}