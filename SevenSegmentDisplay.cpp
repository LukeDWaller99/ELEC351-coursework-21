#include "SevenSegmentDisplay.h"

SevenSegmentDisplay::SevenSegmentDisplay() : digits(PE_2, PE_3, PE_4, PE_5, PE_6, PE_7, PE_8, PE_9), 
                                            display(PE_10, PE_11), output_enable(PE_15) {}

void SevenSegmentDisplay::clear(int number)
{
    /*
    Clears the seven segment displays

    Args
        number (int): dictates which seven segment display to clear
                      0 = both
                      1 = left hand
                      2 = right hand
    */
    if (number == 1) { // clear left hand display
        output_enable = 1;
        display = 0b10;
        digits = 0b11111111;
    }
    else if (number ==2) { // clear right hand display
    output_enable = 1;
        display = 0b01;
        digits = 0b11111111;
    }
    else { //clear both displays
    output_enable = 1;
        display = 0b11;
        digits = 0b11111111;
    }
}