/* 
* SinusMatrixLedEffect.hpp
*
*/

#ifndef __SINUSMATRIXLEDEFFECT_HPP__
#define __SINUSMATRIXLEDEFFECT_HPP__

#include <ILedEffect.hpp>

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
class SinusMatrixLedEffect : public MATRIX<ledLine, width, height>, public ILedEffect
{
public:

	static LedEffectName const name;

private:

	uint8_t hue;

public:

	SinusMatrixLedEffect(uint16_t Hz);
	~SinusMatrixLedEffect();

	void reset() override;
	void paint() override;

	operator LedEffectName () const { return name; }

private:

	SinusMatrixLedEffect(const SinusMatrixLedEffect&) = delete;
	SinusMatrixLedEffect& operator=(const SinusMatrixLedEffect&) = delete;

};

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
LedEffectName const SinusMatrixLedEffect<MATRIX, ledLine, width, height>::name = "SINUS";

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
SinusMatrixLedEffect<MATRIX, ledLine, width, height>::SinusMatrixLedEffect(uint16_t Hz)
    : ILedEffect(Hz), hue(0)
{
    reset();
}

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
SinusMatrixLedEffect<MATRIX, ledLine, width, height>::~SinusMatrixLedEffect()
{
}

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
void SinusMatrixLedEffect<MATRIX, ledLine, width, height>::reset()
{
    ILedEffect::reset();
    MATRIX<ledLine, width, height>::clearAllLeds();
}

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
void SinusMatrixLedEffect<MATRIX, ledLine, width, height>::paint()
{
    MATRIX<ledLine, width, height>::clearAllLeds();

    hue++;

    if (MATRIX<ledLine, width, height>::getWidth() > MATRIX<ledLine, width, height>::getHeight())
    {
        for (uint8_t x = 0; x < MATRIX<ledLine, width, height>::getWidth(); x++)
        {
            uint8_t y = beatsin8(speed + x, 0, MATRIX<ledLine, width, height>::getHeight() - 1);
            MATRIX<ledLine, width, height>::getPixel(x, y) = ColorFromPalette(RainbowColors_p, x * 7 + hue, 255);
        }
    }
    else
    {
        for (uint8_t y = 0; y < MATRIX<ledLine, width, height>::getHeight(); y++) {
            uint8_t x = beatsin8(speed + y, 0, MATRIX<ledLine, width, height>::getWidth() - 1);
            MATRIX<ledLine, width, height>::getPixel(x, y) = ColorFromPalette(RainbowColors_p, x * 7 + hue, 255);
        }
    }
}

#endif
