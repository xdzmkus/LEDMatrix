/*
* BouncingBallsMatrixLedEffect.hpp
*
*/

#ifndef __BOUNSINGBALLSMATRIXLEDEFFECT_HPP__
#define __BOUNSINGBALLSMATRIXLEDEFFECT_HPP__

#include <ILedEffect.hpp>
#include "ILedMatrix.hpp"

template<template <CRGB* const, const uint8_t, const uint8_t> class MATRIX, CRGB* const ledLine, const uint8_t width, const uint8_t height>
class BouncingBallsMatrixLedEffect : public MATRIX<ledLine, width, height>, public ILedEffect
{
private:

	const float Gravity = 9.8;
	const float MaxVelocity;

public:

	static const char* const name;

protected:

	struct BOUNCING_COLUMN
	{
		struct BOUNCING_BALL
		{
			CRGB	color;
			uint8_t position;
			float	altitude;
			float	velocity;
			uint8_t dampingPercentage;
			unsigned long startTime;
		}
		*balls = nullptr;

		uint8_t numBalls;
	}
	*bouncingColumns = nullptr;

public:
	BouncingBallsMatrixLedEffect(uint16_t Hz, uint8_t maxBallsCount = 1);
	~BouncingBallsMatrixLedEffect();

	void reset() override;
	void paint() override;

	operator const char* () const { return name; }

private:
	BouncingBallsMatrixLedEffect(const BouncingBallsMatrixLedEffect&) = delete;
	BouncingBallsMatrixLedEffect& operator=(const BouncingBallsMatrixLedEffect&) = delete;
};

template<template <CRGB* const, const uint8_t, const uint8_t> class MATRIX, CRGB* const ledLine, const uint8_t width, const uint8_t height>
const char* const BouncingBallsMatrixLedEffect<MATRIX, ledLine, width, height>::name = "BOUNCINGBALLS";

template<template <CRGB* const, const uint8_t, const uint8_t> class MATRIX, CRGB* const ledLine, const uint8_t width, const uint8_t height>
BouncingBallsMatrixLedEffect<MATRIX, ledLine, width, height>::BouncingBallsMatrixLedEffect(uint16_t Hz, uint8_t maxBallsCount)
	: ILedEffect(Hz), MaxVelocity(sqrt(2 * Gravity * (MATRIX<ledLine, width, height>::getHeight() - 1)))
{
	bouncingColumns = new typename BouncingBallsMatrixLedEffect<MATRIX, ledLine, width, height>::BOUNCING_COLUMN[MATRIX<ledLine, width, height>::getWidth()];

	if (bouncingColumns != nullptr)
	{
		for (uint8_t x = 0; x < MATRIX<ledLine, width, height>::getWidth(); x++)
		{
			bouncingColumns[x].numBalls = random8(0, maxBallsCount + 1);
			if (bouncingColumns[x].numBalls != 0)
			{
				bouncingColumns[x].balls = new typename BouncingBallsMatrixLedEffect<MATRIX, ledLine, width, height>::BOUNCING_COLUMN::BOUNCING_BALL[bouncingColumns[x].numBalls];
				if (bouncingColumns[x].balls == nullptr)
					bouncingColumns[x].numBalls = 0;
			}
		}
	}

	reset();
}

template<template <CRGB* const, const uint8_t, const uint8_t> class MATRIX, CRGB* const ledLine, const uint8_t width, const uint8_t height>
BouncingBallsMatrixLedEffect<MATRIX, ledLine, width, height>::~BouncingBallsMatrixLedEffect()
{
	if (bouncingColumns != nullptr)
	{
		for (uint8_t x = 0; x < MATRIX<ledLine, width, height>::getWidth(); x++)
		{
			if (bouncingColumns[x].numBalls != 0)
				delete[] bouncingColumns[x].balls;
		}
		delete[] bouncingColumns;
	}
}

template<template <CRGB* const, const uint8_t, const uint8_t> class MATRIX, CRGB* const ledLine, const uint8_t width, const uint8_t height>
void BouncingBallsMatrixLedEffect<MATRIX, ledLine, width, height>::reset()
{
	ILedEffect::reset();

	if (bouncingColumns != nullptr)
	{
		for (uint8_t x = 0; x < MATRIX<ledLine, width, height>::getWidth(); x++)
		{
			for (uint8_t i = 0; i < bouncingColumns[x].numBalls; i++)
			{
				bouncingColumns[x].balls[i].color = MATRIX<ledLine, width, height>::getRandomColor();
				bouncingColumns[x].balls[i].startTime = getClock();
				bouncingColumns[x].balls[i].altitude = 0;
				bouncingColumns[x].balls[i].position = 0;
				bouncingColumns[x].balls[i].velocity = MaxVelocity;
				bouncingColumns[x].balls[i].dampingPercentage = random8(70, 90);
			}
		}
	}

	MATRIX<ledLine, width, height>::clearAllLeds();
}

template<template <CRGB* const, const uint8_t, const uint8_t> class MATRIX, CRGB* const ledLine, const uint8_t width, const uint8_t height>
void BouncingBallsMatrixLedEffect<MATRIX, ledLine, width, height>::paint()
{
	if (bouncingColumns == nullptr)
		return;

	for (uint8_t x = 0; x < MATRIX<ledLine, width, height>::getWidth(); x++)
	{
		for (uint8_t i = 0; i < bouncingColumns[x].numBalls; i++)
		{
			float timeOfFlying = static_cast<float>(getClock() - bouncingColumns[x].balls[i].startTime) / ILedEffect::CLOCKS_IN_SEC;
			bouncingColumns[x].balls[i].altitude = bouncingColumns[x].balls[i].velocity * timeOfFlying - 0.5 * Gravity * pow(timeOfFlying, 2.0);

			if (bouncingColumns[x].balls[i].altitude < 0)
			{
				bouncingColumns[x].balls[i].altitude = 0;
				bouncingColumns[x].balls[i].velocity *= bouncingColumns[x].balls[i].dampingPercentage / 100.0;
				bouncingColumns[x].balls[i].startTime = getClock();

				if (bouncingColumns[x].balls[i].velocity < 0.01)
				{
					bouncingColumns[x].balls[i].velocity = MaxVelocity;
					bouncingColumns[x].balls[i].dampingPercentage = random8(70, 90);
				}
			}

			MATRIX<ledLine, width, height>::getPixel(x, bouncingColumns[x].balls[i].position) = CRGB::Black;

			bouncingColumns[x].balls[i].position = round(bouncingColumns[x].balls[i].altitude);

			MATRIX<ledLine, width, height>::getPixel(x, bouncingColumns[x].balls[i].position) = bouncingColumns[x].balls[i].color;
		}
	}
}

#endif

