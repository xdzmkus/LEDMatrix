/* 
* StarfallMatrixLedEffect.h
*
*/

#ifndef __STARFALLMATRIXLEDEFFECT_H__
#define __STARFALLMATRIXLEDEFFECT_H__

#include <ILedEffect.h>
#include "IMatrixToLineConverter.h"

class StarfallMatrixLedEffect : public ILedEffect
{
public:

	static const char* const name;

private:

	const IMatrixToLineConverter* converter;
	const CRGB rgb;

	CRGB fade;

public:

	StarfallMatrixLedEffect(const IMatrixToLineConverter* converter, CRGB leds[], uint16_t count, uint16_t Hz, CRGB color = CRGB::Black);
	~StarfallMatrixLedEffect();

	void init() override;
	bool paint() override;

	operator const char* () const { return name; }

private:

	StarfallMatrixLedEffect(const StarfallMatrixLedEffect&) = delete;
	StarfallMatrixLedEffect& operator=(const StarfallMatrixLedEffect&) = delete;
};

#endif
