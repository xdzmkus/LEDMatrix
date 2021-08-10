/* 
* RunningStringMatrixLedEffect.hpp
*
*/

#ifndef __RUNNINGSTRINGMATRIXLEDEFFECT_HPP__
#define __RUNNINGSTRINGMATRIXLEDEFFECT_HPP__

#include <ILedEffect.hpp>
#include "fonts/windows-1251-5x8.h"

template<template <CRGB* const, const uint8_t, const uint8_t> class MATRIX, CRGB* const ledLine, const uint8_t width, const uint8_t height>
class RunningStringMatrixLedEffect : public MATRIX<ledLine, width, height>, public ILedEffect
{
public:

	static const char* const name;

private:

	const String str;
	const uint8_t yOffset;
	const CRGB rgb;

	int16_t offset;

public:

	RunningStringMatrixLedEffect(uint16_t Hz, String text, uint8_t yOffset = 0, CRGB color = CRGB::Black);
	~RunningStringMatrixLedEffect();

	void reset() override;
	void paint() override;

	operator const char* () const { return name; }

protected:

	uint8_t get5x8Column(unsigned char ascii, uint8_t columnNumber);

	void draw5x8Letter(unsigned char ascii, CRGB color, int16_t xOffset, int16_t yOffset);

private:

	RunningStringMatrixLedEffect(const RunningStringMatrixLedEffect&) = delete;
	RunningStringMatrixLedEffect& operator=(const RunningStringMatrixLedEffect&) = delete;

};

template<template <CRGB* const, const uint8_t, const uint8_t> class MATRIX, CRGB* const ledLine, const uint8_t width, const uint8_t height>
const char* const RunningStringMatrixLedEffect<MATRIX, ledLine, width, height>::name = "RUNNINGSTRING";

template<template <CRGB* const, const uint8_t, const uint8_t> class MATRIX, CRGB* const ledLine, const uint8_t width, const uint8_t height>
RunningStringMatrixLedEffect<MATRIX, ledLine, width, height>::RunningStringMatrixLedEffect(uint16_t Hz, String text, uint8_t yOffset, CRGB color)
    : ILedEffect(Hz), str(text), yOffset(yOffset), rgb(color ? color : MATRIX<ledLine, width, height>::getRandomColor())
{
    reset();
}

template<template <CRGB* const, const uint8_t, const uint8_t> class MATRIX, CRGB* const ledLine, const uint8_t width, const uint8_t height>
RunningStringMatrixLedEffect<MATRIX, ledLine, width, height>::~RunningStringMatrixLedEffect()
{
}

template<template <CRGB* const, const uint8_t, const uint8_t> class MATRIX, CRGB* const ledLine, const uint8_t width, const uint8_t height>
void RunningStringMatrixLedEffect<MATRIX, ledLine, width, height>::reset()
{
    ILedEffect::reset();

    offset = MATRIX<ledLine, width, height>::getWidth();

    MATRIX<ledLine, width, height>::clearAllLeds();
}

template<template <CRGB* const, const uint8_t, const uint8_t> class MATRIX, CRGB* const ledLine, const uint8_t width, const uint8_t height>
void RunningStringMatrixLedEffect<MATRIX, ledLine, width, height>::paint()
{
    MATRIX<ledLine, width, height>::clearAllLeds();

    for (uint8_t i = 0; i < str.length(); i++)
    {
        draw5x8Letter(str[i], rgb, offset + i * 6, yOffset);
    }

    // whole string scrolled
    if (-static_cast<int16_t>(str.length() * 6) > --offset)
    {
        reset();
    }
}

template<template <CRGB* const, const uint8_t, const uint8_t> class MATRIX, CRGB* const ledLine, const uint8_t width, const uint8_t height>
uint8_t RunningStringMatrixLedEffect<MATRIX, ledLine, width, height>::get5x8Column(unsigned char ascii, uint8_t columnNumber)
{
    if (columnNumber >= 5)
        return 0;

    if (ascii < 32 || (ascii > 126 && ascii < 192))
    {
        return 0;														// для непечатных и отсутствующих символов - пробел (или любой другой)
    }

    if (ascii <= 126)
    {
        return pgm_read_byte(&(font5x8[ascii - 32][columnNumber]));		// для английских букв и символов
    }

    return pgm_read_byte(&(font5x8[ascii - 97][columnNumber]));			// для русских букв
}

template<template <CRGB* const, const uint8_t, const uint8_t> class MATRIX, CRGB* const ledLine, const uint8_t width, const uint8_t height>
void RunningStringMatrixLedEffect<MATRIX, ledLine, width, height>::draw5x8Letter(unsigned char ascii, CRGB color, int16_t xOffset, int16_t yOffset)
{
    // check if oversized
    if (xOffset >= MATRIX<ledLine, width, height>::getWidth() || yOffset >= MATRIX<ledLine, width, height>::getHeight() || xOffset <= -5 || yOffset <= -8)
        return;

    // get visible part of character
    uint8_t firstColumn = xOffset >= 0 ? 0 : -xOffset;
    uint8_t lastColumn = min(5, MATRIX<ledLine, width, height>::getWidth() - xOffset);
    uint8_t firstRow = yOffset >= 0 ? 0 : -yOffset;
    uint8_t lastRow = min(8, MATRIX<ledLine, width, height>::getHeight() - yOffset);

    // draw char
    for (uint8_t x = firstColumn; x < lastColumn; x++)
    {
        uint8_t columnByte = get5x8Column(ascii, x);

        for (uint8_t y = firstRow; y < lastRow; y++)
        {
            MATRIX<ledLine, width, height>::getPixel(xOffset + x, yOffset + y) = (columnByte & (1 << (7 - y))) ? color : CRGB::Black;
        }
    }
}

#endif
