#include <sampling.h>

sampler::sampler():LDR(AN_LDR_PIN) {
    sampleThread.start(callback(this, &sampler::sample));

}


void sampler::sample(){
    while(true){
    ThisThread::flags_wait_any(1);
    if(sampleLock.trylock_for(1ms)==1){
        //shift everything one to the right
        
        //samples[0] = LDR.read_u16(); //read the LDR
        sampleLock.unlock();        //hand back the lock
    }
    else {
       printf("timeout\n");
    }
    ThisThread::flags_clear(1);
    }
}
