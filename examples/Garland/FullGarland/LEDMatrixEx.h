// LEDMatrixEx.h

#ifndef _LEDMATRIXEX_h
#define _LEDMATRIXEX_h

#include <FastLED.h>
#include "LEDMatrix.h"
#include "IMatrixToLineConverter.h"
#include "BouncingBallsMatrixLedEffect.h"
#include "BugsMatrixLedEffect.h"
#include "FireMatrixLedEffect.h"
#include "GravityMatrixLedEffect.h"
#include "NoiseMatrixLedEffect.h"
#include "RunningStringMatrixLedEffect.h"
#include "SinusMatrixLedEffect.h"
#include "SnowMatrixLedEffect.h"
#include "StarfallMatrixLedEffect.h"
// LEDLine effects:
#include <RainbowLedEffect.h>
#include <SparklesLedEffect.h>
#include <ThreeColorLedEffect.h>

class LEDMatrixEx : public LEDMatrix
{
private:

	static const uint8_t NUM_EFFECTS = 17;
	const char* availableEffects[NUM_EFFECTS] =
	{
		SparklesLedEffect::name,
		RainbowLedEffect::name,
		BugsMatrixLedEffect::name,
		"LAVA_NOISE",
		BouncingBallsMatrixLedEffect::name,
		"OCEAN_NOISE",
		SnowMatrixLedEffect::name,
		"RAINBOW_NOISE",
		StarfallMatrixLedEffect::name,
		"CLOUD_NOISE",
		FireMatrixLedEffect::name,
		"FOREST_NOISE",
		SinusMatrixLedEffect::name,
		"PARTY_NOISE",
		GravityMatrixLedEffect::name,
		"WRW_NOISE",
		RunningStringMatrixLedEffect::name
	};

public:

	LEDMatrixEx(IMatrixToLineConverter* converter, CRGB leds[], uint16_t count) : LEDMatrix(converter, leds, count)
	{
		text = String(":) 2021 (:");
	};

	~LEDMatrixEx()
	{
	};

	uint8_t getAllEffectsNumber() const override
	{
		return NUM_EFFECTS;
	};

	const char* const* getAllEffectsNames() const override
	{
		return availableEffects;
	};

	bool setEffectByName(const char* effectName) override
	{
		bool restart = isOn();

		if (strcmp(ThreeColorLedEffect::name, effectName) == 0) {
			delete effect; effect = new ThreeColorLedEffect(leds, numLeds, 30, { CRGB::White, 4, CRGB::Red, 3, CRGB::White, 4 });
		}
		else if (strcmp(BouncingBallsMatrixLedEffect::name, effectName) == 0) {
			delete effect; effect = new BouncingBallsMatrixLedEffect(matrix, leds, numLeds, 10, 3);
		}
		else if (strcmp(BugsMatrixLedEffect::name, effectName) == 0) {
			delete effect; effect = new BugsMatrixLedEffect(matrix, leds, numLeds, 20, random8(40, 80));
		}
		else if (strcmp(FireMatrixLedEffect::name, effectName) == 0) {
			delete effect; effect = new FireMatrixLedEffect(matrix, leds, numLeds, 10);
		}
		else if (strcmp(GravityMatrixLedEffect::name, effectName) == 0) {
			delete effect; effect = new GravityMatrixLedEffect(matrix, leds, numLeds, random8(5, 30));
		}
		else if (strcmp(RunningStringMatrixLedEffect::name, effectName) == 0) {
			delete effect; effect = new RunningStringMatrixLedEffect(matrix, leds, numLeds, random(5, 30), text);
		}
		else if (strcmp(SinusMatrixLedEffect::name, effectName) == 0) {
			delete effect; effect = new SinusMatrixLedEffect(matrix, leds, numLeds, random(10, 50));
		}
		else if (strcmp(SnowMatrixLedEffect::name, effectName) == 0) {
			delete effect; effect = new SnowMatrixLedEffect(matrix, leds, numLeds, 2);
		}
		else if (strcmp(StarfallMatrixLedEffect::name, effectName) == 0) {
			delete effect; effect = new StarfallMatrixLedEffect(matrix, leds, numLeds, 10);
		}
		else if (strcmp("PARTY_NOISE", effectName) == 0) {
			delete effect; effect = new NoiseMatrixLedEffect(matrix, leds, numLeds, random(10, 50), PartyColors_p, random(5, 60));
			effect->setId("PARTY_NOISE");
		}
		else if (strcmp("CLOUD_NOISE", effectName) == 0) {
			delete effect; effect = new NoiseMatrixLedEffect(matrix, leds, numLeds, random(8, 30), CloudColors_p);
			effect->setId("CLOUD_NOISE");
		}
		else if (strcmp("LAVA_NOISE", effectName) == 0) {
			delete effect; effect = new NoiseMatrixLedEffect(matrix, leds, numLeds, random(8, 30), LavaColors_p);
			effect->setId("LAVA_NOISE");
		}
		else if (strcmp("OCEAN_NOISE", effectName) == 0) {
			delete effect; effect = new NoiseMatrixLedEffect(matrix, leds, numLeds, random(8, 30), OceanColors_p);
			effect->setId("OCEAN_NOISE");
		}
		else if (strcmp("FOREST_NOISE", effectName) == 0) {
			delete effect; effect = new NoiseMatrixLedEffect(matrix, leds, numLeds, random(8, 30), ForestColors_p);
			effect->setId("FOREST_NOISE");
		}
		else if (strcmp("RAINBOW_NOISE", effectName) == 0) {
			delete effect; effect = new NoiseMatrixLedEffect(matrix, leds, numLeds, random(10, 50), RainbowColors_p, random(5, 80));
			effect->setId("RAINBOW_NOISE");
		}
		else if (strcmp("WRW_NOISE", effectName) == 0) {
			delete effect; effect = new NoiseMatrixLedEffect(matrix, leds, numLeds, random(10, 30), wrwPalette, random(5, 50));
			effect->setId("WRW_NOISE");
		}
		else {
			return LEDMatrix::setEffectByName(effectName);
		}

		// start new effect if previous one was started
		if (restart)
		{
			turnOn();
		}

		return true;
	};
};


#endif
