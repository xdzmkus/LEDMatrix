/*
* BouncingBallsMatrixLedEffect.cpp
*
*/

#include "BouncingBallsMatrixLedEffect.h"

const char* const BouncingBallsMatrixLedEffect::name = "BOUNSINGBALLS";

BouncingBallsMatrixLedEffect::BouncingBallsMatrixLedEffect(const IMatrixConverter* converter, CRGB leds[], uint16_t count, uint16_t Hz, uint8_t maxBallsCount)
	: LedEffect(leds, count, Hz), converter(converter)
{
	bouncingColumns = new BOUNCING_COLUMN[converter->getWidth()];

	for (uint8_t x = 0; x < converter->getWidth(); x++)
	{
		bouncingColumns[x].numBalls = (x % maxBallsCount == 0) ? random(1, maxBallsCount + 1) : 0;
		if (bouncingColumns[x].numBalls != 0)
			bouncingColumns[x].balls = new BOUNCING_COLUMN::BOUNCING_BALL[bouncingColumns[x].numBalls];
	}

	init();
}

BouncingBallsMatrixLedEffect::~BouncingBallsMatrixLedEffect()
{
	for (uint8_t x = 0; x < converter->getWidth(); x++)
	{
		if (bouncingColumns[x].numBalls != 0)
			delete[] bouncingColumns[x].balls;
	}
	delete[] bouncingColumns;
}

void BouncingBallsMatrixLedEffect::init()
{
	for (uint8_t x = 0; x < converter->getWidth(); x++)
	{
		for (int i = 0; i < bouncingColumns[x].numBalls; i++)
		{
			bouncingColumns[x].balls[i].color = getRandomColor();
			bouncingColumns[x].balls[i].dampening = 0.90 - static_cast<float>(i) / static_cast<float>(bouncingColumns[x].numBalls << 1);
			bouncingColumns[x].balls[i].StartHeight = random(1, converter->getHeight());
			bouncingColumns[x].balls[i].ImpactVelocityStart = sqrt(-2 * Gravity * bouncingColumns[x].balls[i].StartHeight);

			bouncingColumns[x].balls[i].height = 0;
			bouncingColumns[x].balls[i].impactVelocity = bouncingColumns[x].balls[i].ImpactVelocityStart;
			bouncingColumns[x].balls[i].position = 0;
			bouncingColumns[x].balls[i].timeSinceLastBounce = 0;
			bouncingColumns[x].balls[i].clockTimeSinceLastBounce = getClock();
		}
	}
	LedEffect::init();
}

bool BouncingBallsMatrixLedEffect::paint()
{
	if (isReady())
		return false;

	for (uint8_t x = 0; x < converter->getWidth(); x++)
	{
		for (int i = 0; i < bouncingColumns[x].numBalls; i++)
		{
			bouncingColumns[x].balls[i].timeSinceLastBounce = getClock() - bouncingColumns[x].balls[i].clockTimeSinceLastBounce;
			bouncingColumns[x].balls[i].height = 0.5 * Gravity * pow(bouncingColumns[x].balls[i].timeSinceLastBounce / LedEffect::CLOCKS_IN_SEC, 2.0)
				+ bouncingColumns[x].balls[i].impactVelocity * bouncingColumns[x].balls[i].timeSinceLastBounce / LedEffect::CLOCKS_IN_SEC;

			if (bouncingColumns[x].balls[i].height < 0)
			{
				bouncingColumns[x].balls[i].height = 0;
				bouncingColumns[x].balls[i].impactVelocity *= bouncingColumns[x].balls[i].dampening;
				bouncingColumns[x].balls[i].clockTimeSinceLastBounce = getClock();

				if (bouncingColumns[x].balls[i].impactVelocity < 0.01)
				{
					bouncingColumns[x].balls[i].impactVelocity = bouncingColumns[x].balls[i].ImpactVelocityStart;
				}
			}

			ledLine[converter->getPixelNumber(x, bouncingColumns[x].balls[i].position)] = CRGB::Black;

			bouncingColumns[x].balls[i].position = round(bouncingColumns[x].balls[i].height * (converter->getHeight() - 1) / bouncingColumns[x].balls[i].StartHeight);

			ledLine[converter->getPixelNumber(x, bouncingColumns[x].balls[i].position)] = bouncingColumns[x].balls[i].color;
		}
	}
	return true;
}
