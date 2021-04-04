/*
* SinusMatrixLedEffect.cpp
*
*/

#include "SinusMatrixLedEffect.h"

const char* const SinusMatrixLedEffect::name = "SINUS";

SinusMatrixLedEffect::SinusMatrixLedEffect(ILedMatrix* converter, uint16_t Hz)
	: ILedEffect(Hz), matrix(converter), hue(0)
{
	reset();
}

SinusMatrixLedEffect::~SinusMatrixLedEffect()
{
}

void SinusMatrixLedEffect::reset()
{
    ILedEffect::reset();
    matrix->clearAllLeds();
}

void SinusMatrixLedEffect::paint()
{
    matrix->clearAllLeds();

    hue++;

    if (matrix->getWidth() > matrix->getHeight())
    {
        for (uint8_t x = 0; x < matrix->getWidth(); x++)
        {
            uint8_t y = beatsin8(speed + x, 0, matrix->getHeight() - 1);
            matrix->getPixel(x, y) = ColorFromPalette(RainbowColors_p, x * 7 + hue, 255);
        }
    }
    else
    {
        for (uint8_t y = 0; y < matrix->getHeight(); y++) {
            uint8_t x = beatsin8(speed + y, 0, matrix->getWidth() - 1);
            matrix->getPixel(x, y) = ColorFromPalette(RainbowColors_p, x * 7 + hue, 255);
        }
    }
}
