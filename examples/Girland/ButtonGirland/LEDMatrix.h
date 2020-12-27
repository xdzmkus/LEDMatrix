// LEDMatrix.h

#ifndef _LEDMATRIX_h
#define _LEDMATRIX_h

#include <FastLED.h>
#include <LedEffect.h>
#include "BottomLeftCorner.h"
#include "BouncingBallsMatrixLedEffect.h"
#include "FireMatrixLedEffect.h"
#include "GravityMatrixLedEffect.h"
#include "NoiseMatrixLedEffect.h"
#include "SinusMatrixLedEffect.h"
#include "SnowMatrixLedEffect.h"
#include "StarfallMatrixLedEffect.h"

class LEDMatrix
{
public:

	static const uint8_t NUM_EFFECTS = 7;
	const char* const availableEffects[NUM_EFFECTS] =
	{
		BouncingBallsMatrixLedEffect::name,
		FireMatrixLedEffect::name,
		GravityMatrixLedEffect::name,
		NoiseMatrixLedEffect::name,
		SinusMatrixLedEffect::name,
		SnowMatrixLedEffect::name,
		StarfallMatrixLedEffect::name
	};

	LEDMatrix(CRGB leds[], uint16_t count) : leds(leds), numLeds(count), isOn(false)
	{
	};

	~LEDMatrix()
	{
		delete effect;
	};

	bool setEffectByName(const char* effectName)
	{
		if (strcmp(BouncingBallsMatrixLedEffect::name, effectName) == 0) {
			delete effect; effect = new BouncingBallsMatrixLedEffect(&matrix, leds, numLeds, 10, 3);
		}
		else if (strcmp(FireMatrixLedEffect::name, effectName) == 0) {
			delete effect; effect = new FireMatrixLedEffect(&matrix, leds, numLeds, 10);
		}
		else if (strcmp(GravityMatrixLedEffect::name, effectName) == 0) {
			delete effect; effect = new GravityMatrixLedEffect(&matrix, leds, numLeds, 10);
		}
		else if (strcmp(NoiseMatrixLedEffect::name, effectName) == 0) {
			delete effect; effect = new NoiseMatrixLedEffect(&matrix, leds, numLeds, 10, wrwPalette);
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

		isOn = true;
		return true;
	};

	bool setNextEffect()
	{
		if (effect != nullptr)
		{
			for (uint8_t idx = 0; idx < NUM_EFFECTS - 1; idx++)
			{
				if (strcmp(availableEffects[idx], static_cast<const char*>(*effect)) == 0)
				{
					return setEffectByName(availableEffects[idx + 1]);
				}
			}
		}

		// first one by default
		return setEffectByName(availableEffects[0]);
	};

	const char* getEffectName() const
	{
		return (effect != nullptr) ? static_cast<const char*>(*effect) : nullptr;
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

	const CRGBPalette16 wrwPalette =
		CRGBPalette16(
			CRGB::White, CRGB::Red, CRGB::Red, CRGB::Red,
			CRGB::White, CRGB::Red, CRGB::Red, CRGB::Red,
			CRGB::White, CRGB::Red, CRGB::Red, CRGB::Red,
			CRGB::White, CRGB::Red, CRGB::Red, CRGB::Red);

};

#endif