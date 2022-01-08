/**
@file ErrorHandler.h
Error Handler class header file
**/

#ifndef H_ERROR_HANDLER
#define H_ERROR_HANDLER
#include "DigitalOut.h"
#include "SevenSegmentDisplay.h"
#include "Thread.h"
#include "mbed.h"
#include "Mutex.h"
#include <uop_msb.h>
using namespace uop_msb;
///BUZZER ENABLE
///set to 0 when using networking
///pre-processor directives will replace buzzer calls with queued print dispatches.
#define BUZZER_ENABLE 0


// ERROR CODE FORMAT
// TWELVE BITS
//   11->8   Severity Code
//   7->4    Module Code
//   3->0    Error ID
// SEVERITY CODES
//  0x0 - WARNING
//  0x1 - CRITICAL
//  0x2 - FATAL
//  0x4 - ENV_ERROR
//  0x9 - ALL CLEAR

//error codes - 10s
#define BUFFER_FULL             0x110   ///FULL BUFFER - SEVERITY CRITICAL
#define BUFFER_LOCK_TIMEOUT     0x111   ///BUFFER LOCK TIMEOUT - CRITICAL
#define TIMER_LOCK_TIMEOUT      0x112   ///TIMER LOCK TIMEOUT - CRITICAL
#define EMPTY_FLUSH             0x013   ///FLUSH WHEN EMPTY - WARNING
#define BUFFER_FLUSH_TIMEOUT    0x114   ///FAILED FLUSH - CRITICAL

//sd card errors - 20s - All Critical
#define MOUNT_ERROR             0x120      ///NO SD CARD MOUNTED - CRITICAL
#define NO_SD_FILE              0x121      ///NO FILE TO WRITE TO - CRITICAL
#define UNMOUNTED_FLUSH         0x122      ///SD CARD SLOT EMPTY, CANNOT FLUSH - CRITICAL

//ENV_ERRORS - 30s
#define T_LOWER 0x430   ///Lower temperature threshold exceeded.
#define T_UPPER 0x431   ///Upper temperature threshold exceeded.
#define P_LOWER 0x433   ///Lower pressure threshold exceeded.
#define P_UPPER 0x434   ///Upper pressure threshold exceeded.
#define L_LOWER 0x435   ///Lower light threshold exceeded.
#define L_UPPER 0x436   ///Upper light threshold exceeded.

//error handler errors
#define FLAG_CLEAR_ERROR 0x299  ///Flag clear error, immediate reset

#define ALL_CLEAR 0x999 ///All clear from modules

/**
Thread-safe error handler class.
**/
class ErrorHandler {
    private:
    /**
    Enumerated value for storing the error severity.
    Corresponds to the first four bits of each error code. All severities will display the error code on the seven segment display.
    **/
    enum errorSeverity{
    WARNING = 0x0,  ///Lights a yellow LED. 
    CRITICAL = 0x1, ///Lights a red LED, sounds a 30 second alarm, and performs a reset of the board.
    FATAL = 0x2,    ///Immediate reset of the board.
    BUFF_FULL = 0x3,///Legacy severity for testing buffer integration.
    ENV_ERR = 0x4,  ///Sounds a buzzer for three seconds.
    CLEAR = 0x9};   ///Clears all outputs.
    ///Function pointer for callbacks
    typedef void(*funcPointer_t)(void);
    DigitalOut yellowLED = TRAF_YEL1_PIN;
    DigitalOut redLED = TRAF_RED1_PIN;
    #if BUZZER_ENABLE == 1
    Buzzer buzz;
    char note = 'C';
    #endif
    ///Seven Segment Display used for displaying the active error code
    SevenSegmentDisplay errorDisplay;
    ///Error flag clearing lock.
    Mutex flagLock;
    ///Pointer to the output error queue
    EventQueue* queue;
    int alarm_status=0;
    /**Function for clearing the Error Handler's thread flags safely
    **/
    void clear_all();
    

    public:
    Thread ERROR_THREAD_NAME;
    /**
    Construct an ErrorHandler object.
    @param errorQueue - Pointer to an eventQueue object to be used for printing the 
                        error messages to a serial output device
    **/
    ErrorHandler(EventQueue* errorQueue);
    ~ErrorHandler();
    /**
    Main error handler functionality. Waits for a flag to be set using setErrorFlag, before
    responding with the appropriate outputs based on the error severity.
    **/
    void error_thread();

    /**
    Public method for passing errors to the error handler. 
    @param errorCode Error code value, should use one of the specified macros in ErrorHandler.h
    **/
    void setErrorFlag(int errorCode);

};

#endif
