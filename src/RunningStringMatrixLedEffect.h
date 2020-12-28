/* 
* RunningStringMatrixLedEffect.h
*
*/

#ifndef __RUNNINGSTRINGMATRIXLEDEFFECT_H__
#define __RUNNINGSTRINGMATRIXLEDEFFECT_H__

#include <LedEffect.h>
#include "IMatrixToLineConverter.h"
#include "fonts/fonts.h"

class RunningStringMatrixLedEffect : public LedEffect
{
public:

	static const char* const name;

private:

	const IMatrixToLineConverter* converter;
	const char *str;
	const CRGB rgb;
	const uint8_t yOffset;

	int16_t offset;
	int16_t minOffset;

public:

	RunningStringMatrixLedEffect(const IMatrixToLineConverter* converter, CRGB leds[], uint16_t count, uint16_t Hz, const char text[], CRGB color = CRGB::Black, uint8_t yOffset = 0);
	~RunningStringMatrixLedEffect();

	void init() override;
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
