/*
* DynamicLEDMatrix.hpp
*
*/

#ifndef _DYNAMICLEDMATRIX_HPP_
#define _DYNAMICLEDMATRIX_HPP_

#include "LEDMatrix.h"
#include "DynamicLEDMatrixEffects.h"
#include "UniversalLEDMatrixEffects.h"

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
class DynamicLEDMatrix : public LEDMatrix
{
private:

	static const uint8_t NUM_EFFECTS;
	static const char* const availableEffects[];

public:

	DynamicLEDMatrix()
	{
		text = String("Dynamic LED Matrix");
	};

	virtual ~DynamicLEDMatrix()
	{
	};

	virtual uint8_t howManyEffects() const override
	{
		return NUM_EFFECTS;
	};

	virtual const char* const* getAllEffectsNames() const override
	{
		return availableEffects;
	};

	virtual bool setEffectByName(const char* effectName) override
	{
		if (strcmp(BouncingBallsMatrixLedEffect<MATRIX, ledLine, width, height>::name, effectName) == 0)
		{
			delete effect; effect = new BouncingBallsMatrixLedEffect<MATRIX, ledLine, width, height>(random(10, 30), min(width, height)>>1);
		}
		else if (strcmp(BugsMatrixLedEffect<MATRIX, ledLine, width, height>::name, effectName) == 0)
		{
			delete effect; effect = new BugsMatrixLedEffect<MATRIX, ledLine, width, height>(random(10, 30), random(1, max(width, height)));
		}
		else if (strcmp(FireMatrixLedEffect<MATRIX, ledLine, width, height>::name, effectName) == 0)
		{
			delete effect; effect = new FireMatrixLedEffect<MATRIX, ledLine, width, height>(random(10, 30));
		}
		else if (strcmp(GravityMatrixLedEffect<MATRIX, ledLine, width, height>::name, effectName) == 0)
		{
			delete effect; effect = new GravityMatrixLedEffect<MATRIX, ledLine, width, height>(random(10, 30));
		}
		else if (strcmp(NoiseMatrixLedEffect<MATRIX, ledLine, width, height>::name, effectName) == 0)
		{
			delete effect; effect = new NoiseMatrixLedEffect<MATRIX, ledLine, width, height>(random(8, 50), wrwPalette_p);
		}
		else if (strcmp(RunningStringMatrixLedEffect<MATRIX, ledLine, width, height>::name, effectName) == 0)
		{
			delete effect; effect = new RunningStringMatrixLedEffect<MATRIX, ledLine, width, height>(random(5, 30), text);
		}
		else if (strcmp(SinusMatrixLedEffect<MATRIX, ledLine, width, height>::name, effectName) == 0)
		{
			delete effect; effect = new SinusMatrixLedEffect<MATRIX, ledLine, width, height>(random(10, 50));
		}
		else if (strcmp(SnowMatrixLedEffect<MATRIX, ledLine, width, height>::name, effectName) == 0)
		{
			delete effect; effect = new SnowMatrixLedEffect<MATRIX, ledLine, width, height>(random(1, 5));
		}
		else if (strcmp(StarfallMatrixLedEffect<MATRIX, ledLine, width, height>::name, effectName) == 0)
		{
			delete effect; effect = new StarfallMatrixLedEffect<MATRIX, ledLine, width, height>(random(10, 30));
		}
		else
		{
			return false;
		}

		if (effect != nullptr) effect->start();

		return true;
	};
};


template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
const uint8_t DynamicLEDMatrix<MATRIX, ledLine, width, height>::NUM_EFFECTS = 9;

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
const char* const DynamicLEDMatrix<MATRIX, ledLine, width, height>::availableEffects[] =
{
	BouncingBallsMatrixLedEffect<MATRIX, ledLine, width, height>::name,
	BugsMatrixLedEffect<MATRIX, ledLine, width, height>::name,
	FireMatrixLedEffect<MATRIX, ledLine, width, height>::name,
	GravityMatrixLedEffect<MATRIX, ledLine, width, height>::name,
	NoiseMatrixLedEffect<MATRIX, ledLine, width, height>::name,
	RunningStringMatrixLedEffect<MATRIX, ledLine, width, height>::name,
	SinusMatrixLedEffect<MATRIX, ledLine, width, height>::name,
	SnowMatrixLedEffect<MATRIX, ledLine, width, height>::name,
	StarfallMatrixLedEffect<MATRIX, ledLine, width, height>::name
};

#endif
