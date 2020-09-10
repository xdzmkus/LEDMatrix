/* 
* MatrixLedEffect.h
*
*/

#ifndef __MATRIXLEDEFFECT_H__
#define __MATRIXLEDEFFECT_H__

#include <ILedEffect.h>
#include "IMatrixConverter.h"

class MatrixLedEffect : public ILedEffect
{
private:

	const IMatrixConverter* converter;
	const CRGB rgb;

public:

	MatrixLedEffect(const IMatrixConverter* converter, CRGB leds[], uint16_t count, uint16_t Hz, CRGB color = CRGB::Green);
	~MatrixLedEffect();

	bool paint() override;

private:

	MatrixLedEffect(const MatrixLedEffect&);
	MatrixLedEffect& operator=(const MatrixLedEffect&) {};

};

#endif
