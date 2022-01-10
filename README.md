# Environmental Sensor

This project is an Environmental sensor with the capability of monitoring the temperature, pressure, and light level of a space. The software make use of multithreading in order to sample at a deterministic rate. This data is then submitted onto a webpage to be monitored, as well as saved onto an SD card in a human-readable format for latter viewing and processing.

Should any of the defined sensor thresholds be crossed, an alarm will be raised and errors logged to the serial output. 



# Contributions

Jack Pendlebury
**Authored**
    - Sampler
    - ErrorHandler
    - CustomQueue
**Contributed**
    - Documentation
    - 7 Segment Display
    - Matrix
    - Serial Capture

Noah Harvey
**Authored**
    - SD Card
    - Buffer
    - 
**Contributed**
    - Stuff

Luke Waller
**Authored**
    - 7 Segment Display
    - Matrix Display
    - NTP Server
    - HTTP Server
    - Serial Capture
**Contributed**
    - Error Handler
    - Custom Queue