/*
* RunningStringMatrixLedEffect.cpp
*
*/

#include "RunningStringMatrixLedEffect.h"

const char* const RunningStringMatrixLedEffect::name = "RUNNINGSTRING";

RunningStringMatrixLedEffect::RunningStringMatrixLedEffect(const IMatrixToLineConverter* converter, CRGB leds[], uint16_t count, uint16_t Hz, String text, uint8_t yOffset, CRGB color)
	: ILedEffect(leds, count, Hz), converter(converter), str(text), yOffset(yOffset), rgb(color ? color : getRandomColor())
{
	reset();
}

RunningStringMatrixLedEffect::~RunningStringMatrixLedEffect()
{
}

void RunningStringMatrixLedEffect::reset()
{
    ILedEffect::reset();

    offset = converter->getWidth();

    clearAllLeds();
}

bool RunningStringMatrixLedEffect::paint()
{
    if (!isReady())
        return false;

    clearAllLeds();

    for (uint8_t i = 0; i < str.length(); i++)
    {
        draw5x8Letter(str[i], rgb, offset + i * 6, yOffset);
    }

    // whole string scrolled
    if (-static_cast<int16_t>(str.length() * 6) > --offset)
    {
        reset();
    }

    return true;
}

uint8_t RunningStringMatrixLedEffect::get5x8Column(unsigned char ascii, uint8_t columnNumber)
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

void RunningStringMatrixLedEffect::draw5x8Letter(unsigned char ascii, CRGB color, int16_t xOffset, int16_t yOffset)
{
    // check if oversized
    if (xOffset >= converter->getWidth() || yOffset >= converter->getHeight() || xOffset <= -5 || yOffset <= -8)
        return;

    // get visible part of character
    uint8_t firstColumn = xOffset >= 0 ? 0 : -xOffset;
    uint8_t lastColumn = min(5, converter->getWidth() - xOffset);
    uint8_t firstRow = yOffset >= 0 ? 0 : -yOffset;
    uint8_t lastRow = min(8, converter->getHeight() - yOffset);

    // draw char
    for (uint8_t x = firstColumn; x < lastColumn; x++)
    {
        uint8_t columnByte = get5x8Column(ascii, x);

        for (uint8_t y = firstRow; y < lastRow; y++)
        {
            ledLine[converter->getPixelNumber(xOffset + x, yOffset + y)] = (columnByte & (1 << (7 - y))) ? color : CRGB::Black;
        }
    }
}
