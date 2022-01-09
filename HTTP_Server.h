#ifndef __HTTP_SERVER__
#define __HTTP_SERVER__

// #include "EthernetInterface.h"
// #include "TCPSocket.h"
// #include <ctime>
// #include <iostream>
// #include <string>
// #include "Thread.h"
// #include "mbed.h"
// #include "uop_msb.h"
// #include "sampling.h"
// using namespace uop_msb;
// using namespace std;

// #define HTTP_STATUS_LINE "HTTP/1.0 200 OK"
// #define HTTP_HEADER_FIELDS "Content-Type: text/html; charset=utf-8"
// #define HTTP_MESSAGE_BODY ""                                     \
// "<html>" "\r\n" \
// "<h1 style=\"text-align: center;\">ELEC351 Coursework webpage - Group D</h1>" "\r\n" \
// "   <p style=\"text-align: center;\">Date and Time: {{1}}</p>" "\r\n" \
// "<h2 style=\"text-align: center;\">Peripherals</h2>"   "\r\n" \
// "   <table style=\"text-align: center; table-layout: fixed; width: 100%;\">""\r\n" \
// "       <tbody>" "\r\n" \
// "       <tr>"                                                               "\r\n" \
// "           <th style=\"border: 1px solid black;\">POT</th>"                "\r\n" \
// "           <th style=\"border: 1px solid black;\">LDR</th>"                "\r\n" \
// "           <th style=\"border: 1px solid black;\">PRESSURE</th>"           "\r\n" \
// "           <th style=\"border: 1px solid black;\">TEMPERATURE</th>"        "\r\n" \
// "       </tr>"                                                              "\r\n" \
// "       <tr>"                                                               "\r\n" \
// "           <td style=\"border: 1px solid black;\">{{2}}</td>"              "\r\n" \
// "           <td style=\"border: 1px solid black;\">{{3}}</td>"              "\r\n" \
// "           <td style=\"border: 1px solid black;\">{{4}}</td>"              "\r\n" \
// "           <td style=\"border: 1px solid black;\">{{5}}</td>"              "\r\n" \
// "       </tr>"                                                              "\r\n" \
// "       </tbody>"                                                           "\r\n" \
// "   </table>"                                                               "\r\n" \
// "</html>"                                                                   "\r\n"
    
// #define HTTP_TEMPLATE HTTP_STATUS_LINE "\r\n"   \
//                       HTTP_HEADER_FIELDS "\r\n" \
//                       "\r\n"                    \
//                       HTTP_MESSAGE_BODY "\r\n"

// class HTTP_server{

//     private:
//     EthernetInterface network;
//     SocketAddress address;
//     TCPSocket socket;
//     TCPSocket* clt_socket;
//     sampler DataSampler;
    

//     public:
//     HTTP_server();
//     Thread HTTP_thread;
//     void HTTP_server_thread(void);
// };

// struct TandD{
//     time_t timestampVal;
// };


#endif