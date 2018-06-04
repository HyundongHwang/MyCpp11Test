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



TEST(p22, make_pair_first_second)
{
    auto pair = std::make_pair("pi", 3.14);
    EXPECT_TRUE(pair.first == "pi");
    EXPECT_TRUE(pair.second == 3.14);
}



TEST(p23, make_tuple_get)
{
    auto tuple = std::make_tuple("pi", 3, 0.14159, "hello", "world");
    EXPECT_TRUE(std::get<0>(tuple) == "pi");
    EXPECT_TRUE(std::get<1>(tuple) == 3);
    EXPECT_TRUE(std::get<2>(tuple) == 0.14159);
    EXPECT_TRUE(std::get<3>(tuple) == "hello");
    EXPECT_TRUE(std::get<4>(tuple) == "world");
}



class MyClass
{
public:
    MyClass(std::string id)
    {
        _id = id;
        std::cout << "MyClass.new _id : " << _id << std::endl;
    }

    virtual ~MyClass()
    {
        std::cout << "MyClass.~ _id : " << _id << std::endl;
    }

    std::string _id;
};

TEST(p32, make_shared_shared_ptr)
{
    auto obj = std::make_shared<MyClass>("hello");
    EXPECT_TRUE(obj.use_count() == 1);

    {
        auto obj2 = obj;
        EXPECT_TRUE(obj.use_count() == 2);
        EXPECT_TRUE(obj2.use_count() == 2);
    }

    EXPECT_TRUE(obj.use_count() == 1);

    obj.reset();
    EXPECT_TRUE(obj.use_count() == 0);

    MyClass* pRawObj = new MyClass("world");
    obj.reset(pRawObj);
    EXPECT_TRUE(obj.use_count() == 1);
}



TEST(p42, chrono_duration)
{
    auto begin = std::chrono::high_resolution_clock::now();
    std::vector<int> vecOrg(1000000, 1234);
    auto vecCopy = vecOrg;
    auto end = std::chrono::high_resolution_clock::now();
    auto span = end - begin;
    auto spanNanao = span.count();
    std::cout << "spanNanao : " << spanNanao << std::endl;
    auto spanSec = std::chrono::duration<double>(span).count();
    std::cout << "spanSec : " << spanSec << std::endl;
    auto spanMin = spanSec / 60;
    std::cout << "spanMin : " << spanMin << std::endl;
    auto spanHour = spanSec / 3660;
    std::cout << "spanHour : " << spanHour << std::endl;
}


template<typename T>
void hhdPrintVector(std::vector<T> v, std::string title)
{
    std::cout << title << " : { ";

    for (auto item : v)
    {
        std::cout << item << ", ";
    }

    std::cout << "}" << std::endl;
}

template<typename T>
void hhdPrintValue(T var, std::string title)
{
    std::cout << title << " : " << var << endl;
}

TEST(p48, vector)
{
    auto ARR = { 1, 2, 3, 4, 5 };
    std::vector<int> v = ARR;
    hhdPrintVector(v, "v");

    auto v2 = v;
    hhdPrintVector(v2, "v2");

    std::vector<int> v3(v.begin(), v.end());
    hhdPrintVector(v3, "v3");

    std::vector<int> v4 = { 1, 2, 3, 4, 5 };
    hhdPrintVector(v4, "v4");
}