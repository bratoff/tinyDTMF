tinyDTMF
========

Tiny Arduino library for decoding DTMF tones on an ATtiny.   

by Bruce Ratoff, KO4XL@yahoo.com, https://github.com/bratoff   

This library uses Goertzel's algorithm to detect and decode DTMF characters.   

Supported processors:   
ATtiny85   
ATtiny84*   
ATtiny84A*   
ATtiny841*   
 * At present, only A0 thru A7 are supported on these processors    

Constructors:   
tinyDTMF(byte analogPin) - analogPin is optional.  Default pin is A2 if not specified.  
tinyDTMF(byte analogPin, byte threshold) - specify magnitude threshold to recognize tones   

Member Functions:   
void begin(void) - Initialize the decoder.   
long getSample(void) - Fill sample buffer.  Returns calculated sample rate in Hz.   
byte procSample(void) - Decode sample buffer, return ASCII key or 0    
byte getKey(void) - return last decoded key or 0    
byte getAin(void) - return analog pin number   
long getSampleFreq(void) - return last calculated sample frequency   
byte getThreshold(void) - return magnitude threshold for recognition   

Version 1.0.0 - 14 Aug 2021 - Initial release
