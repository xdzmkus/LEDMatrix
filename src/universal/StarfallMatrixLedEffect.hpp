/* 
* StarfallMatrixLedEffect.hpp
*
*/

#ifndef __STARFALLMATRIXLEDEFFECT_HPP__
#define __STARFALLMATRIXLEDEFFECT_HPP__

#include <ILedEffect.hpp>

template<template <CRGB* const, const uint8_t, const uint8_t> class MATRIX, CRGB* const ledLine, const uint8_t width, const uint8_t height>
class StarfallMatrixLedEffect : public MATRIX<ledLine, width, height>, public ILedEffect
{
public:

	static const char* const name;

private:

	const CRGB rgb;

	CRGB fade;

public:

	StarfallMatrixLedEffect(uint16_t Hz, CRGB color = CRGB::Black);
	~StarfallMatrixLedEffect();

	void reset() override;
	void paint() override;

	operator const char* () const { return name; }

private:

	StarfallMatrixLedEffect(const StarfallMatrixLedEffect&) = delete;
	StarfallMatrixLedEffect& operator=(const StarfallMatrixLedEffect&) = delete;

};


template<template <CRGB* const, const uint8_t, const uint8_t> class MATRIX, CRGB* const ledLine, const uint8_t width, const uint8_t height>
const char* const StarfallMatrixLedEffect<MATRIX, ledLine, width, height>::name = "STARFALL";

template<template <CRGB* const, const uint8_t, const uint8_t> class MATRIX, CRGB* const ledLine, const uint8_t width, const uint8_t height>
StarfallMatrixLedEffect<MATRIX, ledLine, width, height>::StarfallMatrixLedEffect(uint16_t Hz, CRGB color)
	: ILedEffect(Hz), rgb(color ? color : MATRIX<ledLine, width, height>::getRandomColor())
{
	fade = CRGB(1 + rgb.r / (MATRIX<ledLine, width, height>::getHeight() + 1), 1 + rgb.g / (MATRIX<ledLine, width, height>::getHeight() + 1), 1 + rgb.b / (MATRIX<ledLine, width, height>::getHeight() + 1));

	reset();
}

template<template <CRGB* const, const uint8_t, const uint8_t> class MATRIX, CRGB* const ledLine, const uint8_t width, const uint8_t height>
StarfallMatrixLedEffect<MATRIX, ledLine, width, height>::~StarfallMatrixLedEffect()
{
}

template<template <CRGB* const, const uint8_t, const uint8_t> class MATRIX, CRGB* const ledLine, const uint8_t width, const uint8_t height>
void StarfallMatrixLedEffect<MATRIX, ledLine, width, height>::reset()
{
	ILedEffect::reset();
	MATRIX<ledLine, width, height>::clearAllLeds();
}

template<template <CRGB* const, const uint8_t, const uint8_t> class MATRIX, CRGB* const ledLine, const uint8_t width, const uint8_t height>
void StarfallMatrixLedEffect<MATRIX, ledLine, width, height>::paint()
{
	for (uint8_t x = 0; x < MATRIX<ledLine, width, height>::getWidth(); x++)
	{
		// shift down all lines
		for (uint8_t y = 0; y < MATRIX<ledLine, width, height>::getHeight() - 1; y++)
		{
			MATRIX<ledLine, width, height>::getPixel(x, y) = MATRIX<ledLine, width, height>::getPixel(x, y + 1);
		}

		// fill randomly top line
		if (!MATRIX<ledLine, width, height>::getPixel(x, MATRIX<ledLine, width, height>::getHeight() - 1) && random8(0, MATRIX<ledLine, width, height>::getWidth()) == 0)
		{
			MATRIX<ledLine, width, height>::getPixel(x, MATRIX<ledLine, width, height>::getHeight() - 1) = rgb;
		}
		else
		{
			MATRIX<ledLine, width, height>::getPixel(x, MATRIX<ledLine, width, height>::getHeight() - 1) -= fade;
		}
	}
}

#endif
