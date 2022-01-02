#ifndef __LED_MATRIX__
#define __LED_MATRIX__

#include "DigitalOut.h"
#include "InterfaceDigitalOut.h"
#include "PinNames.h"
#include "uop_msb.h"
#include "mbed.h"
#include "SPI.h"

class LEDMatrix{
    private:
    SPI matrix_spi;   // MOSI, MISO, SCLK
    DigitalOut matrix_spi_cs;            //Chip Select ACTIVE LOW
    DigitalOut matrix_spi_oe;           //Output Enable ACTIVE LOW 
    
    public:

    LEDMatrix();
    void clear();
    void writeMatrix(int RHC, int LHC, int ROW);
    void plot(double samples[]);
    void test();

};
#endif