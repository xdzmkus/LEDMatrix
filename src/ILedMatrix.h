/*
* ILedMatrix.h
* 
*         ^
* HEIGHT  |  x   x   x   x
* 2       |  x   x   x   x
* 1       |  x   x   x   x
* 0       |  x   x   x   x
*         0------------------->
*            0   1   2  WIDTH
*/

#ifndef _ILEDMATRIX_H
#define _ILEDMATRIX_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <ILedLine.h>
#include <FastLED.h>


class ILedMatrix : public ILedLine
{
private:

	const uint8_t width;
	const uint8_t height;

public:

	ILedMatrix(CRGB leds[], uint8_t width, uint8_t height);
	virtual ~ILedMatrix();

	uint8_t getWidth() const;
	uint8_t getHeight() const;

	CRGB* getAllPixels();
	CRGB& getPixel(uint8_t x, uint8_t y);

protected:

	virtual uint16_t getPixelNumber(uint8_t x, uint8_t y) const = 0;
};

#endif
