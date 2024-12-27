
#ifndef DEFINES_H
#define DEFINES_H

//screen history RTC memory requires a static array of a known size.
//although this value will probably only ever be used in sleep_memory.h, it didn't make much sense that the screen
//history size should be specified there
#define SCREEN_HISTORY_SIZE 10

#endif