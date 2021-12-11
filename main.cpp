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

int main() {
    display.test();
    matrix.test();
    while (true){
    wait_us(1000000);
    printf("Temp = %f \n",sensor.getTemperature());
    printf("Humidity = %f \n",sensor.getHumidity());
    printf("Pressure = %f \n",sensor.getPressure());
    printf("Light = %f \n", LDR.read());
    }
}
