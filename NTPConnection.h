/**
@file NTPConnection.h
NTP Connection class header file.
**/

#ifndef __NTP_CLIENT__
#define __NTP_CLIENT__

#include "CustomQueue.h"
#include "ErrorHandler.h"
#include "NTPClient.h"
#include <ctime>

/// Macro to control the HTTP Server usage
/// Set Macro to 0 if HTTP Server not being used, otherwise set to 1
/// Pre-processer directives will eiother try to connect the NTP server or use existing connection
#define HTTP_SERVER_USED 1

/**
NTP Connection class. 
While this class is not thread safe. The way it is called ensures that is will not interrupt any other processes.
This class is primarily used during the Monitor's initialisation stage, and so will not interfere with any of the 
real-time operations.
**/
class NTPConnection{

    private:
    /// pointer to the Network Interface 
    NetworkInterface* NTPInterface;
    /// pointer to the output error queue
    CustomQueue* printQueue;
    ///pointer to the error handler
    ErrorHandler* errorHandler;


    public:
    time_t timestamp; ///< variable of time_t to save the value obtained by the NTP Server
    /**
    Construct the Network Connecion obejct. This constructor must be given a pointer to an event queue and
    the Error Handler. This allows it report errors and output the current state of the connection.
    This class will not function without these two arguments and no other constructor is provided.
    @code
    CustomQueue* printQueue = new CustomQueue();
    ErrorHandler EH(&printQueue);
    NTPConnection NTP(&printQueue, &EH);
    @endcode
    @param printQueue - pointer to a print queue object to allow the to use a print to output information
    @param errorHanlder - point to an error handler object to allow the NTP to raise errors 
    Connects to NTP server to get current date and time and stores this locally
    **/
    NTPConnection(CustomQueue* printQueue, ErrorHandler* errorHandler);
    /**
        Get time the current local time
    **/
    time_t getTime();

};

#endif