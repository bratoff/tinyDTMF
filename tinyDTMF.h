// tinyDTMF - Tiny Arduino library for decoding DTMF tones
// by Bruce Ratoff, KO4XL@yahoo.com, https://github.com/bratoff
//

// Standard .h wrapper
#ifndef _tinyDTMF_H_
#define _tinyDTMF_H_

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include <WProgram.h>
#endif

#include <avr/pgmspace.h>

// Default input pin
#define _APIN		A2

// Intended sample rate in KHz
#define _SAMPLE_RATE 4000UL
#define _US_PER_SAMPLE 1000000UL/_SAMPLE_RATE

// Size of sample buffer
#define _SAMPLE_SIZE 128

// Detection threshold
#define _DTMF_THRESH 16

// Number of DTMF frequencies to be detected
#define _NUM_FREQ 8

class tinyDTMF
{

private:

	byte _ain;					// Analog input pin to use
	byte _key;					// ASCII of last key detected, 0 if none
	unsigned long _sampleFreq;		// Calculated sample rate after filling buffer
	byte _threshold;		// Threshold magnitude (db) to detect tones
	
public:

	// Default constructor
  tinyDTMF() : _ain(_APIN), _key(0), _threshold(_DTMF_THRESH) {};
	// Constructor with different input pin
  tinyDTMF(byte aPin) : _key(0), _threshold(_DTMF_THRESH), _ain(aPin) {};
	// Constructor with input pin and threshold
  tinyDTMF(byte aPin, byte thresh) : _key(0), _threshold(thresh), _ain(aPin) {};

  void begin(void);					// Initializer
	long getSample(void);			// Fill sample buffer, return calculated sample rate
	byte procSample(void);		// Process sample, return key or 0
	byte getKey(void);				// Return last detected key or 0

	byte getAin(void) {return(_ain);}
	long getSampleFreq(void) {return(_sampleFreq);}
	byte getThreshold(void) {return(_threshold);}
	byte getMag(int i);				// Return magnitude of tone i in last sample
};

#endif // _tinyDTMF_H_ defined
