#include <gtest/gtest.h>
#include "hysteresis.hpp"

#define GTEST_COUT std::cerr << "[   INFO   ] "

TEST(Hysterese, differentLimits){
    Hysteresis<double> hyst(0, 10, 0);
    double testValues[]     = { 1,10, 9, 8,01,20, 8, 9,10, 7, 6, 8,22};
    double expectedValues[] = { 1,10,10,10,10,20, 8, 8, 8, 7, 6, 6,22};

    for( unsigned int i = 0; i < sizeof(testValues)/sizeof(testValues[0]); i++){
        double actualValue;
        actualValue = hyst.hysteresis(testValues[i]);
        ASSERT_EQ(actualValue, expectedValues[i]);
    }
}

//Runs all tests
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    //testing::GTEST_FLAG(filter) = "*Orito**";
    return RUN_ALL_TESTS();
}