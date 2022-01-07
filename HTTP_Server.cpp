#include "HTTP_Server.h"
#include <cstddef>



HTTP_server::HTTP_server() {

  printf("Basic HTTP server example\n");

  network.connect();

  network.get_ip_address(&address);

  // print the IP address of the network - if no IP output NONE
  printf("IP address: %s\n",
         address.get_ip_address() ? address.get_ip_address() : "None");

  // open a TCP socket on the networking interface bnound to port 80
  socket.open(&network);
  socket.bind(80);

  int error = socket.listen(5);
  if (error == 0) {
    printf("Listening OK\n\r");
  } else {
    printf("Listen Error = %d\n\r", error);
    socket.close();
    // ERROR
    return;
  }

  HTTP_thread.start(callback(this, &HTTP_server::HTTP_server_thread));
};

void HTTP_server::HTTP_server_thread(void) {

    AnalogIn pot(PA_0);

  printf("HTTP THREAD STARTED\n");
  while (true) {

    printf("Waiting\n");
    clt_socket = socket.accept();

    printf("Connected...\n");

    //constructing the webpage
    float potVal = pot;

    char buff[6];
    sprintf(buff, "%5.3f", potVal);

    string html = string(HTTP_TEMPLATE);

    size_t index = html.find("{{2}}"); //find the placeholder {{2}}
    if (index){
        html.replace(index, 5, buff);
    }

    nsapi_size_or_error_t ret =
        clt_socket->send(html.c_str(), strlen(html.c_str()));

    printf("Sent %d bytes\n", ret);

    clt_socket->close();

    ThisThread::sleep_for(1s);
  }
}