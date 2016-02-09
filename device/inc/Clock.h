#ifndef CLOCK_OBJECT
#define CLOCK_OBJECT

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    int8_t hours;
    int8_t  minutes;
    int8_t  seconds;
    int16_t  mills;   
} Time;


char *clockConverter (char *input, const char *format, Time &time);

bool icreaseTime (Time &time);
bool decreaseTime (Time &time);
void resetTime (Time &time);


uint32_t parseHours (char *input, char delimiter);
uint32_t parseMinutes (char *input, char delimiter);
uint32_t parseSeconds (char *input, char delimiter);
uint32_t parseMils (char *input, char delimiter);

uint32_t insertHours (char *input, uint32_t hh, char delimiter);
uint32_t insertMinutes (char *input, uint32_t mm,  char delimiter);
uint32_t insertSeconds (char *input, uint32_t ss,  char delimiter);
uint32_t insertMils (char *input, char delimiter);

uint32_t incrementAny (char *input, uint8_t pos, char delimiter);

Time &parseBack (char *input, char delimiter);

#endif


