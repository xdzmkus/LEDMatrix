/*
* GravityMatrixLedEffect.cpp
*
*/

#include "GravityMatrixLedEffect.h"

const char* const GravityMatrixLedEffect::name = "GRAVITY";

GravityMatrixLedEffect::GravityMatrixLedEffect(const IMatrixConverter* converter, CRGB leds[], uint16_t count, uint16_t Hz)
	: LedEffect(leds, count, Hz), converter(converter)
{
	gravities = new GRAVITY[converter->getWidth()];

	init();
}

GravityMatrixLedEffect::~GravityMatrixLedEffect()
{
	if (gravities != nullptr)
	{
		delete[] gravities;
	}
}

void GravityMatrixLedEffect::init()
{
	if (gravities != nullptr)
	{
		uint8_t max = converter->getHeight() - 1;
		uint8_t phase = random8(0, converter->getHeight());

		for (uint8_t x = 0; x < converter->getWidth(); x++, phase++)
		{
			gravities[x].color = getRandomColor();
			gravities[x].startTime = getClock();
			gravities[x].height = 1;
			gravities[x].position = 0;
			gravities[x].velocity = sqrt(2 * Gravity * ((phase / max) % 2 == 0 ? (phase % max + 1) : (max - 1 - (phase % max))));
		}
	}

	clearAllLeds();
}

bool GravityMatrixLedEffect::paint()
{
	if (!isReady() || gravities == nullptr)
		return false;

	bool allGround = true;

	for (uint8_t x = 0; x < converter->getWidth(); x++)
	{
		if (gravities[x].height > 0)
		{
			float timeOfFlying = static_cast<float>(getClock() - gravities[x].startTime) / LedEffect::CLOCKS_IN_SEC;
			gravities[x].height = gravities[x].velocity * timeOfFlying - 0.5 * Gravity * timeOfFlying * timeOfFlying;

			if (gravities[x].height <= 0)
			{
				gravities[x].height = 0;
			}
			else
			{
				allGround = false;
			}

			ledLine[converter->getPixelNumber(x, gravities[x].position)] = CRGB::Black;

			gravities[x].position = round(gravities[x].height);

			ledLine[converter->getPixelNumber(x, gravities[x].position)] = gravities[x].color;
		}
	}

	if (allGround)
	{
		init();
	}

	return true;
}
