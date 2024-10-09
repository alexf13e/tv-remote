
#ifndef ACTIONLIST_H
#define ACTIONLIST_H

#include <chrono>
#include <thread>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>

#include "hal/rmt_types.h"
#include "remotes.h"
#include "ir_transmitter.h"


#define MS_BETWEEN_ACTIONS 300

bool actionListRunning = false;
std::unordered_map<std::string, std::vector<const RemoteButtonSignal*>> ALL_ACTION_LISTS;


void createActionLists()
{
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

    ALL_ACTION_LISTS["ITV_1"] = {
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

    ALL_ACTION_LISTS["TALKTV"] = {                                  //////////////////////////////////////////////////////////////////////////////////////

    };

    ALL_ACTION_LISTS["FREEVIEW"] = {
        RemoteTV::KEY_1,
        RemoteTV::KEY_0,
        RemoteTV::KEY_0
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
    std::cout << "transmitting " << actionListID << std::endl;
    for (const RemoteButtonSignal* action : ALL_ACTION_LISTS[actionListID]) //action wants to be a copy as it is modified
    {
        //send action to be broadcast by IR
        for (int i = 0; i < action->length; i++)
        {
            std::cout << action->words[i].duration0 << "-" << action->words[i].duration1 << " ";
        }
        std::cout << std::endl;

        IRTransmitter::transmit(action->words, action->length);

        std::this_thread::sleep_for(std::chrono::milliseconds(MS_BETWEEN_ACTIONS));
    }
    std::cout << "finished transmitting" << std::endl;
    actionListRunning = false;
}

#endif