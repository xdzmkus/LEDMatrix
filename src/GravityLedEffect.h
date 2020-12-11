/* 
* GravityLedEffect.h
*
*/

#ifndef __GRAVITYLEDEFFECT_H__
#define __GRAVITYLEDEFFECT_H__

#include <LedEffect.h>
#include "IMatrixConverter.h"

class GravityLedEffect : public LedEffect
{
public:

	static const char* const name;

private:

	const IMatrixConverter* converter;

	const CRGB rgb;

	struct GRAVITY
	{
		uint8_t limit;
		bool direction;
		uint8_t velocity;
	}
	*gravities;

	MillisTimer newShot;

public:

	GravityLedEffect(const IMatrixConverter* converter, CRGB leds[], uint16_t count, uint16_t Hz, CRGB color = CRGB::Black, uint16_t interval = 3000);
	~GravityLedEffect();

	void init() override;
	bool paint() override;

	operator const char* () const { return name; }

private:

	void shiftPixel(uint8_t x, uint8_t y);

	GravityLedEffect(const GravityLedEffect&) = delete;
	GravityLedEffect& operator=(const GravityLedEffect&) = delete;
};

#endif
