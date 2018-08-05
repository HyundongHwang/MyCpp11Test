#include "stdafx.h"


std::mutex _gMutex;
std::condition_variable _gCv;

void threadFunc()
{
    std::cout << "do work ..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::cout << "work finish !!!" << std::endl;
    _gCv.notify_all();
    std::cout << "thread func end !!!" << std::endl;
}

TEST(thread, conditional_varialbe_wait)
{
    std::cout << "start thread ..." << std::endl;
    std::thread t(threadFunc);
    std::unique_lock<std::mutex> lock(_gMutex);
    std::cout << "wait ..." << std::endl;
    _gCv.wait(lock);

    std::cout << "thread join ..." << std::endl;
    t.join();

    std::cout << "thread join !!! " << std::endl;
}

