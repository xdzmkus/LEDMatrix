#ifndef _LEDMATRIXEX_h
#define _LEDMATRIXEX_h

#include <FastLED.h>
#include "ILedEffect.h"

class LEDMatrixEx
{
protected:

	ILedEffect** effects;
	const uint8_t numEffects;
	bool isOn = false;

public:

	LEDMatrixEx(ILedEffect* effects[], uint8_t numEffects) : effects(effects), numEffects(numEffects)
	{
	}

	virtual ~LEDMatrixEx()
	{
	}

	virtual void pause()
	{
		isOn = false;
		for (uint8_t x = 0; x < numEffects; x++)
		{
			effects[x]->stop();
		}
	}
	virtual void resume()
	{
		isOn = true;
		for (uint8_t x = 0; x < numEffects; x++)
		{
			effects[x]->start();
		}
	}

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
