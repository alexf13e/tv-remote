
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
        this->duration = std::chrono::milliseconds(duration_ms);
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
    std::chrono::milliseconds duration;

    ActionRepeatIRForMilliseconds(ActionRemoteSignal* actionRemoteSignal, uint32_t duration_ms)
    {
        this->type = REPEAT;
        this->actionRemoteSignal = actionRemoteSignal;
        this->duration = std::chrono::milliseconds(duration_ms);
    }

    void run()
    {
        std::chrono::system_clock::time_point t0 = std::chrono::system_clock::now();

        while (std::chrono::system_clock::now() - t0 < duration)
        {
            IRTransmitter::transmit(actionRemoteSignal->signal.words);
            std::this_thread::sleep_for(repeat_interval);
        }
    }
};

#endif