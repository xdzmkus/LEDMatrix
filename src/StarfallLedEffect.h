/* 
* StarfallLedEffect.h
*
*/

#ifndef __STARFALLLEDEFFECT_H__
#define __STARFALLLEDEFFECT_H__

#include <LedEffect.h>
#include "IMatrixConverter.h"

class StarfallLedEffect : public LedEffect
{
public:

	static const char* const name;

private:

	const IMatrixConverter* converter;
	const CRGB rgb;

	CRGB fade;

public:

	StarfallLedEffect(const IMatrixConverter* converter, CRGB leds[], uint16_t count, uint16_t Hz, CRGB color = CRGB::Black);
	~StarfallLedEffect();

	bool paint() override;

	operator const char* () const { return name; }

private:

	StarfallLedEffect(const StarfallLedEffect&) = delete;
	StarfallLedEffect& operator=(const StarfallLedEffect&) = delete;
};

#endif
