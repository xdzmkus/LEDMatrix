
#ifndef _LEDMATRIX_h
#define _LEDMATRIX_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <FastLED.h>
#include "LedEffect.h"

class LEDMatrix
{
protected:

	LedEffect** effects;
	const uint8_t numEffects;
	bool isOn = false;

public:

	LEDMatrix(LedEffect* effects[], uint8_t numEffects)	: effects(effects), numEffects(numEffects)
	{
	}

	virtual ~LEDMatrix()
	{
	}

	virtual void pause() { isOn = false; }
	virtual void resume() { isOn = true; }

	virtual bool isChanged()
	{
		if (!isOn || effects == NULL || numEffects == 0)
			return false;

		bool isChanged = false;

		for (uint8_t i = 0; i < numEffects; i++)
		{
			isChanged |= effects[i]->paint();
		}

		return isChanged;
	}
};

#endif
