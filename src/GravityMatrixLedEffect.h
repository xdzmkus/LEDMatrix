/* 
* GravityMatrixLedEffect.h
*
*/

#ifndef __GRAVITYMATRIXLEDEFFECT_H__
#define __GRAVITYMATRIXLEDEFFECT_H__

#include <LedEffect.h>
#include "IMatrixConverter.h"

class GravityMatrixLedEffect : public LedEffect
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

	GravityMatrixLedEffect(const IMatrixConverter* converter, CRGB leds[], uint16_t count, uint16_t Hz, CRGB color = CRGB::Black, uint16_t interval = 3000);
	~GravityMatrixLedEffect();

	void init() override;
	bool paint() override;

	operator const char* () const { return name; }

private:

	void shiftPixel(uint8_t x, uint8_t y);

	GravityMatrixLedEffect(const GravityMatrixLedEffect&) = delete;
	GravityMatrixLedEffect& operator=(const GravityMatrixLedEffect&) = delete;
};

#endif
