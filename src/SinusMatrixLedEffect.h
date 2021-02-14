/* 
* SinusMatrixLedEffect.h
*
*/

#ifndef __SINUSMATRIXLEDEFFECT_H__
#define __SINUSMATRIXLEDEFFECT_H__

#include <ILedEffect.h>
#include "ILedMatrix.h"

class SinusMatrixLedEffect : public ILedEffect
{
public:

	static const char* const name;

private:

	ILedMatrix* matrix;

	uint8_t hue;

public:

	SinusMatrixLedEffect(ILedMatrix* converter, uint16_t Hz);
	~SinusMatrixLedEffect();

	void reset() override;
	bool paint() override;

	operator const char* () const { return name; }

private:

	SinusMatrixLedEffect(const SinusMatrixLedEffect&) = delete;
	SinusMatrixLedEffect& operator=(const SinusMatrixLedEffect&) = delete;
};

#endif
