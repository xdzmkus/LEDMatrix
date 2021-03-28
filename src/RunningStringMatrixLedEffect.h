/* 
* RunningStringMatrixLedEffect.h
*
*/

#ifndef __RUNNINGSTRINGMATRIXLEDEFFECT_H__
#define __RUNNINGSTRINGMATRIXLEDEFFECT_H__

#include <ILedEffect.h>
#include "ILedMatrix.h"
#include "fonts/windows-1251-5x8.h"

class RunningStringMatrixLedEffect : public ILedEffect
{
public:

	static const char* const name;

private:

	ILedMatrix* matrix;

	const String str;
	const uint8_t yOffset;
	const CRGB rgb;

	int16_t offset;

public:

	RunningStringMatrixLedEffect(ILedMatrix* converter, uint16_t Hz, String text, uint8_t yOffset = 0, CRGB color = CRGB::Black);
	~RunningStringMatrixLedEffect();

	void reset() override;
	bool paint() override;

	operator const char* () const { return name; }

protected:

	uint8_t get5x8Column(unsigned char ascii, uint8_t columnNumber);

	void draw5x8Letter(unsigned char ascii, CRGB color, int16_t xOffset, int16_t yOffset);

private:

	RunningStringMatrixLedEffect(const RunningStringMatrixLedEffect&) = delete;
	RunningStringMatrixLedEffect& operator=(const RunningStringMatrixLedEffect&) = delete;
};

#endif
