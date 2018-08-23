#include "HhdLog.h"

#if defined(ANDROID) || defined(__ANDROID__)
#include <android/log.h>
#endif

std::mutex HhdLog::_mtxPrint;

void HhdLog::PrintLog(std::string strFilePath, int nLineNum, std::string strFuncName, std::string strFormat, ...)
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