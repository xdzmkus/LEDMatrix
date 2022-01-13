/* 
* NoiseMatrixLedEffect.hpp
*
*/

#ifndef __NOISEMATRIXLEDEFFECT_HPP__
#define __NOISEMATRIXLEDEFFECT_HPP__

#include <ILedEffect.hpp>

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
class NoiseMatrixLedEffect : public MATRIX<ledLine, width, height>, public ILedEffect
{
public:

	static LedEffectName const name;

private:

	uint8_t noise[width][height];

	const CRGBPalette16 currentPalette;

	// We're using the x/y dimensions to map to the x/y pixels on the matrix.
	// We'll use the z-axis for "time". Speed determines how fast time moves forward.
	// Try 1 for a very slow moving effect, or 60 for something that ends up looking like water.
//	const uint8_t speed = 20;

	// If we're runing at a low "speed", some 8-bit artifacts become visible from frame-to-frame.
	// In order to reduce this, we can do some fast data-smoothing.
	// The amount of data smoothing we're doing depends on "speed".
	const uint8_t dataSmoothing = 120;

	// Scale determines how far apart the pixels in our noise matrix are.
	// Try changing these values around to see how it affects the motion of the display.
	// The higher the value of scale, the more "zoomed out" the noise will be.
	// A value of 1 will be so zoomed in, you'll mostly see solid colors.
	const uint8_t scale = 30;

	uint16_t nX = 0;
	uint16_t nY = 0;
	uint16_t nZ = 0;
	uint8_t ihue = 0;

public:

	NoiseMatrixLedEffect(uint16_t Hz, const CRGBPalette16 &palette, uint8_t zoom = 30);
	~NoiseMatrixLedEffect();

	void reset() override;
	void paint() override;

	operator LedEffectName () const { return id.length() == 0 ? name : id.c_str(); }

private:

	NoiseMatrixLedEffect(const NoiseMatrixLedEffect&) = delete;
	NoiseMatrixLedEffect& operator=(const NoiseMatrixLedEffect&) = delete;

};


template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
LedEffectName const NoiseMatrixLedEffect<MATRIX, ledLine, width, height>::name = "NOISE";

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
NoiseMatrixLedEffect<MATRIX, ledLine, width, height>::NoiseMatrixLedEffect(uint16_t Hz, const CRGBPalette16& palette, uint8_t zoom)
	: ILedEffect(Hz), currentPalette(palette), dataSmoothing((Hz < 50) ? (200 - Hz * 4) : 0), scale(zoom)
{
    reset();
}

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
NoiseMatrixLedEffect<MATRIX, ledLine, width, height>::~NoiseMatrixLedEffect()
{
}

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
void NoiseMatrixLedEffect<MATRIX, ledLine, width, height>::reset()
{
    ILedEffect::reset();

    nX = random16();
    nY = random16();
    nZ = random16();
    ihue = 0;

    MATRIX<ledLine, width, height>::clearAllLeds();
}

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
void NoiseMatrixLedEffect<MATRIX, ledLine, width, height>::paint()
{
    for (uint8_t x = 0; x < MATRIX<ledLine, width, height>::getWidth(); x++)
    {
        for (uint8_t y = 0; y < MATRIX<ledLine, width, height>::getHeight(); y++)
        {
            uint8_t data = inoise8(nX + scale * x, nY + scale * y, nZ);
            data = qsub8(data, 16);
            data = qadd8(data, scale8(data, 39));

            if (dataSmoothing > 0)
                noise[x][y] = scale8(noise[x][y], dataSmoothing) + scale8(data, 256 - dataSmoothing);
            else
                noise[x][y] = data;

            uint8_t index = noise[x][y] + ihue;

            // brighten up, as the color palette itself often contains the light/dark dynamic range desired
            uint8_t bri = (noise[x][y] > 127) ? 255 : dim8_raw(noise[x][y] * 2);

           
           MATRIX<ledLine, width, height>::getPixel(x, y) = ColorFromPalette(currentPalette, index, bri);
        }
    }

    // apply slow drift to X and Y, just for visual variation.
    nZ += speed;
    nX += speed / 8;
    nY -= speed / 16;
    ihue++;
}

#endif
