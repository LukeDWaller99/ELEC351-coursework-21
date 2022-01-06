#include "Networking.h"

NetworkConnection::NetworkConnection() {
    NetworkingThread.start(callback(this, &NetworkConnection::networkingThread));
};

NetworkConnection::~NetworkConnection(){
}
void NetworkConnection::getTime() {
    // ACCEPT INCOMING CONNECTIONS
//   TCPSocket *clt_sock = Socket.accept();
}

void NetworkConnection::networkingThread(void){
    while (true) {}
}