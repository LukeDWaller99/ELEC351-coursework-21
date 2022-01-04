//#include "sd2.h"

//FILE *fp; //file pointer
//bufferClass SDBuffer;
// //SDPins SDPinNumbers(PB_5, PB_4, PB_3, PE_3);
// //SDBlockDevice mysd(PB_5, PB_4, PB_3, PE_3);
////SDBlockDevice mysd;
//SDBlockDevice SDDetect(PF_4);

// SDCard::SDCard(){
//     //SDThread.start(callback(this, &SDCard::SDRun));
// }

// void SDCard::initSD(){
//     if(mysd.init() != 0){
//         printQueue.call(mountError);
//     }

//     else{
//         SDState = 1;
//         //green led
//         printQueue.call(mountedSD);
//     }
// }

// void SDCard::testWriteSD(){
//     if(mysd.init() != 0){
//         //if(SDDetect = 0){
//         printQueue.call(mountError);
//         //}
//     }
    
//     FATFileSystem fs("sd", &mysd);
//     FILE *fp = fopen("sd/test.txt", "w");

//     if(fp == NULL){
//         printQueue.call(noSDFile);
//         mysd.deinit();
//     }

//     SDState = 1;
//     //green led on
//     printQueue.call(mountedSD);

//     //testing 
//     fprintf(fp, "there are words here\n");
//     fclose(fp);
//     mysd.deinit();
//     return;

// }

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

// //     }
// //     fclose(fp); //close file
// // } //end writeSD function

// void SDCard::unmountSD(){
//     SDCard::testWriteSD();
//     //SDCard::writeSD();
//     mysd.deinit();
//     SDState = 0;
//     //green led off
//     printQueue.call(unmountedSD);
// } //end unmountSD function


// // void SDCard::readSD(){
// //     if(0 != mysd.init()){
// //         printf("init failed\n");
// //     }
// //     FATFileSystem fs("sd", &mysd);
// //     FILE *fp = fopen("sd/test.txt", "r");

// //     if(fp == NULL) {
// //         error("Could not open or find file for read\n");
// //         mysd.deinit();
// //         return;
// //     } else {
// //         //Put some text in the file...
// //         char buff[64]; buff[63] = 0;
// //         while (!feof(fp)) {
// //             fgets(buff, 63, fp);
// //             printf("%s\n", buff);
// //         }
// //         //Tidy up here
// //         fclose(fp);
// //         printf("SD Write done...\n");
// //         mysd.deinit();
// //         return;
// //     }
// // }
// //thread for SD writing
// /*
// void SDCard::SDRun(){
//     initSD();
//     while(1){
//         if(SDState == 1){
//             //if buffer is almost full at 1 minute, flush
//            // if(longer than a minute) && (newIDX == (oldIDX - (round(BUFFERSIZE*0.1))))){
//                writeSD();
//            }
//             else if( )
//             //if buffer hasn't filled in an hour, still flush

//         }
//     }
// } //end SDThread function
// */