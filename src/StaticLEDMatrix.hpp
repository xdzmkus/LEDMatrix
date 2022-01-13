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

	static FireMatrixLedEffect<MATRIX, ledLine, width, height>	fireEffect;
	static GravityMatrixLedEffect<MATRIX, ledLine, width, height> gravityEffect;
	static NoiseMatrixLedEffect<MATRIX, ledLine, width, height> noiseEffect;
	static RunningStringMatrixLedEffect<MATRIX, ledLine, width, height>	runningStrEffect;
	static SinusMatrixLedEffect<MATRIX, ledLine, width, height>	sinusEffect;
	static SnowMatrixLedEffect<MATRIX, ledLine, width, height> snowEffect;
	static StarfallMatrixLedEffect<MATRIX, ledLine, width, height> starfallEffect;
	static BouncingLinesMatrixLedEffect<MATRIX, ledLine, width, height, NUM_LINES> bouncingLinesEffect;

	ILedEffect* effects[NUM_EFFECTS] =
	{
		&fireEffect,
		&gravityEffect,
		&noiseEffect,
		&runningStrEffect,
		&sinusEffect,
		&snowEffect,
		&starfallEffect,
		&bouncingLinesEffect
	};

	LedEffectName const availableEffects[NUM_EFFECTS] =
	{
		fireEffect.name,
		gravityEffect.name,
		noiseEffect.name,
		runningStrEffect.name,
		sinusEffect.name,
		snowEffect.name,
		starfallEffect.name,
		bouncingLinesEffect.name
	};

public:

	StaticLEDMatrix()
	{
	};

	virtual ~StaticLEDMatrix()
	{
	};

	virtual uint8_t howManyEffects() const override
	{
		return NUM_EFFECTS;
	};

	virtual LedEffectName const* getAllEffectsNames() const override
	{
		return availableEffects;
	};

	virtual bool setEffectByName(LedEffectName effectName) override
	{
		for (uint8_t idx = 0; idx < NUM_EFFECTS; ++idx)
		{
			if (strcmp(*(effects[idx]), effectName) == 0)
			{
				activeEffect = effects[idx];
				activeEffect->start();
				return true;
			}
		}

		return false;
	};
};


template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
FireMatrixLedEffect<MATRIX, ledLine, width, height>	StaticLEDMatrix<MATRIX, ledLine, width, height>::fireEffect(random(10, 30));

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
GravityMatrixLedEffect<MATRIX, ledLine, width, height> StaticLEDMatrix<MATRIX, ledLine, width, height>::gravityEffect(random(10, 30));

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
NoiseMatrixLedEffect<MATRIX, ledLine, width, height> StaticLEDMatrix<MATRIX, ledLine, width, height>::noiseEffect(random(8, 50), wrwPalette_p);

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
RunningStringMatrixLedEffect<MATRIX, ledLine, width, height> StaticLEDMatrix<MATRIX, ledLine, width, height>::runningStrEffect(random(5, 30), "Static LED Matrix");

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
SinusMatrixLedEffect<MATRIX, ledLine, width, height> StaticLEDMatrix<MATRIX, ledLine, width, height>::sinusEffect(random(10, 50));

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
SnowMatrixLedEffect<MATRIX, ledLine, width, height> StaticLEDMatrix<MATRIX, ledLine, width, height>::snowEffect(random(1, 5));

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
StarfallMatrixLedEffect<MATRIX, ledLine, width, height> StaticLEDMatrix<MATRIX, ledLine, width, height>::starfallEffect(random(10, 30));

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
BouncingLinesMatrixLedEffect<MATRIX, ledLine, width, height, NUM_LINES> StaticLEDMatrix<MATRIX, ledLine, width, height>::bouncingLinesEffect(random(10, 30));

#endif
