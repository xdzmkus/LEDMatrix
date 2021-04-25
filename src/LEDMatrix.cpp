/*
* LEDMatrix.cpp
*
*/

#include "LEDMatrix.h"

#include "BouncingBallsMatrixLedEffect.h"
#include "BugsMatrixLedEffect.h"
#include "FireMatrixLedEffect.h"
#include "GravityMatrixLedEffect.h"
#include "NoiseMatrixLedEffect.h"
#include "RunningStringMatrixLedEffect.h"
#include "SinusMatrixLedEffect.h"
#include "SnowMatrixLedEffect.h"
#include "StarfallMatrixLedEffect.h"

const uint8_t LEDMatrix::NUM_EFFECTS = 9;

const char* LEDMatrix::availableEffects[NUM_EFFECTS] =
{
	BouncingBallsMatrixLedEffect::name,
	BugsMatrixLedEffect::name,
	FireMatrixLedEffect::name,
	GravityMatrixLedEffect::name,
	NoiseMatrixLedEffect::name,
	RunningStringMatrixLedEffect::name,
	SinusMatrixLedEffect::name,
	SnowMatrixLedEffect::name,
	StarfallMatrixLedEffect::name
};

const TProgmemRGBPalette16 LEDMatrix::wrwPalette_p PROGMEM =
{
	CRGB::White, CRGB::Red, CRGB::Red, CRGB::Red,
	CRGB::White, CRGB::Red, CRGB::Red, CRGB::Red,
	CRGB::White, CRGB::Red, CRGB::Red, CRGB::Red,
	CRGB::White, CRGB::Red, CRGB::Red, CRGB::Red
};

LEDMatrix::LEDMatrix(ILedMatrix* converter)
	: LEDLine(converter->getAllPixels(), converter->getHeight() * converter->getWidth()), matrix(converter)
{
	text = String(RunningStringMatrixLedEffect::name);
}

LEDMatrix::~LEDMatrix()
{
	delete effect;
}

uint8_t LEDMatrix::getAllEffectsNumber() const
{
	return NUM_EFFECTS;
}

const char* const* LEDMatrix::getAllEffectsNames() const
{
	return availableEffects;
}

bool LEDMatrix::setEffectByName(const char* effectName)
{
	bool restart = isOn();

	if (strcmp(BouncingBallsMatrixLedEffect::name, effectName) == 0) {
		delete effect; effect = new BouncingBallsMatrixLedEffect(matrix, 10);
	}
	else if (strcmp(BugsMatrixLedEffect::name, effectName) == 0) {
		delete effect; effect = new BugsMatrixLedEffect(matrix, 10);
	}
	else if (strcmp(FireMatrixLedEffect::name, effectName) == 0) {
		delete effect; effect = new FireMatrixLedEffect(matrix, 10);
	}
	else if (strcmp(GravityMatrixLedEffect::name, effectName) == 0) {
		delete effect; effect = new GravityMatrixLedEffect(matrix, 10);
	}
	else if (strcmp(NoiseMatrixLedEffect::name, effectName) == 0) {
		delete effect; effect = new NoiseMatrixLedEffect(matrix, 10, wrwPalette_p);
	}
	else if (strcmp(RunningStringMatrixLedEffect::name, effectName) == 0) {
		delete effect; effect = new RunningStringMatrixLedEffect(matrix, 10, text);
	}
	else if (strcmp(SinusMatrixLedEffect::name, effectName) == 0) {
		delete effect; effect = new SinusMatrixLedEffect(matrix, 10);
	}
	else if (strcmp(SnowMatrixLedEffect::name, effectName) == 0) {
		delete effect; effect = new SnowMatrixLedEffect(matrix, 2);
	}
	else if (strcmp(StarfallMatrixLedEffect::name, effectName) == 0) {
		delete effect; effect = new StarfallMatrixLedEffect(matrix, 10);
	}
	else {
		return LEDLine::setEffectByName(effectName);
	}

	// start new effect if previous one was started
	if (restart)
	{
		turnOn();
	}

	return true;
}

void LEDMatrix::setRunningString(const char* data, uint16_t len)
{
	text = String(data);
}
