/* 
* GravityMatrixLedEffect.h
*
*/

#ifndef __GRAVITYMATRIXLEDEFFECT_H__
#define __GRAVITYMATRIXLEDEFFECT_H__

#include <ILedEffect.h>
#include "ILedMatrix.h"

class GravityMatrixLedEffect : public ILedEffect
{
public:

	static const char* const name;

private:

	ILedMatrix* matrix;

	const float Gravity = 9.8;

	uint8_t hue = 0;

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

	GravityMatrixLedEffect(ILedMatrix* converter, uint16_t Hz);
	~GravityMatrixLedEffect();

	void reset() override;
	bool paint() override;

	operator const char* () const { return name; }

private:

	GravityMatrixLedEffect(const GravityMatrixLedEffect&) = delete;
	GravityMatrixLedEffect& operator=(const GravityMatrixLedEffect&) = delete;
};

#endif
