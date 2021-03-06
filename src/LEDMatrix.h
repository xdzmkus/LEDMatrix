/*
* LEDMatrix.h
*
*/

#ifndef _LEDMATRIX_H_
#define _LEDMATRIX_H_

#include <FastLED.h>
#include <LEDLine.h>
#include "ILedMatrix.h"

class LEDMatrix : public LEDLine
{
private:

	static const uint8_t NUM_EFFECTS;
	static const char* availableEffects[];

public:

	LEDMatrix(ILedMatrix* converter);

	virtual ~LEDMatrix();

	virtual uint8_t getAllEffectsNumber() const override;

	virtual const char* const* getAllEffectsNames() const override;

	virtual bool setEffectByName(const char* effectName) override;

	virtual void setRunningString(const char* data, uint16_t len);


protected:

	ILedMatrix* matrix;

	String text;

	static const TProgmemRGBPalette16 wrwPalette_p PROGMEM;
};

#endif
