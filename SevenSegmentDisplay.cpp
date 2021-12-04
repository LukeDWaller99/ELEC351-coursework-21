#include "SevenSegmentDisplay.h"
#include "iothub_message_private.h"
#include "mbed_retarget.h"
#include "mbed_wait_api.h"

#define OFF 0b00000000;
#define ZERO 0b01111011;
#define ON 0b11111111;
#define ONE 0b01100000;
#define TWO 0b00111110;
#define THREE 0b01111100;
#define FOUR 0b01100101;
#define FIVE 0b01011101;
#define SIX 0b01011111;
#define SEVEN 0b01110000;
#define EIGHT 0b01111111;
#define NINE 0b01110101;
#define decimal 0b10000000;

SevenSegmentDisplay::SevenSegmentDisplay()
    : digits(PE_2, PE_3, PE_4, PE_5, PE_6, PE_7, PE_8, PE_9),
      display(PE_10, PE_11), output_enable(PE_15) {}

void SevenSegmentDisplay::clear() {
  /*
  Clears both the seven segment displays
  */

  output_enable = 1;
  display = 0b11;
  digits = OFF;
  output_enable = 0;
}

void SevenSegmentDisplay::clear(int number) {
  display = number;
  output_enable = 1;
  digits = OFF;
  output_enable = 0;
}

void SevenSegmentDisplay::setDigit(int digit, int number) {
  /*
  Set the number of a given seven segment display

  Args:
      digit (int) : sets which seven segment displayto write to
      number (int) : sets the number to output in the 7 segmenet to display
  */
  int output = 0;
  switch (number) {
  case 1:
    output = ONE;
    break;
  case 2:
    output = TWO;
    break;
  case 3:
    output = THREE;
    break;
  case 4:
    output = FOUR;
    break;
  case 5:
    output = FIVE;
    break;
  case 6:
    output = SIX;
    break;
  case 7:
    output = SEVEN;
    break;
  case 8:
    output = EIGHT;
    break;
  case 9:
    output = NINE;
    break;
  default:
    output = ZERO;
    break;
  }
  display = digit;
  output_enable = 1;
  digits = output;
  output_enable = 0;
}

void SevenSegmentDisplay::setNumber(int number) {
  if (number == 0) {
    setDigit(1, 0);
    setDigit(2, 0);
  }
  while (number > 0) {
    for (int i = 2; i > 0; i--) {
      int digit = number % 10;
      number /= 10;
      setDigit(i, digit);
    }
  }
}

void SevenSegmentDisplay::test() {
  for (int i = 0; i < 100; i = i + 11) {
    setNumber(i);
    wait_us(500000);
  }
}

void SevenSegmentDisplay::operator=(int number)
{
    setNumber(number);
}