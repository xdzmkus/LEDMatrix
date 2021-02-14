/* 
* StarfallMatrixLedEffect.h
*
*/

#ifndef __STARFALLMATRIXLEDEFFECT_H__
#define __STARFALLMATRIXLEDEFFECT_H__

#include <ILedEffect.h>
#include "ILedMatrix.h"

class StarfallMatrixLedEffect : public ILedEffect
{
public:

	static const char* const name;

private:

	ILedMatrix* matrix;
	const CRGB rgb;

	CRGB fade;

public:

	StarfallMatrixLedEffect(ILedMatrix* converter, uint16_t Hz, CRGB color = CRGB::Black);
	~StarfallMatrixLedEffect();

	void reset() override;
	bool paint() override;

	operator const char* () const { return name; }

private:

	StarfallMatrixLedEffect(const StarfallMatrixLedEffect&) = delete;
	StarfallMatrixLedEffect& operator=(const StarfallMatrixLedEffect&) = delete;
};

#endif
