/**
@file LEDMatrix.h
 class header file.

 Author - Luke Waller
**/

#ifndef __LED_MATRIX__
#define __LED_MATRIX__

#include "DigitalOut.h"
#include "InterfaceDigitalOut.h"
#include "PinNames.h"
#include "uop_msb.h"
#include "mbed.h"
#include "SPI.h"


/**
Thread-safe LED Matrix class. This class allows the use of the 
MSB's LED Matrix display in a thread-safe manner. It includes a
method for plotting a bar display of eight values, as well as test
functions for verifiying correct operation.
**/
class LEDMatrix{
    private:
    SPI matrix_spi;                 ///< MOSI, MISO, SCLK
    DigitalOut matrix_spi_cs;       ///<Chip Select ACTIVE LOW
    DigitalOut matrix_spi_oe;       ///<Output Enable ACTIVE LOW
    int samples[8];                 ///<Internal buffer for holding sample values to be plotted.

    public:
    Thread MatrixThread;            ///<Thread to run the matrix update function
    /**
    Default constructor. This initialises the matrix_spi with the correct
    pins, resets the internal buffer, and then starts the MatrixThread thread
    object with the matrixThread method as a callback.
    **/
    LEDMatrix();
    /**
    Method to clear the display. This method transmits three sets of 
    0x00 to the matrix display, clearing it. 
    **/
    void clear();
    /**
    Writes the speicifc bytes to the LED Matrix LED.
    @param RHC value between 0 - 255 to control the leds on RHC
    @param LHC value between 0 - 255 to control the leds on LHC
    @param value between 0 - 7 to control the leds on ROWS
    Returns:
        Null
    **/
    void writeMatrix(int RHC, int LHC, int ROW);
    /**
    Method to plot the samples as a bar graph. This method will automatically
    plot the samples in the internal buffer as a graph. Use the update function to 
    add new samples to be plotted.
    **/
    void plot();
    /**
    Test method. Tests the functionality of the display by lighting up
    the rows in order.
    **/
    void test();
    /**
    Internal buffer update function. This method allows the internal buffer,
    used with the Plot method. By calling this in another class, values can be passed in.
    @param updatedSamples eight value buffer of values 0-16, controlling how many lights on each row should be lit.
    **/
    void update(int updatedSamples[]);
    /**
    Callback method for the the MatrixThread thread object. This method is passed to the thread 
    as a callback, and simply runs the 'plot' method to mimic a hold functionality.
    **/
    void matrixThread();

};
#endif