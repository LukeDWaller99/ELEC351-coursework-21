#include "sd.h"
#include <cstdio>

// bufferClass obj;
// bool SDState = 0;
// FILE *fp; //file pointer
bufferClass SDBuffer;
// SDPins SDPinNumbers(PB_5, PB_4, PB_3, PE_3);
SDBlockDevice mysd(PB_5, PB_4, PB_3, PF_3);
// SDBlockDevice mysd;
// SDBlockDevice SDDetect(PF_4);
DigitalOut greenLED(PC_6);
DigitalIn SDDetect(PF_4);
// extern unsigned int newIDX = buffer_size - 1; 
// extern unsigned int oldIDX = buffer_size - 1;


SDCard::SDCard() {
    //SDThread.start(callback(this, &SDCard::SDRun));
    //SDThread.start(callback(this, &SDCard::initSD));
}

// //function to flash green LED before and after the SD card is being written
// to, LED alternates state during write process void flashGreen() {
// 	//always leaves green on
// 	greenLED = 0;
// 	ThisThread::sleep_for(500);		//put this thread to sleep for
// 500ms 	greenLED = 1;
// }

void SDCard::initSD() {
  if (SDDetect == 1) {
    printQueue.call(mountError);
    greenLED = 0;
  }else{
    SDState = 1;
    greenLED = 1;
    printQueue.call(mountedSD);
  }
}

void SDCard::flashGreen(){
    greenLED = 0;
    ThisThread::sleep_for(100ms);
    greenLED = 1;
}

// // void SDCardISR(){
// //     SDThread.flags_set(SDDetect)
// // }


int SDCard::mywrite_sdcard()
{
    printf("Initialise and write to a file\n");
 int err;
    // call the SDBlockDevice instance initialisation method.

    err=mysd.init();
    if ( 0 != err) {
        printf("Init failed %d\n",err);
        greenLED = 0;
        return -1;
    }
    
    FATFileSystem fs("sd", &mysd);
    FILE *fp = fopen("/sd/test.txt","w");
    if(fp == NULL) {
        error("Could not open file for write\n");
        mysd.deinit();
        greenLED = 0;
        return -1;
    } else {
        //*****************************************
        SDCard::flashGreen();
        SDBuffer.flushBuffer(*fp);
        //greenLED = 1;   




        //*****************************************
        
        //Tidy up here
        fclose(fp);
        //SDCard::flashGreen();
        printf("SD Write done...\n");
        mysd.deinit();
        return 0;
    }
    
}

int SDCard::myread_sdcard()
{
    printf("Initialise and read from a file\n");

    // call the SDBlockDevice instance initialisation method.
    if ( 0 != mysd.init()) {
        printf("Init failed \n");
        return -1;
    }
    
    FATFileSystem fs("sd", &mysd);
    FILE *fp = fopen("/sd/test.txt","r");
    if(fp == NULL) {
        error("Could not open or find file for read\n");
        mysd.deinit();
        return -1;
    } else {
        //Put some text in the file...
        char buff[64]; buff[63] = 0;
        while (!feof(fp)) {
            fgets(buff, 63, fp);
            printf("%s\n", buff);
        }
        //Tidy up here
        fclose(fp);
        printf("SD read done...\n");
        mysd.deinit();
        return 0;
    }
}





























































// void SDCard::testWriteSD() {

//   int err;
//   err = mysd.init();
//   if (0 != err) {
//       printf("%d\n", err);
//     printQueue.call(mountError);
//     return;
//   }

//   FATFileSystem fs("sd", &mysd);
//   FILE *fp = fopen("sd/test.txt", "w");

//   if (fp == NULL) {
//     printQueue.call(noSDFile);
//     mysd.deinit();
//   }

//   //SDState = 1;
//   //green led on
//   printQueue.call(mountedSD);

//   //testing
//   //printQueue.call(fprintf(fp, "there are words here\n"));
//   fprintf(fp, "there are words here\n");
//   fclose(fp);
//   SDCard::flashGreen();
//   //printQueue.call(printf, "WE GOT THIS FAR\n");
//   mysd.deinit();
// }

// // void SDCard::writeDataSD() {

// //   int err;
// //   err = mysd.init();
// //   if (0 != err) {
// //       printf("%d\n", err);
// //     printQueue.call(mountError);
// //     return;
// //   }

// //   FATFileSystem fs("sd", &mysd);
// //   FILE *fp = fopen("sd/test.txt", "w");

// //   if (fp == NULL) {
// //     printQueue.call(noSDFile);
// //     mysd.deinit();
// //   }

// //   //SDState = 1;
// //   //green led on
// //   printQueue.call(mountedSD);
// //     SDBuffer.flushBuffer(*fp);
// //   //testing
// //   //printQueue.call(fprintf(fp, "there are words here\n"));
// //   fprintf(fp, "there are words here\n");
// //   fclose(fp);
// //   SDCard::flashGreen();
// //   printQueue.call(printf, "WE GOT THIS FAR\n");
// //   mysd.deinit();
// // }


// // void SDCard::writeSD(){

// //     FATFileSystem fs("sd", &mysd);
// //     FILE *fp = fopen("sd/test.txt", "w");

// //     if (fp == NULL){ //cannot open file - it does not exist
// //         //errorSeverity(CRITICAL);
// //         printQueue.call(noSDFile);
// //     }

// //     if(SDState == 0){//sd card is unmounted
// //         //criticalError
// //         printQueue.call(unmountedFlush);
// //     }else{
// //         SDBuffer.flushBuffer(*fp); //flush buffer data
// //         //NOW STORE THE TIME OF THIS FLUSH
// //         //TO TRACK TIME UNTIL NEXT FLUSH
// //         //*********


// //         //*********

// //     }
// //     fclose(fp); //close file
// // } //end writeSD function

// // void SDCard::unmountSD(){
// //     SDCard::testWriteSD();
// //     //SDCard::writeSD();
// //     mysd.deinit();
// //     SDState = 0;
// //     //green led off
// //     printQueue.call(unmountedSD);
// // } //end unmountSD function

// void SDCard::readSD() {
//   if (mysd.init() != 0) {
//     printf("init failed\n");
//   }
//   FATFileSystem fs("sd", &mysd);
//   FILE *fp = fopen("sd/test.txt", "r");

//   if (fp == NULL) {
//     error("Could not open or find file for read\n");
//     mysd.deinit();
//     return;
//   } else {
//     // Put some text in the file...
//     char buff[64];
//     buff[63] = 0;
//     while (!feof(fp)) {
//       fgets(buff, 63, fp);
//       printf("%s\n", buff);
//     }
//     // Tidy up here
//     fclose(fp);
//     printf("SD Write done...\n");
//     mysd.deinit();
//     return;
//   }
// }

// // void SDCard::SDRun(){
// //     initSD();
// //     while(1){
// //         if(SDDetect == 0){

// //             if(newIDX == (oldIDX - (round(buffer_size * 0.1)))){
// //                 SDCard::writeSD();

// //             }
// //             //******************************************************
// //             //if buffer is almost full at 1 minute, flush
// //             // if(longer than a minute) && (newIDX == (oldIDX -
// //             //(round(buffer_size*0.1))))){ writeSD();
// //             //}
// //             //else if( )
// //             //if buffer hasn't filled in an hour, still flush
// //             //******************************************************
// //         }
// //     }
// // } //end SDThread function
