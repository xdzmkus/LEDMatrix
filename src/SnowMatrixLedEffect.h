/* 
* SnowMatrixLedEffect.h
*
*/

#ifndef __SNOWMATRIXLEDEFFECT_H__
#define __SNOWMATRIXLEDEFFECT_H__

#include <ILedEffect.h>
#include "ILedMatrix.h"

class SnowMatrixLedEffect : public ILedEffect
{
public:

	static const char* const name;

private:

	ILedMatrix* matrix;

	const uint16_t snowflakeCount;

	MillisTimer dirTimer;

	const uint8_t fade;

	bool direction = false;

public:

	SnowMatrixLedEffect(ILedMatrix* converter, uint16_t Hz, unsigned long changeDirection = 2000UL, uint8_t fadeSpeed = 70);
	~SnowMatrixLedEffect();

	bool isReady() override;

	void reset() override;
	void paint() override;

	operator const char* () const { return name; }

private:

	SnowMatrixLedEffect(const SnowMatrixLedEffect&) = delete;
	SnowMatrixLedEffect& operator=(const SnowMatrixLedEffect&) = delete;
};

#endif
