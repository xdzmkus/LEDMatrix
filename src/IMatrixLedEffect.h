/*
* IMatrixLedEffect.h
*
*/


#ifndef __IMATRIXLEDEFFECT_H__
#define __IMATRIXLEDEFFECT_H__

#include "ILedEffect.h"

class IMatrixLedEffect : public ILedEffect
{
private:

	uint8_t width;
	uint8_t height;

public:

	IMatrixLedEffect(CRGB leds[], uint8_t width, uint8_t height);
	virtual ~IMatrixLedEffect();

	virtual uint16_t getPixelNumber(uint8_t x, uint8_t y);

	uint8_t getWidth() const { return width; }
	uint8_t getHeight() const { return height; }
};

#endif