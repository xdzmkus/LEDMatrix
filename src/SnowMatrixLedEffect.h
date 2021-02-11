/* 
* SnowMatrixLedEffect.h
*
*/

#ifndef __SNOWMATRIXLEDEFFECT_H__
#define __SNOWMATRIXLEDEFFECT_H__

#include <ILedEffect.h>
#include "IMatrixToLineConverter.h"

class SnowMatrixLedEffect : public ILedEffect
{
public:

	static const char* const name;

private:

	const IMatrixToLineConverter* converter;

	const uint16_t snowflakeCount;

	const uint8_t fade;

public:

	SnowMatrixLedEffect(const IMatrixToLineConverter* converter, CRGB leds[], uint16_t count, uint16_t Hz, uint8_t fadeSpeed = 70);
	~SnowMatrixLedEffect();

	void reset() override;
	bool paint() override;

	operator const char* () const { return name; }

private:

	SnowMatrixLedEffect(const SnowMatrixLedEffect&) = delete;
	SnowMatrixLedEffect& operator=(const SnowMatrixLedEffect&) = delete;
};

#endif
