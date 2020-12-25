// LEDMatrix.h

#ifndef _LEDMATRIX_h
#define _LEDMATRIX_h

#include <FastLED.h>
#include <LedEffect.h>
#include <BugsLedEffect.h>
#include <SparklesLedEffect.h>
#include <ThreeColorLedEffect.h>

#include "BottomLeftCorner.h"
#include "StarfallMatrixLedEffect.h"
#include "FireMatrixLedEffect.h"
#include "GravityMatrixLedEffect.h"
#include "NoiseMatrixLedEffect.h"
#include "BouncingBallsMatrixLedEffect.h"
#include "SinusMatrixLedEffect.h"

class LEDMatrix
{
public:

	static const uint8_t NUM_EFFECTS = 14;
	const char* const availableEffects[NUM_EFFECTS] =
	{
		BugsLedEffect::name,
		"LAVA",
		BouncingBallsMatrixLedEffect::name,
		"OCEAN",
		SparklesLedEffect::name,
		"RAINBOW",
		StarfallMatrixLedEffect::name,
		"CLOUD",
		FireMatrixLedEffect::name,
		"FOREST",
		SinusMatrixLedEffect::name,
		"PARTY",
		GravityMatrixLedEffect::name,
		"WRW"
	};

	LEDMatrix(CRGB leds[], uint16_t count) : leds(leds), numLeds(count), isOn(false), currentEffectIdx(0)
	{
	};

	~LEDMatrix()
	{
		if (effect != nullptr)
		{
			delete effect;
		}
	};

	bool setEffectByName(const char* effectName)
	{
		if (strcmp(BugsLedEffect::name, effectName) == 0) {
			delete effect; effect = new BugsLedEffect(leds, numLeds, 20, 50);
		}
		else if (strcmp(SparklesLedEffect::name, effectName) == 0) {
			delete effect; effect = new SparklesLedEffect(leds, numLeds, 10);
		}
		else if (strcmp(ThreeColorLedEffect::name, effectName) == 0) {
			delete effect; effect = new ThreeColorLedEffect(leds, numLeds, 30, { CRGB::White, 4, CRGB::Red, 3, CRGB::White, 4 }, 1, 2);
		}
		else if (strcmp(BouncingBallsMatrixLedEffect::name, effectName) == 0) {
			delete effect; effect = new BouncingBallsMatrixLedEffect(&matrix, leds, numLeds, 10, 3);
		}
		else if (strcmp(StarfallMatrixLedEffect::name, effectName) == 0) {
			delete effect; effect = new StarfallMatrixLedEffect(&matrix, leds, numLeds, 10);
		}
		else if (strcmp(FireMatrixLedEffect::name, effectName) == 0) {
			delete effect; effect = new FireMatrixLedEffect(&matrix, leds, numLeds, 10);
		}
		else if (strcmp(SinusMatrixLedEffect::name, effectName) == 0) {
			delete effect; effect = new SinusMatrixLedEffect(&matrix, leds, numLeds, random(10, 50));
		}
		else if (strcmp(GravityMatrixLedEffect::name, effectName) == 0) {
			delete effect; effect = new GravityMatrixLedEffect(&matrix, leds, numLeds, 10);
		}
		else if (strcmp("PARTY", effectName) == 0) {
			delete effect; effect = new NoiseMatrixLedEffect(&matrix, leds, numLeds, random(10, 50), PartyColors_p, random(5, 60));
		}
		else if (strcmp("CLOUD", effectName) == 0) {
			delete effect; effect = new NoiseMatrixLedEffect(&matrix, leds, numLeds, random(8, 30), CloudColors_p);
		}
		else if (strcmp("LAVA", effectName) == 0) {
			delete effect; effect = new NoiseMatrixLedEffect(&matrix, leds, numLeds, random(8, 30), LavaColors_p);
		}
		else if (strcmp("OCEAN", effectName) == 0) {
			delete effect; effect = new NoiseMatrixLedEffect(&matrix, leds, numLeds, random(8, 30), OceanColors_p);
		}
		else if (strcmp("FOREST", effectName) == 0) {
			delete effect; effect = new NoiseMatrixLedEffect(&matrix, leds, numLeds, random(8, 30), ForestColors_p);
		}
		else if (strcmp("RAINBOW", effectName) == 0) {
			delete effect; effect = new NoiseMatrixLedEffect(&matrix, leds, numLeds, random(10, 50), RainbowColors_p, random(5, 80));
		}
		else if (strcmp("WRW", effectName) == 0) {
			delete effect; effect = new NoiseMatrixLedEffect(&matrix, leds, numLeds, random(10, 30), wrwPalette, random(5, 50));
		}
		else {
			return false;
		}

		// find effect index
		for (uint8_t idx = 0; idx < NUM_EFFECTS; idx++)
		{
			if (strcmp(availableEffects[idx], effectName) == 0)
			{
				currentEffectIdx = idx;
				break;
			}
		}

		isOn = true;
		return true;

	};

	bool setNextEffect()
	{
		if (++currentEffectIdx > NUM_EFFECTS - 1)
			currentEffectIdx = 0;

		return setEffectByName(availableEffects[currentEffectIdx]);
	};

	const char* getEffectName() const
	{
		return (effect != nullptr) ? availableEffects[currentEffectIdx] : nullptr;
	};

	void pause()
	{
		isOn = false;
		if (effect != nullptr) effect->stop();
	};
	void resume()
	{
		isOn = true;
		if (effect != nullptr) effect->start();
	};
	bool paint()
	{
		return (!isOn || effect == nullptr) ? false : effect->paint();
	};
	bool isRunning()
	{
		return isOn;
	};

protected:

	CRGB* const leds;
	const uint16_t numLeds;

	LedEffect* effect = nullptr;
	BottomLeftCorner<MATRIX_W, MATRIX_H> matrix;
	bool isOn;

	uint8_t currentEffectIdx;

	const CRGBPalette16 wrwPalette = CRGBPalette16(
		CRGB::White, CRGB::Red, CRGB::Red, CRGB::Red,
		CRGB::White, CRGB::Red, CRGB::Red, CRGB::Red,
		CRGB::White, CRGB::Red, CRGB::Red, CRGB::Red,
		CRGB::White, CRGB::Red, CRGB::Red, CRGB::Red);
};


#endif
