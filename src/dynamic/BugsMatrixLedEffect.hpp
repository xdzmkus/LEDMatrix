/* 
* BugsMatrixLedEffect.hpp
*
*/

#ifndef __BUGSMATRIXLEDEFFECT_HPP__
#define __BUGSMATRIXLEDEFFECT_HPP__

#include "ILedEffect.hpp"

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
class BugsMatrixLedEffect : public MATRIX<ledLine, width, height>, public ILedEffect
{
public:

	static LedEffectName const name;

protected:

	const uint8_t BUGS_MAX_SPEED = 20;

	const uint8_t numBugs;

	struct BUGS
	{
		CRGB color;
		int8_t speed;
		uint8_t xPos;
		uint8_t yPos;
		bool hvDir;
	}
	*bugs = nullptr;

public:
	BugsMatrixLedEffect(uint16_t Hz, uint8_t bugs = 1);
	~BugsMatrixLedEffect();
	
	void reset() override;
	void paint() override;

	operator LedEffectName () const {	return name; }

private:

	BugsMatrixLedEffect(const BugsMatrixLedEffect&) = delete;
	BugsMatrixLedEffect& operator=(const BugsMatrixLedEffect&) = delete;

};

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
LedEffectName const BugsMatrixLedEffect<MATRIX, ledLine, width, height>::name = "BUGS";

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
BugsMatrixLedEffect<MATRIX, ledLine, width, height>::BugsMatrixLedEffect(uint16_t Hz, uint8_t bugsCount)
	: ILedEffect(Hz), numBugs(bugsCount)
{
	if (numBugs > 0)
	{
		bugs = new BUGS[numBugs];
	}

	reset();
}

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
BugsMatrixLedEffect<MATRIX, ledLine, width, height>::~BugsMatrixLedEffect()
{
	if (bugs != nullptr)
	{
		delete[] bugs;
		bugs = nullptr;
	}
}

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
void BugsMatrixLedEffect<MATRIX, ledLine, width, height>::reset()
{
	ILedEffect::reset();

	if (bugs != nullptr)
	{
		for (uint8_t i = 0; i < numBugs; i++)
		{
			bugs[i].color = MATRIX<ledLine, width, height>::getRandomColor();
			bugs[i].xPos = random8(0, MATRIX<ledLine, width, height>::getWidth());
			bugs[i].yPos = random8(0, MATRIX<ledLine, width, height>::getHeight());
			bugs[i].speed += random(-5, 6);
			bugs[i].hvDir = static_cast<bool>(random8() % 2);
		}
	}

	MATRIX<ledLine, width, height>::clearAllLeds();
}

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
void BugsMatrixLedEffect<MATRIX, ledLine, width, height>::paint()
{
	if (bugs == nullptr)
		return;
		
	for (uint8_t i = 0; i < numBugs; i++)
	{
		MATRIX<ledLine, width, height>::getPixel(bugs[i].xPos, bugs[i].yPos) -= bugs[i].color;

		bugs[i].speed += random(-5, 6);
		if (abs(bugs[i].speed) > BUGS_MAX_SPEED)
		{
			bugs[i].speed = 0;
			bugs[i].hvDir = static_cast<bool>(random8() % 2);
		}

		if (bugs[i].hvDir)
		{
			int16_t newXpos = bugs[i].xPos + bugs[i].speed / 10;
			if (newXpos < 0 || newXpos >= MATRIX<ledLine, width, height>::getWidth())
			{
				bugs[i].hvDir = !bugs[i].hvDir;
				bugs[i].xPos = constrain(newXpos, 0, (MATRIX<ledLine, width, height>::getWidth() - 1));
			}
			else
			{
				bugs[i].xPos = static_cast<uint8_t>(newXpos);
			}
		}
		else
		{
			int16_t newYpos = bugs[i].yPos + bugs[i].speed / 10;
			if (newYpos < 0 || newYpos >= MATRIX<ledLine, width, height>::getHeight())
			{
				bugs[i].hvDir = !bugs[i].hvDir;
				bugs[i].yPos = constrain(newYpos, 0, (MATRIX<ledLine, width, height>::getHeight() - 1));
			}
			else
			{
				bugs[i].yPos = static_cast<uint8_t>(newYpos);
			}
		}
    
		MATRIX<ledLine, width, height>::getPixel(bugs[i].xPos, bugs[i].yPos) += bugs[i].color;
	}
}

#endif
