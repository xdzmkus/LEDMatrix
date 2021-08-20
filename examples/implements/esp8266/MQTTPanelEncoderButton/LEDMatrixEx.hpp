// LEDMatrixEx.hpp

#ifndef _LEDMATRIXEX_HPP_
#define _LEDMATRIXEX_HPP_

#include "LEDMatrix.h"
#include "UniversalLEDMatrixEffects.h"
#include "StaticLEDMatrixEffects.h"
#include "DynamicLEDMatrixEffects.h"
#include <DynamicLEDLineEffects.h>

#define NUM_LINES 2

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
class LEDMatrixEx : public LEDMatrix
{
private:

	enum NOISE_NAME { WRW = 0, OCEAN, RAINBOW, CLOUD, FOREST, PARTY, LAVA };

	const char* const noiseEffects[7] =
	{
		"WRW_NOISE",
		"OCEAN_NOISE",
		"RAINBOW_NOISE",
		"CLOUD_NOISE",
		"FOREST_NOISE",
		"PARTY_NOISE",
		"LAVA_NOISE"
	};

	static const uint8_t NUM_EFFECTS = 20;
	const char* availableEffects[NUM_EFFECTS] =
	{
		ColorsLedEffect<leds, width * height>::name,
		SparklesLedEffect<leds, width * height>::name,
		RainbowLedEffect<leds, width * height>::name,
		ThreeColorLedEffect<leds, width * height>::name,

		BugsMatrixLedEffect<MATRIX, ledLine, width, height>::name,
		noiseEffects[WRW],
		BouncingBallsMatrixLedEffect<MATRIX, ledLine, width, height>::name,
		noiseEffects[OCEAN],
		SnowMatrixLedEffect<MATRIX, ledLine, width, height>::name,
		noiseEffects[RAINBOW],
		StarfallMatrixLedEffect<MATRIX, ledLine, width, height>::name,
		noiseEffects[CLOUD],
		FireMatrixLedEffect<MATRIX, ledLine, width, height>::name,
		noiseEffects[FOREST],
		SinusMatrixLedEffect<MATRIX, ledLine, width, height>::name,
		noiseEffects[PARTY],
		GravityMatrixLedEffect<MATRIX, ledLine, width, height>::name,
		noiseEffects[LAVA],
		RunningStringMatrixLedEffect<MATRIX, ledLine, width, height>::name,

		BouncingLinesMatrixLedEffect<MATRIX, ledLine, width, height, NUM_LINES>::name,
	};

public:

	LEDMatrixEx()
	{
		text = String("Dynamic LED Matrix Extended");
	};

	~LEDMatrixEx()
	{
	};

	uint8_t howManyEffects() const override
	{
		return NUM_EFFECTS;
	};

	const char* const* getAllEffectsNames() const override
	{
		return availableEffects;
	};

	bool setEffectByName(const char* effectName) override
	{
		if (strcmp(BouncingLinesMatrixLedEffect<MATRIX, ledLine, width, height, NUM_LINES>::name, effectName) == 0)
		{
			delete effect; effect = new BouncingLinesMatrixLedEffect<MATRIX, ledLine, width, height, NUM_LINES>(random(5, 20));
		}
		else if (strcmp(BouncingBallsMatrixLedEffect<MATRIX, ledLine, width, height>::name, effectName) == 0)
		{
			delete effect; effect = new BouncingBallsMatrixLedEffect<MATRIX, ledLine, width, height>(10, 3);
		}
		else if (strcmp(BugsMatrixLedEffect<MATRIX, ledLine, width, height>::name, effectName) == 0)
		{
			delete effect; effect = new BugsMatrixLedEffect<MATRIX, ledLine, width, height>(random(10, 30), random(width, width << 1));
		}
		else if (strcmp(FireMatrixLedEffect<MATRIX, ledLine, width, height>::name, effectName) == 0)
		{
			delete effect; effect = new FireMatrixLedEffect<MATRIX, ledLine, width, height>(random(10, 30));
		}
		else if (strcmp(GravityMatrixLedEffect<MATRIX, ledLine, width, height>::name, effectName) == 0)
		{
			delete effect; effect = new GravityMatrixLedEffect<MATRIX, ledLine, width, height>(random(10, 30));
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
		else if (strcmp(noiseEffects[LAVA], effectName) == 0)
		{
			delete effect; effect = new NoiseMatrixLedEffect<MATRIX, ledLine, width, height>(random(8, 30), LavaColors_p, random(5, 50));
			if (effect != NULL) effect->setId(noiseEffects[LAVA]);
		}
		else if (strcmp(noiseEffects[OCEAN], effectName) == 0)
		{
			delete effect; effect = new NoiseMatrixLedEffect<MATRIX, ledLine, width, height>(random(8, 30), OceanColors_p, random(5, 50));
			if (effect != NULL) effect->setId(noiseEffects[OCEAN]);
		}
		else if (strcmp(noiseEffects[RAINBOW], effectName) == 0)
		{
			delete effect; effect = new NoiseMatrixLedEffect<MATRIX, ledLine, width, height>(random(8, 50), RainbowColors_p, random(5, 50));
			if (effect != NULL) effect->setId(noiseEffects[RAINBOW]);
		}
		else if (strcmp(noiseEffects[CLOUD], effectName) == 0)
		{
			delete effect; effect = new NoiseMatrixLedEffect<MATRIX, ledLine, width, height>(random(8, 30), CloudColors_p, random(5, 50));
			if (effect != NULL) effect->setId(noiseEffects[CLOUD]);
		}
		else if (strcmp(noiseEffects[FOREST], effectName) == 0)
		{
			delete effect; effect = new NoiseMatrixLedEffect<MATRIX, ledLine, width, height>(random(8, 30), ForestColors_p, random(5, 50));
			if (effect != NULL) effect->setId(noiseEffects[FOREST]);
		}
		else if (strcmp(noiseEffects[PARTY], effectName) == 0)
		{
			delete effect; effect = new NoiseMatrixLedEffect<MATRIX, ledLine, width, height>(random(8, 50), PartyColors_p, random(5, 50));
			if (effect != NULL) effect->setId(noiseEffects[PARTY]);
		}
		else if (strcmp(noiseEffects[WRW], effectName) == 0)
		{
			delete effect; effect = new NoiseMatrixLedEffect<MATRIX, ledLine, width, height>(random(10, 30), wrwPalette_p, random(5, 50));
			if (effect != NULL) effect->setId(noiseEffects[WRW]);
		}
		else if (strcmp(ColorsLedEffect<leds, width * height>::name, effectName) == 0)
		{
			delete effect; effect = new ColorsLedEffect<leds, width * height>(random(10, 30));
		}
		else if (strcmp(SparklesLedEffect<leds, width * height>::name, effectName) == 0)
		{
			delete effect; effect = new SparklesLedEffect<leds, width* height>(random(10, 30));
		}
		else if (strcmp(RainbowLedEffect<leds, width * height>::name, effectName) == 0)
		{
			delete effect; effect = new RainbowLedEffect<leds, width * height>(random(10, 30));
		}
		else if (strcmp(ThreeColorLedEffect<leds, width * height>::name, effectName) == 0)
		{
			delete effect; effect = new ThreeColorLedEffect<leds, width * height>(30, { CRGB::White, 3, CRGB::Red, 2, CRGB::White, 3 });
		}
		else
		{
			return false;
		}

		if (effect != nullptr) effect->start();

		return true;
	};
};


#endif
