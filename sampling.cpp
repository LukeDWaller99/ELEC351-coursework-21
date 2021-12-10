#include <sampling.h>

sampler::sampler():LDR(AN_LDR_PIN) {
    sample_thread.start(sample)

}


sampler::sample(){
    while(true){
    ThisThread::flags_wait_any(1);
    if(sampleLock.trylock_for(1ms)==1){
        //shift everything one to the right
        int i;
        for(i=999;i<0;i--){
            samples[i+1] = samples[i];
        }
        samples[0] = LDR.read_u16(); //read the LDR
        sampleLock.unlock();        //hand back the lock
    }
    else {
       printf("timeout\n");
    }
    ThisThread::flags_clear(1);
    }
}
