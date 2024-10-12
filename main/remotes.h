
#ifndef REMOTES_H
#define REMOTES_H

#include "RemoteButtonSignal.h"


//specify a remote to contain all the buttons with namespace REMOTE_NAME
namespace RemoteTV
{ //panasonic N2QAYB 001179
    
    //Codes for remote buttons go here.
    //BUTTON(BUTTON_NAME, "CODE") creates a new button in the remote called BUTTON_NAME with the specified code.
    //Code must be in quotes.
    //Get codes by using receiver program and monitoring the output over usb. pressing a button on the actual remote
    //should result in it getting printed out.
    //Codes are concatenated 32-bit hex values of type rmt_symbol_word_t, which describe pairs of high and low in the
    //ir signal https://docs.espressif.com/projects/esp-idf/en/v5.3.1/esp32/api-reference/peripherals/rmt.html

    BUTTON(POWER, "");

    BUTTON(INPUT_AV, "");
    BUTTON(INPUT_TV, "");

    BUTTON(HOME, "");
    BUTTON(GUIDE, "");

    BUTTON(OK, "");
    BUTTON(RETURN, "");
    BUTTON(EXIT, "");
    BUTTON(INFO, "");

    BUTTON(NAV_LEFT, "");
    BUTTON(NAV_RIGHT, "");
    BUTTON(NAV_UP, "");
    BUTTON(NAV_DOWN, "");

    BUTTON(RED, "");
    BUTTON(GREEN, "");
    BUTTON(YELLOW, "");
    BUTTON(BLUE, "");

    BUTTON(VOL_UP, "");
    BUTTON(VOL_DOWN, "");
    BUTTON(VOL_MUTE, "");
    
    BUTTON(CHAN_UP, "");
    BUTTON(CHAN_DOWN, "");

    BUTTON(KEY_0, "");
    BUTTON(KEY_1, "06cf8da901b381b1051681b101b281b001b381b201b081b001b381b301b381b101b381af01b481b101b281b001b281b201b381b101b381b0051581b101b381b101b481b001b381b001b381b001b381b001b281b101b181b201b281b201b381b1050181b001b381c601b381b001b281b101b381b101b281b101b381b101b381b001b381b101b281b101b381b001b381b101b281b1051681b1019c81b101b381c601b281b101b381b101b281b101b281b101b381b1051681b101b381b001b281b1051581b2000081b2");
    BUTTON(KEY_2, "");
    BUTTON(KEY_3, "");
    BUTTON(KEY_4, "");
    BUTTON(KEY_5, "");
    BUTTON(KEY_6, "");
    BUTTON(KEY_7, "");
    BUTTON(KEY_8, "");
    BUTTON(KEY_9, "");
};

#endif
