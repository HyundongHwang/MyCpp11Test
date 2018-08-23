#include "stdafx.h"











#include <string>
#include <thread>
#include <mutex>
#include <iostream>
#include <chrono>
#include <cstdarg>

#define HHDLOG2(szFormat, ...) \
    HhdLog2::PrintLog(__FILE__, __LINE__, __FUNCTION__, szFormat, ##__VA_ARGS__);


class HhdLog2
{
public:
    static void PrintLog(std::string strFilePath, int nLineNum, std::string strFuncName, std::string strFormat, ...);
private:
    static std::mutex _mtxPrint;
};

std::mutex HhdLog2::_mtxPrint;

void HhdLog2::PrintLog(std::string strFilePath, int nLineNum, std::string strFuncName, std::string strFormat, ...)
{
    std::lock_guard<std::mutex> lg(_mtxPrint);

    auto curTime = std::time(nullptr);
    char szCurTime[1024] = {0,};
    std::strftime(szCurTime, sizeof(szCurTime), "%m-%d %H:%M:%S", std::localtime(&curTime));

    auto now = std::chrono::system_clock::now();
    auto ms = (std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000).count();

    auto tid = std::this_thread::get_id();

    char szLog[1024] = { 0, };
    va_list args;
    va_start(args, strFormat);
    vsprintf(szLog, strFormat.c_str(), args);
    va_end(args);
    char szPrettyLog[1024] = { 0, };

    if (strFilePath.empty())
    {
        std::snprintf(
            szPrettyLog,
            1024,
            "%s.%d %d D %s \n",
            szCurTime,
            ms,
            tid,
            szLog
        );
    }
    else
    {
        auto iStart = strFilePath.find_last_of('/');

        if (iStart == std::string::npos)
        {
            iStart = strFilePath.find_last_of('\\');
        }

        auto strFileName = strFilePath.substr(iStart + 1, strFilePath.size() - iStart - 1);

        std::snprintf(
            szPrettyLog,
            1024,
            "%s.%d %d D %s:%d:%s %s \n",
            szCurTime,
            ms,
            tid,
            strFileName.c_str(),
            nLineNum,
            strFuncName.c_str(),
            szLog
        );
    }

    std::cout << szPrettyLog << std::endl;

    #if defined(ANDROID) || defined(__ANDROID__)
        __android_log_write(ANDROID_LOG_DEBUG, "HHDLOG", szPrettyLog);
    #endif
}

TEST(HHDLOG2_test, HHDLOG2_test)
{
    HHDLOG2("HHDLOG2");
}






TEST(HHDLOG_test, HHDLOG_test)
{
    HHDLOG("HHDLOG");
}



TEST(print_cur_time, print_cur_time)
{
    std::time_t t = std::time(nullptr);
    char mbstr[100];
    std::strftime(mbstr, sizeof(mbstr), "%m-%d %H:%M:%S", std::localtime(&t));
    hhdPrintValue(mbstr, "mbstr");
}



class MyStr
{
public:
    MyStr()
    {
        std::cout << "MyStr create" << std::endl;
    }

    virtual ~MyStr()
    {
        std::cout << "MyStr dest" << std::endl;
    }

    std::string str;
};

void func_for_move(std::vector<MyStr> urlList)
{
    std::cout << "func_for_move start" << std::endl;

    auto urlList2 = std::move(urlList);

    for (auto item : urlList2)
    {
        hhdPrintValue(item.str, "item.str");
    }

    std::cout << "func_for_move end" << std::endl;
}

TEST(move, move)
{
    std::vector<MyStr> urlList;
    {
        MyStr ms;
        ms.str = "hello";
        urlList.push_back(ms);
        ms.str = "world";
        urlList.push_back(ms);
    }
    std::cout << "func_for_move call start" << std::endl;
    func_for_move(std::move(urlList));
    std::cout << "func_for_move call end" << std::endl;
}



std::shared_ptr<std::vector<std::string>> urlList2;

void func2(std::shared_ptr<std::vector<std::string>> urlList)
{
    auto cnt = urlList.use_count();
    hhdPrintValue(cnt, "func2 cnt");
    urlList2 = urlList;
    auto cnt2 = urlList.use_count();
    hhdPrintValue(cnt2, "func2 cnt2");
    auto d = 0;
    urlList2.reset();
}


std::shared_ptr<std::vector<std::string>> urlList1;

void func1(std::shared_ptr<std::vector<std::string>> urlList)
{
    auto cnt = urlList.use_count();
    hhdPrintValue(cnt, "cnt");
    urlList1 = urlList;
    auto cnt2 = urlList.use_count();
    hhdPrintValue(cnt2, "cnt2");
    func2(urlList);
    auto cnt3 = urlList.use_count();
    hhdPrintValue(cnt3, "cnt3");
    auto d = 0;
    urlList1.reset();
}


TEST(test, test)
{
    auto urlList = std::make_shared<std::vector<std::string>>();
    urlList->push_back("hello");
    urlList->push_back("world");
    func1(urlList);
    auto cnt = urlList.use_count();
    hhdPrintValue(cnt, "test cnt");
}



TEST(epoch_time, epoch_time)
{
    auto now = std::chrono::system_clock::now();
    auto epoch = now.time_since_epoch().count();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    auto now2 = std::chrono::system_clock::now();
    auto epoch2 = now2.time_since_epoch().count();
    auto diff = epoch2 - epoch;
}



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
    std::tuple<std::string, int, double, std::string, std::string> tuple = std::make_tuple("pi", 3, 0.14159, "hello", "world");
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
        std::cout << __FUNCTION__ << " _id : " << _id << std::endl;
    }

    virtual ~MyClass()
    {
        std::cout << __FUNCTION__ << " _id : " << _id << std::endl;
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



TEST(p49, map_unordered_map)
{
    std::map<std::string, int> m = { {"hello", 5}, {"world", 5}, {"hhd", 3} };
    hhdPrintMap<std::string, int>(m, "m");
    std::unordered_map<std::string, int> m2 = { { "hello", 5 },{ "world", 5 },{ "hhd", 3 } };
    hhdPrintUnorderdMap<std::string, int>(m2, "m2");
    std::unordered_map<std::string, int> m3(m.begin(), m.end());
    hhdPrintUnorderdMap<std::string, int>(m3, "m3");
    auto a1 = m3["hello"];
    auto a2 = m3["hellohello"];
    auto a3 = m3.find("hellohello");
    auto mhello = m.find("hello");
    auto myhhd = m.find("myhhd");
    m3.erase("hellohello");
    const char* str = "12345";
    std::string log(str, str + 3);
}



TEST(p57, array)
{
    std::array<int, 10> arr{ 1, 2, 3, 4, 5 };
    hhdPrintArray(arr, "arr");
}



TEST(p82, stack)
{
    std::stack<int> st;
    st.push(1);
    st.push(2);
    st.push(3);

    hhdPrintValue(st.top(), "st.top()");
    st.pop();

    hhdPrintValue(st.top(), "st.top()");
    st.pop();

    hhdPrintValue(st.top(), "st.top()");
    st.pop();
}



TEST(p82, queue)
{
    std::queue<int> qqq;
    qqq.push(1);
    qqq.push(2);
    qqq.push(3);

    hhdPrintValue(qqq.front(), "qqq.front()");
    qqq.pop();

    hhdPrintValue(qqq.front(), "qqq.front()");
    qqq.pop();

    hhdPrintValue(qqq.front(), "qqq.front()");
    qqq.pop();
}



struct SquareFuncType
{
    void operator() (int& i)
    {
        i = i * i;
    }
};

TEST(p99, func_type_for_each)
{
    std::vector<int> v{ 1, 2, 3, 4, 5 };
    std::for_each(v.begin(), v.end(), SquareFuncType());
    hhdPrintVector(v, "v");
}



TEST(p100, lambda_func)
{
    std::vector<int> v{ 1, 2, 3, 4, 5 };
    std::for_each(v.begin(), v.end(), [](int& i) { i = i * i; });
    hhdPrintVector(v, "v");
}



TEST(p107, algorithm_count_count_if_any_of)
{
    std::string str = "asdfghjklaqwertyazxcvbnFDSAREWQVCXZ";
    hhdPrintValue(std::count(str.begin(), str.end(), 'a'), "count a");
    hhdPrintValue(std::count_if(str.begin(), str.end(), [](char c) { auto res = std::isupper(c); return res; }), "count CAP");
    hhdPrintValue(std::any_of(str.begin(), str.end(), [](char c) { auto res = std::isupper(c); return res; }), "any of CAP");
    hhdPrintValue(std::any_of(str.begin(), str.end(), [](char c) { auto res = (c >= '0' && c <= '9'); return res; }), "any of NUM");
}



TEST(p111, algorithm_search)
{
    std::vector<int> v{ 3, 4, 5, 6, 7, 8, 9 };
    std::vector<int> v2{ 5, 6, 7 };
    std::vector<int> v3{ 5, 6, 7, 9 };
    auto res = std::search(v.begin(), v.end(), v2.begin(), v2.end());
    if (res != v.end())
    {
        auto dist = res - v.begin();
        hhdPrintValue(dist, "dist");
    }

    auto res2 = std::search(v.begin(), v.end(), v3.begin(), v3.end());
    if (res2 != v.end())
    {
        auto dist2 = res2 - v2.begin();
        hhdPrintValue(dist2, "dist2");
    }
}




TEST(p111, algorithm_copy_n_if)
{
    std::vector<int> v{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    std::vector<int> v2(10);
    std::copy(v.begin(), v.end(), v2.begin());
    hhdPrintVector(v2, "v2");

    std::vector<int> v3(10);
    std::copy_n(v.begin(), 3, v3.begin());
    hhdPrintVector(v3, "v3");

    std::vector<int> v4(10);
    std::copy_if(v.begin(), v.end(), v4.begin(), [](int i) { auto res = (i % 2 == 0); return res; });
    hhdPrintVector(v4, "v4");
}



TEST(p112, algorithm_replace_if)
{
    std::vector<int> v{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    std::vector<int> v2(v);
    std::replace(v2.begin(), v2.end(), 5, 500);
    hhdPrintVector(v2, "v2");

    std::vector<int> v3(v);
    std::replace_if(v3.begin(), v3.end(), [](int i) {return (i % 2 == 0); }, 999);
    hhdPrintVector(v3, "v3");
}



TEST(p113, algorithm_remove_if)
{
    std::vector<int> v{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    std::vector<int> v2(v);
    std::remove(v2.begin(), v2.end(), 5);
    hhdPrintVector(v2, "v2");

    std::vector<int> v3(v);
    std::remove_if(v3.begin(), v3.end(), [](int i) {return (i % 2 == 0); });
    hhdPrintVector(v3, "v3");
}




TEST(p119, algorithm_reverse)
{
    std::vector<int> v{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    std::vector<int> v2(v);
    std::reverse(v2.begin(), v2.end());
    hhdPrintVector(v2, "v2");
}




TEST(p120, algorithm_rotate)
{
    std::vector<int> v{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    std::vector<int> v2(v);
    std::rotate(v2.begin(), v2.begin() + 5, v2.end());
    hhdPrintVector(v2, "5 step rotate");
}



TEST(p121, algorithm_random_shuffle)
{
    std::vector<int> v{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    std::vector<int> v2(v);
    std::random_shuffle(v2.begin(), v2.end());
    hhdPrintVector(v2, "random_shuffle");
}



TEST(p126, algorithm_sort)
{
    std::vector<int> v{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3 };

    std::vector<int> v2(v);
    hhdPrintValue(std::is_sorted(v2.begin(), v2.end()), "v2 is sort");
    std::sort(v2.begin(), v2.end());
    hhdPrintVector(v2, "sort");
    hhdPrintValue(std::is_sorted(v2.begin(), v2.end()), "v2 is sort");
}



TEST(p129, algorithm_set)
{
    std::vector<int> v{ 0, 1, 2, 3, 4, 5 };
    std::vector<int> v2{ 3, 4, 5, 6, 7, 8 };

    std::vector<int> vMerge;
    std::merge(v.begin(), v.end(), v2.begin(), v2.end(), std::back_inserter(vMerge));
    hhdPrintVector(vMerge, "merge");

    std::vector<int> vDiff;
    std::set_difference(v.begin(), v.end(), v2.begin(), v2.end(), std::back_inserter(vDiff));
    hhdPrintVector(vDiff, "set_difference");

    std::vector<int> vIntersection;
    std::set_intersection(v.begin(), v.end(), v2.begin(), v2.end(), std::back_inserter(vIntersection));
    hhdPrintVector(vIntersection, "set_intersection");

    std::vector<int> vUnion;
    std::set_union(v.begin(), v.end(), v2.begin(), v2.end(), std::back_inserter(vUnion));
    hhdPrintVector(vUnion, "set_union");
}



TEST(p154, str_find_rfind_first_of_last_of)
{
    std::string str = "hellocppworldcppbravo";
    hhdPrintValue(str.find("cpp"), "str.find(cpp)");
    hhdPrintValue(str.find("csharp"), "str.find(csharp)");
    hhdPrintValue(str.find("csharp") == std::string::npos, "str.find(csharp) == std::string::npos");
    hhdPrintValue(str.rfind("cpp"), "str.rfind(cpp)");
    hhdPrintValue(str.find_first_of("cpp", 0), "str.find_first_of(cpp, 0)");
    hhdPrintValue(str.find_first_of("cpp", 10), "str.find_first_of(cpp, 10)");
}




TEST(p156, str_replace)
{
    std::string str = "hellocpphelloworldcppbravo";
    std::string res = str;
    std::string from = "hello";
    std::string to = "hi";

    auto i = 0;
    while (true)
    {
        i = res.find(from, i);

        if (i == std::string::npos)
            break;

        res.replace(i, from.length(), to);
        i += to.length();
    }

    hhdPrintValue(res, "res");
    EXPECT_TRUE(res.compare("hicpphiworldcppbravo") == 0);
    EXPECT_TRUE(res == "hicpphiworldcppbravo");
}



TEST(p158, to_string_stoi_stod)
{
    auto str = std::to_string(1234);
    hhdPrintValue(str, "str");
    auto str2 = std::to_string(1234.5678);
    hhdPrintValue(str2, "str2");
    auto num = std::stoi("5678");
    hhdPrintValue(num, "num");
    auto num2 = std::stod("-12.34");
    hhdPrintValue(num2, "num2");
}



TEST(p161, regex)
{
    std::string str = "my phone number is 010-1234-5678. call me.";
    std::string regStr = R"(\d\d\d-\d\d\d\d-\d\d\d\d)";
    std::string regStr2 = "(C\\+\\+)";
    std::regex rgx(regStr);
    std::smatch res;
    std::regex_search(str, res, rgx);
    hhdPrintValue(res[0], "res[0]");
}



TEST(p182, getline)
{
    std::string line;
    std::cout << "input one line : ";
    std::getline(std::cin, line);
    std::cout << "this line ... " << line << std::endl;
}



TEST(p183, simple_format)
{
    auto num = 2011;
    std::cout << num << std::endl;
    std::cout << std::hex;
    std::cout << num << std::endl;
    std::cout << std::dec;
    std::cout << num << std::endl;
}


TEST(p185, complex_format)
{
    auto num = -2011.0123456789;
    std::cout << "num : " << num << std::endl;
    std::cout << std::setw(20);
    std::cout << "std::setw(20) : " << num << std::endl;
    std::cout << std::left;
    std::cout << "std::left : " << num << std::endl;

    std::cout << std::fixed;
    std::cout << "std::fixed : " << num << std::endl;
    std::cout << std::setprecision(5);
    std::cout << "std::setprecision(5) : " << num << std::endl;
    std::cout << std::setprecision(10);
    std::cout << "std::setprecision(10) : " << num << std::endl;

    std::cout << std::scientific;
    std::cout << "std::scientific : " << num << std::endl;
    std::cout << std::setprecision(5);
    std::cout << "std::setprecision(5) : " << num << std::endl;
    std::cout << std::setprecision(10);
    std::cout << "std::setprecision(10) : " << num << std::endl;
}




TEST(p186, xxx)
{

}












