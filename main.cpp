/*
 * Copyright (c) 2020 Arm Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "mbed_wait_api.h"
#include "uop_msb.h"
#include "rtos/ThisThread.h"
#include "NTPClient.h"
#include <cstring>
#include <string.h>
#include <LEDMatrix.h>
#include "SevenSegmentDisplay.h"
#include <ErrorHandler.h>
#include "CustomQueue.h"
using namespace uop_msb;
//EventQueue* queue = new EventQueue();
CustomQueue queue;

ErrorHandler EH(&queue);
LEDMatrix matrix;
Thread t;


int main() {
    while(true)
    {
    wait_us(100000);
    EH.setErrorFlag(T_UPPER);
    wait_us(5000000);
    EH.setErrorFlag(ALL_CLEAR);
    wait_us(100000);
    EH.setErrorFlag(EMPTY_FLUSH);
    wait_us(5000000);
    EH.setErrorFlag(ALL_CLEAR);
    wait_us(5000000);
    EH.setErrorFlag(BUFFER_FULL);
    }
}
