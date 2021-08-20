/*
* StaticLEDMatrix.hpp
*
*/

#ifndef _STATICLEDMATRIX_HPP_
#define _STATICLEDMATRIX_HPP_

#define NUM_LINES 4

#include "LEDMatrix.h"
#include "UniversalLEDMatrixEffects.h"
#include "StaticLEDMatrixEffects.h"

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
class StaticLEDMatrix : public LEDMatrix
{
private:

	static const uint8_t NUM_EFFECTS = 8;

	FireMatrixLedEffect<MATRIX, ledLine, width, height>				eff1;
	GravityMatrixLedEffect<MATRIX, ledLine, width, height>			eff2;
	NoiseMatrixLedEffect<MATRIX, ledLine, width, height>			eff3;
	RunningStringMatrixLedEffect<MATRIX, ledLine, width, height>	eff4;
	SinusMatrixLedEffect<MATRIX, ledLine, width, height>			eff5;
	SnowMatrixLedEffect<MATRIX, ledLine, width, height>				eff6;
	StarfallMatrixLedEffect<MATRIX, ledLine, width, height>			eff7;

	BouncingLinesMatrixLedEffect<MATRIX, ledLine, width, height, NUM_LINES> eff8;

	const char* const availableEffects[NUM_EFFECTS] = { eff1.name, eff2.name, eff3.name, eff4.name, eff5.name, eff6.name, eff7.name, eff8.name };

	ILedEffect* const effects[NUM_EFFECTS] = { &eff1, &eff2, &eff3, &eff4, &eff5, &eff6, &eff7, &eff8 };

public:

	StaticLEDMatrix() :
		eff1(random(10, 30)),
		eff2(random(10, 30)),
		eff3(random(8, 50), wrwPalette_p),
		eff4(random(5, 30), "Static LED Matrix"),
		eff5(random(10, 50)),
		eff6(random(1, 5)),
		eff7(random(10, 30)),
		eff8(random(10, 30))
	{
	};

	virtual ~StaticLEDMatrix()
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
		for (uint8_t idx = 0; idx < NUM_EFFECTS; ++idx)
		{
			if (strcmp(*(effects[idx]), effectName) == 0)
			{
				effect = effects[idx];
				effect->start();
				return true;
			}
		}

		return false;
	};
};

/*
template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
const uint8_t StaticLEDMatrix<leds, numLeds>::NUM_EFFECTS = 8;
*/

#endif
