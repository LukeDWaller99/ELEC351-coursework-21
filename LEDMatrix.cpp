#include "LEDMatrix.h"
#include "ThisThread.h"

LEDMatrix::LEDMatrix()
    : matrix_spi(PC_12, PC_11, PC_10), matrix_spi_cs(PB_6),
      matrix_spi_oe(PB_12) {
  for (int i = 0; i <= 7; i++) {
    samples[i] = 0;
  }
  MatrixThread.start(callback(this, &LEDMatrix::matrixThread));
}

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

{
  matrix_spi_cs = 0;     // chip select low to write
  matrix_spi.write(LHC); // LHS COL
  matrix_spi.write(RHC); // RHS COL
  matrix_spi.write(ROW); // ROX RHS
  matrix_spi_cs = 1;     // chip select high to stop write
}

void LEDMatrix::plot() {

  int sampleLHS;
  int sampleRHS;

  for (int i = 0; i <= 7; i++) {
    switch (samples[i]) {
    case 1:
      sampleLHS = 0b10000000;
      sampleRHS = 0b00000000;
      break;
    case 2:
      sampleLHS = 0b11000000;
      sampleRHS = 0b00000000;
      break;
    case 3:
      sampleLHS = 0b11100000;
      sampleRHS = 0b00000000;
      break;
    case 4:
      sampleLHS = 0b11110000;
      sampleRHS = 0b00000000;
      break;
    case 5:
      sampleLHS = 0b11111000;
      sampleRHS = 0b00000000;
      break;
    case 6:
      sampleLHS = 0b11111100;
      sampleRHS = 0b00000000;
      break;
    case 7:
      sampleLHS = 0b11111110;
      sampleRHS = 0b00000000;
      break;
    case 8:
      sampleLHS = 0b11111111;
      sampleRHS = 0b00000000;
      break;
    case 9:
      sampleLHS = 0b11111111;
      sampleRHS = 0b10000000;
      break;
    case 10:
      sampleLHS = 0b11111111;
      sampleRHS = 0b11000000;
      break;
    case 11:
      sampleLHS = 0b11111111;
      sampleRHS = 0b11100000;
      break;
    case 12:
      sampleLHS = 0b11111111;
      sampleRHS = 0b11110000;
      break;
    case 13:
      sampleLHS = 0b11111111;
      sampleRHS = 0b11111000;
      break;
    case 14:
      sampleLHS = 0b11111111;
      sampleRHS = 0b11111100;
      break;
    case 15:
      sampleLHS = 0b11111111;
      sampleRHS = 0b11111110;
      break;
    case 16:
      sampleLHS = 0b11111111;
      sampleRHS = 0b11111111;
      break;
    default:
      sampleLHS = 0b00000000;
      sampleRHS = 0b00000000;
      break;
    }
    writeMatrix(sampleRHS, sampleLHS, i);
  }
}

void LEDMatrix::test() {
  for (int i = 7; i >= 0; i--) {
    writeMatrix(0xff, 0xff, i);
    thread_sleep_for(100);
  }
  for (int i = 0; i <= 7; i++) {
    writeMatrix(0xff, 0xff, i);
    thread_sleep_for(100);
  }
  clear();
}

void LEDMatrix::update(int updatedSamples[]) {
  for (int i = 0; i <= 7; i++) {
    samples[i] = updatedSamples[i];
  }
}

void LEDMatrix::matrixThread() {
  while (true) {
    plot();
  }
}
