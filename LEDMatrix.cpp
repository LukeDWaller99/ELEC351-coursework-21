#include "LEDMatrix.h"
#include <algorithm>
#include <cstdio>

#define SCAN_RATE_MS 50

LEDMatrix::LEDMatrix()
    : matrix_spi(PC_12, PC_11, PC_10), matrix_spi_cs(PB_6),
      matrix_spi_oe(PB_12) {}

void LEDMatrix::clear() {
  /*
  Clears the LED matrix on the UoP MSB
  */
  matrix_spi_cs = 0; // chip select low to write
  matrix_spi.write(0x00);
  matrix_spi.write(0x00);
  matrix_spi.write(0x00);
  matrix_spi_cs = 1; // chip select high to stop write
}

void LEDMatrix::writeMatrix(int RHC, int LHC, int ROW)
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
  matrix_spi_cs = 0;     // chip select low to write
  matrix_spi.write(LHC); // LHS COL
  matrix_spi.write(RHC); // RHS COL
  matrix_spi.write(ROW); // ROX RHS
  matrix_spi_cs = 1;     // chip select high to stop write
}

void LEDMatrix::plot(double samples[]) {
  int sample = 0;
  int sampleMSB = 0;
  int sampleLSB = 0;
  for (int i = 0; i <= 7; i++) {
    sample = samples[i];

    // sample = (sample>>4)|(sample<<4);

    sampleMSB = sample >> 8;
    sampleLSB = sample;
    // sampleLSB = (sampleLSB>>4)|(sampleLSB<<4);

    writeMatrix(sampleLSB, sampleMSB, i);
    // wait_us(500000);
  }
}

void LEDMatrix::test() {

int LHC = 0b10101010;
for(int ROW = 0; ROW <=7; ROW++)
{
    matrix_spi_cs = 0;     // chip select low to write
    matrix_spi.write(LHC); // LHS COL
    matrix_spi.write(0x00); // RHS COL
    matrix_spi.write(ROW); // ROX RHS
    matrix_spi_cs = 1;     // chip select high to stop write
    LHC = LHC >> 1;
    thread_sleep_for(SCAN_RATE_MS);
}
int RHC = 0b10000000;
for (int ROW = 0; ROW <=7; ROW++)
{
    matrix_spi_cs = 0;     // chip select low to write
    matrix_spi.write(LHC); // LHS COL
    matrix_spi.write(0x00); // RHS COL
    matrix_spi.write(ROW); // ROX RHS
    matrix_spi_cs = 1;     // chip select high to stop write
    RHC = LHC >> 1;
    thread_sleep_for(SCAN_RATE_MS);
}

}
