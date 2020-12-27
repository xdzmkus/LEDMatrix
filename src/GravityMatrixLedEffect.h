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

	const float Gravity = 9.8;

	struct GRAVITY
	{
		CRGB	color;
		uint8_t	position;
		float	height;
		float	velocity;
		unsigned long startTime;
	}
	*gravities = nullptr;

public:

	GravityMatrixLedEffect(const IMatrixConverter* converter, CRGB leds[], uint16_t count, uint16_t Hz);
	~GravityMatrixLedEffect();

	void init() override;
	bool paint() override;

	operator const char* () const { return name; }

private:

	GravityMatrixLedEffect(const GravityMatrixLedEffect&) = delete;
	GravityMatrixLedEffect& operator=(const GravityMatrixLedEffect&) = delete;
};

#endif
