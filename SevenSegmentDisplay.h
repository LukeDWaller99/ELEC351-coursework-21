#ifndef __Seven_Segment_Display__
#define __Seven_Segment_Display__
#include "mbed.h"
#include "mbed_retarget.h"
#include "mbed_wait_api.h"

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
    void test();
    void operator=(int number);
};

#endif