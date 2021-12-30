#ifndef H_ERROR_HANDLER
#define H_ERROR_HANDLER
#include "DigitalOut.h"
#include "SevenSegmentDisplay.h"
#include "Thread.h"
#include "mbed.h"
#include "Mutex.h"
#include <uop_msb.h>
using namespace uop_msb;

//ERROR CODE FORMAT
//TWELVE BITS
//  11->8   Severity Code
//  7->4    Module Code
//  3->0    Error ID
//SEVERITY CODES
// 0x0 - WARNING
// 0x1 - CRITICAL
// 0x2 - FATAL
// 0x3 - BUFF_FULL
// 0x4 - ENV_ERROR

//error codes - 10s
#define BUFFER_FULL             0x110   //CRITICAL
#define BUFFER_LOCK_TIMEOUT     0x111   //CRITICAL
#define TIMER_LOCK_TIMEOUT      0x112   //CRITICAL
#define EMPTY_FLUSH             0x013   //WARNING
#define BUFFER_FLUSH_TIMEOUT    0x114   //CRITICAL 

//sd card errors - 20s - All Critical
#define MOUNT_ERROR             0x120      //checked if card mounted, it is not
#define NO_SD_FILE              0x121      //cannot find a new file to write in
#define UNMOUNTED_FLUSH         0x122      //you are trying to flush to an empty sd card slot

//ENV_ERRORS - 30s
#define T_LOWER 0x430 
#define T_UPPER 0x431
#define P_LOWER 0x433
#define P_UPPER 0x434
#define L_LOWER 0x435
#define L_UPPER 0x436

//error handler errors
#define FLAG_CLEAR_ERROR 0x299 // FATAL ERROR


class ErrorHandler {
    private:
    enum errorSeverity{
    WARNING = 0x0, 
    CRITICAL = 0x1, 
    FATAL = 0x2,
    BUFF_FULL = 0x3,
    ENV_ERR = 0x4};
    typedef void(*funcPointer_t)(void);
    DigitalOut yellowLED = TRAF_YEL1_PIN;
    DigitalOut redLED = TRAF_RED1_PIN;
    Buzzer buzz;
    SevenSegmentDisplay errorDisplay;
    Mutex flagLock;
    int alarm_status=0;
    char note = 'C';
    void clear_all();
    //void alarm(errorSeverity severity);
    //void alarmstop();
    

    public:
    Thread ERROR_THREAD_NAME;
    EventQueue errorQueue;
    ErrorHandler();
    ~ErrorHandler();
    void error_thread();
    void severityHandler();
    void setErrorFlag(int errorCode);
    //void alarmtest();



};

#endif
