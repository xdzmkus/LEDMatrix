/*
* BouncingBallsMatrixLedEffect.cpp
*
*/

#include "BouncingBallsMatrixLedEffect.h"

const char* const BouncingBallsMatrixLedEffect::name = "BOUNCINGBALLS";

BouncingBallsMatrixLedEffect::BouncingBallsMatrixLedEffect(ILedMatrix* converter, uint16_t Hz, uint8_t maxBallsCount)
	: ILedEffect(Hz), matrix(converter), MaxVelocity(sqrt(2 * Gravity * (converter->getHeight() - 1)))
{
	bouncingColumns = new BOUNCING_COLUMN[matrix->getWidth()];

	if (bouncingColumns != nullptr)
	{
		for (uint8_t x = 0; x < matrix->getWidth(); x++)
		{
			bouncingColumns[x].numBalls = random8(0, maxBallsCount + 1);
			if (bouncingColumns[x].numBalls != 0)
			{
				bouncingColumns[x].balls = new BOUNCING_COLUMN::BOUNCING_BALL[bouncingColumns[x].numBalls];
				if (bouncingColumns[x].balls == nullptr)
					bouncingColumns[x].numBalls = 0;
			}
		}
	}

	reset();
}

BouncingBallsMatrixLedEffect::~BouncingBallsMatrixLedEffect()
{
	if (bouncingColumns != nullptr)
	{
		for (uint8_t x = 0; x < matrix->getWidth(); x++)
		{
			if (bouncingColumns[x].numBalls != 0)
				delete[] bouncingColumns[x].balls;
		}
		delete[] bouncingColumns;
	}
}

void BouncingBallsMatrixLedEffect::reset()
{
	ILedEffect::reset();

	if (bouncingColumns != nullptr)
	{
		for (uint8_t x = 0; x < matrix->getWidth(); x++)
		{
			for (uint8_t i = 0; i < bouncingColumns[x].numBalls; i++)
			{
				bouncingColumns[x].balls[i].color = matrix->getRandomColor();
				bouncingColumns[x].balls[i].startTime = getClock();
				bouncingColumns[x].balls[i].height = 0;
				bouncingColumns[x].balls[i].position = 0;
				bouncingColumns[x].balls[i].velocity = MaxVelocity;
				bouncingColumns[x].balls[i].dampingPercentage = random8(70, 90);
			}
		}
	}

	matrix->clearAllLeds();
}

void BouncingBallsMatrixLedEffect::paint()
{
	if (bouncingColumns == nullptr)
		return;

	for (uint8_t x = 0; x < matrix->getWidth(); x++)
	{
		for (uint8_t i = 0; i < bouncingColumns[x].numBalls; i++)
		{
			float timeOfFlying = static_cast<float>(getClock() - bouncingColumns[x].balls[i].startTime) / ILedEffect::CLOCKS_IN_SEC;
			bouncingColumns[x].balls[i].height = bouncingColumns[x].balls[i].velocity * timeOfFlying - 0.5 * Gravity * pow(timeOfFlying, 2.0);

			if (bouncingColumns[x].balls[i].height < 0)
			{
				bouncingColumns[x].balls[i].height = 0;
				bouncingColumns[x].balls[i].velocity *= bouncingColumns[x].balls[i].dampingPercentage / 100.0;
				bouncingColumns[x].balls[i].startTime = getClock();

				if (bouncingColumns[x].balls[i].velocity < 0.01)
				{
					bouncingColumns[x].balls[i].velocity = MaxVelocity;
					bouncingColumns[x].balls[i].dampingPercentage = random8(70, 90);
				}
			}

			matrix->getPixel(x, bouncingColumns[x].balls[i].position) = CRGB::Black;

			bouncingColumns[x].balls[i].position = round(bouncingColumns[x].balls[i].height);

			matrix->getPixel(x, bouncingColumns[x].balls[i].position) = bouncingColumns[x].balls[i].color;
		}
	}
}
