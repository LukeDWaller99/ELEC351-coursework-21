#include "LEDMatrix.h"

    LEDMatrix::LEDMatrix(): matrix_spi(PC_12, PC_11, PC_10), matrix_spi_cs(PB_6), matrix_spi_oe(PB_12){}

    void LEDMatrix::clear()
    {
        /*
        Clears the LED matrix on the UoP MSB
        */
        matrix_spi_cs = 0; // chip select low to write
        matrix_spi.write(0x00);
        matrix_spi.write(0x00);
        matrix_spi.write(0x00);
        matrix_spi_cs = 1; // chip select high to stop write
    }

    void LEDMatrix::write(int RHC, int LHC, int ROW)
    /*
        Writes the speicifc bytes to the LED Matrix LED

        Args:
            RHC (int) : value between 0 - 255 to control the leds on RHC
            LHC (int) : value between 0 - 255 to control the leds on LHC
            ROW (int) : value between 0 - 7 to control the leds on ROWS

        Returns:
            Null
    */
    {
        matrix_spi_cs = 0; // chip select low to write
        matrix_spi.write(RHC); // RHS COL
        matrix_spi.write(LHC); // LHS COL
        matrix_spi.write(ROW); // ROX RHS
        matrix_spi_cs = 1; // chip select high to stop write
    }
