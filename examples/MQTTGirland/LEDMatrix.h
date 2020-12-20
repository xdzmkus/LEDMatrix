// LEDMatrix.h

#ifndef _LEDMATRIX_h
#define _LEDMATRIX_h

#include <FastLED.h>
#include <LedEffect.h>

#include "BottomLeftCorner.h"
#include "StarfallMatrixLedEffect.h"
#include "FireMatrixLedEffect.h"
#include "GravityMatrixLedEffect.h"
#include "NoiseMatrixLedEffect.h"
#include "BouncingBallsMatrixLedEffect.h"

class LEDMatrix
{
public:

	static const uint8_t NUM_EFFECTS = 10;
	const char* const availableEffects[NUM_EFFECTS] =
	{
		"RAINBOW",
		BouncingBallsMatrixLedEffect::name,
		"OCEAN",
		"LAVA",
		StarfallMatrixLedEffect::name,
		"CLOUD",
		FireMatrixLedEffect::name,
		"FOREST",
		GravityMatrixLedEffect::name,
		"PARTY"
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
		if (strcmp(BouncingBallsMatrixLedEffect::name, effectName) == 0) {
			delete effect; effect = new BouncingBallsMatrixLedEffect(new BottomLeftCorner<MATRIX_W, MATRIX_H>, leds, numLeds, 10, 3);
		}
		else if (strcmp(StarfallMatrixLedEffect::name, effectName) == 0) {
			delete effect; effect = new StarfallMatrixLedEffect(new BottomLeftCorner<MATRIX_W, MATRIX_H>, leds, numLeds, 10);
		}
		else if (strcmp(FireMatrixLedEffect::name, effectName) == 0) {
			delete effect; effect = new FireMatrixLedEffect(new BottomLeftCorner<MATRIX_W, MATRIX_H>, leds, numLeds, 10);
		}
		else if (strcmp(GravityMatrixLedEffect::name, effectName) == 0) {
			delete effect; effect = new GravityMatrixLedEffect(new BottomLeftCorner<MATRIX_W, MATRIX_H>, leds, numLeds, 1000);
		}
		else if (strcmp("PARTY", effectName) == 0) {
			delete effect; effect = new NoiseMatrixLedEffect(new BottomLeftCorner<MATRIX_W, MATRIX_H>, leds, numLeds, 10, PartyColors_p);
		}
		else if (strcmp("CLOUD", effectName) == 0) {
			delete effect; effect = new NoiseMatrixLedEffect(new BottomLeftCorner<MATRIX_W, MATRIX_H>, leds, numLeds, 10, CloudColors_p);
		}
		else if (strcmp("LAVA", effectName) == 0) {
			delete effect; effect = new NoiseMatrixLedEffect(new BottomLeftCorner<MATRIX_W, MATRIX_H>, leds, numLeds, 10, LavaColors_p);
		}
		else if (strcmp("OCEAN", effectName) == 0) {
			delete effect; effect = new NoiseMatrixLedEffect(new BottomLeftCorner<MATRIX_W, MATRIX_H>, leds, numLeds, 10, OceanColors_p);
		}
		else if (strcmp("FOREST", effectName) == 0) {
			delete effect; effect = new NoiseMatrixLedEffect(new BottomLeftCorner<MATRIX_W, MATRIX_H>, leds, numLeds, 10, ForestColors_p);
		}
		else if (strcmp("RAINBOW", effectName) == 0) {
			delete effect; effect = new NoiseMatrixLedEffect(new BottomLeftCorner<MATRIX_W, MATRIX_H>, leds, numLeds, 10, RainbowColors_p);
		}
		else {
			return false;
		}

		// find effect index
		for (uint8_t idx = 0; idx < NUM_EFFECTS - 1; idx++)
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
	bool isOn;

	uint8_t currentEffectIdx;
};


#endif
