
#include "SerialIn.h"

SerialIn::SerialIn(CustomQueue* printQueue, sampler* serialSamples, bufferClass* serialBuffer) {
  pQ = printQueue;
  serialSampler = serialSamples;
  serialBuff = serialBuffer;
  SerialInstructions();
  SerialWatcher.start(callback(this, &SerialIn::SerialListener));

}

void SerialIn::SerialListener() {

  while (true) {
        char input[20];
        char matrix[20];
        float tempThres;
        float pressThres;
        float lightThres;
        float upperArry[3];
        float lowerArray[3];

        scanf("%s", input);

        // swtich case here for all the different chars that are incoming
        switch (input[0]) {
        case 'l': // latest value
            pQ->custom.call(printf, "\nFetching Latest Values...\n");
            serialBuff->fetchLatestRecord();
            // fetch latest data
        break;
        case 'b': // buffered
            pQ->custom.call(printf, "\nFetching Current Number of Samples In Buffer...\n");
            serialBuff->bufferCount();
            // read number of samples currently in buffer
        break;
        case 'f': // flush
            pQ->custom.call(printf, "\nFlushing Buffer...\n");
            serialBuff->flushBuffer();
            // write all samples to buffer and flush sd card
        break;
        case 's': // set_high or low
            pQ->custom.call(printf, "\nSelect Limits:\n");
            pQ->custom.call(printf, "\nTemperature\n");
            scanf("%f", &tempThres);
            pQ->custom.call(printf, "\nPressure\n");
            scanf("%f", &pressThres);
            pQ->custom.call(printf, "\nLight\n");
            scanf("%f", &lightThres);
            switch (input[4]) {
                case 'h':
                    // set the higher limits
                    upperArry[0] = tempThres;
                    upperArry[1] = pressThres;
                    upperArry[2] = lightThres;
                    pQ->custom.call(printf, "\nUpdating Upper Limits...\n");
                    serialSampler->threshold.bind_upper(upperArry);
                    pQ->custom.call(printf, "\nUpdatied Upper Limits...\n");
                break;
                case 'l':
                    // set the lower limits
                    lowerArray[0] = tempThres;
                    lowerArray[1] = pressThres;
                    lowerArray[2] = lightThres;
                    pQ->custom.call(printf, "\nUpdating Lower Limits...\n");
                    serialSampler->threshold.bind_lower(lowerArray);   
                    pQ->custom.call(printf, "\nUpdatedLower Limits...\n");
                break;
                default:
                    pQ->custom.call(printf, "\nINVALID INPUT\n");
                break;
            }
            break;
        case 'p': // plot
                // plot the desired feature onto matrix
        pQ->custom.call(printf, "\nT, P, L ?\n");
        scanf("%s", matrix);
        switch (matrix[0]) {
        case 'T':
            // change to temperature
            pQ->custom.call(printf, "\nChanging Matrix to Temperature\n");
            serialSampler->sensorChange(matrix[0]);
            break;
        case 'P':
            // change to pressure
            pQ->custom.call(printf, "\nChanging Matrix to Pressure\n");
            serialSampler->sensorChange(matrix[0]);
            break;
        case 'L':
            // change to light
            pQ->custom.call(printf, "\nChanging Matrix to Light...\n");
            serialSampler->sensorChange(matrix[0]);
            break;
        default:
            pQ->custom.call(printf, "\nINVALID\n");
            break;
        }
        break;
        case 'h': // help
            Help();
        break;
        default:
        pQ->custom.call(printf, "\nINVALID INPUT\n");
        break;
        }
    SerialInstructions();
    }
}

// prints out the instruction list
void SerialIn::SerialInstructions() {
  pQ->custom.call(printf, "\nRemote Function Calls:\n");
  pQ->custom.call(printf, "\tlatest\n"      \
                                   "\tbuffered\n"   \
                                   "\tflush\n"      \
                                   "\tset_low\n"    \
                                   "\tset_high\n"   \
                                   "\tplot\n"       \
                                   "\thelp\n"
                                   "Enter function to continue...\n" );
}

// prints out the help 'menu'
void SerialIn::Help(){
  pQ->custom.call(printf, "\tlatest - gets the latest value stored in the buffer\n"      \
                                   "\tbuffered - output the current number of samples stored in the buffer\n"   \
                                   "\tflush - flushes the buffer and wrties the outputs to the SD card\n"      \
                                   "\tset_low - sets the lower thresholds for Temperature, Pressure, and Light Level.\n"    \
                                   "\t\t Type in function name. Hit enter. Then the parameters one by one\n"\
                                   "\tset_high - sets the upper thresholds for Temperature, Pressure, and Light Level\n"   \
                                   "\t\t Type in function name. Hit enter. Then the parameters one by one\n"\
                                   "\tplot - changes the currnet output on the LED Matrix\n"       \
                                    "\t\t Type in function name. Hit enter. Then type 'T', 'P', or 'L'\n"\
                                   "\thelp - brings up the help menu\n");  
}

// test code for the serial in
void SerialIn::SerialTest(){
    pQ->custom.call(printf, "Testing Serial in...\n Type something:\n");
    char test[20];
    scanf("%s", test);
    pQ->custom.call(printf, "Echo: %s", test);
}