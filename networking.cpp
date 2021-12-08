#include "mbed.h"
#include "networking.h"
using namespace std;

void test_connection(){
    EthernetInterface ethernet;
    ethernet.set_network(IP, NETMASK, GATEWAY);
    ethernet.connect;
    printf("\n\r IP ADDRESS: '%s'\n\r", ethernet.get_ip_address)));

    //setting up the server
    TCPSocket srv;
    TCPSocket* clt_sock;
    SocketAddress clt_addr;

    //open server
    srv.open(&ethernet);

    //bind ports
    srv.bind(ethernet.get_ip_address(), 8080);

    srv.listen(3); //3 connections, 3 of our boards?

    while (true){
        clt_sock = srv.accept(); //accepts and wait for incoming

        string dataAcquired;
        char (dateString)[64];
        char (timeString)[64];
        char (tempString)[64];
        char (LDRString)[64];
        char (pressureString)[64];

        recordHTML HTMLData = createHTMLRecord();

        //display values
        sprintf(dateString, "%d/%d/%d\n\r", HTMLData.day, HTMLData.month, HTMLData.year);
        //and so on

        dataAcquired = HTTP_BODY_1;
        dataAcquired += dateString;

        //and so on
    }

}