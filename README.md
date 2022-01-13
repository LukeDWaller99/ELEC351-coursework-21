# Environmental Monitor

This project is an Environmental Monitor with the capability of monitoring the temperature, pressure, and light level of a space. The software make use of multithreading in order to sample at a deterministic rate. This data is then submitted onto a webpage to be monitored, as well as saved onto an SD card in a human-readable format for latter viewing and processing.

Should any of the defined sensor thresholds be crossed, an alarm will be raised and errors logged to the serial output. 

# Requirement
The specification outlines multiple requirments needed for the environmental sensor to be consider fit for purpose. The table belows contains this list of requirements, as well as where they have been fulfilled in the design.

|Requirement    | Fullfilled                                        | Where     |
| :----         | :----                                             | :----     |
|1              | Yes                                               | sampler.h |
|2              | Yes                                               | buffer.h  |
|3              | Yes                                               | buffer.h  |
|4              | Yes - buzzer can be enabled via macro             | sampler.h |
|5              | Yes                                               |throughout |
|6              | Yes - no spin locks or busy/waits                 |throughout |
|7              | Partial - serial capture only                     |SerialIn.h |
|8              | Partial - internal webpage                        |HTTP_Server.h|
|9              | Yes - error handler will respond to time outs     |throughout|
|10             | Yes                                               |ErrorHandler.h|
|11             | Yes                                               |LEDMatrix.h|
|12             | Yes                                               |throughout |

# Dependancies
The main.c file contains all the instantiations needed in the correct order. However, several of the modules of this project can be used as standalone classes. To ensure that these classes are instantiated correctly, the dependencies are listed below. Generally, a 'CustomQueue' will need to be instantiated before anything else, to provide an output logging method.

**Error Handler**
    - Custom Queue
        - For error logging

**Sampler**
    - Error Handler
        - To control error outputs

# Usuage of the Environmental Sensor

## Error Codes
In the event of an error, the Error Handler will automatically perform actions based on the severity:

Severity  | Actions
------------- | -------------
Warning  | Display error code, light yellow LED
Critical  | Display error code, light red LED, alarm for 30s then reset
Fatal | Immediate reset

A list of error codes can be seen below:

| Module    | Severity  | Code  | Description |
| :----:    | :----:    | :---- | :---- |
| Buffer    | Critical  | 10    | Buffer is full. This is indicative of a further problem, as the buffer cannot flush to the SD Card output.|
| ^         | Critical  | 11    | Buffer Lock Timeout. |
| ^         | Critical  | 12    | Timer Lock Timeout.|
| ^         | Warning   | 13    | Empty Flush. The buffer has attempted to flush when empty. |
| ^         | Critical  | 14    | Buffer Flush Timeout. The buffer has failed to acquire the lock in time. |
| SD Card   | Critical  | 20    | No SD Card is mounted. | 
| ^         | Critical  | 21    | No SD Card File. The SD Card may be full. |
|^          | Critical  | 22    | SD Card slot is empty, and the buffer cannot flush. |
|Networking |           |       |                               |
|^          | Fatal     | 40    | No network interface found    |
|^          | Fatal     | 41    | Could not connect to server   |
|^          | Fatal     | 42    | Failed to get time from NTP server|
|^          | Fatal     | 43    | Could not get IP address|
|^          | Fatal     | 44    | Listener Error |
| ErrorHandler | Fatal  | 99    | Flag clear error. The error handler is unresponsive. |

The same output is used to display environmental warnings.
Severity  | Actions
------------- | -------------
Environmental  | Display error code, Alarm for 3s

A list of environmental errors can be seen below:
| Sensor        | Code  | Description |
| :----:        | :---- | :----
| Temperature   | 30    | Lower temperature limit exceeded. |
| ^             | 31    | Upper temperature limit exceeded. |
| Pressure      | 33    | Lower pressure limit exceeded     |
| ^             | 34    | Upper pressure limit exceeded     |
| Light         | 35    | Lower light limit exceeded        |
| ^             | 36    | Upper light limit exceeded        |

## Sending Commands
The Environmental Sensor includes the ability to send commands via a serial interface. Upon initialising, a help screen will be displayed on the connected serial monitor. This list of commands is also available below:
| Command        | Syntax  | Description |
| :----:        | :---- | :----
| latest   | latest    | Fetch the latest date/time and data sample and display it over serial. |
| buffered             | buffered    | Read the number of samples in the buffer and return it. |
| flush      | flush    | Write all the samples in the buffer to the SD. Sends acknowledgement after.   |
| set low             | setlow (at prompt) - type each limit   | Changes lower alarm threshold     |
| set high         | sethigh (at prompt) - type each limit    | Changes upper alarm threshold        |
| plot             | plot - (at prompt) char (T/P/L)    | Change the matrix display to plot a different sensors data.        |

## Changing the Matrix Display
To change the sensor data that the matrix is displaying, two methods are provided. Either send the serial command 'plot' alongside either T/P/L for
the desired sensor. Alternatively, button A can be pressed to toggle to the next sensor. This will take effect on the next sample interval.

## Buffer Details
The buffer class contains the SD card functionality, writing to the SD card in a format that is human readblae and easy to edit. The environmental data samples obtained every 10s are buffered in internal memory, and after a minute, 
when the buffer has reached 90% capacity the samples (tracked by semaphores), along with the time and date they were obtained, are written to the SD card. The thread safe buffer has been encapsulated in one class, using semaphores to track the sample count and space available.
Writing to the buffer when full returns an error and lights a red LED. You are warned when attempting to flush an empty buffer.
Two threads are use to raise flags for writing to the buffer and flushing.
Appropriate mitigation of deadlocks and thread starvation by using timeouts. Timeouts are logged as critical erros in the serial.
The ErrorHandler object is referenced by pointer to output the error codes. The CustomQueue is an event queue used to output additional information to the serial.

initSD : Using the sd card detectors interrupt capable pin to check the sd card is mounted. It is also initiated in the bufferClass constructor. green LED lights when mounted, upon removal it turns off.
writeBufferAuto : Obtain every data set along with the data and time stamp for when it was taken (aquired from network time server). Writing to the buffer while full, mutex locks that are not acquried 
or time out will return errors and flash a red led and serial buzzer message is output for 30s. Buffer and time data is protected by mutex locks.
flushBuffer : Samples, time and data flushed to text file. An empty flush produces a warning.
bufferCount : return number of data sets being held in buffer to be output when requested by SerialIn.
fetchLatestRecord : return the latest data set being held in but to be output when requested by SerialIn.

# Contributions

**Jack Pendlebury**

*Authored*
    - sampler
    - ErrorHandler
    - CustomQueue

*Contributed*
    - Documentation
    - SevenSegmentDisplay
    - LEDMatrix
    - SerialIn

**Noah Harvey**

*Authored*
    - SD Card
    - Buffer
    

*Contributed*
    - SerialIn
    - ErrorHandler

**Luke Waller**

*Authored*
    - SevenSegmentDisplay
    - LEDMatrix
    - NTPConnection
    - HTTP_Server
    - SerialIn

*Contributed*
    - ErrorHandler
    - CustomQueue
    - Buffer

