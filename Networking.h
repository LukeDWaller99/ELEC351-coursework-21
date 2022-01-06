#ifndef __NETWORKING__
#define __NETWORKING__

#include "EthernetInterface.h"
#include "NetworkStack.h"
#include "SocketAddress.h"
#include "TCPSocket.h"
#include "net_interface.h"
#include "Thread.h"
#include "mbed.h"
#include <uop_msb.h>
#include <cstdio>
#include "Callback.h"
#include "ThisThread.h"

#define HTTP_STATUS_LINE "HTTP/1.0 200 OK"
#define HTTP_HEAER_FIELDS "Content-Type: text/html; charset=utf-8"

#define HTTP_MESSAGE_BODY ""                                                       \
"<html>"                                                                    "\r\n" \
"<h1 style="text-align: center;">ELEC351 Coursework webpage</h1>"           "\r\n" \
"   <p style=\"text-align: center;\">Time: {{0}}</p>"                       "\r\n" \
"   <p style=\"text-align: center;\">Date: {{1}}</p>"                       "\r\n" \
"<h2 style=\"text-align: center;\">Peripherals</h2>"                        "\r\n" \
"   <table style=\"text-align: center; table-layout: fixed; width: 100%;\">""\r\n" \
"       <tbody>"                                                            "\r\n" \
"       <tr>"                                                               "\r\n" \
"           <th style=\"border: 1px solid black;\">POT</th>"                "\r\n" \
"           <th style=\"border: 1px solid black;\">LDR</th>"                "\r\n" \
"           <th style=\"border: 1px solid black;\">PRESSURE</th>"           "\r\n" \
"           <th style=\"border: 1px solid black;\">TEMPRATURE</th>"         "\r\n" \
"       </tr>"                                                              "\r\n" \
"       <tr>"                                                               "\r\n" \
"           <td style=\"border: 1px solid black;\">{{3}}</td>"              "\r\n" \
"           <td style=\"border: 1px solid black;\">{{4}}</td>"              "\r\n" \
"           <td style=\"border: 1px solid black;\">{{5}}</td>"              "\r\n" \
"           <td style=\"border: 1px solid black;\">{{6}}</td>"              "\r\n" \
"       </tr>"                                                              "\r\n" \
"       </tbody>"                                                           "\r\n" \
"   </table>"                                                               "\r\n" \
"</html>"                                                                   "\r\n"

class NetworkConnection{

    private:
    SocketAddress IP;
    EthernetInterface network;

    public:
    Thread NetworkingThread;
    NetworkConnection();
    ~NetworkConnection();
    void getTime();
    void networkingThread();

};

#endif