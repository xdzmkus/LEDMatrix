#include "IMatrixLedEffect.h"

IMatrixLedEffect::IMatrixLedEffect(CRGB leds[], uint8_t width, uint8_t height)
	: ILedEffect(leds, width*height), width(width), height(height)
{
}


IMatrixLedEffect::~IMatrixLedEffect()
{
}

uint16_t IMatrixLedEffect::getPixelNumber(uint8_t x, uint8_t y)
{
	if (x > getWidth() - 1) x = getWidth() - 1;
	if (y > getHeight() - 1) y = getHeight() - 1;

	return (x * getHeight()) + ((x % 2 == 0) ? y : (getHeight() - 1 - y));
}
