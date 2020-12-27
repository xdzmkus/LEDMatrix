/* 
* SinusMatrixLedEffect.h
*
*/

#ifndef __SINUSMATRIXLEDEFFECT_H__
#define __SINUSMATRIXLEDEFFECT_H__

#include <LedEffect.h>
#include "IMatrixConverter.h"

class SinusMatrixLedEffect : public LedEffect
{
public:

	static const char* const name;

private:

	const IMatrixConverter* converter;

	uint8_t hue;

public:

	SinusMatrixLedEffect(const IMatrixConverter* converter, CRGB leds[], uint16_t count, uint16_t Hz);
	~SinusMatrixLedEffect();

	void init() override;
	bool paint() override;

	operator const char* () const { return name; }

private:

	SinusMatrixLedEffect(const SinusMatrixLedEffect&) = delete;
	SinusMatrixLedEffect& operator=(const SinusMatrixLedEffect&) = delete;
};

#endif
