/* 
* FireMatrixLedEffect.h
*
*/

#ifndef __FIREMATRIXLEDEFFECT_H__
#define __FIREMATRIXLEDEFFECT_H__

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define MATRIX_HEIGHT 8
#define MATRIX_WIDTH 9

#include <LedEffect.h>
#include "IMatrixConverter.h"

class FireMatrixLedEffect : public LedEffect
{
public:

	static const char* const name;

	static const uint8_t valueMask[MATRIX_HEIGHT][MATRIX_WIDTH] PROGMEM;

	static const uint8_t hueMask[MATRIX_HEIGHT][MATRIX_WIDTH] PROGMEM;

private:

	const IMatrixConverter* converter;

protected:

	uint8_t matrixValue[MATRIX_HEIGHT][MATRIX_WIDTH];
	uint8_t line[MATRIX_WIDTH];
	uint8_t pcnt = 0;


public:

	FireMatrixLedEffect(const IMatrixConverter* converter, CRGB leds[], uint16_t count, uint16_t Hz);
	~FireMatrixLedEffect();

	void init() override;
	bool paint() override;

	operator const char* () const { return name; }

private:

	FireMatrixLedEffect(const FireMatrixLedEffect&) = delete;
	FireMatrixLedEffect& operator=(const FireMatrixLedEffect&) = delete;
};

#endif
