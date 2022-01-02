#ifndef H_SAMPLING
#define H_SAMPLING
#include <mbed.h>
#include <Mutex.h>
#include <uop_msb.h>
#include "LEDMatrix.h"
//#include "buffer.h"


/**
Structure for holding the sample data. The samples struct 
**/
struct samples{
    float temp, pressure, LDR;
};

struct limits{
    float t_upper,t_lower,p_upper,p_lower,l_upper,l_lower;
    /**
    Default limit constructor. This creates an instance of limits with the default values.
    **/
    limits(){
        t_upper = 35;
        t_lower = 20;
        p_upper = 1100;
        p_lower = 1000;
        l_upper = 0.8;
        l_lower = 0.2;
    }
    /**
    Constructor with default override. This constructor creates an instance of the struct limits, with the 
    threshold values set equal to the input array.
    @param limits   Array of limits, upper and lower, temperature, pressure, light.
                    Temperature in Celsius.
                    Pressure in millibars.
                    Light value as a 0->1 float value.
    **/
    limits(float limits[6]){
        t_upper = limits[0];
        t_lower = limits[1];
        p_upper = limits[2];
        p_lower = limits[3];
        l_upper = limits[4];
        l_lower = limits[5];
    }
    void bind(float limits[6]){
        t_upper = limits[0];
        t_lower = limits[1];
        p_upper = limits[2];
        p_lower = limits[3];
        l_upper = limits[4];
        l_lower = limits[5];
    }
};

class sampler {
    private:
    typedef void(*funcPointer_t)(void);
    enum sensor_type{
        TEMP,
        PRESSURE,
        LIGHT
    };
    limits threshold;
    float limits[6];
    Mutex sampleLock;
    Ticker sampleTick;
    Thread sampleThread,matrixThread;
    LEDMatrix matrix;
    //bufferClass sampleBuffer;
    uop_msb::EnvSensor sensor;
    AnalogIn LDR;

    
    void sample();
    /**
    ISR to raise the sample flag. This interrupt service routine is triggered by a ticker attached in the class
    constructor, and fires every second waking the sampler up.
    **/
    void sampleflag();
    /**
    Quantise the internal buffer to sixteen levels and send to the matrix. This function quantises the 
    internal buffer to sixteen levels, before sending the selected measurement (Temp, Pressure, Light), to the
    matrix display to be displayed graphically.
    @param selectedSensor   Stores what measurement is to be output onto the matrix display. The function
                            only quantises the desired measurement values.
     **/
    void matrixInterface();
    void quantise(sensor_type selectedSensor);
    public:
    samples internal_buffer[8];
    samples sampleData;
    /**
    Default constructor. This constructor will instantiate an instance of the sample module with the
    default alarm thresholds.
    **/
    sampler();
    /**
    Alternative constructor that overrides default limits. This constructor can be used 
    if limit values other than the defaults are required.
    @param limits   Array of limits, upper and lower, temperature, pressure, light.
                    Temperature in Celsius.
                    Pressure in millibars.
                    Light value as a 0->1 float value.
    **/
    sampler(float limits[6]);
    void displayLimits();
    ~sampler();
    
};



#endif