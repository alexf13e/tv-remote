
#ifndef ACTIONLIST_H
#define ACTIONLIST_H

#include <chrono>
#include <thread>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>

#include "app-window.h" //for setting button enable/disable

#include "action.h"
#include "power.h"
#include "remotes.h"



//tv won't respond to the same ir signal being sent too closely together, but forcing all signals to be slower is annoying
#define MS_BETWEEN_DIFFERENT_IR_SIGNALS 200
#define MS_BETWEEN_SAME_IR_SIGNALS 300

#define WAIT_FOR_MS(DURATION) new ActionDelayMilliseconds(DURATION)
#define REPEAT_SIGNAL_FOR_MS(ACTION_REMOTE_SIGNAL, DURATION) new ActionRepeatIRForMilliseconds(ACTION_REMOTE_SIGNAL, DURATION)

std::unordered_map<std::string, std::vector<ActionBase*>> ALL_ACTION_LISTS;


void createActionLists()
{
    //////////////////////////////////////// GENERAL MACROS ////////////////////////////////////////

    ALL_ACTION_LISTS["STARTUP_TV"] = {
        RemoteSpeakers::POWER,
        WAIT_FOR_MS(500),
        RemoteSpeakers::TV_AUDIO,
        REPEAT_SIGNAL_FOR_MS(RemoteTV::POWER, 500),
        WAIT_FOR_MS(5000),
        RemoteTV::EXIT
    };

    ALL_ACTION_LISTS["SHUTDOWN_TV"] = {
        REPEAT_SIGNAL_FOR_MS(RemoteTV::POWER, 500),
        RemoteSpeakers::POWER
    };

    ALL_ACTION_LISTS["FREEVIEW_PLAY"] = {
        RemoteTV::FREEVIEW_PLAY,
        WAIT_FOR_MS(1000),
        RemoteTV::NAV_RIGHT,
        RemoteTV::OK
    };



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

    ALL_ACTION_LISTS["TV_PICTURE"] = {
        RemoteTV::PICTURE
    };

    ALL_ACTION_LISTS["TV_INPUT_TV"] = {
        RemoteTV::INPUT_TV
    };

    ALL_ACTION_LISTS["TV_INPUT_AV"] = {
        RemoteTV::INPUT_AV
    };

    ALL_ACTION_LISTS["TV_MENU"] = {
        RemoteTV::MENU
    };

    ALL_ACTION_LISTS["TV_TEXT"] = {
        RemoteTV::TEXT
    };

    ALL_ACTION_LISTS["TV_STTL"] = {
        RemoteTV::STTL
    };

    ALL_ACTION_LISTS["TV_APPS"] = {
        RemoteTV::APPS
    };

    ALL_ACTION_LISTS["TV_NETFLIX"] = {
        RemoteTV::NETFLIX
    };

    ALL_ACTION_LISTS["TV_HOME"] = {
        RemoteTV::HOME
    };

    ALL_ACTION_LISTS["TV_GUIDE"] = {
        RemoteTV::GUIDE
    };

    ALL_ACTION_LISTS["TV_INFO"] = {
        RemoteTV::INFO
    };

    ALL_ACTION_LISTS["TV_EXIT"] = {
        RemoteTV::EXIT
    };

    ALL_ACTION_LISTS["TV_OK"] = {
        RemoteTV::OK
    };

    ALL_ACTION_LISTS["TV_OPTION"] = {
        RemoteTV::OPTION
    };

    ALL_ACTION_LISTS["TV_RETURN"] = {
        RemoteTV::RETURN
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

    ALL_ACTION_LISTS["TV_VOL_UP"] = {
        RemoteTV::VOL_UP
    };

    ALL_ACTION_LISTS["TV_VOL_DOWN"] = {
        RemoteTV::VOL_DOWN
    };

    ALL_ACTION_LISTS["TV_VOL_MUTE"] = {
        RemoteTV::VOL_MUTE
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

    ALL_ACTION_LISTS["TV_HELP"] = {
        RemoteTV::HELP
    };

    ALL_ACTION_LISTS["TV_FREEVIEW_PLAY"] = {
        RemoteTV::FREEVIEW_PLAY
    };

    ALL_ACTION_LISTS["TV_REWIND"] = {
        RemoteTV::REWIND
    };

    ALL_ACTION_LISTS["TV_PLAY"] = {
        RemoteTV::PLAY
    };

    ALL_ACTION_LISTS["TV_FAST_FORWARD"] = {
        RemoteTV::FAST_FORWARD
    };

    ALL_ACTION_LISTS["TV_SKIP_BACK"] = {
        RemoteTV::SKIP_BACK
    };

    ALL_ACTION_LISTS["TV_PAUSE"] = {
        RemoteTV::PAUSE
    };

    ALL_ACTION_LISTS["TV_SKIP_FORWARD"] = {
        RemoteTV::SKIP_FORWARD
    };

    ALL_ACTION_LISTS["TV_MY_APP"] = {
        RemoteTV::MY_APP
    };

    ALL_ACTION_LISTS["TV_STOP"] = {
        RemoteTV::STOP
    };

    ALL_ACTION_LISTS["TV_RECORD"] = {
        RemoteTV::RECORD
    };

    //////////////////////////////////////// SPEAKERS REMOTE ////////////////////////////////////////

    ALL_ACTION_LISTS["SPEAKERS_POWER"] = {
        RemoteSpeakers::POWER
    };

    ALL_ACTION_LISTS["SPEAKERS_SLEEP"] = {
        RemoteSpeakers::SLEEP
    };

    ALL_ACTION_LISTS["SPEAKERS_CBL_SAT"] = { //YouView
        RemoteSpeakers::CBL_SAT
    };

    ALL_ACTION_LISTS["SPEAKERS_MEDIA_PLAYER"] = { //HDR
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

    ALL_ACTION_LISTS["SPEAKERS_TV_AUDIO"] = { //TV
        RemoteSpeakers::TV_AUDIO
    };

    ALL_ACTION_LISTS["SPEAKERS_TUNER"] = { //Radio
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


    //////////////////////////////////////// HDR REMOTE ////////////////////////////////////////

    ALL_ACTION_LISTS["HDR_POWER"] = {
        RemoteHDR::POWER
    };

    ALL_ACTION_LISTS["HDR_DRIVE_SELECT"] = {
        RemoteHDR::DRIVE_SELECT
    };

    ALL_ACTION_LISTS["HDR_KEY_0"] = {
        RemoteHDR::KEY_0
    };

    ALL_ACTION_LISTS["HDR_KEY_1"] = {
        RemoteHDR::KEY_1
    };

    ALL_ACTION_LISTS["HDR_KEY_2"] = {
        RemoteHDR::KEY_2
    };

    ALL_ACTION_LISTS["HDR_KEY_3"] = {
        RemoteHDR::KEY_3
    };

    ALL_ACTION_LISTS["HDR_KEY_4"] = {
        RemoteHDR::KEY_4
    };

    ALL_ACTION_LISTS["HDR_KEY_5"] = {
        RemoteHDR::KEY_5
    };

    ALL_ACTION_LISTS["HDR_KEY_6"] = {
        RemoteHDR::KEY_6
    };

    ALL_ACTION_LISTS["HDR_KEY_7"] = {
        RemoteHDR::KEY_7
    };

    ALL_ACTION_LISTS["HDR_KEY_8"] = {
        RemoteHDR::KEY_8
    };

    ALL_ACTION_LISTS["HDR_KEY_9"] = {
        RemoteHDR::KEY_9
    };

    ALL_ACTION_LISTS["HDR_LAST_VIEW"] = {
        RemoteHDR::LAST_VIEW
    };

    ALL_ACTION_LISTS["HDR_NET"] = {
        RemoteHDR::NET
    };

    ALL_ACTION_LISTS["HDR_CHAN_UP"] = {
        RemoteHDR::CHAN_UP
    };

    ALL_ACTION_LISTS["HDR_CHAN_DOWN"] = {
        RemoteHDR::CHAN_DOWN
    };

    ALL_ACTION_LISTS["HDR_FREEVIEW_PLAY"] = {
        RemoteHDR::FREEVIEW_PLAY
    };

    ALL_ACTION_LISTS["HDR_NETFLIX"] = {
        RemoteHDR::NETFLIX
    };

    ALL_ACTION_LISTS["HDR_SKIP_BACK"] = {
        RemoteHDR::SKIP_BACK
    };

    ALL_ACTION_LISTS["HDR_SKIP_FORWARD"] = {
        RemoteHDR::SKIP_FORWARD
    };

    ALL_ACTION_LISTS["HDR_REWIND"] = {
        RemoteHDR::REWIND
    };

    ALL_ACTION_LISTS["HDR_FAST_FORWARD"] = {
        RemoteHDR::FAST_FORWARD
    };

    ALL_ACTION_LISTS["HDR_STOP"] = {
        RemoteHDR::STOP
    };

    ALL_ACTION_LISTS["HDR_PAUSE"] = {
        RemoteHDR::PAUSE
    };

    ALL_ACTION_LISTS["HDR_PLAY"] = {
        RemoteHDR::PLAY
    };

    ALL_ACTION_LISTS["HDR_INFO"] = {
        RemoteHDR::INFO
    };

    ALL_ACTION_LISTS["HDR_EXIT"] = {
        RemoteHDR::EXIT
    };

    ALL_ACTION_LISTS["HDR_OPTION"] = {
        RemoteHDR::OPTION
    };

    ALL_ACTION_LISTS["HDR_RETURN"] = {
        RemoteHDR::RETURN
    };

    ALL_ACTION_LISTS["HDR_VIDEO_MENU"] = {
        RemoteHDR::VIDEO_MENU
    };

    ALL_ACTION_LISTS["HDR_GUIDE"] = {
        RemoteHDR::GUIDE
    };

    ALL_ACTION_LISTS["HDR_FUNCITON_MENU"] = {
        RemoteHDR::FUNCTION_MENU
    };

    ALL_ACTION_LISTS["HDR_NAV_UP"] = {
        RemoteHDR::NAV_UP
    };

    ALL_ACTION_LISTS["HDR_NAV_DOWN"] = {
        RemoteHDR::NAV_DOWN
    };

    ALL_ACTION_LISTS["HDR_NAV_LEFT"] = {
        RemoteHDR::NAV_LEFT
    };

    ALL_ACTION_LISTS["HDR_NAV_RIGHT"] = {
        RemoteHDR::NAV_RIGHT
    };

    ALL_ACTION_LISTS["HDR_OK"] = {
        RemoteHDR::OK
    };

    ALL_ACTION_LISTS["HDR_RED"] = {
        RemoteHDR::RED
    };

    ALL_ACTION_LISTS["HDR_GREE"] = {
        RemoteHDR::GREEN
    };

    ALL_ACTION_LISTS["HDR_YELLO"] = {
        RemoteHDR::YELLOW
    };

    ALL_ACTION_LISTS["HDR_BLUE"] = {
        RemoteHDR::BLUE
    };

    ALL_ACTION_LISTS["HDR_RECORD"] = {
        RemoteHDR::RECORD
    };

    ALL_ACTION_LISTS["HDR_REC_MODE"] = {
        RemoteHDR::REC_MODE
    };

    ALL_ACTION_LISTS["HDR_TEXT"] = {
        RemoteHDR::TEXT
    };

    ALL_ACTION_LISTS["HDR_SUBTITLE"] = {
        RemoteHDR::SUBTITLE
    };

    ALL_ACTION_LISTS["HDR_PROG_CHECK"] = {
        RemoteHDR::PROG_CHECK
    };

    ALL_ACTION_LISTS["HDR_CHAPTER"] = {
        RemoteHDR::CHAPTER
    };

    ALL_ACTION_LISTS["HDR_DELETE"] = {
        RemoteHDR::DELETE
    };

    ALL_ACTION_LISTS["HDR_RADIO_TV"] = {
        RemoteHDR::RADIO_TV
    };

    ALL_ACTION_LISTS["HDR_AD"] = {
        RemoteHDR::AD
    };

    ALL_ACTION_LISTS["HDR_TIME_SLIP"] = {
        RemoteHDR::TIME_SLIP
    };

    ALL_ACTION_LISTS["HDR_MANUAL_SKIP_MINUS_10"] = {
        RemoteHDR::MANUAL_SKIP_MINUS_10
    };

    ALL_ACTION_LISTS["HDR_MANUAL_SKIP_PLUS_60"] = {
        RemoteHDR::MANUAL_SKIP_PLUS_60
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

namespace ActionListRunner
{
    constexpr std::chrono::milliseconds poll_interval = std::chrono::milliseconds(50);
    
    slint::ComponentHandle<AppWindow>* p_main_window;

    std::string current_action_list_id;
    std::thread action_worker;
    bool action_list_running = false;
    bool initialised = false;

    namespace
    {
        void run_action_list(const std::string& action_list_id)
        {
            //last resort safety check to prevent trying to run multiple lists at the same time
            if (action_list_running) return; 

            if (ALL_ACTION_LISTS.find(action_list_id) == ALL_ACTION_LISTS.end())
            {
                std::cerr << "No action list exists with id: " << action_list_id << std::endl;
                return;
            }

            Power::disable_sleep(); //prevent from sleeping in case of long action lists
            (*p_main_window)->global<DisableIRButtons>().set_active(true);
            action_list_running = true;

            const std::vector<ActionBase*>& action_list = ALL_ACTION_LISTS[action_list_id];
            for (int i = 0; i < action_list.size(); i++)
            {
                //std::cout << "running action " << i << std::endl;
                action_list[i]->run();

                //if two IR signals back to back, need a little time between them
                //if they are the same, the tv won't respond unless a bit more time is given
                //if they are different, then can send them more quickly
                //if the next action is a something else, no need to wait

                if (i + 1 < action_list.size() && action_list[i]->compare_type(action_list[i + 1]))
                {
                    //there are two IR signals back to back

                    if (action_list[i]->compare_content(action_list[i + 1]))
                    {
                        //they are the same signal, wait a bit longer
                        //std::cout << "waiting longer time between same signal" << std::endl;
                        std::this_thread::sleep_for(std::chrono::milliseconds(MS_BETWEEN_SAME_IR_SIGNALS));
                    }
                    else
                    {
                        //they are different, can wait a bit less
                        //std::cout << "waiting shorter time between different signal" << std::endl;
                        std::this_thread::sleep_for(std::chrono::milliseconds(MS_BETWEEN_DIFFERENT_IR_SIGNALS));
                    }
                }
                // else
                // {
                //     std::cout << "not waiting between non ir signals" << std::endl;
                // }
            }

            action_list_running = false;
            (*p_main_window)->global<DisableIRButtons>().set_active(false);
            (*p_main_window)->global<DisableIRButtons>().invoke_clear_permitted_screen_change();
            Power::enable_sleep();
        }

        void begin()
        {
            while (true)
            {
                std::this_thread::sleep_for(poll_interval);
                if (current_action_list_id == "") continue;

                //an action list has been requested to be run
                run_action_list(current_action_list_id);

                current_action_list_id = "";
            }
        }
    }

    void init()
    {
        if (initialised)
        {
            std::cerr << "attempted to initialise action list runner more than once" << std::endl;
            return;
        }

        current_action_list_id = "";
        action_worker = std::thread(begin);

        initialised = true;
    }

    bool request_run(const std::string& action_list_id)
    {
        //return if the request was successful so feedback can be given

        if (current_action_list_id != "") return false; //something running, don't accept another request

        current_action_list_id = action_list_id;
        return true;
    }
}

#endif