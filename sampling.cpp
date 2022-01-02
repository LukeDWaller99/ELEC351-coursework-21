#include <sampling.h>

//extern samples sampledData;

sampler::sampler():LDR(AN_LDR_PIN) {
    sampleThread.start(callback(this, &sampler::sample));
    matrixThread.start(callback(this, &sampler::matrixInterface));
    sampleTick.attach(callback(this, &sampler::sampleflag),1s);
}


sampler::sampler(float limits[6]):LDR(AN_LDR_PIN) {
    //set threshold values
    threshold.bind(limits);
    sampleThread.start(callback(this, &sampler::sample));
    matrixThread.start(callback(this, &sampler::matrixInterface));
    sampleTick.attach(callback(this, &sampler::sampleflag),1s);
}

void sampler::sampleflag(){
    sampler::sampleThread.flags_set(1);
}

void sampler::sample(){
    while(true){
    ThisThread::flags_wait_any(1);
    if(sampleLock.trylock_for(1ms)==1){
        
        sampleData.temp = sensor.getTemperature();
        sampleData.pressure = sensor.getPressure();
        sampleData.LDR = LDR.read();
        
        //samples[0] = LDR.read_u16(); //read the LDR
        sampleLock.unlock();        //hand back the lock
        //shift internal buffer left
        int i;
        for(i=1;i<8;i++){
            internal_buffer[i-1] = internal_buffer[i];
        }
        //add latest result to buffer
        internal_buffer[7]=sampleData;
    }
    else {
       printf("timeout\n");
    }
    ThisThread::flags_clear(1);
    matrixThread.flags_set(1); //signal matrix thread to do something
    }
}

void sampler::matrixInterface(){
    while(true){
        ThisThread::flags_wait_any(1);
        quantise(LIGHT);
        ThisThread::flags_clear(1);
    }
}

void sampler::quantise(sensor_type selectedSensor){
    int i;
    float quantInterval,rawVals[8],quantVals[8],upper,lower,min,max;
    //Calculate quantisation interval
    if (selectedSensor == TEMP){
        upper = threshold.t_upper;  //extract upper limit
        lower = threshold.t_lower;  //extract lower limit
        for(i=0;i<8;i++){
            rawVals[i]=internal_buffer[i].temp;
        }
    }
    else if (selectedSensor == PRESSURE) {
        upper = threshold.p_upper;  //extract upper limit
        lower = threshold.p_lower;  //extract lower limit
        for(i=0;i<8;i++){
            rawVals[i]=internal_buffer[i].pressure;
        }
    }
    else {
        upper = threshold.l_upper;  //extract upper limit
        lower = threshold.l_lower;  //extract lower limit
        for(i=0;i<8;i++){
            rawVals[i]=internal_buffer[i].LDR;
        }
    }
    quantInterval = (upper-lower)/10;
    min = lower -(3*quantInterval);
    max = upper +(3*quantInterval);
    for(i=0;i<8;i++){
        quantVals[i]=(rawVals[i]-lower)/quantInterval;
        printf("%d \t%f \n",i,quantVals[i]);
    }

}

void sampler::displayLimits(){
    printf("Temperature \tUpper = %2.1f \tLower = %2.1f\n",threshold.t_upper,threshold.t_lower);
    printf("Pressure \tUpper = %3.1f \tLower = %3.1f\n",threshold.p_upper,threshold.p_lower);
    printf("Light \t\tUpper = %1.2f \tLower = %1.2f\n",threshold.l_upper,threshold.l_lower);
}