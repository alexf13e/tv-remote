
#ifndef REMOTE_BUTTON_SIGNAL_H
#define REMOTE_BUTTON_SIGNAL_H

#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

#include "hal/rmt_types.h"


struct RemoteButtonSignal
{
    //our remotes are using panasonic and denon-k protocols which are almost identical http://www.hifi-remote.com/johnsfine/DecodeIR.html
    //other remotes can probably still use long string code instead of a 64 bit number
    //if other parameters like carrier signal or bit encoding greatly vary between remotes, then the namespaces in
    //remotes.h will probably need extending to be structs/classes with a property for signal parameters, and the ir
    //transmitter will need to allow these parameters to be passed...

    static const uint16_t time_unit = 432; //microseconds
    static const uint8_t num_signal_bits = 48; //how long the main part of the signal is
    
    //representations of 0 and 1 as high and low times
    static constexpr rmt_symbol_word_t symbol_zero = {
        .duration0 = time_unit,     //for 1 time unit
        .level0 = 1,                //set signal high
        .duration1 = time_unit,     //then for 1 time unit
        .level1 = 0,                //set low
    };

    static constexpr rmt_symbol_word_t symbol_one = {
        .duration0 = time_unit,
        .level0 = 1,
        .duration1 = time_unit * 3,
        .level1 = 0,
    };

    //the high and low timings at the start and end of each signal (was the same in all the tested remotes)
    static constexpr rmt_symbol_word_t symbol_start = {
        .duration0 = time_unit * 8,
        .level0 = 1,
        .duration1 = time_unit * 4,
        .level1 = 0,
    };

    static constexpr rmt_symbol_word_t symbol_end = {
        .duration0 = time_unit,
        .level0 = 1,
        .duration1 = 0,
        .level1 = 0,
    };

    //words is an array of rmt_symbol_word_t
    //each word describes the timing for one pair of low-high or high-low in the signal
    std::vector<rmt_symbol_word_t> words;


    //build array of rmt words from long string of rmt_symbol_word_t.val as hex concatenated together
    //e.g. 066d8d41018381cd018481ca04cc81cb018481cc04ce81c9018581cb04cd81c9018581cb018681c904ce81c9018581ca018581ca04cd81c904cd81cb018681ca018681c9018581c9018581ca018581c9018581c9019e81b2018781c604ce81c9019d81b304e481b2018581cb018381ca018381cc04cc81cb04e481b404ce81ca04e481b404cc81cc018281cd018281cc018281cc018281cd018281cd018281cc018281cc018281cd018181cd018181cd018081ce04c981cf04cb81ce018081ce04c781ce000081cd
    RemoteButtonSignal(std::string code)
    {        
        //each word is 32 bits, so 8 characters to describe each word
        if (code.length() <= 0 || code.length() % 8 != 0)
        {
            //code is not valid, it should have a whole number of 8 character sections
            return;
        }

        words = std::vector<rmt_symbol_word_t>(code.length() / 8); //allocate space for words in vector

        for (int i = 0; i < words.size(); i++)
        {
            //take the next chunk of 8 characters, convert to a number, save to array of words
            std::stringstream converter;
            converter << std::hex << code.substr(i * 8, 8);
            converter >> words[i].val;
        }
    }

    //build array of rmt words from decoded remote signal as one 64 bit value.
    //e.g. 101100000000000111110001010000000011001001010100 or 0xb001f1403254 as hex
    RemoteButtonSignal(uint64_t code)
    {
        words.push_back(symbol_start);

        for (int i = 0; i < num_signal_bits; i++)
        {
            if (code & 1) //if the next bit in the code is a 1
            {
                words.push_back(symbol_one);
            }
            else
            {
                words.push_back(symbol_zero);
            }

            code >>= 1; //shift the code along by 1 bit so the next bit to read is at the front
        }

        words.push_back(symbol_end);
    }
};

#endif