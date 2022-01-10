#include "mbed.h"
#include "mbed_wait_api.h"
#include "rtos/ThisThread.h"
#include <cstdio>
#include <sampling.h>
#include "ErrorHandler.h"


using namespace uop_msb;
using namespace std;

DigitalOut clk(LED1);
DigitalOut dat(LED2);
DigitalOut enable(LED3);
//DigitalOut bit(LED4);
#define BUFF_LEN    32
#define MSG_LEN     64
#define DATA_LEN    MSG_LEN - 2

UnbufferedSerial           serial(USBTX, USBRX,115200);
Thread              thread1;
Thread              thread2;
EventQueue          eventQueue;
Mutex               mutex;
ConditionVariable   cond(mutex);
char                recvBuff[BUFF_LEN] = { 0 };
size_t              recvLen;
char                message[MSG_LEN] = { 0 };

// The following variable is protected by locking the mutex
char                data[DATA_LEN] = { 0 };

void taskPrintData()
{
    ThisThread::sleep_for(1s);
    while (1) {

        // Now it is safe to access data in this thread
        printf("Data received: %s\r\n", data);
        memset(data, 0, DATA_LEN);  // empty data to make space for new data
    }
}

void onSerialReceived(void)
{
    while (serial.readable()) {
        // Read serial
        recvLen = serial.read(recvBuff, BUFF_LEN);

        if (message[strlen(message) - 1] == '\n') {
            // message complete
            
            // copy the chars from the message to the data storage
            strcat(data, &message[1]);      // omit first char (which is 's')
            memset(message, 0, MSG_LEN);

            break;
        }
    }
    memset(recvBuff, 0, BUFF_LEN);
}

void onSigio(void)
{
    eventQueue.call(onSerialReceived);
}

int main()
{
    printf("Starting..\r\n");
    thread1.start(taskPrintData);
    thread2.start(callback(&eventQueue, &EventQueue::dispatch_forever));
    serial.sigio(callback(onSigio));

    while (1) {
        ThisThread::sleep_for(10ms);
    }
}