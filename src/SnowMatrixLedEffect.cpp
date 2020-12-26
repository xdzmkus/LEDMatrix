/*
* SnowMatrixLedEffect.cpp
*
*/

#include "SnowMatrixLedEffect.h"

const char* const SnowMatrixLedEffect::name = "SNOW";

SnowMatrixLedEffect::SnowMatrixLedEffect(const IMatrixConverter* converter, CRGB leds[], uint16_t count, uint16_t Hz)
	: LedEffect(leds, count, Hz), converter(converter), snowflakeCount(count/10), fade(255/converter->getHeight())
{
	init();
}

SnowMatrixLedEffect::~SnowMatrixLedEffect()
{
}

bool SnowMatrixLedEffect::paint()
{
	if (!isReady())
		return false;

	uint16_t restSnowflakes = 0;

	if (random8() % 2 == 0)
	{
		// shift down all lines and fade
		for (uint8_t y = 0; y < converter->getHeight() - 1; y++)
		{
			// shift left all lines
			for (uint8_t x = 0; x < converter->getWidth() - 1; x++)
			{
				ledLine[converter->getPixelNumber(x, y)] = ledLine[converter->getPixelNumber(x + 1, y + 1)].subtractFromRGB(fade);
				if (ledLine[converter->getPixelNumber(x, y)])
					restSnowflakes++;
			}
			// clear right column
			ledLine[converter->getPixelNumber(converter->getWidth() - 1, y)] = CRGB::Black;
		}
	}
	else
	{
		// shift down all lines and fade
		for (uint8_t y = 0; y < converter->getHeight() - 1; y++)
		{
			// shift right all lines
			for (uint8_t x = converter->getWidth() - 1; x > 0; x--)
			{
				ledLine[converter->getPixelNumber(x, y)] = ledLine[converter->getPixelNumber(x - 1, y + 1)].subtractFromRGB(fade);
				if (ledLine[converter->getPixelNumber(x, y)])
					restSnowflakes++;
			}
			// clear left column
			ledLine[converter->getPixelNumber(0, y)] = CRGB::Black;
		}
	}


	// clear top line
	for (uint8_t x = 0; x < converter->getWidth(); x++)
	{
		ledLine[converter->getPixelNumber(x, converter->getHeight() - 1)] = CRGB::Black;
	}

	// fill randomly snowflakes
	while (restSnowflakes < snowflakeCount)
	{
		uint8_t x = random(0, converter->getWidth());
		uint8_t y = random(converter->getHeight()/2, converter->getHeight());

		ledLine[converter->getPixelNumber(x, y)] = CRGB::White;

		restSnowflakes++;
	}

	return true;
}
