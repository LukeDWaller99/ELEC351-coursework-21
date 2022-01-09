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
    /**
    Sets the number of a given seven segment display

    @param digit Digit sets which seven segment display to write to
    @param number Sets the number to output in the 7 segmenet to display
    **/
    void setDigit(int digit, int number);

    /**
    Sets the number to be outputted on the seven segment display
    @param number The number to be outputted onto the seven segment display (in HEX)
    **/
    void setNumber(int number);
    void test();
    void operator=(int number);
};

#endif