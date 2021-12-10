#ifndef H_ERROR_HANDLER
#define H_ERROR_HANDLER
#include "DigitalOut.h"
#include "mbed.h"
#include "Mutex.h"
#include <uop_msb.h>
using namespace uop_msb;


#define ERROR_THREAD_NAME eHandler

//error codes
#define BUFFER_FULL = 10               //CRITICAL
#define BUFFER_LOCK_TIMEOUT = 11       //CRITICAL
#define TIMER_LOCK_TIMEOUT = 12        //CRITICAL
#define EMPTY_FLUSH = 13               //WARNING
#define BUFFER_FLUSH_TIMEOUT = 14      

//sd card errors
#define MOUNT_ERROR = 20        //checked if card mounted, it is not
#define NO_SD_FILE = 21         //cannot find a new file to write in
#define UNMOUNTED_FLUSH = 22    //you are trying to flush to an empty sd card slot

//error handler errors
#define FLAG_CLEAR_ERROR 99

class error_handler {
    private:
    DigitalOut yellowLED = TRAF_YEL1_PIN;
    DigitalOut redLED = TRAF_RED1_PIN;
    Mutex flagLock;
    static int flag_value;
    void clear_all();

    public:
    error_handler();
    ~error_handler();
    static void error_thread();
    void severityHandler();

    enum errorSeverity{
    CRITICAL, WARNING
};


};





#endif

