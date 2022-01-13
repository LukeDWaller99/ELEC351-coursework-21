/**
@file SerialIn.h
Serial Input class header file.
**/

#ifndef __SERIAL_IN__
#define __SERIAL_IN__

#include "CustomQueue.h"
#include "mbed.h"
#include "sampling.h"
#include "buffer.h"

/**
Thread-safe Serial Input class.
Instructions for use outputted to the serial terminal on start-up
**/
class SerialIn{
    private:
    CustomQueue* pQ;            ///< Pointer to the custom queue for status outputs
    sampler* serialSampler;     ///< Pointer to the sampler to change threshold limits
    bufferClass* serialBuff;    ///< Pointer to the buffer to flush, get samples, and number of samples
    Thread SerialWatcher;       ///< Thread for the main Serial Inputs
    

    public:
    /**
    Construct the Serial In object. This constructor must be given pointers to the custom print queue, sampler, and buffer.
    This allows the class to communicate with the sampler, buffer, and print queue.
    This class will not function without these three arguments and no other constructor is provided.
    @code
    SerialIn serialInput(&printQueue, &SampleModule, &mainBuffer);
    @endcode
    @param pintQueue - pointer to a print queue object to allow the use of a print to output information
    @param serialSamples - pointer to the sampler object to allow changes to the input thresholds
    @param serialBuffer - pointer to the buffer object to allow communication between the buffer and serial input
    Instantiates the Serial In class, outputs the serial instructions, and starts the SerialWatcher thread.
    **/
    SerialIn(CustomQueue* printQueue, sampler* serialSamples, bufferClass* serialBuffer);
    /**
        Main Functionality of the Serial In class. Takes the Serial Data and deicdes what to do from the basic set of instructions.
        The appropriate action is then applied. 
        This function is a blocking function because of the scanf. As it is in its own thread, it does inhibit the other operations.
    **/
    void SerialListener();
    /**
        Prints onto the serial terminal the methods that can be called from the serial terminal.
    **/
    void SerialInstructions();
    /**
        Test code to check the serial inputs and outputs are working correctly
    **/
    void SerialTest();
    /**
        Prints onto the serial terminal the instructions required to use the various functions
    **/ 
    void Help();
};

#endif