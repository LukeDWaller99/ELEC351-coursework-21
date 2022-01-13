# Environmental Sensor

This project is an Environmental sensor with the capability of monitoring the temperature, pressure, and light level of a space. The software make use of multithreading in order to sample at a deterministic rate. This data is then submitted onto a webpage to be monitored, as well as saved onto an SD card in a human-readable format for latter viewing and processing.

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
| latest   | latest    | Fetch the latest date/time and data sample and display it over serial |
| buffered             | buffered    |  |
| Pressure      | 33    | Lower pressure limit exceeded     |
| ^             | 34    | Upper pressure limit exceeded     |
| Light         | 35    | Lower light limit exceeded        |
| ^             | 36    | Upper light limit exceeded        |

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
    - SerialCapture

**Noah Harvey**

*Authored*
    - SD Card
    - Buffer
    - 

*Contributed*
    - Stuff

**Luke Waller**

*Authored*
    - SevenSegmentDisplay
    - LEDMatrix
    - NTPConnection
    - HTTP_Server
    - Serial Capture

*Contributed*
    - ErrorHandler
    - CustomQueue

