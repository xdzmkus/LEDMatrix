/*
* LEDMatrix.cpp
*
*/

#include "LEDMatrix.h"

const TProgmemRGBPalette16 LEDMatrix::wrwPalette_p PROGMEM =
{
	CRGB::White, CRGB::Red, CRGB::Red, CRGB::Red,
	CRGB::White, CRGB::Red, CRGB::Red, CRGB::Red,
	CRGB::White, CRGB::Red, CRGB::Red, CRGB::Red,
	CRGB::White, CRGB::Red, CRGB::Red, CRGB::Red
};

void LEDMatrix::setRunningString(const char* data, uint16_t len)
{
	text = String(data);
}
