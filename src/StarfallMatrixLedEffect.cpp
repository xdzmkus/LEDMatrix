/*
* StarfallMatrixLedEffect.cpp
*
*/

#include "StarfallMatrixLedEffect.h"

const char* const StarfallMatrixLedEffect::name = "STARFALL";

StarfallMatrixLedEffect::StarfallMatrixLedEffect(const IMatrixToLineConverter* converter, CRGB leds[], uint16_t count, uint16_t Hz, CRGB color)
	: LedEffect(leds, count, Hz), converter(converter), rgb(color ? color : getRandomColor())
{
	fade = CRGB(1 + rgb.r/(converter->getHeight()+1), 1 + rgb.g/(converter->getHeight()+1), 1 + rgb.b/(converter->getHeight()+1));

	init();
}

StarfallMatrixLedEffect::~StarfallMatrixLedEffect()
{
}

void StarfallMatrixLedEffect::init()
{
	clearAllLeds();
}

bool StarfallMatrixLedEffect::paint()
{
	if (!isReady())
		return false;

	for (uint8_t x = 0; x < converter->getWidth(); x++)
	{
		// shift down all lines
		for (uint8_t y = 0; y < converter->getHeight() - 1; y++)
		{
			ledLine[converter->getPixelNumber(x, y)] = ledLine[converter->getPixelNumber(x, y + 1)];
		}

		// fill randomly top line
		uint16_t pixel = converter->getPixelNumber(x, converter->getHeight() - 1);

		if (!ledLine[pixel] && random8(0, converter->getWidth()) == 0)
		{
			ledLine[pixel] = rgb;
		}
		else
		{
			ledLine[pixel] -= fade;
		}
	}
	return true;
}
