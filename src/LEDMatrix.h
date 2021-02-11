/*
* LEDMatrix.h
*
*/

#ifndef _LEDMATRIX_H_
#define _LEDMATRIX_H_

#include <FastLED.h>
#include "LEDLine.h"
#include "IMatrixToLineConverter.h"

class LEDMatrix : public LEDLine
{
private:

	static const uint8_t NUM_EFFECTS;
	static const char* availableEffects[];

public:

	LEDMatrix(IMatrixToLineConverter* converter, CRGB leds[], uint16_t count);

	virtual ~LEDMatrix();

	virtual uint8_t getAllEffectsNumber() const override;

	virtual const char* const* getAllEffectsNames() const override;

	virtual bool setEffectByName(const char* effectName) override;

	virtual void setRunningString(const char* data, uint16_t len);


protected:

	IMatrixToLineConverter* matrix;

	String text;

	const CRGBPalette16 wrwPalette =
		CRGBPalette16(
			CRGB::White, CRGB::Red, CRGB::Red, CRGB::Red,
			CRGB::White, CRGB::Red, CRGB::Red, CRGB::Red,
			CRGB::White, CRGB::Red, CRGB::Red, CRGB::Red,
			CRGB::White, CRGB::Red, CRGB::Red, CRGB::Red);

};

#endif
