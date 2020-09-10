/*
* MatrixLedEffect.cpp
*
*/

#include "MatrixLedEffect.h"

MatrixLedEffect::MatrixLedEffect(const IMatrixConverter* converter, CRGB leds[], uint16_t count, uint16_t Hz, CRGB color)
	: ILedEffect(leds, count, Hz), converter(converter), rgb(color)
{
}

MatrixLedEffect::~MatrixLedEffect()
{
}

bool MatrixLedEffect::MatrixLedEffect::paint()
{
	if (!ILedEffect::paint())
		return false;

	// shift down all lines
	for (uint8_t x = 0; x < converter->getWidth(); x++)
	{
		for (uint8_t y = 0; y < converter->getHeight() - 1; y++)
		{
			ledLine[converter->getPixelNumber(x, y)] = getPixelColor(converter->getPixelNumber(x, y + 1));
		}
	}
	// fill randomly top line
	for (uint8_t x = 0; x < converter->getWidth(); x++)
	{
		uint16_t pixel = converter->getPixelNumber(x, converter->getHeight() - 1);
		uint32_t thisColor = getPixelColor(pixel);

		if (thisColor == 0)
		{
			ledLine[pixel] = 0x00FF00 * (random(0, 10) == 0);
		}
		else if (thisColor < 0x002000)
		{
			ledLine[pixel] = 0;
		}
		else
		{
			ledLine[pixel] = thisColor - 0x002000;
		}
	}
	return true;
}
