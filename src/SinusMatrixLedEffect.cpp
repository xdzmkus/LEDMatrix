/*
* SinusMatrixLedEffect.cpp
*
*/

#include "SinusMatrixLedEffect.h"

const char* const SinusMatrixLedEffect::name = "SINUS";

SinusMatrixLedEffect::SinusMatrixLedEffect(const IMatrixConverter* converter, CRGB leds[], uint16_t count, uint16_t Hz)
	: LedEffect(leds, count, Hz), converter(converter), hue(0)
{
	init();
}

SinusMatrixLedEffect::~SinusMatrixLedEffect()
{
}

void SinusMatrixLedEffect::init()
{
    clearAllLeds();
}

bool SinusMatrixLedEffect::paint()
{
	if (!isReady())
		return false;

    clearAllLeds();

    hue++;

    if (converter->getWidth() > converter->getHeight())
    {
        for (uint8_t x = 0; x < converter->getWidth(); x++)
        {
            uint8_t y = beatsin8(speed + x, 0, converter->getHeight() - 1);
            ledLine[converter->getPixelNumber(x, y)] = ColorFromPalette(RainbowColors_p, x * 7 + hue, 255);
        }
    }
    else
    {
        for (uint8_t y = 0; y < converter->getHeight(); y++) {
            uint8_t x = beatsin8(speed + y, 0, converter->getWidth() - 1);
            ledLine[converter->getPixelNumber(x, y)] = ColorFromPalette(RainbowColors_p, x * 7 + hue, 255);
        }
    }

	return true;
}
