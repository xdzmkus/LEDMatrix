/*
* LEDMatrix.h
*
*/

#ifndef _LEDMATRIX_H_
#define _LEDMATRIX_H_

#include <LEDLine.h>
#include <FastLED.h>

class LEDMatrix : public LEDLine
{

public:

	virtual void setRunningString(const char* data, uint16_t len);

protected:

	String text;

	static const TProgmemRGBPalette16 wrwPalette_p PROGMEM;
};

#endif
