
#ifndef ACTIONLIST_H
#define ACTIONLIST_H

#include <chrono>
#include <cstdint>
#include <thread>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>
#include <iostream>

#include "hal/rmt_types.h"
#include "remotes.h"
#include "IRTransmitter.h"


#define MS_BETWEEN_IR_SIGNALS 100


enum ActionType
{
    REMOTE_SIGNAL,
    DELAY,
};

struct AbstractAction
{
    ActionType type;

    virtual void run() = 0;
};

typedef struct ActionRemoteSignal : AbstractAction
{
    const IRSignalContainer* signal;

    ActionRemoteSignal(const IRSignalContainer* signal)
    {
        type = REMOTE_SIGNAL;
        this->signal = signal;
    }

    void run()
    {
        //send signal to be broadcast by IR
        IRTransmitter::transmit(signal->words);
    }
} *RemoteSignal;

typedef struct ActionDelayMilliseconds : AbstractAction
{
    uint32_t duration;

    ActionDelayMilliseconds(uint32_t duration)
    {
        type = DELAY;
        this->duration = duration;
    }

    void run()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(duration));
    }
} *DelayMilliseconds;


std::unordered_map<std::string, std::vector<AbstractAction*>> ALL_ACTION_LISTS;
bool actionListRunning = false;



void createActionLists()
{
    //////////////////////////////////////// CHANNELS ////////////////////////////////////////

    ALL_ACTION_LISTS["BBC_ONE_HD"] = {
        RemoteSignal(RemoteTV::KEY_1),
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_1)
    };

    ALL_ACTION_LISTS["BBC_TWO_HD"] = {
        RemoteSignal(RemoteTV::KEY_1),
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_2)
    };

    ALL_ACTION_LISTS["BBC_THREE_HD"] = {
        RemoteSignal(RemoteTV::KEY_1),
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_7)
    };

    ALL_ACTION_LISTS["BBC_FOUR_HD"] = {
        RemoteSignal(RemoteTV::KEY_1),
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_6)
    };

    ALL_ACTION_LISTS["BBC_NEWS"] = {
        RemoteSignal(RemoteTV::KEY_2),
        RemoteSignal(RemoteTV::KEY_3),
        RemoteSignal(RemoteTV::KEY_1)
    };

    ALL_ACTION_LISTS["ITV1_HD"] = {
        RemoteSignal(RemoteTV::KEY_1),
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_3)
    };

    ALL_ACTION_LISTS["ITV_1"] = {
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_3),
    };

    ALL_ACTION_LISTS["ITV2"] = {
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_7)
    };

    ALL_ACTION_LISTS["ITV3"] = {
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_1),
        RemoteSignal(RemoteTV::KEY_0)
    };

    ALL_ACTION_LISTS["ITV4"] = {
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_2),
        RemoteSignal(RemoteTV::KEY_6)
    };

    ALL_ACTION_LISTS["CHANNEL_4_HD"] = {
        RemoteSignal(RemoteTV::KEY_1),
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_4)
    };

    ALL_ACTION_LISTS["CHANNEL_5_HD"] = {
        RemoteSignal(RemoteTV::KEY_1),
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_5)
    };

    ALL_ACTION_LISTS["FILM4"] = {
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_1),
        RemoteSignal(RemoteTV::KEY_4)
    };

    ALL_ACTION_LISTS["MORE4"] = {
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_1),
        RemoteSignal(RemoteTV::KEY_8)
    };

    ALL_ACTION_LISTS["E4"] = {
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_1),
        RemoteSignal(RemoteTV::KEY_3)
    };

    ALL_ACTION_LISTS["DAVE"] = {
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_1),
        RemoteSignal(RemoteTV::KEY_9)
    };

    ALL_ACTION_LISTS["YESTERDAY"] = {
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_2),
        RemoteSignal(RemoteTV::KEY_7)
    };

    ALL_ACTION_LISTS["QUEST"] = {
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_1),
        RemoteSignal(RemoteTV::KEY_2)
    };

    ALL_ACTION_LISTS["DMAX"] = {
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_3),
        RemoteSignal(RemoteTV::KEY_9)
    };

    ALL_ACTION_LISTS["ITV_BE"] = {
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_2),
        RemoteSignal(RemoteTV::KEY_8)
    };

    ALL_ACTION_LISTS["ITV1+1"] = {
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_3),
        RemoteSignal(RemoteTV::KEY_5)
    };

    ALL_ACTION_LISTS["ITV2+1"] = {
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_2),
        RemoteSignal(RemoteTV::KEY_9)
    };

    ALL_ACTION_LISTS["CHANNEL_4+1"] = {
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_1),
        RemoteSignal(RemoteTV::KEY_5)
    };

    ALL_ACTION_LISTS["CHANNEL_5+1"] = {
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_3),
        RemoteSignal(RemoteTV::KEY_8)
    };

    ALL_ACTION_LISTS["E4+1"] = {
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_3),
        RemoteSignal(RemoteTV::KEY_0)
    };

    ALL_ACTION_LISTS["5USA"] = {
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_2),
        RemoteSignal(RemoteTV::KEY_1)
    };

    ALL_ACTION_LISTS["5STAR"] = {
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_3),
        RemoteSignal(RemoteTV::KEY_2)
    };

    ALL_ACTION_LISTS["BLAZE"] = {
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_6),
        RemoteSignal(RemoteTV::KEY_4)
    };

    ALL_ACTION_LISTS["E4_EXTRA"] = {
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_3),
        RemoteSignal(RemoteTV::KEY_1)
    };

    ALL_ACTION_LISTS["FILM4+1"] = {
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_4),
        RemoteSignal(RemoteTV::KEY_7)
    };

    ALL_ACTION_LISTS["5SELECT"] = {
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_4),
        RemoteSignal(RemoteTV::KEY_6)
    };

    ALL_ACTION_LISTS["CBS_REALITY"] = {
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_6),
        RemoteSignal(RemoteTV::KEY_7)
    };

    ALL_ACTION_LISTS["THATS_TV"] = {
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_5),
        RemoteSignal(RemoteTV::KEY_6)
    };

    ALL_ACTION_LISTS["4SEVEN"] = {
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_4),
        RemoteSignal(RemoteTV::KEY_9)
    };

    ALL_ACTION_LISTS["HGTV"] = {
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_4),
        RemoteSignal(RemoteTV::KEY_4)
    };

    ALL_ACTION_LISTS["REALLY"] = {
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_1),
        RemoteSignal(RemoteTV::KEY_7)
    };

    ALL_ACTION_LISTS["DRAMA"] = {
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_2),
        RemoteSignal(RemoteTV::KEY_0)
    };

    ALL_ACTION_LISTS["TALKINGPICTURES_TV"] = {
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_8),
        RemoteSignal(RemoteTV::KEY_2)
    };

    ALL_ACTION_LISTS["SKY_MIX"] = {
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_1),
        RemoteSignal(RemoteTV::KEY_1)
    };

    ALL_ACTION_LISTS["SKY_ARTS"] = {
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_3),
        RemoteSignal(RemoteTV::KEY_6)
    };

    ALL_ACTION_LISTS["GREAT_MOVIES"] = {
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_3),
        RemoteSignal(RemoteTV::KEY_4)
    };

    ALL_ACTION_LISTS["GREAT_ACTION"] = {
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_4),
        RemoteSignal(RemoteTV::KEY_2)
    };

    ALL_ACTION_LISTS["5ACTION"] = {
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_3),
        RemoteSignal(RemoteTV::KEY_3)
    };

    ALL_ACTION_LISTS["SKY_NEWS"] = {
        RemoteSignal(RemoteTV::KEY_2),
        RemoteSignal(RemoteTV::KEY_3),
        RemoteSignal(RemoteTV::KEY_3)
    };

    ALL_ACTION_LISTS["QUEST_RED"] = {
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_4),
        RemoteSignal(RemoteTV::KEY_0)
    };

    ALL_ACTION_LISTS["W"] = {
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_2),
        RemoteSignal(RemoteTV::KEY_5)
    };

    ALL_ACTION_LISTS["TOGETHER_TV"] = {
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_8),
        RemoteSignal(RemoteTV::KEY_3)
    };

    ALL_ACTION_LISTS["CHALLENGE"] = {
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_4),
        RemoteSignal(RemoteTV::KEY_8)
    };

    ALL_ACTION_LISTS["FREEVIEW"] = {
        RemoteSignal(RemoteTV::KEY_1),
        RemoteSignal(RemoteTV::KEY_0),
        RemoteSignal(RemoteTV::KEY_0)
    };


    //////////////////////////////////////// TV REMOTE ////////////////////////////////////////

    ALL_ACTION_LISTS["TV_POWER"] = {
        RemoteSignal(RemoteTV::POWER)
    };

    ALL_ACTION_LISTS["TV_INPUT_TV"] = {
        RemoteSignal(RemoteTV::INPUT_TV)
    };

    ALL_ACTION_LISTS["TV_INPUT_AV"] = {
        RemoteSignal(RemoteTV::INPUT_AV)
    };

    ALL_ACTION_LISTS["TV_HOME"] = {
        RemoteSignal(RemoteTV::HOME)
    };

    ALL_ACTION_LISTS["TV_GUIDE"] = {
        RemoteSignal(RemoteTV::GUIDE)
    };

    ALL_ACTION_LISTS["TV_OK"] = {
        RemoteSignal(RemoteTV::OK)
    };

    ALL_ACTION_LISTS["TV_RETURN"] = {
        RemoteSignal(RemoteTV::RETURN)
    };

    ALL_ACTION_LISTS["TV_EXIT"] = {
        RemoteSignal(RemoteTV::EXIT)
    };

    ALL_ACTION_LISTS["TV_INFO"] = {
        RemoteSignal(RemoteTV::INFO)
    };

    ALL_ACTION_LISTS["TV_NAV_LEFT"] = {
        RemoteSignal(RemoteTV::NAV_LEFT)
    };

    ALL_ACTION_LISTS["TV_NAV_RIGHT"] = {
        RemoteSignal(RemoteTV::NAV_RIGHT)
    };

    ALL_ACTION_LISTS["TV_NAV_UP"] = {
        RemoteSignal(RemoteTV::NAV_UP)
    };

    ALL_ACTION_LISTS["TV_NAV_DONW"] = {
        RemoteSignal(RemoteTV::NAV_DOWN)
    };

    ALL_ACTION_LISTS["TV_RED"] = {
        RemoteSignal(RemoteTV::RED)
    };

    ALL_ACTION_LISTS["TV_GREEN"] = {
        RemoteSignal(RemoteTV::GREEN)
    };

    ALL_ACTION_LISTS["TV_YELLOW"] = {
        RemoteSignal(RemoteTV::YELLOW)
    };

    ALL_ACTION_LISTS["TV_BLUE"] = {
        RemoteSignal(RemoteTV::BLUE)
    };

    ALL_ACTION_LISTS["TV_CHAN_UP"] = {
        RemoteSignal(RemoteTV::CHAN_UP)
    };

    ALL_ACTION_LISTS["TV_CHAN_DOWN"] = {
        RemoteSignal(RemoteTV::CHAN_DOWN)
    };


    //////////////////////////////////////// SOUND REMOTE ////////////////////////////////////////

    ALL_ACTION_LISTS["SOUND_POWER"] = {
        RemoteSignal(RemoteSpeakers::POWER)
    };

    ALL_ACTION_LISTS["SOUND_VOL_UP"] = {
        RemoteSignal(RemoteSpeakers::VOL_UP)
    };

    ALL_ACTION_LISTS["SOUND_VOL_DOWN"] = {
        RemoteSignal(RemoteSpeakers::VOL_DOWN)
    };

    ALL_ACTION_LISTS["SOUND_VOL_MUTE"] = {
        RemoteSignal(RemoteSpeakers::VOL_MUTE)
    };
}


void runActionList(const std::string& actionListID)
{
    if (actionListRunning) return;

    if (ALL_ACTION_LISTS.find(actionListID) == ALL_ACTION_LISTS.end())
    {
        std::cerr << "No action list exists with id: " << actionListID << std::endl;
        return;
    }

    actionListRunning = true;
    const std::vector<AbstractAction*>& actionList = ALL_ACTION_LISTS[actionListID];
    for (int i = 0; i < actionList.size(); i++)
    {
        actionList[i]->run();

        if (i + 1 < actionList.size() && actionList[i + 1]->type == REMOTE_SIGNAL)
        {
            //if the next action is an IR signal, want to delay a little to give it time to finish sending before
            //the next one starts.
            //but if the next action is a something else, no need to wait (and if its a delay, it would be confusing
            //to wait this extra time on top of the specified delay)

            std::this_thread::sleep_for(std::chrono::milliseconds(MS_BETWEEN_IR_SIGNALS));
        }
    }
    actionListRunning = false;
}

#endif