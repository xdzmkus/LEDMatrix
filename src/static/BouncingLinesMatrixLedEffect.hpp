/*
* BouncingLinesMatrixLedEffect.hpp
*
*/

#ifndef __BOUNSINGLINESMATRIXLEDEFFECT_HPP__
#define __BOUNSINGLINESSMATRIXLEDEFFECT_HPP__

#include <ILedEffect.hpp>
#include "ILedMatrix.hpp"

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height, const uint8_t linesCount>
class BouncingLinesMatrixLedEffect : public MATRIX<ledLine, width, height>, public ILedEffect
{
private:

	const float Gravity = 9.8;
	uint8_t hue = 0;

public:

	static const char* const name;

protected:

	struct BOUNCING_LINES
	{
		CRGB	color;
		uint8_t position;
		bool	direction;
	}
	lines[linesCount];


public:
	BouncingLinesMatrixLedEffect(uint16_t Hz);
	~BouncingLinesMatrixLedEffect();

	void reset() override;
	void paint() override;

	operator const char* () const { return name; }

private:
	BouncingLinesMatrixLedEffect(const BouncingLinesMatrixLedEffect&) = delete;
	BouncingLinesMatrixLedEffect& operator=(const BouncingLinesMatrixLedEffect&) = delete;
};

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height, const uint8_t linesCount>
const char* const BouncingLinesMatrixLedEffect<MATRIX, ledLine, width, height, linesCount>::name = "BOUNCINGLINES";

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height, const uint8_t linesCount>
BouncingLinesMatrixLedEffect<MATRIX, ledLine, width, height, linesCount>::BouncingLinesMatrixLedEffect(uint16_t Hz)
	: ILedEffect(Hz)
{
	reset();
}

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height, const uint8_t linesCount>
BouncingLinesMatrixLedEffect<MATRIX, ledLine, width, height, linesCount>::~BouncingLinesMatrixLedEffect()
{
}

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height, const uint8_t linesCount>
void BouncingLinesMatrixLedEffect<MATRIX, ledLine, width, height, linesCount>::reset()
{
	ILedEffect::reset();

	float period = (MATRIX<ledLine, width, height>::getHeight() - 1) / (linesCount - 1);

	for (uint8_t i = 0; i < linesCount; i++)
	{
		lines[i].color = MATRIX<ledLine, width, height>::getRandomColor();

		lines[i].position = static_cast<uint8_t>(round(i * period));

		if (lines[i].position >= MATRIX<ledLine, width, height>::getHeight() / 2)
		{
			lines[i].direction = false;
		}
		else
		{
			lines[i].direction = true;
		}
	}

	MATRIX<ledLine, width, height>::clearAllLeds();
}

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height, const uint8_t linesCount>
void BouncingLinesMatrixLedEffect<MATRIX, ledLine, width, height, linesCount>::paint()
{
	*this >> 0;

	MATRIX<ledLine, width, height>::clearColumn(0);

	for (uint8_t i = 0; i < linesCount; i++)
	{
		if (lines[i].direction)
		{
			lines[i].position++;
			if (lines[i].position == MATRIX<ledLine, width, height>::getHeight() - 1)
			{
				lines[i].direction = false;
			}
		}
		else
		{
			lines[i].position--;
			if (lines[i].position == 0)
			{
				lines[i].direction = true;
			}
		}

		MATRIX<ledLine, width, height>::getPixel(0, lines[i].position) = ColorFromPalette(RainbowColors_p, (i + 1) * hue, 255);;
	}

	hue += speed;
}

#endif

