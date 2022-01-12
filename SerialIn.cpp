
#include "SerialIn.h"


SerialIn::SerialIn(CustomQueue* printQueue, sampler* sample, bufferClass* buffer){
    printQueue = printQueue;
    sample = sample;
    buffer = buffer;
    SerialInstructions();
    SerialWatcher.start(callback(this, &SerialIn::SerialListener));
}

void SerialIn::SerialListener(){

    while (true) {
        char input[20];
        float tempThres;
        float pressThres;
        float lightThres;
        char matrix[20];
        scanf("%s", input);

            //swtich case here for all the different chars that are incoming
            switch (input[0]) {
            case 'l': // latest value
                printQueue->custom.call(printf, "Fetching Latest Values\n");
                //fetch latest data 
            break;
            case 'b': // buffered
                printQueue->custom.call(printf, "Fetching Current Number of Samples In Buffer\n");
                // read number of samples currently in buffer
            break;
            case 'f': // flush
                printQueue->custom.call(printf, "Flushing Buffer");
                //write all samples to buffer and flush sd card
            break;
            case 's': // set_high or low 
                printQueue->custom.call(printf, "Select Limits:");
                printQueue->custom.call(printf, "Temperature");
                scanf("%f", &tempThres);
                printQueue->custom.call(printf, "Pressure");
                scanf("%f", &pressThres);
                printQueue->custom.call(printf, "Light");     
                scanf("%f", &lightThres);
                switch (input[4]) {
                case 'h':
                    // set the higher limits 
                    float upperArry[] = [tempThres, pressThres, lightThres]; 
                    printQueue->custom.call(printf, "Light");  
                    sample->threshold.bind_upper(upperArry);
                break;
                case 'l':
                    // set the lower limits
                    printQueue->custom.call(printf, "Light");  
                break;
                default:
                    printQueue->custom.call(printf, "INVALID INPUT");
                return;
                }   
            break;
            case 'p': // plot
            // plot the desired feature onto matrix
                printQueue->custom.call(printf, "T, P, L ?");
                scanf("%s", matrix);
                switch (matrix[0]) {
                case 'T':
                    // change to temperature
                    printQueue->custom.call(printf, "Changing Matrix to Temperature");
                    sample->sensorChange(matrix[0]);
                break;
                case 'P':
                    // change to pressure
                    printQueue->custom.call(printf, "Changing Matrix to Pressure ");
                    sample->sensorChange(matrix[0]);
                break;
                case 'L':
                    // change to light
                    printQueue->custom.call(printf, "Chaning Matrix to Light");
                    sample->sensorChange(matrix[0]);
                return;
                }
            break;
            case 'h': // help 
                SerialInstructions();
            break;
            default:
                printQueue->custom.call(printf, "INVALID INPUT");
            break;
            }
        }
    }
}

void SerialIn::SerialInstructions(){
    printQueue->custom.call(printf, "Remote Function Calls:");
}