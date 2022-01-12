
#include "SerialIn.h"

SerialIn::SerialIn(CustomQueue* printQueue, sampler* sample, bufferClass* buffer) {
  printQueue = printQueue;
  sample = sample;
  buffer = buffer;
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
            printQueue->custom.call(printf, "Fetching Latest Values\n");
            buffer->printBufferContents();
            // fetch latest data
        break;
        case 'b': // buffered
            printQueue->custom.call(printf, "Fetching Current Number of Samples In Buffer\n");
            buffer->bufferCount();
            // read number of samples currently in buffer
        break;
        case 'f': // flush
            printQueue->custom.call(printf, "Flushing Buffer\n");
            buffer->flushBuffer();
            // write all samples to buffer and flush sd card
        break;
        case 's': // set_high or low
            printQueue->custom.call(printf, "Select Limits:\n");
            printQueue->custom.call(printf, "Temperature\n");
            scanf("%f", &tempThres);
            printQueue->custom.call(printf, "Pressure\n");
            scanf("%f", &pressThres);
            printQueue->custom.call(printf, "Light\n");
            scanf("%f", &lightThres);
            switch (input[4]) {
                case 'h':
                    // set the higher limits
                    upperArry[0] = tempThres;
                    upperArry[1] = pressThres;
                    upperArry[2] = lightThres;
                    printQueue->custom.call(printf, "Light\n");
                    sample->threshold.bind_upper(upperArry);
                break;
                case 'l':
                    // set the lower limits
                    lowerArray[0] = tempThres;
                    lowerArray[1] = pressThres;
                    lowerArray[2] = lightThres;
                    printQueue->custom.call(printf, "Light\n");
                    sample->threshold.bind_lower(lowerArray);
                    
                break;
                default:
                    printQueue->custom.call(printf, "INVALID INPUT\n");
                break;
            }
            break;
        case 'p': // plot
                // plot the desired feature onto matrix
        printQueue->custom.call(printf, "T, P, L ?\n");
        scanf("%s", matrix);
        switch (matrix[0]) {
        case 'T':
            // change to temperature
            printQueue->custom.call(printf, "Changing Matrix to Temperature\n");
            sample->sensorChange(matrix[0]);
            break;
        case 'P':
            // change to pressure
            printQueue->custom.call(printf, "Changing Matrix to Pressure\n");
            sample->sensorChange(matrix[0]);
            break;
        case 'L':
            // change to light
            printQueue->custom.call(printf, "Chaning Matrix to Light\n");
            sample->sensorChange(matrix[0]);
            return;
        }
        break;
        case 'h': // help
        SerialInstructions();
        break;
        default:
        printQueue->custom.call(printf, "INVALID INPUT\n");
        break;
        }
    }
}

void SerialIn::SerialInstructions() {
  printQueue->custom.call(printf, "Remote Function Calls:\n");
  printQueue->custom.call(printf, "latest\n\r"      \
                                   "buffered\n\r"   \
                                   "flush\n\r"      \
                                   "set_low\n\r"    \
                                   "set_high\n\r"   \
                                   "plot\n\r"       \
                                   "Enter function to cotinue...\n\r" );
}