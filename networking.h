#ifndef __NETWORKING__
#define __NETWORKING__

#include "mbed.h"
#include "EthernetInterface.h"
#include "TCPServer.h"
#include "TCPSocket.h"
#include <iostream>
#include <string> 


#include "buffer.h"

#define IP        "10.0.0.10"
#define NETMASK   "255.0.0.0"
#define GATEWAY   "10.0.0.1"


extern void test_connection();


#endif