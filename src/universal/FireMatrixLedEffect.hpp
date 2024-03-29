/* 
* FireMatrixLedEffect.hpp
*
*/

#ifndef __FIREMATRIXLEDEFFECT_HPP__
#define __FIREMATRIXLEDEFFECT_HPP__

#define MATRIX_HEIGHT 8
#define MATRIX_WIDTH 9

#include <ILedEffect.hpp>

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
class FireMatrixLedEffect : public MATRIX<ledLine, width, height>, public ILedEffect
{
public:

	static LedEffectName const name;

	static const uint8_t valueMask[MATRIX_HEIGHT][MATRIX_WIDTH] PROGMEM;

	static const uint8_t hueMask[MATRIX_HEIGHT][MATRIX_WIDTH] PROGMEM;

protected:

	uint8_t matrixValue[MATRIX_HEIGHT][MATRIX_WIDTH];
	uint8_t line[MATRIX_WIDTH];
	uint8_t pcnt = 0;

public:

	FireMatrixLedEffect(uint16_t Hz);
	~FireMatrixLedEffect();

	void reset() override;
	void paint() override;

	operator LedEffectName () const { return name; }

private:

	FireMatrixLedEffect(const FireMatrixLedEffect&) = delete;
	FireMatrixLedEffect& operator=(const FireMatrixLedEffect&) = delete;
};

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
LedEffectName const FireMatrixLedEffect<MATRIX, ledLine, width, height>::name = "FIRE";

// these values are subtracted from the generated values to give a shape to the animation
template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
const uint8_t FireMatrixLedEffect<MATRIX, ledLine, width, height>::valueMask[MATRIX_HEIGHT][MATRIX_WIDTH] PROGMEM =
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
template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
const uint8_t FireMatrixLedEffect<MATRIX, ledLine, width, height>::hueMask[MATRIX_HEIGHT][MATRIX_WIDTH] PROGMEM =
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

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
FireMatrixLedEffect<MATRIX, ledLine, width, height>::FireMatrixLedEffect(uint16_t Hz) : ILedEffect(Hz)
{
	reset();
}

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
void FireMatrixLedEffect<MATRIX, ledLine, width, height>::reset()
{
	ILedEffect::reset();
	MATRIX<ledLine, width, height>::clearAllLeds();
}

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
FireMatrixLedEffect<MATRIX, ledLine, width, height>::~FireMatrixLedEffect()
{
}

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
void FireMatrixLedEffect<MATRIX, ledLine, width, height>::paint()
{
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
	for (uint8_t y = MATRIX<ledLine, width, height>::getHeight() - 1; y > 0; y--)
	{
		for (uint8_t x = 0; x < MATRIX<ledLine, width, height>::getWidth(); x++)
		{
			uint8_t mX = x % MATRIX_WIDTH;

			if (y < MATRIX_HEIGHT)
			{
				int nextv = (static_cast<int16_t>((10 - pcnt) * matrixValue[y][mX] + pcnt * matrixValue[y - 1][mX]) / 10) - pgm_read_byte(&(valueMask[y][mX]));

				MATRIX<ledLine, width, height>::getPixel(x, y) = CHSV(pgm_read_byte(&(hueMask[y][mX])), 255, static_cast<uint8_t>(max(0, nextv)));
			}
			else
			{
				if (y == MATRIX_HEIGHT && random(0, 20) == 0)
				{
					MATRIX<ledLine, width, height>::getPixel(x, y) = CRGB::Black;
				}
				else
				{
					MATRIX<ledLine, width, height>::getPixel(x, y) = MATRIX<ledLine, width, height>::getPixel(x, y - 1);
				}
			}
		}
	}

	// first row interpolates with the "next" line
	for (uint8_t x = 0; x < MATRIX<ledLine, width, height>::getWidth(); x++)
	{
		uint8_t mX = x % MATRIX_WIDTH;

		MATRIX<ledLine, width, height>::getPixel(x, 0) = CHSV(pgm_read_byte(&(hueMask[0][mX])), 255, static_cast<uint8_t>(static_cast<int16_t>((10 - pcnt) * matrixValue[0][mX] + pcnt * line[mX]) / 10));
	}

	pcnt += 3;

	if (pcnt >= 10) pcnt = 0;
}

#endif
