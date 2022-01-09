#ifndef H_SAMPLING
#define H_SAMPLING
#include <mbed.h>
#include <Mutex.h>
#include <uop_msb.h>
#include "LEDMatrix.h"
#include "ErrorHandler.h"
//#include "buffer.h"

#define SAMPLE_INTERVAL 10s

/**
Structure for holding the sample data. The samples struct holds three values, one from each sensor, taken at 1 
second intervals.
**/
struct samples{
    float temp, pressure, LDR;
};

/**
Structure for holding quantised samples. This structure is primarily used for interfacing with the 
matrix display, and holds eight interger values.
**/
struct quantised_samples{
    int qsamples[8];
};

struct limits{
    float t_upper,t_lower,p_upper,p_lower,l_upper,l_lower;
    /**
    Default limit constructor. This creates an instance of limits with the default values.
    **/
    limits(){
        t_upper = 35;
        t_lower = 15;
        p_upper = 1050;
        p_lower = 950;
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

    /**
    Method to set all limits at once using an array. This method allows an array to be used to override all limits
    at once, with a single call. Limits cannot be selectively over-written, the array must have a length of six.
    @param limits   Array of limits, upper and lower, temperature, pressure, light.
                    Temperature in Celsius.
                    Pressure in millibars.
                    Light value as a 0->1 float value.

    **/
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
        LIGHT,
    };
    InterruptIn BT_A;                           ///< Button for controlling the matrix output sensor
    limits threshold;                           ///< limits object to store the current alarm thresholds
    //sensor_type currentSensor = LIGHT;        ///< Current sensor output, default is 'LIGHT'
    //float limits[6];    
    Mutex sampleLock;                           ///< Mutex Lock to ensure thread safety on sample values
    Ticker sampleTick;                          ///< Ticker interrupt to trigger sampling at once per second
    Thread sampleThread,matrixThread;           ///< Thread declarations.
    LEDMatrix matrix;                           ///< LED Matrix display for outputting sample bar graphs.
    //bufferClass sampleBuffer;
    uop_msb::EnvSensor sensor;        
    AnalogIn LDR;
    ErrorHandler* EH;                           ///< Error Handler

    /**
    Main sampling function. This function contains the majority of the sampler's functionality. After being
    awoken by the ticker, it reads the sensor values in a thread safe manner, checking the values against the 
    set thresholds. If any of these thresholds are broken, the appropriate error flag is raised. The sample data is 
    quantised and then sent to the matrix.
    **/
    void sample();

    /**
    ISR to raise the sample flag. This interrupt service routine is triggered by a ticker attached in the class
    constructor, and fires every second waking the sampler up.
    **/
    void sampleflag();

    /**
    ISR to flip the sensor flag. The sensor flag indicates that the desired output sensor has been changed.
    **/
    void sensorflag();

    /**
    Quantise the internal buffer to sixteen levels and send to the matrix. This function quantises the 
    internal buffer to sixteen levels, before sending the selected measurement (Temp, Pressure, Light), to the
    matrix display to be displayed graphically.
    @param selectedSensor   Stores what measurement is to be output onto the matrix display. The function
                            only quantises the desired measurement values.
     **/
    void quantise(sensor_type selectedSensor);

    /**
    Matrix interface funtion.
    **/
    void matrixInterface();

    /**
    Function to check the incoming sample against the alarm threshold.

    **/
    void thresholdCheck();

    public:
    quantised_samples matrix_input; ///<Holds quantised values to be passed to the matrix display
    samples internal_buffer[8];     ///<Internal buffer to hold samples for the matrix.
    samples sampleData;
    /**
    Default constructor. This constructor will instantiate an instance of the sample module with the
    default alarm thresholds.
    **/
    sampler(ErrorHandler* OutputHandler);
    /**
    Alternative constructor that overrides default limits. This constructor can be used 
    if limit values other than the defaults are required.
    @param limits   Array of limits, upper and lower, temperature, pressure, light.
                    Temperature in Celsius.
                    Pressure in millibars.
                    Light value as a 0->1 float value.
    **/
    sampler(ErrorHandler* OutputHandler, float limits[6]);
    void displayLimits();
    ~sampler();
    sensor_type currentSensor = LIGHT;      ///<Current sensor output, default is 'LIGHT'
};



#endif