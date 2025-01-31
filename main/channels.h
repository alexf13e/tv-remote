
#ifndef CHANNELS_H
#define CHANNELS_H

#include <string>
#include <unordered_map>

#define CHANNEL(NAME, NUMBER) TV_CHANNELS[NAME] = std::to_string(NUMBER)

std::unordered_map<std::string, std::string> TV_CHANNELS;

void create_tv_channels()
{
    CHANNEL("BBC_ONE_HD", 101);

    CHANNEL("BBC_TWO_HD", 102);

    CHANNEL("BBC_THREE_HD", 107);

    CHANNEL("BBC_FOUR_HD", 106);

    CHANNEL("BBC_NEWS", 231);

    CHANNEL("ITV1_HD", 103);

    CHANNEL("ITV1", 3);

    CHANNEL("ITV2", 6);

    CHANNEL("ITV3", 10);

    CHANNEL("ITV4", 26);

    CHANNEL("CHANNEL_4_HD", 104);

    CHANNEL("CHANNEL_5_HD", 105);

    CHANNEL("FILM4", 14);

    CHANNEL("MORE4", 18);

    CHANNEL("E4", 13);

    CHANNEL("DAVE", 19);

    CHANNEL("YESTERDAY", 27);

    CHANNEL("QUEST", 12);

    CHANNEL("DMAX", 39);

    CHANNEL("ITV_BE", 28);

    CHANNEL("ITV1+1", 35);

    CHANNEL("ITV2+1", 29);

    CHANNEL("CHANNEL_4+1", 15);

    CHANNEL("CHANNEL_5+1", 38);

    CHANNEL("E4+1", 30);

    CHANNEL("5USA", 21);

    CHANNEL("5STAR", 32);

    CHANNEL("BLAZE", 64);

    CHANNEL("E4_EXTRA", 31);

    CHANNEL("FILM4+1", 47);

    CHANNEL("5SELECT", 46);

    CHANNEL("CBS_REALITY", 67);

    CHANNEL("THATS_TV", 56);

    CHANNEL("4SEVEN", 49);

    CHANNEL("HGTV", 44);

    CHANNEL("REALLY", 17);

    CHANNEL("DRAMA", 20);

    CHANNEL("TALKINGPICTURES_TV", 82);

    CHANNEL("SKY_MIX", 11);

    CHANNEL("SKY_ARTS", 36);

    CHANNEL("GREAT_MOVIES", 34);

    CHANNEL("GREAT_ACTION", 42);

    CHANNEL("5ACTION", 33);

    CHANNEL("SKY_NEWS", 233);

    CHANNEL("QUEST_RED", 40);

    CHANNEL("W", 25);

    CHANNEL("TOGETHER_TV", 83);

    CHANNEL("CHALLENGE", 48);

    CHANNEL("FREEVIEW", 100);
}



#endif