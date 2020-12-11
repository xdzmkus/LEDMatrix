// LEDMatrix.h

#ifndef _LEDMATRIX_h
#define _LEDMATRIX_h

#include <FastLED.h>
#include <LedEffect.h>
#include "BottomLeftCorner.h"
#include "StarfallLedEffect.h"
#include "FireLedEffect.h"
#include "GravityLedEffect.h"

class LEDMatrix
{
public:

	static const uint8_t NUM_EFFECTS = 3;
	const char* const availableEffects[NUM_EFFECTS] =
	{
		StarfallLedEffect::name,
		FireLedEffect::name,
		GravityLedEffect::name
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
		if (strcmp(StarfallLedEffect::name, effectName) == 0) {
			delete effect; effect = new StarfallLedEffect(new BottomLeftCorner<MATRIX_W, MATRIX_H>, leds, numLeds, 10);
		}
		else if (strcmp(FireLedEffect::name, effectName) == 0) {
			delete effect; effect = new FireLedEffect(new BottomLeftCorner<MATRIX_W, MATRIX_H>, leds, numLeds, 10);
		}
		else if (strcmp(GravityLedEffect::name, effectName) == 0) {
			delete effect; effect = new GravityLedEffect(new BottomLeftCorner<MATRIX_W, MATRIX_H>, leds, numLeds, 1000);
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
