tinyDTMF
========

Tiny Arduino library for decoding DTMF tones.

by Bruce Ratoff, KO4XL@yahoo.com, https://github.com/bratoff

This library uses Goertzel's algorithm to detect and decode
DTMF characters.

Functions:

tinyDTMF(byte analogPin) - Constructor.  Default pin is A2 if not specified.   

void begin(void) - Initialize the decoder.   

void getSample(void) - Fill sample buffer   

byte procSample(void) - Decode sample buffer, return ASCII key or 0    

byte getKey(void) - return last decoded key or 0    

Version 1.0.0 - 27 Jul 2021 - Initial release
