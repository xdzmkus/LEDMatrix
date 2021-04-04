/*
* SnowMatrixLedEffect.cpp
*
*/

#include "SnowMatrixLedEffect.h"

const char* const SnowMatrixLedEffect::name = "SNOW";

SnowMatrixLedEffect::SnowMatrixLedEffect(ILedMatrix* converter, uint16_t Hz, uint8_t fadeSpeed)
	: ILedEffect(Hz), matrix(converter), snowflakeCount(converter->getWidth()), fade(fadeSpeed)
{
	reset();
}

SnowMatrixLedEffect::~SnowMatrixLedEffect()
{
}

void SnowMatrixLedEffect::reset()
{
	ILedEffect::reset();
	matrix->clearAllLeds();
}

void SnowMatrixLedEffect::paint()
{
	uint16_t restSnowflakes = 0;

	if (random8() % 2 == 0)
	{
		// shift down all lines and fade
		for (uint8_t y = 0; y < matrix->getHeight() - 1; y++)
		{
			// shift left all lines
			for (uint8_t x = 0; x < matrix->getWidth() - 1; x++)
			{
				matrix->getPixel(x, y) = matrix->getPixel(x + 1, y + 1).subtractFromRGB(fade);
				if (matrix->getPixel(x, y))
					restSnowflakes++;
			}
			// clear right column
			matrix->getPixel(matrix->getWidth() - 1, y) = CRGB::Black;
		}
	}
	else
	{
		// shift down all lines and fade
		for (uint8_t y = 0; y < matrix->getHeight() - 1; y++)
		{
			// shift right all lines
			for (uint8_t x = matrix->getWidth() - 1; x > 0; x--)
			{
				matrix->getPixel(x, y) = matrix->getPixel(x - 1, y + 1).subtractFromRGB(fade);
				if (matrix->getPixel(x, y))
					restSnowflakes++;
			}
			// clear left column
			matrix->getPixel(0, y) = CRGB::Black;
		}
	}

	// clear top line
	for (uint8_t x = 0; x < matrix->getWidth(); x++)
	{
		matrix->getPixel(x, matrix->getHeight() - 1) = CRGB::Black;
	}

	// fill randomly snowflakes
	while (restSnowflakes < snowflakeCount)
	{
		uint8_t x = random8(0, matrix->getWidth());
		uint8_t y = random8(1, matrix->getHeight());

		matrix->getPixel(x, y) = CRGB::White;

		restSnowflakes++;
	}
}

