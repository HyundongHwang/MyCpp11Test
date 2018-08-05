#include "stdafx.h"
//
// Created by USER on 2018-07-18.
//

#include "LsPerfCounter.h"

namespace livestreamer {

    LsPerfCounter *LsPerfCounter::_pThis = nullptr;
    std::mutex LsPerfCounter::_mtStatic;


    LsPerfCounter::LsPerfCounter() {
        std::lock_guard<std::mutex> lg(_mtThis);
    }

    LsPerfCounter::~LsPerfCounter() {
        std::lock_guard<std::mutex> lg(_mtThis);

        if (_pThis == nullptr)
            return;

        delete _pThis;
        _pThis = nullptr;
    }

    LsPerfCounter *LsPerfCounter::GetInstance() {
        std::lock_guard<std::mutex> lg(_mtStatic);

        if (_pThis == nullptr) {
            _pThis = new LsPerfCounter();
        }

        return _pThis;
    }

    void LsPerfCounter::AddCallback(
        PFUNC_START_CALLBACK pStartCallback,
        PFUNC_STOP_CALLBACK pStopCallback,
        PFUNC_COUNT_CALLBACK pCountCallback) {
        std::lock_guard<std::mutex> lg(_mtThis);

        if (pStartCallback != nullptr) {
            _vStartCallback.push_back(pStartCallback);
        }

        if (pStopCallback != nullptr) {
            _vStopCallback.push_back(pStopCallback);
        }

        if (pCountCallback != nullptr) {
            _vCountCallback.push_back(pCountCallback);
        }

    }

    void LsPerfCounter::Start(std::string key)
    {
        std::lock_guard<std::mutex> lg(_mtThis);
        auto now = std::chrono::system_clock::now();
        _mapTimeStamp[key] = now;

        for (PFUNC_START_CALLBACK fp : _vStartCallback) {
            fp(key);
        }
    }

    void LsPerfCounter::Stop(std::string key)
    {
        std::lock_guard<std::mutex> lg(_mtThis);
        auto now = std::chrono::system_clock::now();

        auto old = _mapTimeStamp[key];
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - old);

        for (PFUNC_STOP_CALLBACK fp : _vStopCallback) {
            fp(key, duration.count());
        }
    }

    void LsPerfCounter::Count(std::string key)
    {
        std::lock_guard<std::mutex> lg(_mtThis);
        for (PFUNC_COUNT_CALLBACK fp : _vCountCallback) {
            fp(key);
        }
    }
}