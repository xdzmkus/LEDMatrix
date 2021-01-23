/*
* BugsMatrixLedEffect.cpp
*
*/

#include "BugsMatrixLedEffect.h"

const char* const BugsMatrixLedEffect::name = "BUGS";

BugsMatrixLedEffect::BugsMatrixLedEffect(const IMatrixToLineConverter* converter, CRGB leds[], uint16_t count, uint16_t Hz, uint8_t bugsCount)
	: ILedEffect(leds, count, Hz), converter(converter), numBugs(bugsCount)
{
	if (numBugs > 0)
	{
		bugs = new BUGS[numBugs];
	}

	init();
}

BugsMatrixLedEffect::~BugsMatrixLedEffect()
{
	if (bugs != nullptr)
	{
		delete[] bugs;
		bugs = nullptr;
	}
}

void BugsMatrixLedEffect::init()
{
	if (bugs != nullptr)
	{
		for (uint8_t i = 0; i < numBugs; i++)
		{
			bugs[i].color = getRandomColor();
			bugs[i].xPos = random8(0, converter->getWidth());
			bugs[i].yPos = random8(0, converter->getHeight());
			bugs[i].speed += random(-5, 6);
			bugs[i].hvDir = static_cast<bool>(random8() % 2);
		}
	}

	clearAllLeds();
}

bool BugsMatrixLedEffect::paint()
{
	if (!isReady() || bugs == nullptr)
		return false;
		
	for (uint8_t i = 0; i < numBugs; i++)
	{
		ledLine[converter->getPixelNumber(bugs[i].xPos, bugs[i].yPos)] -= bugs[i].color;

		bugs[i].speed += random(-5, 6);
		if (abs(bugs[i].speed) > BUGS_MAX_SPEED)
		{
			bugs[i].speed = 0;
			bugs[i].hvDir = static_cast<bool>(random8() % 2);
		}

		if (bugs[i].hvDir)
		{
			int16_t newXpos = bugs[i].xPos + bugs[i].speed / 10;
			if (newXpos < 0 || newXpos >= converter->getWidth())
			{
				bugs[i].hvDir = !bugs[i].hvDir;
				bugs[i].xPos = constrain(newXpos, 0, converter->getWidth() - 1);
			}
			else
			{
				bugs[i].xPos = static_cast<uint8_t>(newXpos);
			}
		}
		else
		{
			int16_t newYpos = bugs[i].yPos + bugs[i].speed / 10;
			if (newYpos < 0 || newYpos >= converter->getHeight())
			{
				bugs[i].hvDir = !bugs[i].hvDir;
				bugs[i].yPos = constrain(newYpos, 0, converter->getHeight() - 1);
			}
			else
			{
				bugs[i].yPos = static_cast<uint8_t>(newYpos);
			}
		}
    
		ledLine[converter->getPixelNumber(bugs[i].xPos, bugs[i].yPos)] += bugs[i].color;
	}

	return true;
}
