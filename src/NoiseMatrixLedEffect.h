/* 
* NoiseMatrixLedEffect.h
*
*/

#ifndef __NOISEMATRIXLEDEFFECT_H__
#define __NOISEMATRIXLEDEFFECT_H__

#include <LedEffect.h>
#include "IMatrixConverter.h"

class NoiseMatrixLedEffect : public LedEffect
{
public:

	static const char* const name;

private:

	const IMatrixConverter* converter;

	uint8_t** noise;

	const CRGBPalette16 currentPalette;

	const uint8_t speed = 20;
	const uint8_t dataSmoothing = 120;
	const uint8_t scale = 30;

	uint16_t nX = 0;
	uint16_t nY = 0;
	uint16_t nZ = 0;
	uint8_t ihue = 0;

public:

	NoiseMatrixLedEffect(const IMatrixConverter* converter, CRGB leds[], uint16_t count, uint16_t Hz, const CRGBPalette16 &palette);
	~NoiseMatrixLedEffect();

	void init() override;
	bool paint() override;

	operator const char* () const { return name; }

private:

	NoiseMatrixLedEffect(const NoiseMatrixLedEffect&) = delete;
	NoiseMatrixLedEffect& operator=(const NoiseMatrixLedEffect&) = delete;
};

#endif
