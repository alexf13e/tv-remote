
#ifndef REMOTE_BUTTON_SIGNAL_H
#define REMOTE_BUTTON_SIGNAL_H

#include <cstddef>
#include <sstream>
#include <string>
#include "hal/rmt_types.h"

struct RemoteButtonSignal
{
    //words is an array of rmt_symbol_word_t
    //each word describes the timing for one pair of low-high or high-low in the signal
    rmt_symbol_word_t* words;
    size_t length;

    RemoteButtonSignal()
    {
        words = nullptr;
        length = 0;
    }

    RemoteButtonSignal(std::string code) //build array of words from long hex string
    {
        words = nullptr;
        length = 0;
        
        //each word is 32 bits, so 8 characters to describe each word
        if (code.length() <= 0 || code.length() % 8 != 0)
        {
            //code is not valid, it should have a whole number of 8 character sections
            return;
        }

        length = code.length() / 8;
        words = new rmt_symbol_word_t[length];

        for (int i = 0; i < length; i++)
        {
            //take the next chunk of 8 characters, convert to a number, save to array of words
            size_t start = i * 8;
            std::stringstream converter;
            converter << std::hex << code.substr(start, 8);
            converter >> words[i].val;
        }
    }

    ~RemoteButtonSignal()
    {
        if (words != nullptr) delete[] words;
    }
};

//macro for more easily adding to list of buttons in each remote
#define BUTTON(NAME, CODE) const RemoteButtonSignal* NAME = new RemoteButtonSignal(CODE)

#endif