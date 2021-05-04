/*
* SnowMatrixLedEffect.cpp
*
*/

#include "SnowMatrixLedEffect.h"

const char* const SnowMatrixLedEffect::name = "SNOW";

SnowMatrixLedEffect::SnowMatrixLedEffect(ILedMatrix* converter, uint16_t Hz, unsigned long changeDirection, uint8_t fadeSpeed)
	: ILedEffect(Hz), matrix(converter), snowflakeCount(converter->getWidth()), dirTimer(changeDirection), fade(fadeSpeed)
{
	reset();

	dirTimer.start();
}

SnowMatrixLedEffect::~SnowMatrixLedEffect()
{
}

bool SnowMatrixLedEffect::isReady()
{
	if (dirTimer.isReady())
		direction = !direction;

	return ILedEffect::isReady();
}

void SnowMatrixLedEffect::reset()
{
	ILedEffect::reset();
	dirTimer.reset();
	matrix->clearAllLeds();
}

void SnowMatrixLedEffect::paint()
{
	uint16_t restSnowflakes = 0;

	if (direction)
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
		}
		// clear right column
		matrix->clearColumn(matrix->getWidth() - 1);
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
		}
		// clear left column
		matrix->clearColumn(0);
	}

	// clear top line
	matrix->clearLine(matrix->getHeight() - 1);

	// fill randomly snowflakes
	while (restSnowflakes < snowflakeCount)
	{
		uint8_t x = random8(0, matrix->getWidth());
		uint8_t y = random8(1, matrix->getHeight());

		matrix->getPixel(x, y) = CRGB::White;

		restSnowflakes++;
	}
}

