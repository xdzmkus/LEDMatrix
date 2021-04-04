/*
* StarfallMatrixLedEffect.cpp
*
*/

#include "StarfallMatrixLedEffect.h"

const char* const StarfallMatrixLedEffect::name = "STARFALL";

StarfallMatrixLedEffect::StarfallMatrixLedEffect(ILedMatrix* converter, uint16_t Hz, CRGB color)
	: ILedEffect(Hz), matrix(converter), rgb(color ? color : matrix->getRandomColor())
{
	fade = CRGB(1 + rgb.r/(matrix->getHeight()+1), 1 + rgb.g/(matrix->getHeight()+1), 1 + rgb.b/(matrix->getHeight()+1));

	reset();
}

StarfallMatrixLedEffect::~StarfallMatrixLedEffect()
{
}

void StarfallMatrixLedEffect::reset()
{
	ILedEffect::reset();
	matrix->clearAllLeds();
}

void StarfallMatrixLedEffect::paint()
{
	for (uint8_t x = 0; x < matrix->getWidth(); x++)
	{
		// shift down all lines
		for (uint8_t y = 0; y < matrix->getHeight() - 1; y++)
		{
			matrix->getPixel(x, y) = matrix->getPixel(x, y + 1);
		}

		// fill randomly top line
		if (!matrix->getPixel(x, matrix->getHeight() - 1) && random8(0, matrix->getWidth()) == 0)
		{
			matrix->getPixel(x, matrix->getHeight() - 1) = rgb;
		}
		else
		{
			matrix->getPixel(x, matrix->getHeight() - 1) -= fade;
		}
	}
}
