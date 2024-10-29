
#ifndef ACTIONLIST_H
#define ACTIONLIST_H

#include <chrono>
#include <thread>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>

#include "remotes.h"
#include "action.h"


//tv won't respond to the same ir signal being sent too closely together, but forcing all signals to be slower is annoying
#define MS_BETWEEN_DIFFERENT_IR_SIGNALS 100
#define MS_BETWEEN_SAME_IR_SIGNALS 300

#define WAIT_FOR_MS(DURATION) new ActionDelayMilliseconds(DURATION)
#define REPEAT_SIGNAL_FOR_MS(ACTION_REMOTE_SIGNAL, DURATION) new ActionRepeatIRForMilliseconds(ACTION_REMOTE_SIGNAL, DURATION)

std::unordered_map<std::string, std::vector<ActionBase*>> ALL_ACTION_LISTS;
bool actionListRunning = false;



void createActionLists()
{
    //////////////////////////////////////// CHANNELS ////////////////////////////////////////

    ALL_ACTION_LISTS["BBC_ONE_HD"] = {
        RemoteTV::KEY_1,
        RemoteTV::KEY_0,
        RemoteTV::KEY_1
    };

    ALL_ACTION_LISTS["BBC_TWO_HD"] = {
        RemoteTV::KEY_1,
        RemoteTV::KEY_0,
        RemoteTV::KEY_2
    };

    ALL_ACTION_LISTS["BBC_THREE_HD"] = {
        RemoteTV::KEY_1,
        RemoteTV::KEY_0,
        RemoteTV::KEY_7
    };

    ALL_ACTION_LISTS["BBC_FOUR_HD"] = {
        RemoteTV::KEY_1,
        RemoteTV::KEY_0,
        RemoteTV::KEY_6
    };

    ALL_ACTION_LISTS["BBC_NEWS"] = {
        RemoteTV::KEY_2,
        RemoteTV::KEY_3,
        RemoteTV::KEY_1
    };

    ALL_ACTION_LISTS["ITV1_HD"] = {
        RemoteTV::KEY_1,
        RemoteTV::KEY_0,
        RemoteTV::KEY_3
    };

    ALL_ACTION_LISTS["ITV1"] = {
        RemoteTV::KEY_0,
        RemoteTV::KEY_0,
        RemoteTV::KEY_3,
    };

    ALL_ACTION_LISTS["ITV2"] = {
        RemoteTV::KEY_0,
        RemoteTV::KEY_0,
        RemoteTV::KEY_7
    };

    ALL_ACTION_LISTS["ITV3"] = {
        RemoteTV::KEY_0,
        RemoteTV::KEY_1,
        RemoteTV::KEY_0
    };

    ALL_ACTION_LISTS["ITV4"] = {
        RemoteTV::KEY_0,
        RemoteTV::KEY_2,
        RemoteTV::KEY_6
    };

    ALL_ACTION_LISTS["CHANNEL_4_HD"] = {
        RemoteTV::KEY_1,
        RemoteTV::KEY_0,
        RemoteTV::KEY_4
    };

    ALL_ACTION_LISTS["CHANNEL_5_HD"] = {
        RemoteTV::KEY_1,
        RemoteTV::KEY_0,
        RemoteTV::KEY_5
    };

    ALL_ACTION_LISTS["FILM4"] = {
        RemoteTV::KEY_0,
        RemoteTV::KEY_1,
        RemoteTV::KEY_4
    };

    ALL_ACTION_LISTS["MORE4"] = {
        RemoteTV::KEY_0,
        RemoteTV::KEY_1,
        RemoteTV::KEY_8
    };

    ALL_ACTION_LISTS["E4"] = {
        RemoteTV::KEY_0,
        RemoteTV::KEY_1,
        RemoteTV::KEY_3
    };

    ALL_ACTION_LISTS["DAVE"] = {
        RemoteTV::KEY_0,
        RemoteTV::KEY_1,
        RemoteTV::KEY_9
    };

    ALL_ACTION_LISTS["YESTERDAY"] = {
        RemoteTV::KEY_0,
        RemoteTV::KEY_2,
        RemoteTV::KEY_7
    };

    ALL_ACTION_LISTS["QUEST"] = {
        RemoteTV::KEY_0,
        RemoteTV::KEY_1,
        RemoteTV::KEY_2
    };

    ALL_ACTION_LISTS["DMAX"] = {
        RemoteTV::KEY_0,
        RemoteTV::KEY_3,
        RemoteTV::KEY_9
    };

    ALL_ACTION_LISTS["ITV_BE"] = {
        RemoteTV::KEY_0,
        RemoteTV::KEY_2,
        RemoteTV::KEY_8
    };

    ALL_ACTION_LISTS["ITV1+1"] = {
        RemoteTV::KEY_0,
        RemoteTV::KEY_3,
        RemoteTV::KEY_5
    };

    ALL_ACTION_LISTS["ITV2+1"] = {
        RemoteTV::KEY_0,
        RemoteTV::KEY_2,
        RemoteTV::KEY_9
    };

    ALL_ACTION_LISTS["CHANNEL_4+1"] = {
        RemoteTV::KEY_0,
        RemoteTV::KEY_1,
        RemoteTV::KEY_5
    };

    ALL_ACTION_LISTS["CHANNEL_5+1"] = {
        RemoteTV::KEY_0,
        RemoteTV::KEY_3,
        RemoteTV::KEY_8
    };

    ALL_ACTION_LISTS["E4+1"] = {
        RemoteTV::KEY_0,
        RemoteTV::KEY_3,
        RemoteTV::KEY_0
    };

    ALL_ACTION_LISTS["5USA"] = {
        RemoteTV::KEY_0,
        RemoteTV::KEY_2,
        RemoteTV::KEY_1
    };

    ALL_ACTION_LISTS["5STAR"] = {
        RemoteTV::KEY_0,
        RemoteTV::KEY_3,
        RemoteTV::KEY_2
    };

    ALL_ACTION_LISTS["BLAZE"] = {
        RemoteTV::KEY_0,
        RemoteTV::KEY_6,
        RemoteTV::KEY_4
    };

    ALL_ACTION_LISTS["E4_EXTRA"] = {
        RemoteTV::KEY_0,
        RemoteTV::KEY_3,
        RemoteTV::KEY_1
    };

    ALL_ACTION_LISTS["FILM4+1"] = {
        RemoteTV::KEY_0,
        RemoteTV::KEY_4,
        RemoteTV::KEY_7
    };

    ALL_ACTION_LISTS["5SELECT"] = {
        RemoteTV::KEY_0,
        RemoteTV::KEY_4,
        RemoteTV::KEY_6
    };

    ALL_ACTION_LISTS["CBS_REALITY"] = {
        RemoteTV::KEY_0,
        RemoteTV::KEY_6,
        RemoteTV::KEY_7
    };

    ALL_ACTION_LISTS["THATS_TV"] = {
        RemoteTV::KEY_0,
        RemoteTV::KEY_5,
        RemoteTV::KEY_6
    };

    ALL_ACTION_LISTS["4SEVEN"] = {
        RemoteTV::KEY_0,
        RemoteTV::KEY_4,
        RemoteTV::KEY_9
    };

    ALL_ACTION_LISTS["HGTV"] = {
        RemoteTV::KEY_0,
        RemoteTV::KEY_4,
        RemoteTV::KEY_4
    };

    ALL_ACTION_LISTS["REALLY"] = {
        RemoteTV::KEY_0,
        RemoteTV::KEY_1,
        RemoteTV::KEY_7
    };

    ALL_ACTION_LISTS["DRAMA"] = {
        RemoteTV::KEY_0,
        RemoteTV::KEY_2,
        RemoteTV::KEY_0
    };

    ALL_ACTION_LISTS["TALKINGPICTURES_TV"] = {
        RemoteTV::KEY_0,
        RemoteTV::KEY_8,
        RemoteTV::KEY_2
    };

    ALL_ACTION_LISTS["SKY_MIX"] = {
        RemoteTV::KEY_0,
        RemoteTV::KEY_1,
        RemoteTV::KEY_1
    };

    ALL_ACTION_LISTS["SKY_ARTS"] = {
        RemoteTV::KEY_0,
        RemoteTV::KEY_3,
        RemoteTV::KEY_6
    };

    ALL_ACTION_LISTS["GREAT_MOVIES"] = {
        RemoteTV::KEY_0,
        RemoteTV::KEY_3,
        RemoteTV::KEY_4
    };

    ALL_ACTION_LISTS["GREAT_ACTION"] = {
        RemoteTV::KEY_0,
        RemoteTV::KEY_4,
        RemoteTV::KEY_2
    };

    ALL_ACTION_LISTS["5ACTION"] = {
        RemoteTV::KEY_0,
        RemoteTV::KEY_3,
        RemoteTV::KEY_3
    };

    ALL_ACTION_LISTS["SKY_NEWS"] = {
        RemoteTV::KEY_2,
        RemoteTV::KEY_3,
        RemoteTV::KEY_3
    };

    ALL_ACTION_LISTS["QUEST_RED"] = {
        RemoteTV::KEY_0,
        RemoteTV::KEY_4,
        RemoteTV::KEY_0
    };

    ALL_ACTION_LISTS["W"] = {
        RemoteTV::KEY_0,
        RemoteTV::KEY_2,
        RemoteTV::KEY_5
    };

    ALL_ACTION_LISTS["TOGETHER_TV"] = {
        RemoteTV::KEY_0,
        RemoteTV::KEY_8,
        RemoteTV::KEY_3
    };

    ALL_ACTION_LISTS["CHALLENGE"] = {
        RemoteTV::KEY_0,
        RemoteTV::KEY_4,
        RemoteTV::KEY_8
    };

    ALL_ACTION_LISTS["FREEVIEW"] = {
        RemoteTV::KEY_1,
        RemoteTV::KEY_0,
        RemoteTV::KEY_0
    };


    //////////////////////////////////////// TV REMOTE ////////////////////////////////////////

    ALL_ACTION_LISTS["TV_POWER"] = {
        REPEAT_SIGNAL_FOR_MS(RemoteTV::POWER, 500)
    };

    ALL_ACTION_LISTS["TV_INPUT_TV"] = {
        RemoteTV::INPUT_TV
    };

    ALL_ACTION_LISTS["TV_INPUT_AV"] = {
        RemoteTV::INPUT_AV
    };

    ALL_ACTION_LISTS["TV_HOME"] = {
        RemoteTV::HOME
    };

    ALL_ACTION_LISTS["TV_GUIDE"] = {
        RemoteTV::GUIDE
    };

    ALL_ACTION_LISTS["TV_OK"] = {
        RemoteTV::OK
    };

    ALL_ACTION_LISTS["TV_RETURN"] = {
        RemoteTV::RETURN
    };

    ALL_ACTION_LISTS["TV_EXIT"] = {
        RemoteTV::EXIT
    };

    ALL_ACTION_LISTS["TV_INFO"] = {
        RemoteTV::INFO
    };

    ALL_ACTION_LISTS["TV_NAV_LEFT"] = {
        RemoteTV::NAV_LEFT
    };

    ALL_ACTION_LISTS["TV_NAV_RIGHT"] = {
        RemoteTV::NAV_RIGHT
    };

    ALL_ACTION_LISTS["TV_NAV_UP"] = {
        RemoteTV::NAV_UP
    };

    ALL_ACTION_LISTS["TV_NAV_DONW"] = {
        RemoteTV::NAV_DOWN
    };

    ALL_ACTION_LISTS["TV_RED"] = {
        RemoteTV::RED
    };

    ALL_ACTION_LISTS["TV_GREEN"] = {
        RemoteTV::GREEN
    };

    ALL_ACTION_LISTS["TV_YELLOW"] = {
        RemoteTV::YELLOW
    };

    ALL_ACTION_LISTS["TV_BLUE"] = {
        RemoteTV::BLUE
    };

    ALL_ACTION_LISTS["TV_CHAN_UP"] = {
        RemoteTV::CHAN_UP
    };

    ALL_ACTION_LISTS["TV_CHAN_DOWN"] = {
        RemoteTV::CHAN_DOWN
    };


    //////////////////////////////////////// SOUND REMOTE ////////////////////////////////////////

    ALL_ACTION_LISTS["SPEAKERS_POWER"] = {
        RemoteSpeakers::POWER
    };

    ALL_ACTION_LISTS["SPEAKERS_VOL_UP"] = {
        RemoteSpeakers::VOL_UP
    };

    ALL_ACTION_LISTS["SPEAKERS_VOL_DOWN"] = {
        RemoteSpeakers::VOL_DOWN
    };

    ALL_ACTION_LISTS["SPEAKERS_MUTE"] = {
        RemoteSpeakers::VOL_MUTE
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
    const std::vector<ActionBase*>& actionList = ALL_ACTION_LISTS[actionListID];
    for (int i = 0; i < actionList.size(); i++)
    {
        actionList[i]->run();

        //if two IR signals back to back, need a little time between them
        //if they are the same, the tv won't respond unless a bit more time is given
        //if they are different, then can send them more quickly
        //if the next action is a something else, no need to wait

        if (i + 1 < actionList.size() && actionList[i]->type == REMOTE_SIGNAL && actionList[i + 1]->type == REMOTE_SIGNAL)
        {
            //there are two IR signals back to back

            if (actionList[i] == actionList[i + 1])
            {
                //they are the same signal, wait a bit longer
                std::this_thread::sleep_for(std::chrono::milliseconds(MS_BETWEEN_SAME_IR_SIGNALS));
            }
            else
            {
                //they are different, can wait a bit less
                std::this_thread::sleep_for(std::chrono::milliseconds(MS_BETWEEN_DIFFERENT_IR_SIGNALS));
            }
        }
    }
    actionListRunning = false;
}

#endif