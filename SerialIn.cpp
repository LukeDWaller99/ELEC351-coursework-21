
#include "SerialIn.h"

SerialIn::SerialIn(CustomQueue* printQueue, sampler* serialSamples, bufferClass* serialBuffer) {
    // SerialIn::SerialIn(CustomQueue* printQueue, sampler* serialSamples){
  pQ = printQueue;
  serialSampler = serialSamples;
  serialBuff = serialBuffer;
  SerialInstructions();
  SerialWatcher.start(callback(this, &SerialIn::SerialListener));
}

void SerialIn::SerialListener() {


  while (true) {
        char input[20];
        float tempThres;
        float pressThres;
        float lightThres;
        char matrix[20];
        float upperArry[3];
        float lowerArray[3];

        scanf("%s", input);

        // swtich case here for all the different chars that are incoming
        switch (input[0]) {
        case 'l': // latest value
            pQ->custom.call(printf, "Fetching Latest Values...\n");
            serialBuff->printBufferContents();
            // fetch latest data
        break;
        case 'b': // buffered
            pQ->custom.call(printf, "Fetching Current Number of Samples In Buffer...\n");
            serialBuff->bufferCount();
            // read number of samples currently in buffer
        break;
        case 'f': // flush
            pQ->custom.call(printf, "Flushing Buffer...\n");
            serialBuff->flushBuffer();
            // write all samples to buffer and flush sd card
        break;
        case 's': // set_high or low
            pQ->custom.call(printf, "Select Limits:\n");
            pQ->custom.call(printf, "Temperature\n");
            scanf("%f", &tempThres);
            pQ->custom.call(printf, "Pressure\n");
            scanf("%f", &pressThres);
            pQ->custom.call(printf, "Light\n");
            scanf("%f", &lightThres);
            switch (input[4]) {
                case 'h':
                    // set the higher limits
                    upperArry[0] = tempThres;
                    upperArry[1] = pressThres;
                    upperArry[2] = lightThres;
                    pQ->custom.call(printf, "Updating Upper Limits...\n");
                    serialSampler->threshold.bind_upper(upperArry);
                return;;
                case 'l':
                    // set the lower limits
                    lowerArray[0] = tempThres;
                    lowerArray[1] = pressThres;
                    lowerArray[2] = lightThres;
                    pQ->custom.call(printf, "Updating Lower Limits...\n");
                    serialSampler->threshold.bind_lower(lowerArray);   
                return;
                default:
                    pQ->custom.call(printf, "INVALID INPUT\n");
                return;
            }
            break;
        case 'p': // plot
                // plot the desired feature onto matrix
        pQ->custom.call(printf, "T, P, L ?\n");
        scanf("%s", matrix);
        switch (matrix[0]) {
        case 'T':
            // change to temperature
            pQ->custom.call(printf, "Changing Matrix to Temperature\n");
            serialSampler->sensorChange(matrix[0]);
            break;
        case 'P':
            // change to pressure
            pQ->custom.call(printf, "Changing Matrix to Pressure\n");
            serialSampler->sensorChange(matrix[0]);
            break;
        case 'L':
            // change to light
            pQ->custom.call(printf, "Chaning Matrix to Light\n");
            serialSampler->sensorChange(matrix[0]);
            return;
        }
        break;
        case 'h': // help
            SerialInstructions();
        break;
        default:
        pQ->custom.call(printf, "INVALID INPUT\n");
        break;
        }
    }
}

void SerialIn::SerialInstructions() {
  pQ->custom.call(printf, "Remote Function Calls:\n");
  pQ->custom.call(printf, "\tlatest\n"      \
                                   "\tbuffered\n"   \
                                   "\tflush\n"      \
                                   "\tset_low\n"    \
                                   "\tset_high\n"   \
                                   "\tplot\n"       \
                                   "\thelp\n"
                                   "Enter function to continue...\n" );
}