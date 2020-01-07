/* 
* MatrixEffect.h
*
*/


#ifndef __MATRIXEFFECT_H__
#define __MATRIXEFFECT_H__

#include "IMatrixLedEffect.h"

class MatrixEffect : public IMatrixLedEffect
{
private:

	CRGB rgb;

public:

	MatrixEffect(CRGB leds[], uint8_t width, uint8_t height, CRGB color = CRGB::Green);
	~MatrixEffect();

	virtual void refresh() override;

private:

	MatrixEffect( const MatrixEffect &c );
	MatrixEffect& operator=( const MatrixEffect &c );

};

#endif
