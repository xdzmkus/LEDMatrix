/* 
* StarfallMatrixLedEffect.h
*
*/

#ifndef __STARFALLMATRIXLEDEFFECT_H__
#define __STARFALLMATRIXLEDEFFECT_H__

#include <LedEffect.h>
#include "IMatrixConverter.h"

class StarfallMatrixLedEffect : public LedEffect
{
public:

	static const char* const name;

private:

	const IMatrixConverter* converter;
	const CRGB rgb;

	CRGB fade;

public:

	StarfallMatrixLedEffect(const IMatrixConverter* converter, CRGB leds[], uint16_t count, uint16_t Hz, CRGB color = CRGB::Black);
	~StarfallMatrixLedEffect();

	bool paint() override;

	operator const char* () const { return name; }

private:

	StarfallMatrixLedEffect(const StarfallMatrixLedEffect&) = delete;
	StarfallMatrixLedEffect& operator=(const StarfallMatrixLedEffect&) = delete;
};

#endif
