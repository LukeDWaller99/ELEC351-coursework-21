/**
@file SevenSegmentDisplay.h
Seven Segment Display class header file.

Author - Luke Waller
**/
#ifndef __Seven_Segment_Display__
#define __Seven_Segment_Display__
#include "mbed.h"
#include "mbed_retarget.h"
#include "mbed_wait_api.h"

class SevenSegmentDisplay{
    private:
    BusOut digits;              ///<digits for 7 segment display
    BusOut display;             ///<selects which 7 segment to display from
    DigitalOut output_enable;   ///<Output enabled ACTIVE LOW

    public:
    /**
    Default constructor. This constructor initialises a Seven Segment
    Display object with the correct pin assignments for the MSB V2 board.
    **/
    SevenSegmentDisplay();
    /**
    Method to clear the displays. Calling the method without any arguments
    will clear both digits, completely clearing the display.
    **/
    void clear();
    /**
    Alternative definition for the clear method. When passing an argument
    to the method it will clear one specific digit of the display.
    @param number integer specifying which digit to clear.
    **/
    void clear(int number);
    /**
    Method to set a single digit of the display. This method takes two arguments,
    and sets the selected digit to the value of the argument passed.
    @param digit Which digit of the display to change.
    @param number The number that the selected digit should be set to.
    **/
    void setDigit(int digit, int number);
    /**
    Method to set both digits of the display. This method sets the display to be
    equal to the integer value passed to it.
    **/
    void setNumber(int number);
    /**
    Test method. This method tests the display by incrementing through different values
    and displaying them on the display.
    **/
    void test();
    /**
    Operator overload for the set number funtion. This operator overload allows
    the display to be set using the following syntax:
    @code
    LEDMatrix = number;
    @endcode
    This streamlines the code within considerably, and makes the code much more readable.
    **/
    void operator=(int number);
};

#endif