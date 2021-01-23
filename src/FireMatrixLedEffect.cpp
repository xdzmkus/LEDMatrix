/*
* FireMatrixLedEffect.cpp
*
*/

#include "FireMatrixLedEffect.h"

const char* const FireMatrixLedEffect::name = "FIRE";

// these values are subtracted from the generated values to give a shape to the animation
const uint8_t FireMatrixLedEffect::valueMask[MATRIX_HEIGHT][MATRIX_WIDTH] PROGMEM =
{
		{32 , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 32  },
		{64 , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 64  },
		{96 , 32 , 0  , 0  , 0  , 0  , 0  , 32 , 96  },
		{128, 64 , 32 , 0  , 0  , 0  , 32 , 64 , 128 },
		{160, 96 , 64 , 32 , 16 , 32 , 64 , 96 , 160 },
		{192, 128, 96 , 64 , 32 , 64 , 96 , 128, 192 },
		{255, 160, 128, 96 , 64 , 96 , 128, 160, 255 },
		{255, 192, 160, 128, 96 , 128, 160, 192, 255 }
};

//these are the hues for the fire, should be between 0 (red) to about 25 (yellow)
const uint8_t FireMatrixLedEffect::hueMask[MATRIX_HEIGHT][MATRIX_WIDTH] PROGMEM =
{
		{1 , 13 , 19 , 25 , 25 , 25 , 22 , 13 , 1 },
		{1 , 11 , 16 , 22 , 25 , 25 , 19 , 11 , 1 },
		{1 , 8  , 16 , 19 , 25 , 19 , 16 , 8  , 1 },
		{1 , 5  , 11 , 13 , 19 , 13 , 11 , 5  , 1 },
		{1 , 5  , 8  , 11 , 13 , 11 , 8  , 5  , 1 },
		{0 , 1  , 5  , 8  , 11 , 8  , 5  , 1  , 0 },
		{0 , 0  , 1  , 5  , 8  , 5  , 1  , 0  , 0 },
		{0 , 0  , 0  , 1  , 3  , 1  , 0  , 0  , 0 }
};

FireMatrixLedEffect::FireMatrixLedEffect(const IMatrixToLineConverter* converter, CRGB leds[], uint16_t count, uint16_t Hz)
	: ILedEffect(leds, count, Hz), converter(converter)
{
	init();
}

void FireMatrixLedEffect::init()
{
	clearAllLeds();
}

FireMatrixLedEffect::~FireMatrixLedEffect()
{
}

bool FireMatrixLedEffect::paint()
{
	if (!isReady())
		return false;

	// update matrix each 4 cycles
	if (pcnt == 0)
	{
		//shift all values in the matrix up one row
		for (uint8_t y = MATRIX_HEIGHT - 1; y > 0; y--)
		{
			for (uint8_t x = 0; x < MATRIX_WIDTH; x++)
			{
				matrixValue[y][x] = matrixValue[y - 1][x];
			}
		}

		for (uint8_t x = 0; x < MATRIX_WIDTH; x++)
		{
			matrixValue[0][x] = line[x];
			// Randomly generate the next line (matrix row)
			line[x] = random(64, 255);
		}
	}

	// each row interpolates with the one before it
	for (uint8_t y = converter->getHeight() - 1; y > 0; y--)
	{
		for (uint8_t x = 0; x < converter->getWidth(); x++)
		{
			uint8_t mX = x % MATRIX_WIDTH;

			if (y < MATRIX_HEIGHT)
			{
				int nextv = (static_cast<int16_t>((10 - pcnt) * matrixValue[y][mX] + pcnt * matrixValue[y - 1][mX]) / 10) - pgm_read_byte(&(valueMask[y][mX]));

				ledLine[converter->getPixelNumber(x, y)] = CHSV(pgm_read_byte(&(hueMask[y][mX])), 255, static_cast<uint8_t>(max(0, nextv)));
			}
			else
			{
				if (y == MATRIX_HEIGHT && random(0, 20) == 0)
				{
					ledLine[converter->getPixelNumber(x, y)] = CRGB::Black;
				}
				else
				{
					ledLine[converter->getPixelNumber(x, y)] = ledLine[converter->getPixelNumber(x, y - 1)];
				}
			}
		}
	}

	// first row interpolates with the "next" line
	for (uint8_t x = 0; x < converter->getWidth(); x++)
	{
		uint8_t mX = x % MATRIX_WIDTH;

		ledLine[converter->getPixelNumber(x, 0)] = CHSV(pgm_read_byte(&(hueMask[0][mX])), 255, static_cast<uint8_t>(static_cast<int16_t>((10 - pcnt) * matrixValue[0][mX] + pcnt * line[mX]) / 10));
	}

	pcnt += 3;

	if (pcnt >= 10) pcnt = 0;

	return true;
}
