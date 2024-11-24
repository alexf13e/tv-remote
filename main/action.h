
#ifndef ACTION_H
#define ACTION_H

#include <chrono>
#include <cstdint>
#include <thread>

#include "IRSignalContainer.h"
#include "IRTransmitter.h"


enum ActionType
{
    REMOTE_SIGNAL,
    DELAY,
    REPEAT
};

struct ActionBase
{
    ActionType type;

    virtual void run() = 0;
};

struct ActionRemoteSignal : ActionBase
{
    IRSignalContainer signal;

    ActionRemoteSignal(std::string code)
    {
        this->type = REMOTE_SIGNAL;
        this->signal = IRSignalContainer(code);
    }

    void run()
    {
        IRTransmitter::transmit(signal.words);
    }
};

struct ActionDelayMilliseconds : ActionBase
{
    std::chrono::milliseconds duration;

    ActionDelayMilliseconds(uint32_t duration_ms)
    {
        this->type = DELAY;
        this->duration = std::chrono::milliseconds(duration);
    }

    void run()
    {
        std::this_thread::sleep_for(duration);
    }
};

struct ActionRepeatIRForMilliseconds : ActionBase
{
    static constexpr std::chrono::milliseconds repeat_interval = std::chrono::milliseconds(100);

    ActionRemoteSignal* actionRemoteSignal;
    uint32_t duration;

    ActionRepeatIRForMilliseconds(ActionRemoteSignal* actionRemoteSignal, uint32_t duration)
    {
        this->type = REPEAT;
        this->actionRemoteSignal = actionRemoteSignal;
        this->duration = duration;
    }

    void run()
    {
        std::chrono::system_clock::time_point t0 = std::chrono::system_clock::now();
        
        auto repeat_period_ended = [&]() {
            std::chrono::system_clock::time_point t1 = std::chrono::system_clock::now();
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
            return ms > this->duration;
        };

        while (!repeat_period_ended())
        {
            IRTransmitter::transmit(actionRemoteSignal->signal.words);
            std::this_thread::sleep_for(repeat_interval);
        }
    }
};

#endif