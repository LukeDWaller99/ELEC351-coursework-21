// #include "HTTP_Server.h"

// //samples sampledData1;

// HTTP_server::HTTP_server() {

//   printf("Basic HTTP server example\n");

//   network.connect();

//   network.get_ip_address(&address);

//   // print the IP address of the network - if no IP output NONE
//   printf("IP address: %s\n",
//          address.get_ip_address() ? address.get_ip_address() : "None");

//   // open a TCP socket on the networking interface bnound to port 80
//   socket.open(&network);
//   socket.bind(80);

//   int error = socket.listen(5);
//   if (error == 0) {
//     printf("Listening OK\n\r");
//   } else {
//     printf("Listen Error = %d\n\r", error);
//     socket.close();
//     // ERROR
//     return;
//   }

//   HTTP_thread.start(callback(this, &HTTP_server::HTTP_server_thread));
// };

// void HTTP_server::HTTP_server_thread(void) {

//     AnalogIn pot(PA_0);

//   printf("HTTP THREAD STARTED\n");
//   while (true) {

//     printf("Waiting\n");
//     clt_socket = socket.accept();

//     printf("Connected...\n");

//     //constructing the webpage
//     float potVal = pot;
//     float LDRVal = sampledData.LDR;
//     float pressureVal = sampledData.pressure;
//     float temperatureVal = sampledData.temp;

//     time_t timestamp;

//     timestamp = time(NULL);

//     char potBuff[6];
//     char LDRBuff[6];
//     char pressureBuff[6];
//     char temperatureBuff[6];
    
//     sprintf(potBuff, "%5.3f", potVal);
//     sprintf(LDRBuff, "%5.3f", LDRVal);
//     sprintf(pressureBuff, "%5.3f", pressureVal);
//     sprintf(temperatureBuff, "%5.3f", temperatureVal);

//     string html = string(HTTP_TEMPLATE);

//     size_t timeDateIndex = html.find("{{1}}");
//     if (timeDateIndex){
//         html.replace(timeDateIndex, 5, ctime(&timestamp));
//     }

//     size_t potIndex = html.find("{{2}}"); //find the placeholder {{2}}
//     if (potIndex){
//         html.replace(potIndex, 5, potBuff);
//     }

//     size_t LDRIndex = html.find("{{3}}");
//     if (LDRIndex){
//         html.replace(LDRIndex, 5, potBuff);
//     }

//     size_t pressureIndex = html.find("{{4}}");
//     if (pressureIndex){
//         html.replace(pressureIndex, 5, potBuff);
//     }

//     size_t temperatureIndex = html.find("{{5}}");
//     if (temperatureIndex){
//         html.replace(temperatureIndex, 5, potBuff);
//     }

//     nsapi_size_or_error_t ret =
//         clt_socket->send(html.c_str(), strlen(html.c_str()));

//     printf("Sent %d bytes\n", ret);

//     clt_socket->close();

//     ThisThread::sleep_for(1s);
//   }
// }
