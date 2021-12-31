#include <sampling.h>

extern samples sampledData;

sampler::sampler():LDR(AN_LDR_PIN) {
    sampleThread.start(callback(this, &sampler::sample));
    sampleTick.attach(callback(this, &sampler::sampleflag),1s);
}

void sampler::sampleflag(){
    sampler::sampleThread.flags_set(1);
}

void sampler::sample(){
    while(true){
    ThisThread::flags_wait_any(1);
    if(sampleLock.trylock_for(1ms)==1){
        
        sampledData.temp = sensor.getTemperature();
        sampledData.pressure = sensor.getPressure();
        sampledData.LDR = LDR.read();
        
        //samples[0] = LDR.read_u16(); //read the LDR
        sampleLock.unlock();        //hand back the lock
    }
    else {
       printf("timeout\n");
    }
    ThisThread::flags_clear(1);
    }
}
