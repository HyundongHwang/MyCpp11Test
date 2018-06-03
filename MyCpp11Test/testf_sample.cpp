#include "stdafx.h"

class testf_sample : public ::testing::Test
{
public:
    testf_sample() {}
    virtual ~testf_sample() {}

    // Sets up the test fixture.
    virtual void SetUp()
    {
        _val = 1000;
    }

    // Tears down the test fixture.
    virtual void TearDown() {}

    int _val = 0;
};



TEST_F(testf_sample, first_success)
{
    EXPECT_TRUE(true);
    EXPECT_TRUE(_val == 1000);
}

TEST_F(testf_sample, second_fail)
{
    //EXPECT_TRUE(false);
}