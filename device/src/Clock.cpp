#include "clock.h"
#include <string.h>

char *clockConverter (char *input, const char *format, Time &time)
{
    if (format == nullptr) {
        return nullptr;
    }
    
    
    int h = time.hours;
    int m = time.minutes;
    int s = time.seconds;
    int ms = time.mills;
    
    
    int l = strlen(format);
    char hh[2];
    char mm[2];
    char ss[2];
    hh[0] = ( (( h / 10 ) % 10) ) % 10 + '0';
	hh[1] =  h  % 10 + '0';
	mm[0] = (m / 10) % 10 + '0';
	mm[1] = m % 10 + '0';
    ss[0] = (s / 10) % 10 + '0';
	ss[1] = s % 10 + '0';
    uint8_t hp = 0, mp = 0, sp = 0;
    for (int i = 0; i <= l; i++) {
        if (format[i] == 'h') {
            input[i] = hh[hp++];
        } else if (format[i] == 'm') {
            input[i] = mm[mp++];
        } else if (format[i] == 's') {
            input[i] = ss[sp++];
        } else if (format[i] == ':') {
            input[i] = ':';    
        }
    }
    return input;
}


    bool icreaseTime (Time &time)
    {
        time.mills++;
        if (time.mills >= 1000) {
                time.seconds++;
                time.mills = 0;
                if (time.seconds >= 60) {
                    time.seconds = 0;
                    time.minutes++;
                    if (time.minutes >= 60) {
                        time.minutes = 0;
                        time.hours++;
                        if (time.hours >= 24) {
                            time.hours = 0;
                            return true;
                        }
                        return false;
                    }
                    return false;
                }
                return false;
        } else {}
    }

    bool decreaseTime (Time &time)
    {
            if (time.mills > 0) {
                time.mills--;
                return false;
            } else if (time.seconds > 0) {
                time.seconds--;
                time.mills = 1000;
                return false;
            } else if (time.minutes > 0) {
                time.minutes--;
                time.seconds = 60;
                return false;
            } else if (time.hours > 0) {
                time.hours--;
                time.minutes = 60;
                return false;
            } else {}
            return true;
    }
    
    void resetTime (Time &time)
        {
            time.mills   = 0;
            time.seconds = 0;
            time.minutes = 0;
            time.hours   = 0;
        }
        
        
    uint32_t parseHours (char *input)
    {
        uint16_t hh = (input[0] - '0') % 10;
        uint16_t h = (input[1] - '0') % 10;
        return hh * 10 + h;
    }
    uint32_t parseMinutes (char *input)
    {
        uint16_t mm = (input[3] - '0') % 10;
        uint16_t m = (input[4] - '0') % 10;
        return mm * 10 + m;
    }
    uint32_t parseSeconds (char *input)
    {
        uint16_t ss = (input[6] - '0') % 10;
        uint16_t s = (input[7] - '0') % 10;
        return ss * 10 + s;
    }
    uint32_t parseMils (char *input)
    {
        return 0;
    }

    uint32_t insertHours (char *input, uint32_t hh, char delimiter)
    {
        char h[2];
        h[0] = ( (( hh / 10 ) % 10) ) % 10 + '0';
        h[1] =  hh  % 10 + '0';
        input[0] = h[0];
        input[1] = h[1];
    }
    uint32_t insertMinutes (char *input, uint32_t mm,  char delimiter)
    {
        char m[2];
        m[0] = ( (( mm / 10 ) % 10) ) % 10 + '0';
        m[1] =  mm  % 10 + '0';
        input[3] = m[0];
        input[4] = m[1];
    }
    uint32_t insertSeconds (char *input, uint32_t ss,  char delimiter)
    {
        char s[2];
        s[0] = ( (( ss / 10 ) % 10) ) % 10 + '0';
        s[1] =  ss  % 10 + '0';
        input[6] = s[0];
        input[7] = s[1];
    }
    uint32_t insertMils (char *input, char delimiter)
    {
        return 0;
    }
        
    uint32_t incrementAny (char *input, uint8_t pos, char delimiter)
    {
        unsigned char i;
        if (input[pos] != delimiter) {
            input[pos] = ((input[pos]) + 1)%10 + '0';
        }
        return 0;
    } 

    Time &parseBack (char *input, char delimiter)
    {
        static Time time;
        time.hours = parseHours(input);
        time.minutes = parseMinutes(input);
        time.seconds = parseSeconds(input);
        time.mills = parseMils(input);
    }        
        
        
        