
#ifndef ACTION_H
#define ACTION_H

#include <chrono>
#include <cstdint>
#include <thread>

#include "IRSignalContainer.h"
#include "IRTransmitter.h"


//////////////// DECLARATIONS //////////////////////////////////////////////////////////////////////////////////////////

enum ActionType
{
    SIGNAL,
    DELAY,
    REPEAT
};

struct ActionBase
{
    ActionType type;

    virtual void run() = 0;
    
    bool compare_type(ActionBase* other);
    virtual bool compare_content(ActionBase* other) = 0;
};

struct ActionRemoteSignal : ActionBase
{
    IRSignalContainer signal;

    ActionRemoteSignal(std::string code);

    void run();
    bool compare_content(ActionBase* other);
};

struct ActionDelayMilliseconds : ActionBase
{
    std::chrono::milliseconds duration;

    ActionDelayMilliseconds(uint32_t duration_ms);

    void run();
    bool compare_content(ActionBase* other);
};

struct ActionRepeatIRForMilliseconds : ActionBase
{
    static constexpr std::chrono::milliseconds repeat_interval = std::chrono::milliseconds(100);

    ActionRemoteSignal* actionRemoteSignal;
    std::chrono::milliseconds duration;

    ActionRepeatIRForMilliseconds(ActionRemoteSignal* actionRemoteSignal, uint32_t duration_ms);

    void run();
    bool compare_content(ActionBase* other);
};



//////////////// DEFINITIONS ///////////////////////////////////////////////////////////////////////////////////////////

bool ActionBase::compare_type(ActionBase* other)
{
    if (this->type == other->type) return true;
    if (this->type == SIGNAL && other->type == REPEAT) return true;
    if (this->type == REPEAT && other->type == SIGNAL) return true;
    return false;
}

ActionRemoteSignal::ActionRemoteSignal(std::string code)
{
    this->type = SIGNAL;
    this->signal = IRSignalContainer(code);
}

void ActionRemoteSignal::run()
{
    IRTransmitter::transmit(signal.words);
}

bool ActionRemoteSignal::compare_content(ActionBase* other)
{
    switch (other->type) {
        case SIGNAL:
            return this == (ActionRemoteSignal*)other;

        case DELAY:
            return false;

        case REPEAT:
            return this == ((ActionRepeatIRForMilliseconds*)other)->actionRemoteSignal;

        default:
            return false;
    }
}


ActionDelayMilliseconds::ActionDelayMilliseconds(uint32_t duration_ms)
{
    this->type = DELAY;
    this->duration = std::chrono::milliseconds(duration_ms);
}

void ActionDelayMilliseconds::run()
{
    std::this_thread::sleep_for(duration);
}

bool ActionDelayMilliseconds::compare_content(ActionBase* other)
{
    switch (other->type) {
        case SIGNAL:
            return false;

        case DELAY:
            return this->duration == ((ActionDelayMilliseconds*)other)->duration;

        case REPEAT:
            return false;

        default:
            return false;
    }
}


ActionRepeatIRForMilliseconds::ActionRepeatIRForMilliseconds(ActionRemoteSignal* actionRemoteSignal, uint32_t duration_ms)
{
    this->type = REPEAT;
    this->actionRemoteSignal = actionRemoteSignal;
    this->duration = std::chrono::milliseconds(duration_ms);
}

void ActionRepeatIRForMilliseconds::run()
{
    std::chrono::system_clock::time_point t0 = std::chrono::system_clock::now();

    while (std::chrono::system_clock::now() - t0 < duration)
    {
        IRTransmitter::transmit(actionRemoteSignal->signal.words);
        std::this_thread::sleep_for(repeat_interval);
    }
}

bool ActionRepeatIRForMilliseconds::compare_content(ActionBase* other)
{
    switch (other->type) {
        case SIGNAL:
            return this->actionRemoteSignal == (ActionRemoteSignal*)other;

        case DELAY:
            return false;

        case REPEAT:
            return this->actionRemoteSignal == ((ActionRepeatIRForMilliseconds*)other)->actionRemoteSignal;
        
        default:
            return false;
    }
}

#endif