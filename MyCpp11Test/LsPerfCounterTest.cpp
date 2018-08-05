#include "stdafx.h"
#include "LsPerfCounter.h"

void myStartCallback(std::string key)
{
    std::cout << "myStartCallback key[" << key << "]" << std::endl;
}

void myStopCallback(std::string key, long durationMs)
{
    std::cout << "myStopCallback key[" << key << "]" << " durationMs["<< durationMs << "]ms" << std::endl;
}

void myCountCallback(std::string key)
{
    std::cout << "myCountCallback key[" << key << "]" << std::endl;
}

TEST(LsPerfCounterTest, test)
{
    livestreamer::LsPerfCounter::GetInstance()->AddCallback(myStartCallback, myStopCallback, myCountCallback);
    livestreamer::LsPerfCounter::GetInstance()->Start("FRAME_LOADING_TIME http://naver.com/abc.m4v");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    livestreamer::LsPerfCounter::GetInstance()->Stop("FRAME_LOADING_TIME http://naver.com/abc.m4v");
    livestreamer::LsPerfCounter::GetInstance()->Count("HTTP_RES_404_NOT_FOUND http://naver.com/def.m4v");
    livestreamer::LsPerfCounter::GetInstance()->Count("HTTP_RES_404_NOT_FOUND http://naver.com/def.m4v");
    livestreamer::LsPerfCounter::GetInstance()->Count("HTTP_RES_404_NOT_FOUND http://naver.com/def.m4v");
}

