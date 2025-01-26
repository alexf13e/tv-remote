
#ifndef EVENT_TIMERS_H
#define EVENT_TIMERS_H

//store all timers here so they can all be accessed and stopped before sleeping

#include <unordered_set>

#include "freertos/timers.h"

namespace Timers {
    namespace {
        std::unordered_set<TimerHandle_t> timers;
    }

    void store(const TimerHandle_t handle)
    {
        timers.insert(handle);
    }

    void stop_all()
    {
        for (TimerHandle_t t : timers)
        {
            xTimerStop(t, 0);
        }
    }
}

#endif