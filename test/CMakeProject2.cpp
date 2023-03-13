#include "CMakeProject2.h"

#include "gtest/gtest.h"

TEST(a, b) {
    cout << testy() << endl;
    ASSERT_EQ(1, testy());
    ASSERT_TRUE(true);
}
//int main() {
//    cout << testy() << endl;
//    return 0;
//}