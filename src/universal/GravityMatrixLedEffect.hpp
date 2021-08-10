/* 
* GravityMatrixLedEffect.hpp
*
*/

#ifndef __GRAVITYMATRIXLEDEFFECT_HPP__
#define __GRAVITYMATRIXLEDEFFECT_HPP__

#include <ILedEffect.hpp>
#include "ILedMatrix.hpp"

template<template <CRGB* const, const uint8_t, const uint8_t> class MATRIX, CRGB* const ledLine, const uint8_t width, const uint8_t height>
class GravityMatrixLedEffect : public MATRIX<ledLine, width, height>, public ILedEffect
{
public:

	static const char* const name;

private:

	const float Gravity = 9.8;

	uint8_t hue = 0;

	struct GRAVITY
	{
		CRGB	color;
		uint8_t	position;
		float	altitude;
		float	velocity;
		unsigned long startTime;
	}
	gravities[width];

public:

	GravityMatrixLedEffect(uint16_t Hz);
	~GravityMatrixLedEffect();

	void reset() override;
	void paint() override;

	operator const char* () const { return name; }

private:

	GravityMatrixLedEffect(const GravityMatrixLedEffect&) = delete;
	GravityMatrixLedEffect& operator=(const GravityMatrixLedEffect&) = delete;
};

template<template <CRGB* const, const uint8_t, const uint8_t> class MATRIX, CRGB* const ledLine, const uint8_t width, const uint8_t height>
const char* const GravityMatrixLedEffect<MATRIX, ledLine, width, height>::name = "GRAVITY";

template<template <CRGB* const, const uint8_t, const uint8_t> class MATRIX, CRGB* const ledLine, const uint8_t width, const uint8_t height>
GravityMatrixLedEffect<MATRIX, ledLine, width, height>::GravityMatrixLedEffect(uint16_t Hz)
	: ILedEffect(Hz)
{
	MATRIX<ledLine, width, height>::clearAllLeds();
	reset();
}

template<template <CRGB* const, const uint8_t, const uint8_t> class MATRIX, CRGB* const ledLine, const uint8_t width, const uint8_t height>
GravityMatrixLedEffect<MATRIX, ledLine, width, height>::~GravityMatrixLedEffect()
{
}

template<template <CRGB* const, const uint8_t, const uint8_t> class MATRIX, CRGB* const ledLine, const uint8_t width, const uint8_t height>
void GravityMatrixLedEffect<MATRIX, ledLine, width, height>::reset()
{
	ILedEffect::reset();

	uint8_t max = MATRIX<ledLine, width, height>::getHeight() - 1;

	for (uint8_t x = 0; x < MATRIX<ledLine, width, height>::getWidth(); x++)
	{
		gravities[x].color = ColorFromPalette(RainbowColors_p, x * 7 + hue, 255);
		gravities[x].startTime = getClock();
		gravities[x].altitude = 1;
		gravities[x].position = 0;
		gravities[x].velocity = sqrt(2 * Gravity * map8(sin8(x * speed + hue), 0, max));
	}

	hue += speed;
}

template<template <CRGB* const, const uint8_t, const uint8_t> class MATRIX, CRGB* const ledLine, const uint8_t width, const uint8_t height>
void GravityMatrixLedEffect<MATRIX, ledLine, width, height>::paint()
{
	bool allGround = true;

	for (uint8_t x = 0; x < MATRIX<ledLine, width, height>::getWidth(); x++)
	{
		if (gravities[x].altitude > 0)
		{
			float timeOfFlying = static_cast<float>(getClock() - gravities[x].startTime) / ILedEffect::CLOCKS_IN_SEC;
			gravities[x].altitude = gravities[x].velocity * timeOfFlying - 0.5 * Gravity * timeOfFlying * timeOfFlying;

			if (gravities[x].altitude <= 0)
			{
				gravities[x].altitude = 0;
			}
			else
			{
				allGround = false;
			}

			MATRIX<ledLine, width, height>::getPixel(x, gravities[x].position) = CRGB::Black;

			gravities[x].position = round(gravities[x].altitude);

			MATRIX<ledLine, width, height>::getPixel(x, gravities[x].position) = gravities[x].color;
		}
	}

	if (allGround)
	{
		reset();
	}
}

#endif
