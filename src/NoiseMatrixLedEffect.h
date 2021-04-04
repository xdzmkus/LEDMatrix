/* 
* NoiseMatrixLedEffect.h
*
*/

#ifndef __NOISEMATRIXLEDEFFECT_H__
#define __NOISEMATRIXLEDEFFECT_H__

#include <ILedEffect.h>
#include "ILedMatrix.h"

class NoiseMatrixLedEffect : public ILedEffect
{
public:

	static const char* const name;

private:

	ILedMatrix* matrix;

	uint8_t** noise = nullptr;

	const CRGBPalette16 currentPalette;

	// We're using the x/y dimensions to map to the x/y pixels on the matrix.
	// We'll use the z-axis for "time". Speed determines how fast time moves forward.
	// Try 1 for a very slow moving effect, or 60 for something that ends up looking like water.
//	const uint8_t speed = 20;

	// If we're runing at a low "speed", some 8-bit artifacts become visible from frame-to-frame.
	// In order to reduce this, we can do some fast data-smoothing.
	// The amount of data smoothing we're doing depends on "speed".
	const uint8_t dataSmoothing = 120;

	// Scale determines how far apart the pixels in our noise matrix are.
	// Try changing these values around to see how it affects the motion of the display.
	// The higher the value of scale, the more "zoomed out" the noise will be.
	// A value of 1 will be so zoomed in, you'll mostly see solid colors.
	const uint8_t scale = 30;

	uint16_t nX = 0;
	uint16_t nY = 0;
	uint16_t nZ = 0;
	uint8_t ihue = 0;

public:

	NoiseMatrixLedEffect(ILedMatrix* converter, uint16_t Hz, const CRGBPalette16 &palette, uint8_t zoom = 30);
	~NoiseMatrixLedEffect();

	void reset() override;
	void paint() override;

	operator const char* () const { return id.length() == 0 ? name : id.c_str(); }

private:

	NoiseMatrixLedEffect(const NoiseMatrixLedEffect&) = delete;
	NoiseMatrixLedEffect& operator=(const NoiseMatrixLedEffect&) = delete;
};

#endif
