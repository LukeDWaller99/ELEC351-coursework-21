#ifndef __Seven_Segment_Display__
#define __Seven_Segment_Display__
#include "mbed.h"


class SevenSegmentDisplay{
    private:
    BusOut digits; // digits for 7 segment display
    BusOut display; //selects which 7 segment to display from
    DigitalOut output_enable; //Output enabled ACTIVE LOW

    public:

    SevenSegmentDisplay();
    void clear();
    void clear(int number);
    void setDigit(int digit, int number);
    void setNumber(int number);
};

#endif