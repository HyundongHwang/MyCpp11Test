#include "stdafx.h"



TEST(p12, chrono_now)
{
    auto now = std::chrono::system_clock::now();

    auto sum = 0;
    for (size_t i = 0; i < 1000000; i++)
    {
        sum += i;
    }

    auto now2 = std::chrono::system_clock::now();
    auto diff = now2 - now;
    std::cout << "diff : " << diff.count() << std::endl;
}



TEST(p16, minmax_pair)
{
    auto NUM_ARR = { -2, 1, 0, 1, 2, 3, 4, 5 };
    auto min = std::min(NUM_ARR);
    EXPECT_TRUE(min == -2);

    auto minmax = std::minmax(NUM_ARR);
    EXPECT_TRUE(minmax.first == -2);
    EXPECT_TRUE(minmax.second == 5);

    auto minmax2 = std::minmax(NUM_ARR, [](int a, int b)
    {
        auto res = std::abs(a) > std::abs(b);
        return res;
    });
    EXPECT_TRUE(minmax2.first == 5);
    EXPECT_TRUE(minmax2.second == 0);
}



TEST(p17, copy_move)
{
    std::vector<int> bigVec(10000000, 2011);
    std::vector<int> vec;

    auto now0 = std::chrono::system_clock::now();
    vec = bigVec;
    auto now1 = std::chrono::system_clock::now();
    auto diff0 = now1 - now0;
    std::cout << "diff0 : " << diff0.count() << std::endl;

    auto now2 = std::chrono::system_clock::now();
    vec = std::move(bigVec);
    auto now3 = std::chrono::system_clock::now();
    auto diff1 = now3 - now2;
    std::cout << "diff1 : " << diff1.count() << std::endl;
}



TEST(p22, make_pair)
{
    auto pair = std::make_pair("pi", 3.14);
    EXPECT_TRUE(pair.first == "pi");
    EXPECT_TRUE(pair.second == 3.14);
}