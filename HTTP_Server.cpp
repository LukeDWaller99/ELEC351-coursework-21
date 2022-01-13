#include "HTTP_Server.h"


samples webData;

HTTP_server::HTTP_server(CustomQueue* printQueue, ErrorHandler* errorHandler, sampler* webSampler) {

    printQueue = printQueue;
    webDataSampler = webSampler;

    printQueue->custom.call(printf, "Opening HTTP Server\n");

    // checks there is currently no ethernet connection active
    if (address == ""){
    // connect to the ethernet interface 
    network.connect();
    }


    // get the network IP address 
    network.get_ip_address(&address);
    // if network interface failed to connect - IP will still be NULL so throw connection error
    if (address == "") {
        errorHandler->setErrorFlag(CONNECTION_ERROR);
    }

    //   print the IP address of the network - if no IP output NONE
    printQueue->custom.call(printf, "IP address: %s\n", address.get_ip_address() ? address.get_ip_address() : "None");

    // if IP address is none - raise a NO_IP_ADDRESS error
    if(address == "None" ){
        errorHandler->setErrorFlag(NO_IP_ADDRESS);
    }

    // open a TCP socket on the networking interface bound to port 80
    socket.open(&network);
    socket.bind(80);

    // sets the socket to listening mode (up to 5 connections)
    int error = socket.listen(5);
    if (error == 0) {
        printQueue->custom.call(printf, "Listening OK\n\r");
    } else {
        printQueue->custom.call(printf, "Listener Error = %d\n\r", error);
        socket.close();
        errorHandler->setErrorFlag(LISTENER_ERROR);
        return;
    }

    HTTP_thread.start(callback(this, &HTTP_server::HTTP_server_thread));
};

void HTTP_server::HTTP_server_thread(void) {

    AnalogIn pot(PA_0);

    while (true) {

        // waits in the blocking state till for an active connection to be accepted
        client_socket = socket.accept();

        //constructing the webpage
        webData = webDataSampler->sampleData;
        float potVal = pot;
        float LDRVal = webData.LDR;
        float pressureVal = webData.pressure;
        float temperatureVal = webData.temp;

        // get the timestamp 
        time_t timestamp;

        timestamp = time(NULL);

        char potBuff[10];
        char LDRBuff[10];
        char pressureBuff[10];
        char temperatureBuff[10];
        
        // get the values and store them as strings
        sprintf(potBuff, "%1.3f", potVal);
        sprintf(LDRBuff, "%1.3f", LDRVal);
        sprintf(pressureBuff, "%1.1f", pressureVal);
        sprintf(temperatureBuff, "%1.3f", temperatureVal);

        string html = string(HTTP_TEMPLATE);

        // find and replace the values in the HTML template before sending them to the server

        size_t timeDateIndex = html.find("{{1}}"); //find the placeholder {{1}}
        if (timeDateIndex){
            html.replace(timeDateIndex, 5, ctime(&timestamp));
        }

        size_t potIndex = html.find("{{2}}"); //find the placeholder {{2}}
        if (potIndex){
            html.replace(potIndex, 5, potBuff);
        }

        size_t LDRIndex = html.find("{{3}}"); //find the placeholder {{3}}
        if (LDRIndex){
            html.replace(LDRIndex, 5, LDRBuff);
        }

        size_t pressureIndex = html.find("{{4}}"); //find the placeholder {{4}}
        if (pressureIndex){
            html.replace(pressureIndex, 5, pressureBuff);
        }

        size_t temperatureIndex = html.find("{{5}}"); //find the placeholder {{5}}
        if (temperatureIndex){
            html.replace(temperatureIndex, 5, temperatureBuff);
        }

        // send the HTML to the web browser 
        client_socket->send(html.c_str(), strlen(html.c_str()));

        // our respsonsibilty to to close the socket - not the web borwser's
        client_socket->close();

        ThisThread::sleep_for(1s);
  }
}