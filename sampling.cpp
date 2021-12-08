#include "mbed.h"
#include "sampling.h"
//jacks hardware header?

void test_sampling(){
    //output adc values
    float ldr = adcIn.read();
    printf("ADC: %f\n\r");

    //get temp
    float temp = sensor.getTemperature();
    printf("Temperature: %5.1f\n\r");

    //get pressue
    float pressure = sensor.getPressure();
     printf("Pressure: %5.1f\n\r");




}