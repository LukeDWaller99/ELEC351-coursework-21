/**
@file HTTP_Server.h
HTTP Server class header file.
**/

#ifndef __HTTP_SERVER__
#define __HTTP_SERVER__

#include "EthernetInterface.h"
#include "TCPSocket.h"
#include <string>
#include "Thread.h"
#include "mbed.h"
#include "sampling.h"
#include "uop_msb.h"
#include "CustomQueue.h"
#include "ErrorHandler.h"
#include "sampling.h"
using namespace uop_msb;
using namespace std;

#define HTTP_STATUS_LINE "HTTP/1.0 200 OK" ///< shows that the responce to the broswer was successful - must return a payload
#define HTTP_HEADER_FIELDS "Content-Type: text/html; charset=utf-8" ///< Provides required imformation about te responce
/// Body of the HTML message the will make up the webpage
#define HTTP_MESSAGE_BODY ""                                     \
"<html>" "\r\n" \
"<h1 style=\"text-align: center;\">ELEC351 Coursework webpage - Group D</h1>" "\r\n" \
"   <p style=\"text-align: center;\">Date and Time: {{1}}</p>" "\r\n" \
"<h2 style=\"text-align: center;\">Peripherals</h2>"   "\r\n" \
"   <table style=\"text-align: center; table-layout: fixed; width: 100%;\">""\r\n" \
"       <tbody>" "\r\n" \
"       <tr>"                                                               "\r\n" \
"           <th style=\"border: 1px solid black;\">POT</th>"                "\r\n" \
"           <th style=\"border: 1px solid black;\">LDR</th>"                "\r\n" \
"           <th style=\"border: 1px solid black;\">PRESSURE</th>"           "\r\n" \
"           <th style=\"border: 1px solid black;\">TEMPERATURE</th>"        "\r\n" \
"       </tr>"                                                              "\r\n" \
"       <tr>"                                                               "\r\n" \
"           <td style=\"border: 1px solid black;\">{{2}}</td>"              "\r\n" \
"           <td style=\"border: 1px solid black;\">{{3}}</td>"              "\r\n" \
"           <td style=\"border: 1px solid black;\">{{4}}</td>"              "\r\n" \
"           <td style=\"border: 1px solid black;\">{{5}}</td>"              "\r\n" \
"       </tr>"                                                              "\r\n" \
"       </tbody>"                                                           "\r\n" \
"   </table>"                                                               "\r\n" \
"</html>"                                                                   "\r\n"

/// Structure of the HTTP responce being sent to the broswer. 
#define HTTP_TEMPLATE HTTP_STATUS_LINE "\r\n"   \
                      HTTP_HEADER_FIELDS "\r\n" \
                      "\r\n"                    \
                      HTTP_MESSAGE_BODY "\r\n"

class HTTP_server{

    private:
    EthernetInterface network;  ///< 
    TCPSocket socket;           ///< 
    TCPSocket* client_socket;   ///< 
    SocketAddress address;      ///<

    sampler* webDataSampler; 

    samples sampledData;
    
    public:
    /**
    Construct the HTTP Server obejct. This constructor must be given a pointer to an event queue and
    the Error Handler. This allows it report errors and output the current state of the connection.
    This class will not function without this two arguments and no other constructor is provided.
    @code
    CustomQueue* printQueue = new CustomQueue();
    ErrorHandler EH(&printQueue);
    HTTP_Server (&printQueue, &EH);
    @endcode
    @param printQueue - pointer to a print queue object to allow the to use a print to output information
    @param errorHanlder - point to an error handler object to allow the NTP to raise errors 
    Connects to NTP server to get current date and time and stores this locally
    **/
    HTTP_server(CustomQueue* printQueue, ErrorHandler* errorHandler, sampler* webSampler);
    /**
        HTTP Server thread. Waits for a request to be sent to the HTTP interface. 
        Accepts the Client Socket
        Constructs the webpage HTML and returns this along with a 200 OK Message
        Once completed it closes the Client Socket and waits for another request.
    **/
    void HTTP_server_thread(void);

    
    Thread HTTP_thread;
};

#endif