
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
bool action_list_running = false;



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
        RemoteTV::KEY_6
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


    //////////////////////////////////////// TV REMOTE SINGLE BUTTONS ////////////////////////////////////////

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

    ALL_ACTION_LISTS["TV_NAV_DOWN"] = {
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

    ALL_ACTION_LISTS["TV_KEY_0"] = {
        RemoteTV::KEY_0
    };

    ALL_ACTION_LISTS["TV_KEY_1"] = {
        RemoteTV::KEY_1
    };

    ALL_ACTION_LISTS["TV_KEY_2"] = {
        RemoteTV::KEY_2
    };

    ALL_ACTION_LISTS["TV_KEY_3"] = {
        RemoteTV::KEY_3
    };

    ALL_ACTION_LISTS["TV_KEY_4"] = {
        RemoteTV::KEY_4
    };

    ALL_ACTION_LISTS["TV_KEY_5"] = {
        RemoteTV::KEY_5
    };

    ALL_ACTION_LISTS["TV_KEY_6"] = {
        RemoteTV::KEY_6
    };

    ALL_ACTION_LISTS["TV_KEY_7"] = {
        RemoteTV::KEY_7
    };

    ALL_ACTION_LISTS["TV_KEY_8"] = {
        RemoteTV::KEY_8
    };

    ALL_ACTION_LISTS["TV_KEY_9"] = {
        RemoteTV::KEY_9
    };


    //////////////////////////////////////// SPEAKERS REMOTE ////////////////////////////////////////

    ALL_ACTION_LISTS["SPEAKERS_POWER"] = {
        RemoteSpeakers::POWER
    };

    ALL_ACTION_LISTS["SPEAKERS_SLEEP"] = {
        RemoteSpeakers::SLEEP
    };

    ALL_ACTION_LISTS["SPEAKERS_CBL_SAT"] = {
        RemoteSpeakers::CBL_SAT
    };

    ALL_ACTION_LISTS["SPEAKERS_MEDIA_PLAYER"] = {
        RemoteSpeakers::MEDIA_PLAYER
    };

    ALL_ACTION_LISTS["SPEAKERS_BLU_RAY"] = {
        RemoteSpeakers::BLU_RAY
    };

    ALL_ACTION_LISTS["SPEAKERS_GAME"] = {
        RemoteSpeakers::GAME
    };

    ALL_ACTION_LISTS["SPEAKERS_AUX1"] = {
        RemoteSpeakers::AUX1
    };

    ALL_ACTION_LISTS["SPEAKERS_AUX2"] = {
        RemoteSpeakers::AUX2
    };

    ALL_ACTION_LISTS["SPEAKERS_PHONO"] = {
        RemoteSpeakers::PHONO
    };

    ALL_ACTION_LISTS["SPEAKERS_TV_AUDIO"] = {
        RemoteSpeakers::TV_AUDIO
    };

    ALL_ACTION_LISTS["SPEAKERS_TUNER"] = {
        RemoteSpeakers::TUNER
    };

    ALL_ACTION_LISTS["SPEAKERS_USB"] = {
        RemoteSpeakers::USB
    };

    ALL_ACTION_LISTS["SPEAKERS_BLUETOOTH"] = {
        RemoteSpeakers::BLUETOOTH
    };

    ALL_ACTION_LISTS["SPEAKERS_INTERNET_RADIO"] = {
        RemoteSpeakers::INTERNET_RADIO
    };

    ALL_ACTION_LISTS["SPEAKERS_HEOS"] = {
        RemoteSpeakers::HEOS
    };

    ALL_ACTION_LISTS["SPEAKERS_CH_PAGE_UP"] = {
        RemoteSpeakers::CH_PAGE_UP
    };

    ALL_ACTION_LISTS["SPEAKERS_CH_PAGE_DOWN"] = {
        RemoteSpeakers::CH_PAGE_DOWN
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

    ALL_ACTION_LISTS["SPEAKERS_ECO"] = {
        RemoteSpeakers::ECO
    };

    ALL_ACTION_LISTS["SPEAKERS_INFO"] = {
        RemoteSpeakers::INFO
    };

    ALL_ACTION_LISTS["SPEAKERS_OPTION"] = {
        RemoteSpeakers::OPTION
    };

    ALL_ACTION_LISTS["SPEAKERS_BACK"] = {
        RemoteSpeakers::BACK
    };

    ALL_ACTION_LISTS["SPEAKERS_SETUP"] = {
        RemoteSpeakers::SETUP
    };

    ALL_ACTION_LISTS["SPEAKERS_NAV_UP"] = {
        RemoteSpeakers::NAV_UP
    };

    ALL_ACTION_LISTS["SPEAKERS_NAV_DOWN"] = {
        RemoteSpeakers::NAV_DOWN
    };

    ALL_ACTION_LISTS["SPEAKERS_NAV_LEFT"] = {
        RemoteSpeakers::NAV_LEFT
    };

    ALL_ACTION_LISTS["SPEAKERS_NAV_RIGHT"] = {
        RemoteSpeakers::NAV_RIGHT
    };

    ALL_ACTION_LISTS["SPEAKERS_ENTER"] = {
        RemoteSpeakers::ENTER
    };

    ALL_ACTION_LISTS["SPEAKERS_SKIP_BACK"] = {
        RemoteSpeakers::SKIP_BACK
    };

    ALL_ACTION_LISTS["SPEAKERS_SKIP_FORWARD"] = {
        RemoteSpeakers::SKIP_FORWARD
    };

    ALL_ACTION_LISTS["SPEAKERS_PLAY_PAUSE"] = {
        RemoteSpeakers::PLAY_PAUSE
    };

    ALL_ACTION_LISTS["SPEAKERS_QUICK_SELECT_1"] = {
        RemoteSpeakers::QUICK_SELECT_1
    };

    ALL_ACTION_LISTS["SPEAKERS_QUICK_SELECT_2"] = {
        RemoteSpeakers::QUICK_SELECT_2
    };

    ALL_ACTION_LISTS["SPEAKERS_QUICK_SELECT_3"] = {
        RemoteSpeakers::QUICK_SELECT_3
    };

    ALL_ACTION_LISTS["SPEAKERS_QUICK_SELECT_4"] = {
        RemoteSpeakers::QUICK_SELECT_4
    };

    ALL_ACTION_LISTS["SPEAKERS_GREEN"] = {
        RemoteSpeakers::GREEN
    };

    ALL_ACTION_LISTS["SPEAKERS_RED"] = {
        RemoteSpeakers::RED
    };

    ALL_ACTION_LISTS["SPEAKERS_BLUE"] = {
        RemoteSpeakers::BLUE
    };

    ALL_ACTION_LISTS["SPEAKERS_YELLOW"] = {
        RemoteSpeakers::YELLOW
    };


    //////////////////////////////////////// HDD REMOTE ////////////////////////////////////////

    ALL_ACTION_LISTS["HDD_POWER"] = {
        RemoteHDD::POWER
    };

    ALL_ACTION_LISTS["HDD_DRIVE_SELECT"] = {
        RemoteHDD::DRIVE_SELECT
    };

    ALL_ACTION_LISTS["HDD_KEY_0"] = {
        RemoteHDD::KEY_0
    };

    ALL_ACTION_LISTS["HDD_KEY_1"] = {
        RemoteHDD::KEY_1
    };

    ALL_ACTION_LISTS["HDD_KEY_2"] = {
        RemoteHDD::KEY_2
    };

    ALL_ACTION_LISTS["HDD_KEY_3"] = {
        RemoteHDD::KEY_3
    };

    ALL_ACTION_LISTS["HDD_KEY_4"] = {
        RemoteHDD::KEY_4
    };

    ALL_ACTION_LISTS["HDD_KEY_5"] = {
        RemoteHDD::KEY_5
    };

    ALL_ACTION_LISTS["HDD_KEY_6"] = {
        RemoteHDD::KEY_6
    };

    ALL_ACTION_LISTS["HDD_KEY_7"] = {
        RemoteHDD::KEY_7
    };

    ALL_ACTION_LISTS["HDD_KEY_8"] = {
        RemoteHDD::KEY_8
    };

    ALL_ACTION_LISTS["HDD_KEY_9"] = {
        RemoteHDD::KEY_9
    };

    ALL_ACTION_LISTS["HDD_LAST_VIEW"] = {
        RemoteHDD::LAST_VIEW
    };

    ALL_ACTION_LISTS["HDD_NET"] = {
        RemoteHDD::NET
    };

    ALL_ACTION_LISTS["HDD_CHAN_UP"] = {
        RemoteHDD::CHAN_UP
    };

    ALL_ACTION_LISTS["HDD_CHAN_DOWN"] = {
        RemoteHDD::CHAN_DOWN
    };

    ALL_ACTION_LISTS["HDD_FREEVIEW_PLAY"] = {
        RemoteHDD::FREEVIEW_PLAY
    };

    ALL_ACTION_LISTS["HDD_NETFLIX"] = {
        RemoteHDD::NETFLIX
    };

    ALL_ACTION_LISTS["HDD_SKIP_BACK"] = {
        RemoteHDD::SKIP_BACK
    };

    ALL_ACTION_LISTS["HDD_SKIP_FORWARD"] = {
        RemoteHDD::SKIP_FORWARD
    };

    ALL_ACTION_LISTS["HDD_REWIND"] = {
        RemoteHDD::REWIND
    };

    ALL_ACTION_LISTS["HDD_FAST_FORWARD"] = {
        RemoteHDD::FAST_FORWARD
    };

    ALL_ACTION_LISTS["HDD_STOP"] = {
        RemoteHDD::STOP
    };

    ALL_ACTION_LISTS["HDD_PAUSE"] = {
        RemoteHDD::PAUSE
    };

    ALL_ACTION_LISTS["HDD_PLAY"] = {
        RemoteHDD::PLAY
    };

    ALL_ACTION_LISTS["HDD_INFO"] = {
        RemoteHDD::INFO
    };

    ALL_ACTION_LISTS["HDD_EXIT"] = {
        RemoteHDD::EXIT
    };

    ALL_ACTION_LISTS["HDD_OPTION"] = {
        RemoteHDD::OPTION
    };

    ALL_ACTION_LISTS["HDD_RETURN"] = {
        RemoteHDD::RETURN
    };

    ALL_ACTION_LISTS["HDD_VIDEO_MENU"] = {
        RemoteHDD::VIDEO_MENU
    };

    ALL_ACTION_LISTS["HDD_GUIDE"] = {
        RemoteHDD::GUIDE
    };

    ALL_ACTION_LISTS["HDD_FUNCITON_MENU"] = {
        RemoteHDD::FUNCTION_MENU
    };

    ALL_ACTION_LISTS["HDD_NAV_UP"] = {
        RemoteHDD::NAV_UP
    };

    ALL_ACTION_LISTS["HDD_NAV_DOWN"] = {
        RemoteHDD::NAV_DOWN
    };

    ALL_ACTION_LISTS["HDD_NAV_LEFT"] = {
        RemoteHDD::NAV_LEFT
    };

    ALL_ACTION_LISTS["HDD_NAV_RIGHT"] = {
        RemoteHDD::NAV_RIGHT
    };

    ALL_ACTION_LISTS["HDD_OK"] = {
        RemoteHDD::OK
    };

    ALL_ACTION_LISTS["HDD_RED"] = {
        RemoteHDD::RED
    };

    ALL_ACTION_LISTS["HDD_GREE"] = {
        RemoteHDD::GREEN
    };

    ALL_ACTION_LISTS["HDD_YELLO"] = {
        RemoteHDD::YELLOW
    };

    ALL_ACTION_LISTS["HDD_BLUE"] = {
        RemoteHDD::BLUE
    };

    ALL_ACTION_LISTS["HDD_REC"] = {
        RemoteHDD::REC
    };

    ALL_ACTION_LISTS["HDD_REC_MODE"] = {
        RemoteHDD::REC_MODE
    };

    ALL_ACTION_LISTS["HDD_TEXT"] = {
        RemoteHDD::TEXT
    };

    ALL_ACTION_LISTS["HDD_SUBTITLE"] = {
        RemoteHDD::SUBTITLE
    };

    ALL_ACTION_LISTS["HDD_PROG_CHECK"] = {
        RemoteHDD::PROG_CHECK
    };

    ALL_ACTION_LISTS["HDD_CHAPTER"] = {
        RemoteHDD::CHAPTER
    };

    ALL_ACTION_LISTS["HDD_DELETE"] = {
        RemoteHDD::DELETE
    };

    ALL_ACTION_LISTS["HDD_RADIO_TV"] = {
        RemoteHDD::RADIO_TV
    };

    ALL_ACTION_LISTS["HDD_AD"] = {
        RemoteHDD::AD
    };

    ALL_ACTION_LISTS["HDD_TIME_SLIP"] = {
        RemoteHDD::TIME_SLIP
    };

    ALL_ACTION_LISTS["HDD_MANUAL_SKIP_MINUS_10"] = {
        RemoteHDD::MANUAL_SKIP_MINUS_10
    };

    ALL_ACTION_LISTS["HDD_MANUAL_SKIP_PLUS_60"] = {
        RemoteHDD::MANUAL_SKIP_PLUS_60
    };


    //////////////////////////////////////// YOUVIEW REMOTE ////////////////////////////////////////

    ALL_ACTION_LISTS["YOUVIEW_POWER"] = {
        RemoteYouView::POWER
    };
    
    ALL_ACTION_LISTS["YOUVIEW_ZOOM"] = {
        RemoteYouView::ZOOM
    };
    
    ALL_ACTION_LISTS["YOUVIEW_TEXT"] = {
        RemoteYouView::TEXT
    };
    
    ALL_ACTION_LISTS["YOUVIEW_HELP"] = {
        RemoteYouView::HELP
    };
    
    ALL_ACTION_LISTS["YOUVIEW_BT_PLAYER"] = {
        RemoteYouView::BT_PLAYER
    };
    
    ALL_ACTION_LISTS["YOUVIEW_HOME"] = {
        RemoteYouView::HOME
    };
    
    ALL_ACTION_LISTS["YOUVIEW_RED"] = {
        RemoteYouView::RED
    };
    
    ALL_ACTION_LISTS["YOUVIEW_GREEN"] = {
        RemoteYouView::GREEN
    };
    
    ALL_ACTION_LISTS["YOUVIEW_YELLOW"] = {
        RemoteYouView::YELLOW
    };
    
    ALL_ACTION_LISTS["YOUVIEW_BLUE"] = {
        RemoteYouView::BLUE
    };
    
    ALL_ACTION_LISTS["YOUVIEW_INFO"] = {
        RemoteYouView::INFO
    };
    
    ALL_ACTION_LISTS["YOUVIEW_VOL_UP"] = {
        RemoteYouView::VOL_UP
    };
    
    ALL_ACTION_LISTS["YOUVIEW_VOL_DOWN"] = {
        RemoteYouView::VOL_DOWN
    };
    
    ALL_ACTION_LISTS["YOUVIEW_VOL_MUTE"] = {
        RemoteYouView::VOL_MUTE
    };
    
    
    ALL_ACTION_LISTS["YOUVIEW_CHAN_UP"] = {
        RemoteYouView::CHAN_UP
    };
    
    ALL_ACTION_LISTS["YOUVIEW_CHAN_DOWN"] = {
        RemoteYouView::CHAN_DOWN
    };
    
    
    ALL_ACTION_LISTS["YOUVIEW_NAV_UP"] = {
        RemoteYouView::NAV_UP
    };
    
    ALL_ACTION_LISTS["YOUVIEW_NAV_DOWN"] = {
        RemoteYouView::NAV_DOWN
    };
    
    ALL_ACTION_LISTS["YOUVIEW_NAV_LEFT"] = {
        RemoteYouView::NAV_LEFT
    };
    
    ALL_ACTION_LISTS["YOUVIEW_NAV_RIGHT"] = {
        RemoteYouView::NAV_RIGHT
    };
    
    ALL_ACTION_LISTS["YOUVIEW_OK"] = {
        RemoteYouView::OK
    };
    
    ALL_ACTION_LISTS["YOUVIEW_BACK"] = {
        RemoteYouView::BACK
    };
    
    ALL_ACTION_LISTS["YOUVIEW_REWIND"] = {
        RemoteYouView::REWIND
    };
    
    ALL_ACTION_LISTS["YOUVIEW_PLAY"] = {
        RemoteYouView::PLAY
    };
    
    ALL_ACTION_LISTS["YOUVIEW_PAUSE"] = {
        RemoteYouView::PAUSE
    };
    
    ALL_ACTION_LISTS["YOUVIEW_FAST_FORWARD"] = {
        RemoteYouView::FAST_FORWARD
    };
    
    ALL_ACTION_LISTS["YOUVIEW_SKIP_BACK"] = {
        RemoteYouView::SKIP_BACK
    };
    
    ALL_ACTION_LISTS["YOUVIEW_RECORD"] = {
        RemoteYouView::RECORD
    };
    
    ALL_ACTION_LISTS["YOUVIEW_STOP"] = {
        RemoteYouView::STOP
    };
    
    ALL_ACTION_LISTS["YOUVIEW_SKIP_FORWARD"] = {
        RemoteYouView::SKIP_FORWARD
    };
    
    ALL_ACTION_LISTS["YOUVIEW_GUIDE"] = {
        RemoteYouView::GUIDE
    };
    
    ALL_ACTION_LISTS["YOUVIEW_SEARCH"] = {
        RemoteYouView::SEARCH
    };
    
    ALL_ACTION_LISTS["YOUVIEW_CLOSE"] = {
        RemoteYouView::CLOSE
    };
    
    ALL_ACTION_LISTS["YOUVIEW_KEY_0"] = {
        RemoteYouView::KEY_0
    };
    
    ALL_ACTION_LISTS["YOUVIEW_KEY_1"] = {
        RemoteYouView::KEY_1
    };
    
    ALL_ACTION_LISTS["YOUVIEW_KEY_2"] = {
        RemoteYouView::KEY_2
    };
    
    ALL_ACTION_LISTS["YOUVIEW_KEY_3"] = {
        RemoteYouView::KEY_3
    };
    
    ALL_ACTION_LISTS["YOUVIEW_KEY_4"] = {
        RemoteYouView::KEY_4
    };
    
    ALL_ACTION_LISTS["YOUVIEW_KEY_5"] = {
        RemoteYouView::KEY_5
    };
    
    ALL_ACTION_LISTS["YOUVIEW_KEY_6"] = {
        RemoteYouView::KEY_6
    };
    
    ALL_ACTION_LISTS["YOUVIEW_KEY_7"] = {
        RemoteYouView::KEY_7
    };
    
    ALL_ACTION_LISTS["YOUVIEW_KEY_8"] = {
        RemoteYouView::KEY_8
    };
    
    ALL_ACTION_LISTS["YOUVIEW_KEY_9"] = {
        RemoteYouView::KEY_9
    };
    
    ALL_ACTION_LISTS["YOUVIEW_AD"] = {
        RemoteYouView::AD
    };
    
    ALL_ACTION_LISTS["YOUVIEW_DELETE_SUB"] = {
        RemoteYouView::DELETE_SUB
    };
}


void runActionList(const std::string& action_list_id)
{
    if (action_list_running) return;

    if (ALL_ACTION_LISTS.find(action_list_id) == ALL_ACTION_LISTS.end())
    {
        std::cerr << "No action list exists with id: " << action_list_id << std::endl;
        return;
    }

    action_list_running = true;
    const std::vector<ActionBase*>& action_list = ALL_ACTION_LISTS[action_list_id];
    for (int i = 0; i < action_list.size(); i++)
    {
        action_list[i]->run();

        //if two IR signals back to back, need a little time between them
        //if they are the same, the tv won't respond unless a bit more time is given
        //if they are different, then can send them more quickly
        //if the next action is a something else, no need to wait

        if (i + 1 < action_list.size() && action_list[i]->type == REMOTE_SIGNAL && action_list[i + 1]->type == REMOTE_SIGNAL)
        {
            //there are two IR signals back to back

            if (action_list[i] == action_list[i + 1])
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
    action_list_running = false;
}

#endif