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
		uint8_t	position;
		float	altitude;
		unsigned long startTime;
		float MaxVelocity;
	}
	gravitiy;

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

	gravitiy.position = 0;
	gravitiy.altitude = 0;
	gravitiy.startTime = getClock();
	gravitiy.MaxVelocity = sqrt(2 * Gravity * (MATRIX<ledLine, width, height>::getHeight() - 1));
}

template<template <CRGB* const, const uint8_t, const uint8_t> class MATRIX, CRGB* const ledLine, const uint8_t width, const uint8_t height>
void GravityMatrixLedEffect<MATRIX, ledLine, width, height>::paint()
{
	*this >> 0;

	float timeOfFlying = static_cast<float>(getClock() - gravitiy.startTime) / ILedEffect::CLOCKS_IN_SEC;
	gravitiy.altitude = gravitiy.MaxVelocity * timeOfFlying - 0.5 * Gravity * timeOfFlying * timeOfFlying;

	if (gravitiy.altitude <= -2)
	{
		gravitiy.startTime = getClock();
	}

	if (gravitiy.altitude < 0)
	{
		gravitiy.altitude = 0;
	}

	hue += speed;

	MATRIX<ledLine, width, height>::getPixel(0, gravitiy.position) = CRGB::Black;

	gravitiy.position = round(gravitiy.altitude);

	MATRIX<ledLine, width, height>::getPixel(0, gravitiy.position) = ColorFromPalette(RainbowColors_p, hue, 255);
}

#endif
