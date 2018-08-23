#pragma once

#include <string>
#include <thread>
#include <mutex>
#include <iostream>
#include <chrono>
#include <cstdarg>


#define HHDLOG(szFormat, ...) \
    HhdLog::PrintLog(__FILE__, __LINE__, __FUNCTION__, szFormat, ##__VA_ARGS__);


class HhdLog
{
public:
    static void PrintLog(std::string strFilePath, int nLineNum, std::string strFuncName, std::string strFormat, ...);
private:
    static std::mutex _mtxPrint;
};