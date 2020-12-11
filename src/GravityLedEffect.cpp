/*
* GravityLedEffect.cpp
*
*/

#include "GravityLedEffect.h"

const char* const GravityLedEffect::name = "GRAVITY";

GravityLedEffect::GravityLedEffect(const IMatrixConverter* converter, CRGB leds[], uint16_t count, uint16_t Hz, CRGB color, uint16_t interval)
	: LedEffect(leds, count, Hz), converter(converter), rgb(color), newShot(MillisTimer(interval, true))
{
	gravities = new GRAVITY[converter->getWidth()];

	init();
}

GravityLedEffect::~GravityLedEffect()
{
	delete[] gravities;
}

void GravityLedEffect::init()
{
	LedEffect::init();

//	double angle = 0.0;

	uint8_t max = converter->getHeight() - 1;
	uint8_t phase = random(0, converter->getHeight());

	for (uint8_t x = 0; x < converter->getWidth(); x++, phase++)
	{
		/* sinusoid
		gravities[x].limit = map(round(100 * sin(angle)), -100, 100, 0, converter->getHeight() - 1);
		angle += (2 * 3.14) / converter->getWidth();
		*/

		gravities[x].limit = (phase / max) % 2 == 0 ? (phase % max + 1) : (max - 1 - (phase % max));
		gravities[x].velocity = (converter->getHeight() - gravities[x].limit) << 1;
		gravities[x].direction = true;

		ledLine[converter->getPixelNumber(x, 0)] = rgb ? rgb : getRandomColor();
	}
}

bool GravityLedEffect::paint()
{
	if (!LedEffect::isReady())
		return false;

	bool allGround = true;

	for (uint8_t x = 0; x < converter->getWidth(); x++)
	{
		// dormancy
		if (gravities[x].limit == 0)
			continue;

		for (uint8_t y = 0; y < converter->getHeight(); y++)
		{
			// shift up or down
			if (ledLine[converter->getPixelNumber(x, y)])
			{
				shiftPixel(x, y);
				break;
			}
		}

		allGround &= ledLine[converter->getPixelNumber(x, 0)];
	}

	if (allGround && newShot.isReady())
	{
		init();
	}

	return true;
}

void GravityLedEffect::shiftPixel(uint8_t x, uint8_t y)
{
	if (gravities[x].velocity == 0)
	{
		if (gravities[x].direction == true)
		{
			ledLine[converter->getPixelNumber(x, y + 1)] = ledLine[converter->getPixelNumber(x, y)];
			ledLine[converter->getPixelNumber(x, y)] = CRGB::Black;
			
			// reached top point - change direction
			if (gravities[x].limit == y + 1)
			{
				gravities[x].direction = false;
			}
		}
		else
		{
			ledLine[converter->getPixelNumber(x, y - 1)] = ledLine[converter->getPixelNumber(x, y)];
			ledLine[converter->getPixelNumber(x, y)] = CRGB::Black;

			// reached ground point - stop
			if (y == 1)
			{
				gravities[x].limit = 0;
			}
		}
		gravities[x].velocity = (converter->getHeight() - gravities[x].limit) + (y) * (y);
	}
	else
		gravities[x].velocity--;
}
