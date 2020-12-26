/* 
* SnowMatrixLedEffect.h
*
*/

#ifndef __SNOWMATRIXLEDEFFECT_H__
#define __SNOWMATRIXLEDEFFECT_H__

#include <LedEffect.h>
#include "IMatrixConverter.h"

class SnowMatrixLedEffect : public LedEffect
{
public:

	static const char* const name;

private:

	const IMatrixConverter* converter;

	const uint16_t snowflakeCount;

	const uint8_t fade;

public:

	SnowMatrixLedEffect(const IMatrixConverter* converter, CRGB leds[], uint16_t count, uint16_t Hz);
	~SnowMatrixLedEffect();

	bool paint() override;

	operator const char* () const { return name; }

private:

	SnowMatrixLedEffect(const SnowMatrixLedEffect&) = delete;
	SnowMatrixLedEffect& operator=(const SnowMatrixLedEffect&) = delete;
};

#endif
