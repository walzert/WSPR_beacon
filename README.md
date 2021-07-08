# Yet another WSPR beacon based on SI5351

I just wanted to make a PCB for ham radio purpose and here is the software part, die PCB will also uploaded. 
The software has 3 relays to control 3 different outputs for antennas, filters and amps combinations.

There is the main part of the software that is getting GPS data, get maidenhead of that, getting time and setting time to RTC.
There is a display that shows most of the informationen.

And there is a time based queue in loop that sends based on different times on different bands.