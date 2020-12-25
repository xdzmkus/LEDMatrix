/*
* BouncingBallsMatrixLedEffect.cpp
*
*/

#include "BouncingBallsMatrixLedEffect.h"

const char* const BouncingBallsMatrixLedEffect::name = "BOUNSINGBALLS";

BouncingBallsMatrixLedEffect::BouncingBallsMatrixLedEffect(const IMatrixConverter* converter, CRGB leds[], uint16_t count, uint16_t Hz, uint8_t maxBallsCount)
	: LedEffect(leds, count, Hz), converter(converter), MaxVelocity(sqrt(2 * Gravity * (converter->getHeight() - 1)))
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
			bouncingColumns[x].balls[i].startTime = getClock();
			bouncingColumns[x].balls[i].height = 0;
			bouncingColumns[x].balls[i].position = 0;
			bouncingColumns[x].balls[i].velocity = MaxVelocity;
			bouncingColumns[x].balls[i].dampingPercentage = random(70, 90);
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
			float timeOfFlying = static_cast<float>(getClock() - bouncingColumns[x].balls[i].startTime) / LedEffect::CLOCKS_IN_SEC;
			bouncingColumns[x].balls[i].height = bouncingColumns[x].balls[i].velocity * timeOfFlying - 0.5 * Gravity * pow(timeOfFlying, 2.0);

			if (bouncingColumns[x].balls[i].height < 0)
			{
				bouncingColumns[x].balls[i].height = 0;
				bouncingColumns[x].balls[i].velocity *= bouncingColumns[x].balls[i].dampingPercentage / 100.0;
				bouncingColumns[x].balls[i].startTime = getClock();

				if (bouncingColumns[x].balls[i].velocity < 0.01)
				{
					bouncingColumns[x].balls[i].velocity = MaxVelocity;
					bouncingColumns[x].balls[i].dampingPercentage = random(70, 90);
				}
			}

			ledLine[converter->getPixelNumber(x, bouncingColumns[x].balls[i].position)] = CRGB::Black;

			bouncingColumns[x].balls[i].position = round(bouncingColumns[x].balls[i].height);

			ledLine[converter->getPixelNumber(x, bouncingColumns[x].balls[i].position)] = bouncingColumns[x].balls[i].color;
		}
	}
	return true;
}
