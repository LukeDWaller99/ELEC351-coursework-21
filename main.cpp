/*
 * Copyright (c) 2020 Arm Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "uop_msb.h"
#include "rtos/ThisThread.h"
#include "NTPClient.h"
#include "azure_c_shared_utility/xlogging.h"
#include <cstring>
#include <string.h>
using namespace uop_msb;

#include "buffer.h"
#include "sd.h"
#include "printQueue.h"

//threads
Thread samplingThread;



int main() {
    //run hardware test 

    //start clock
    //samplingThread.start(fetchSample);

    // return 0;
}

