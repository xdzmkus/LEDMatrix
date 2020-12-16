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

	static const uint8_t NUM_EFFECTS = 5;
	const char* const availableEffects[NUM_EFFECTS] =
	{
		StarfallMatrixLedEffect::name,
		FireMatrixLedEffect::name,
		GravityMatrixLedEffect::name,
		NoiseMatrixLedEffect::name,
		BouncingBallsMatrixLedEffect::name
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
		if (strcmp(StarfallMatrixLedEffect::name, effectName) == 0) {
			delete effect; effect = new StarfallMatrixLedEffect(new BottomLeftCorner<MATRIX_W, MATRIX_H>, leds, numLeds, 10);
		}
		else if (strcmp(FireMatrixLedEffect::name, effectName) == 0) {
			delete effect; effect = new FireMatrixLedEffect(new BottomLeftCorner<MATRIX_W, MATRIX_H>, leds, numLeds, 10);
		}
		else if (strcmp(GravityMatrixLedEffect::name, effectName) == 0) {
			delete effect; effect = new GravityMatrixLedEffect(new BottomLeftCorner<MATRIX_W, MATRIX_H>, leds, numLeds, 1000);
		}
		else if (strcmp(NoiseMatrixLedEffect::name, effectName) == 0) {
			delete effect; effect = new NoiseMatrixLedEffect(new BottomLeftCorner<MATRIX_W, MATRIX_H>, leds, numLeds, 10, PartyColors_p);
		}
		else if (strcmp(BouncingBallsMatrixLedEffect::name, effectName) == 0) {
			delete effect; effect = new BouncingBallsMatrixLedEffect(new BottomLeftCorner<MATRIX_W, MATRIX_H>, leds, numLeds, 10, 3);
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
	bool isOn;
};


#endif
