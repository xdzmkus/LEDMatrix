/*
* MatrixEffect.cpp
*
*/


#include "MatrixEffect.h"

// default constructor
MatrixEffect::MatrixEffect(CRGB leds[], uint8_t width, uint8_t height, CRGB color)
	: IMatrixLedEffect(leds, width, height), rgb(color)
{
}

// default destructor
MatrixEffect::~MatrixEffect()
{
}

void MatrixEffect::refresh()
{
	// shift down all lines
	for (uint8_t x = 0; x < getWidth(); x++)
	{
		for (uint8_t y = 0; y < getHeight() - 1; y++)
		{
			ledStrip[getPixelNumber(x, y)] = getPixelColor(getPixelNumber(x, y + 1));
		}
	}
	// fill randomly top line
	for (uint8_t x = 0; x < getWidth(); x++)
	{
		uint16_t pixel = getPixelNumber(x, getHeight() - 1);
		uint32_t thisColor = getPixelColor(pixel);

		if (thisColor == 0)
		{
			ledStrip[pixel] = 0x00FF00 * (random(0, 10) == 0);
		}
		else if (thisColor < 0x002000)
		{
			ledStrip[pixel] = 0;
		}
		else
		{
			ledStrip[pixel] = thisColor - 0x002000;
		}
	}
}
