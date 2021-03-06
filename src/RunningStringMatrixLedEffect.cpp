/*
* RunningStringMatrixLedEffect.cpp
*
*/

#include "RunningStringMatrixLedEffect.h"

const char* const RunningStringMatrixLedEffect::name = "RUNNINGSTRING";

RunningStringMatrixLedEffect::RunningStringMatrixLedEffect(ILedMatrix* converter, uint16_t Hz, String text, uint8_t yOffset, CRGB color)
	: ILedEffect(Hz), matrix(converter), str(text), yOffset(yOffset), rgb(color ? color : matrix->getRandomColor())
{
	reset();
}

RunningStringMatrixLedEffect::~RunningStringMatrixLedEffect()
{
}

void RunningStringMatrixLedEffect::reset()
{
    ILedEffect::reset();

    offset = matrix->getWidth();

    matrix->clearAllLeds();
}

void RunningStringMatrixLedEffect::paint()
{
    matrix->clearAllLeds();

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

uint8_t RunningStringMatrixLedEffect::get5x8Column(unsigned char ascii, uint8_t columnNumber)
{
    if (columnNumber >= 5)
        return 0;

    if (ascii < 32 || (ascii > 126 && ascii < 192))
    {
        return 0;														// ��� ���������� � ������������� �������� - ������ (��� ����� ������)
    }

    if (ascii <= 126)
    {
        return pgm_read_byte(&(font5x8[ascii - 32][columnNumber]));		// ��� ���������� ���� � ��������
    }

    return pgm_read_byte(&(font5x8[ascii - 97][columnNumber]));			// ��� ������� ����
}

void RunningStringMatrixLedEffect::draw5x8Letter(unsigned char ascii, CRGB color, int16_t xOffset, int16_t yOffset)
{
    // check if oversized
    if (xOffset >= matrix->getWidth() || yOffset >= matrix->getHeight() || xOffset <= -5 || yOffset <= -8)
        return;

    // get visible part of character
    uint8_t firstColumn = xOffset >= 0 ? 0 : -xOffset;
    uint8_t lastColumn = min(5, matrix->getWidth() - xOffset);
    uint8_t firstRow = yOffset >= 0 ? 0 : -yOffset;
    uint8_t lastRow = min(8, matrix->getHeight() - yOffset);

    // draw char
    for (uint8_t x = firstColumn; x < lastColumn; x++)
    {
        uint8_t columnByte = get5x8Column(ascii, x);

        for (uint8_t y = firstRow; y < lastRow; y++)
        {
            matrix->getPixel(xOffset + x, yOffset + y) = (columnByte & (1 << (7 - y))) ? color : CRGB::Black;
        }
    }
}
