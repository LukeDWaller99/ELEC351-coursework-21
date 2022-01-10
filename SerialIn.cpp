
#include "SerialIn.h"

// https://os.mbed.com/docs/mbed-os/v6.15/apis/unbufferedserial.html

SerialIn::SerialIn(UnbufferedSerial* serial_port){
    SerialInThread.start(callback(this, &SerialIn::SerialListener));
    serial_port = serial_port;
    serial_port->baud(115200);
    serial_port->format(
        /* bits */ 8,
        /* parity */ SerialBase::None,
        /* stop bit */ 1
    );

    serial_port->attach(Serial_IRQ, SerialBase::RxIrq);
}

void SerialIn::SerialListener(){

    while (true) {
        // wait till the flag goes high
            char input;
            if(serial_port.read(&input, 1)){ //check the interrupt to clear it
                //swtich case here for all the different chars that are incoming
            }
        // lower the  flag 
    }

}

void Serial_IRQ(){
    // set flag her the thread here
}