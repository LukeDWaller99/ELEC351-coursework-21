#include <sampling.h>

//extern samples sampledData;

sampler::sampler(ErrorHandler* OutputHandler):LDR(AN_LDR_PIN),BT_A(BTN1_PIN) {
    EH = OutputHandler;
    sampleThread.start(callback(this, &sampler::sample));
    matrixThread.start(callback(this, &sampler::matrixInterface));
    sampleTick.attach(callback(this, &sampler::sampleflag),10s);
    BT_A.rise(callback(this, &sampler::sensorflag));
}


sampler::sampler(ErrorHandler* OutputHandler,float limits[6]):LDR(AN_LDR_PIN),BT_A(BTN1_PIN) {
    EH = OutputHandler;
    //set threshold values
    threshold.bind(limits);
    sampleThread.start(callback(this, &sampler::sample));
    matrixThread.start(callback(this, &sampler::matrixInterface));
    sampleTick.attach(callback(this, &sampler::sampleflag),10s);
}

void sampler::sampleflag(){
    sampler::sampleThread.flags_set(1);
}

void sampler::sensorflag(){
    sampler::matrixThread.flags_set(2);
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
    int flags;
    while(true){
        ThisThread::flags_wait_any(1); //wait for wake-up call
        flags = ThisThread::flags_get(); //check if sensor needs to be cycled
        if (flags == 2) {
        switch (currentSensor) {
            case TEMP :
                currentSensor = PRESSURE;
            break;
            case PRESSURE:
                currentSensor = LIGHT;
            break;
            case LIGHT :
                currentSensor = TEMP;
            break;
            default:
                currentSensor = TEMP;
            }
        }
        quantise(currentSensor);
        thresholdCheck();
        matrix.update(matrix_input.qsamples);
        ThisThread::flags_clear(3);
    }
}

void sampler::quantise(sensor_type selectedSensor){
    int i,quantVals[8];
    float quantInterval,rawVals[8],upper,lower,min,max;
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
            rawVals[i]=1-internal_buffer[i].LDR;
        }
    }
    quantInterval = (upper-lower)/10;
    min = lower -(3*quantInterval);
    max = upper +(3*quantInterval);
    for(i=0;i<8;i++){
        quantVals[i]=static_cast<int>((rawVals[i]-lower)/quantInterval);
        if (quantVals[i] < 0){
            quantVals[i] = 0;
        }
        matrix_input.qsamples[i] = quantVals[i];
        //printf("%d \t%d \n",i,quantVals[i]);
    }

}

void sampler::thresholdCheck(){
    limits threshold_t = threshold;
    float temp,pres,light;
    int clear_flag = 0;
    samples data = sampleData;
    //check temperature
    temp = sampleData.temp;
    pres = sampleData.pressure;
    light = sampleData.LDR;

    //check temp
    if (temp>threshold_t.t_upper){
        EH->setErrorFlag(T_UPPER);
        
        prevAlarmFlag = 1;
    }
    else if (temp<threshold_t.t_lower) {
        EH->setErrorFlag(T_LOWER);
        
        prevAlarmFlag = 1;
    }
    else{
        clear_flag++;
    }

    //check pressure
    if (pres>threshold_t.p_upper){
        EH->setErrorFlag(P_UPPER);
        
        prevAlarmFlag = 1;
    }
    else if (pres<threshold_t.p_lower) {
        EH->setErrorFlag(P_LOWER);
        
        prevAlarmFlag = 1;
    }
    else {
        clear_flag++;
    }
    //check light
    if (light>threshold_t.l_upper){
        EH->setErrorFlag(L_LOWER);
        
        prevAlarmFlag = 1;
    }
    else if (light<threshold_t.l_lower) {
        EH->setErrorFlag(L_UPPER);
        
        prevAlarmFlag = 1;
    }
    else {
        clear_flag++;
    }
    if (clear_flag == 3){
        if (
        prevAlarmFlag == 1){
        EH->setErrorFlag(ALL_CLEAR);
        }
    }
    else {
    clear_flag = 0;
    }
}

void sampler::displayLimits(){
    printf("Temperature \tUpper = %2.1f \tLower = %2.1f\n",threshold.t_upper,threshold.t_lower);
    printf("Pressure \tUpper = %3.1f \tLower = %3.1f\n",threshold.p_upper,threshold.p_lower);
    printf("Light \t\tUpper = %1.2f \tLower = %1.2f\n",threshold.l_upper,threshold.l_lower);
}