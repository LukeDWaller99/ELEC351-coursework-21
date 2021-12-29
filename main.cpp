/*
 * Copyright (c) 2020 Arm Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "mbed_wait_api.h"
#include "uop_msb.h"
#include "rtos/ThisThread.h"
#include "NTPClient.h"
#include "azure_c_shared_utility/xlogging.h"
#include <cstring>
#include <string.h>
#include <LEDMatrix.h>
#include "SevenSegmentDisplay.h"
using namespace uop_msb;

LEDMatrix matrix;
SevenSegmentDisplay display;
EnvSensor sensor;
AnalogIn LDR(AN_LDR_PIN);
Buzzer buzz;
char note = C;
char note2 = D;
char note3 = E;
char* notename,notename2,notename3;



int main() {
    notename = &note;
    notename2 = note2;
    notename3 = note3;
    display.test();
    matrix.test();
    buzz.playTone(notename);
    wait_us(1000000);
    buzz.rest();
    buzz.playTone(&note2,uop_msb::Buzzer::HIGHER_OCTAVE);
    wait_us(1000000);
    buzz.rest();
    buzz.playTone(&note3,uop_msb::Buzzer::LOWER_OCTAVE);
    wait_us(1000000);
    buzz.rest();
    while (true){
    wait_us(1000000);
    printf("Temp = %f \n",sensor.getTemperature());
    printf("Humidity = %f \n",sensor.getHumidity());
    printf("Pressure = %f \n",sensor.getPressure());
    printf("Light = %f \n", LDR.read());
    }
}
