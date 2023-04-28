/* 
* SnowMatrixLedEffect.hpp
*
*/

#ifndef __SNOWMATRIXLEDEFFECT_HPP__
#define __SNOWMATRIXLEDEFFECT_HPP__

#include <ILedEffect.hpp>

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
class SnowMatrixLedEffect : public MATRIX<ledLine, width, height>, public ILedEffect
{
public:

	static LedEffectName const name;

private:

	const uint16_t snowflakeCount;

	MillisTimer dirTimer;

	const uint8_t fade;

	bool direction = false;

public:

	SnowMatrixLedEffect(uint16_t Hz, unsigned long changeDirection = 2000UL, uint8_t fadeSpeed = 70);
	~SnowMatrixLedEffect();

	bool isReady() override;

	void reset() override;
	void paint() override;

	operator LedEffectName () const { return name; }

private:

	SnowMatrixLedEffect(const SnowMatrixLedEffect&) = delete;
	SnowMatrixLedEffect& operator=(const SnowMatrixLedEffect&) = delete;

};


template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
LedEffectName const SnowMatrixLedEffect<MATRIX, ledLine, width, height>::name = "SNOW";

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
SnowMatrixLedEffect<MATRIX, ledLine, width, height>::SnowMatrixLedEffect(uint16_t Hz, unsigned long changeDirection, uint8_t fadeSpeed)
	: ILedEffect(Hz), snowflakeCount(MATRIX<ledLine, width, height>::getWidth()), dirTimer(changeDirection), fade(fadeSpeed)
{
	reset();

	dirTimer.start();
}

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
SnowMatrixLedEffect<MATRIX, ledLine, width, height>::~SnowMatrixLedEffect()
{
}

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
bool SnowMatrixLedEffect<MATRIX, ledLine, width, height>::isReady()
{
	if (dirTimer.isReady())
		direction = !direction;

	return ILedEffect::isReady();
}

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
void SnowMatrixLedEffect<MATRIX, ledLine, width, height>::reset()
{
	ILedEffect::reset();
	dirTimer.reset();
	MATRIX<ledLine, width, height>::clearAllLeds();
}

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
void SnowMatrixLedEffect<MATRIX, ledLine, width, height>::paint()
{
	uint16_t restSnowflakes = 0;

	if (direction)
	{
		// shift down all lines and fade
		for (uint8_t y = 0; y < MATRIX<ledLine, width, height>::getHeight() - 1; y++)
		{
			// shift left all lines
			for (uint8_t x = 0; x < MATRIX<ledLine, width, height>::getWidth() - 1; x++)
			{
				MATRIX<ledLine, width, height>::getPixel(x, y) = MATRIX<ledLine, width, height>::getPixel(x + 1, y + 1).subtractFromRGB(fade);
				if (MATRIX<ledLine, width, height>::getPixel(x, y))
					restSnowflakes++;
			}
		}
		// clear right column
		MATRIX<ledLine, width, height>::clearColumn(MATRIX<ledLine, width, height>::getWidth() - 1);
	}
	else
	{
		// shift down all lines and fade
		for (uint8_t y = 0; y < MATRIX<ledLine, width, height>::getHeight() - 1; y++)
		{
			// shift right all lines
			for (uint8_t x = MATRIX<ledLine, width, height>::getWidth() - 1; x > 0; x--)
			{
				MATRIX<ledLine, width, height>::getPixel(x, y) = MATRIX<ledLine, width, height>::getPixel(x - 1, y + 1).subtractFromRGB(fade);
				if (MATRIX<ledLine, width, height>::getPixel(x, y))
					restSnowflakes++;
			}
		}
		// clear left column
		MATRIX<ledLine, width, height>::clearColumn(0);
	}

	// clear top line
	MATRIX<ledLine, width, height>::clearLine(MATRIX<ledLine, width, height>::getHeight() - 1);

	// fill randomly snowflakes
	while (restSnowflakes < snowflakeCount)
	{
		uint8_t x = random8(0, MATRIX<ledLine, width, height>::getWidth());
		uint8_t y = random8(1, MATRIX<ledLine, width, height>::getHeight());

		MATRIX<ledLine, width, height>::getPixel(x, y) = CRGB::White;

		restSnowflakes++;
	}
}

#endif
