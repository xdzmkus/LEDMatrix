// LEDMatrix.h

#ifndef _LEDMATRIX_h
#define _LEDMATRIX_h

#include <FastLED.h>
#include <LedEffect.h>
#include <SparklesLedEffect.h>
#include <ThreeColorLedEffect.h>

#include "ZigZagFromBottomRightToUpAndLeft.h"
#include "BouncingBallsMatrixLedEffect.h"
#include "BugsMatrixLedEffect.h"
#include "FireMatrixLedEffect.h"
#include "GravityMatrixLedEffect.h"
#include "NoiseMatrixLedEffect.h"
#include "RunningStringMatrixLedEffect.h"
#include "SinusMatrixLedEffect.h"
#include "SnowMatrixLedEffect.h"
#include "StarfallMatrixLedEffect.h"

class LEDMatrix
{
public:

	static const uint8_t NUM_EFFECTS = 17;
	const char* const availableEffects[NUM_EFFECTS] =
	{
		SparklesLedEffect::name,
		ThreeColorLedEffect::name,
		BugsMatrixLedEffect::name,
		"LAVA",
		BouncingBallsMatrixLedEffect::name,
		"OCEAN",
		SnowMatrixLedEffect::name,
		"RAINBOW",
		StarfallMatrixLedEffect::name,
		"CLOUD",
		FireMatrixLedEffect::name,
		"FOREST",
		SinusMatrixLedEffect::name,
		"PARTY",
		GravityMatrixLedEffect::name,
		"WRW",
		RunningStringMatrixLedEffect::name
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
		if (strcmp(SparklesLedEffect::name, effectName) == 0) {
			delete effect; effect = new SparklesLedEffect(leds, numLeds, 10);
		}
		else if (strcmp(ThreeColorLedEffect::name, effectName) == 0) {
			delete effect; effect = new ThreeColorLedEffect(leds, numLeds, 30, { CRGB::White, 3, CRGB::Red, 2, CRGB::White, 3 });
		}
		else if (strcmp(BouncingBallsMatrixLedEffect::name, effectName) == 0) {
			delete effect; effect = new BouncingBallsMatrixLedEffect(&matrix, leds, numLeds, 10, 3);
		}
		else if (strcmp(BugsMatrixLedEffect::name, effectName) == 0) {
			delete effect; effect = new BugsMatrixLedEffect(&matrix, leds, numLeds, 20, random8(25, 50));
		}
		else if (strcmp(FireMatrixLedEffect::name, effectName) == 0) {
			delete effect; effect = new FireMatrixLedEffect(&matrix, leds, numLeds, 10);
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
		else if (strcmp(RunningStringMatrixLedEffect::name, effectName) == 0) {
			delete effect; effect = new RunningStringMatrixLedEffect(&matrix, leds, numLeds, random(5, 30), text);
		}
		else if (strcmp(SinusMatrixLedEffect::name, effectName) == 0) {
			delete effect; effect = new SinusMatrixLedEffect(&matrix, leds, numLeds, random(10, 50));
		}
		else if (strcmp(SnowMatrixLedEffect::name, effectName) == 0) {
			delete effect; effect = new SnowMatrixLedEffect(&matrix, leds, numLeds, 2);
		}
		else if (strcmp(StarfallMatrixLedEffect::name, effectName) == 0) {
			delete effect; effect = new StarfallMatrixLedEffect(&matrix, leds, numLeds, 10);
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

	void setRunningString(const char* data, uint16_t len)
	{
		text = String(data);
	}

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
	ZigZagFromBottomRightToUpAndLeft<MATRIX_W, MATRIX_H> matrix;
	bool isOn;

	uint8_t currentEffectIdx;

	String text = String(RunningStringMatrixLedEffect::name);

	const CRGBPalette16 wrwPalette =
		CRGBPalette16(
			CRGB::White, CRGB::Red, CRGB::Red, CRGB::Red,
			CRGB::White, CRGB::Red, CRGB::Red, CRGB::Red,
			CRGB::White, CRGB::Red, CRGB::Red, CRGB::Red,
			CRGB::White, CRGB::Red, CRGB::Red, CRGB::Red
		);
};


#endif
