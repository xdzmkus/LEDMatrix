/*
* BugsMatrixLedEffect.cpp
*
*/

#include "BugsMatrixLedEffect.h"

const char* const BugsMatrixLedEffect::name = "BUGS";

BugsMatrixLedEffect::BugsMatrixLedEffect(ILedMatrix* converter, uint16_t Hz, uint8_t bugsCount)
	: ILedEffect(Hz), matrix(converter), numBugs(bugsCount)
{
	if (numBugs > 0)
	{
		bugs = new BUGS[numBugs];
	}

	reset();
}

BugsMatrixLedEffect::~BugsMatrixLedEffect()
{
	if (bugs != nullptr)
	{
		delete[] bugs;
		bugs = nullptr;
	}
}

void BugsMatrixLedEffect::reset()
{
	ILedEffect::reset();

	if (bugs != nullptr)
	{
		for (uint8_t i = 0; i < numBugs; i++)
		{
			bugs[i].color = matrix->getRandomColor();
			bugs[i].xPos = random8(0, matrix->getWidth());
			bugs[i].yPos = random8(0, matrix->getHeight());
			bugs[i].speed += random(-5, 6);
			bugs[i].hvDir = static_cast<bool>(random8() % 2);
		}
	}

	matrix->clearAllLeds();
}

void BugsMatrixLedEffect::paint()
{
	if (bugs == nullptr)
		return;
		
	for (uint8_t i = 0; i < numBugs; i++)
	{
		matrix->getPixel(bugs[i].xPos, bugs[i].yPos) -= bugs[i].color;

		bugs[i].speed += random(-5, 6);
		if (abs(bugs[i].speed) > BUGS_MAX_SPEED)
		{
			bugs[i].speed = 0;
			bugs[i].hvDir = static_cast<bool>(random8() % 2);
		}

		if (bugs[i].hvDir)
		{
			int16_t newXpos = bugs[i].xPos + bugs[i].speed / 10;
			if (newXpos < 0 || newXpos >= matrix->getWidth())
			{
				bugs[i].hvDir = !bugs[i].hvDir;
				bugs[i].xPos = constrain(newXpos, 0, matrix->getWidth() - 1);
			}
			else
			{
				bugs[i].xPos = static_cast<uint8_t>(newXpos);
			}
		}
		else
		{
			int16_t newYpos = bugs[i].yPos + bugs[i].speed / 10;
			if (newYpos < 0 || newYpos >= matrix->getHeight())
			{
				bugs[i].hvDir = !bugs[i].hvDir;
				bugs[i].yPos = constrain(newYpos, 0, matrix->getHeight() - 1);
			}
			else
			{
				bugs[i].yPos = static_cast<uint8_t>(newYpos);
			}
		}
    
		matrix->getPixel(bugs[i].xPos, bugs[i].yPos) += bugs[i].color;
	}
}
