// tinyDTMF - Tiny Arduino library for decoding DTMF tones
// by Bruce Ratoff, KO4XL@yahoo.com, https://github.com/bratoff
//

#include "tinyDTMF.h"


// List of DTMF frequencies
static const int _freqList[] PROGMEM = {697,770,852,941,1209,1336,1477,1633};

// Character map by [low tone][high tone]
static const char _dtmfChar[4][4] PROGMEM = {
	'1','2','3','A',
	'4','5','6','B',
	'7','8','9','C',
	'*','0','#','D'
	};

static unsigned char _samples[_SAMPLE_SIZE];	// Sample array
static  char _im[_NUM_FREQ];	// Magnitudes (in db) of tones detected
static float _coeff[_NUM_FREQ];	// Goertzel coefficient for each tone


// Initializer for class
void tinyDTMF::begin(void) {
	char f;
	
#if defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny25__)
	DDRB &= ~(1<<(_ain & 3));	// Force analog pin to be input
	DIDR0 |= (1<<(_ain & 3));	// Turn off digital input buffer for this pin to save power
#elif defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__) || \
			defined(__AVR_ATtiny24A__) || defined(__AVR_ATtiny44A__) || defined(__AVR_ATtiny84A__) || \
		  defined( __AVR_ATtiny441__ ) || defined( __AVR_ATtiny841__ )
	DDRA &= ~(1<<(_ain & 7));
	DIDR0 |= (1<<(_ain & 7));
#else
	pinMode(_ain, INPUT);
#endif
	_sampleFreq = getSample();	// Establish actual sample frequency
	// Compute coefficents for each tone frequency
	for(f=0; f<_NUM_FREQ; f++) {
		_coeff[f] = 2*cos(2*M_PI*pgm_read_word(&_freqList[f])/_sampleFreq);
	}
}

// Fill sample buffer
long tinyDTMF::getSample(void) {
  unsigned char i;
  unsigned long ts,st;
  
  ts = micros();            // Capture start time for sample rate calculation
  st = ts + _US_PER_SAMPLE;    // Calculate end of first sample period
  // Now fill the sample buffer at the intended rate
  for (i=0; i<_SAMPLE_SIZE; i++) {
    _samples[i] = (byte)(analogRead(_ain)>>2);  // Capture sample, scale to 8 bits
    while(micros() < st) ;  // Run out rest of sampling period
    st += _US_PER_SAMPLE;      // Calculate end of next sample period
  }

  // Now divide # of samples by elapsed time to return actual sample rate
  return(1000000*_SAMPLE_SIZE/(micros()-ts));
}

// Process sample, return key or 0
byte tinyDTMF::procSample(void) {
   float s;                           // Goertzel variables
  float s_prev;
  float s_prev2;
  float magn;
	int i;
  char f;
  int tl,th;

	_key = 0;

  // Look for each tone
  tl = -1;
	th = -1;
  for (f=0;f<_NUM_FREQ;f++) {
    s_prev=0;
    s_prev2=0;
    for (i=0;i<_SAMPLE_SIZE;i++) {
      s=0.01960784*_samples[i]+s_prev*_coeff[f]-s_prev2;
      s_prev2=s_prev;
      s_prev=s;
    }
    // Get magnitude
    magn=sqrt(s_prev2*s_prev2+s_prev*s_prev-_coeff[f]*s_prev*s_prev2);
    // Get maginitude in dB
    _im[f] = 16*log10(magn+0.001);
		if(_im[f] < 0) _im[f] = 0;			// Ignore negative numbers

    // See if we have a possible tone
		if(_im[f] > _threshold) {		// Above minimum magnitude?
      if(f<4) {										// Low (row) group tests
        if(tl < 0) tl=f;					// If we don't have one yet, this is it
        if(_im[tl] < _im[f]) tl=f;	// else is this one stronger than what we have
      } else {
        if(th < 0) th=f;					// Same logic for high(column) group
        if(_im[th] < _im[f]) th=f;
      }
    }
  }
	
	// Do we have a valid tone pair?
	if(tl < 0 || th < 0) {
		return(0);
	} else {
		int dif = _im[tl] - _im[th];	// Compare magnitudes
		if(abs(dif) < 3) {
			_key = pgm_read_byte(&_dtmfChar[tl][th-4]);	// If 1db difference or less, tones are valid
			return(_key);
		} else {
			return(0);
		}
	}
}

// Return last detected key or 0
byte tinyDTMF::getKey(void) {
	return(_key);
}

// Return magnitude of tone i in last sample
byte tinyDTMF::getMag(int i) {
	return(_im[i]);
}
