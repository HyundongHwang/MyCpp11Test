//
// Created by USER on 2018-07-18.
//

#pragma once

#include <mutex>
#include <unordered_map>
#include <string>



namespace livestreamer {

    class LsPerfCounter {
    public:
        typedef void(*PFUNC_START_CALLBACK)(std::string key);
        typedef void(*PFUNC_STOP_CALLBACK)(std::string key, long durationMs);
        typedef void(*PFUNC_COUNT_CALLBACK)(std::string key);

        virtual ~LsPerfCounter();
        static LsPerfCounter* GetInstance();

        void AddCallback(
            PFUNC_START_CALLBACK pStartCallback,
            PFUNC_STOP_CALLBACK pStopCallback,
            PFUNC_COUNT_CALLBACK pCountCallback);

        void Start(std::string key);
        void Stop(std::string key);
        void Count(std::string key);

    private:

        LsPerfCounter();

        static LsPerfCounter* _pThis;
        static std::mutex _mtStatic;
        std::mutex _mtThis;
        std::unordered_map<std::string, std::chrono::time_point<std::chrono::system_clock>> _mapTimeStamp;
        std::vector<PFUNC_START_CALLBACK> _vStartCallback;
        std::vector<PFUNC_STOP_CALLBACK> _vStopCallback;
        std::vector<PFUNC_COUNT_CALLBACK> _vCountCallback;
    };
}