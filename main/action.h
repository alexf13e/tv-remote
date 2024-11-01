
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
    const IRSignalContainer* signal;

    ActionRemoteSignal(uint64_t code)
    {
        this->type = REMOTE_SIGNAL;
        this->signal = new IRSignalContainer(code);
    }

    void run()
    {
        IRTransmitter::transmit(signal->words);
    }
};

struct ActionDelayMilliseconds : ActionBase
{
    uint32_t duration;

    ActionDelayMilliseconds(uint32_t duration)
    {
        this->type = DELAY;
        this->duration = duration;
    }

    void run()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(duration));
    }
};

struct ActionRepeatIRForMilliseconds : ActionBase
{
    static constexpr uint32_t milliseconds_between_repeats = 100;

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
            IRTransmitter::transmit(actionRemoteSignal->signal->words);
            std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds_between_repeats));
        }
    }
};

#endif