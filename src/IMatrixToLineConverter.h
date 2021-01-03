/**
        ^
HEIGHT  |  x   x   x   x
2       |  x   x   x   x
1       |  x   x   x   x
0       |  x   x   x   x
        0------------------->
           0   1   2  WIDTH
*/

#ifndef __IMATRIXTOLINECONVERTER_H__
#define __IMATRIXTOLINECONVERTER_H__

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class IMatrixToLineConverter
{
public:
	virtual uint8_t getWidth() const = 0;
	virtual uint8_t getHeight() const = 0;
	virtual uint16_t getPixelNumber(uint8_t x, uint8_t y) const = 0;
};

#endif
