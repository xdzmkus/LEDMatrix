// LEDMatrixEx.h

#ifndef _LEDMATRIXEX_h
#define _LEDMATRIXEX_h

#include <FastLED.h>
#include "LEDMatrix.h"
#include "ILedMatrix.h"
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

class LEDMatrixEx : public LEDMatrix
{
private:

	enum NOISE_NAME { WRW = 0, OCEAN, RAINBOW, CLOUD, FOREST, PARTY, LAVA };

	const char* noiseEffects[7] =
	{
		"WRW_NOISE",
		"OCEAN_NOISE",
		"RAINBOW_NOISE",
		"CLOUD_NOISE",
		"FOREST_NOISE",
		"PARTY_NOISE",
		"LAVA_NOISE"
	};

	static const uint8_t NUM_EFFECTS = 17;
	const char* availableEffects[NUM_EFFECTS] =
	{
		RainbowLedEffect::name,
		SparklesLedEffect::name,
		BugsMatrixLedEffect::name,
		noiseEffects[WRW],
		BouncingBallsMatrixLedEffect::name,
		noiseEffects[OCEAN],
		SnowMatrixLedEffect::name,
		noiseEffects[RAINBOW],
		StarfallMatrixLedEffect::name,
		noiseEffects[CLOUD],
		FireMatrixLedEffect::name,
		noiseEffects[FOREST],
		SinusMatrixLedEffect::name,
		noiseEffects[PARTY],
		GravityMatrixLedEffect::name,
		noiseEffects[LAVA],
		RunningStringMatrixLedEffect::name
	};

public:

	LEDMatrixEx(ILedMatrix* converter) : LEDMatrix(converter)
	{
		text = String(":-)");
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

		if (strcmp(SparklesLedEffect::name, effectName) == 0) {
			delete effect; effect = new SparklesLedEffect(leds, numLeds, 10, CRGB::Aquamarine);
		}
		else if (strcmp(BouncingBallsMatrixLedEffect::name, effectName) == 0) {
			delete effect; effect = new BouncingBallsMatrixLedEffect(matrix, 10, 3);
		}
		else if (strcmp(BugsMatrixLedEffect::name, effectName) == 0) {
			delete effect; effect = new BugsMatrixLedEffect(matrix, 20, random8(25, 50));
		}
		else if (strcmp(FireMatrixLedEffect::name, effectName) == 0) {
			delete effect; effect = new FireMatrixLedEffect(matrix, 10);
		}
		else if (strcmp(GravityMatrixLedEffect::name, effectName) == 0) {
			delete effect; effect = new GravityMatrixLedEffect(matrix, random8(5, 30));
		}
		else if (strcmp(RunningStringMatrixLedEffect::name, effectName) == 0) {
			delete effect; effect = new RunningStringMatrixLedEffect(matrix, random(5, 30), text);
		}
		else if (strcmp(SinusMatrixLedEffect::name, effectName) == 0) {
			delete effect; effect = new SinusMatrixLedEffect(matrix, random(10, 50));
		}
		else if (strcmp(SnowMatrixLedEffect::name, effectName) == 0) {
			delete effect; effect = new SnowMatrixLedEffect(matrix, 2);
		}
		else if (strcmp(StarfallMatrixLedEffect::name, effectName) == 0) {
			delete effect; effect = new StarfallMatrixLedEffect(matrix, 10);
		}
		else if (strcmp(noiseEffects[LAVA], effectName) == 0) {
			delete effect; effect = new NoiseMatrixLedEffect(matrix, random(8, 30), LavaColors_p, random(5, 50));
			if (effect != NULL) effect->setId(noiseEffects[LAVA]);
		}
		else if (strcmp(noiseEffects[OCEAN], effectName) == 0) {
			delete effect; effect = new NoiseMatrixLedEffect(matrix, random(8, 30), OceanColors_p, random(5, 50));
			if (effect != NULL) effect->setId(noiseEffects[OCEAN]);
		}
		else if (strcmp(noiseEffects[RAINBOW], effectName) == 0) {
			delete effect; effect = new NoiseMatrixLedEffect(matrix, random(10, 50), RainbowColors_p, random(5, 50));
			if (effect != NULL) effect->setId(noiseEffects[RAINBOW]);
		}
		else if (strcmp(noiseEffects[CLOUD], effectName) == 0) {
			delete effect; effect = new NoiseMatrixLedEffect(matrix, random(8, 30), CloudColors_p, random(5, 50));
			if (effect != NULL) effect->setId(noiseEffects[CLOUD]);
		}
		else if (strcmp(noiseEffects[FOREST], effectName) == 0) {
			delete effect; effect = new NoiseMatrixLedEffect(matrix, random(8, 30), ForestColors_p, random(5, 50));
			if (effect != NULL) effect->setId(noiseEffects[FOREST]);
		}
		else if (strcmp(noiseEffects[PARTY], effectName) == 0) {
			delete effect; effect = new NoiseMatrixLedEffect(matrix, random(10, 50), PartyColors_p, random(5, 50));
			if (effect != NULL) effect->setId(noiseEffects[PARTY]);
		}
		else if (strcmp(noiseEffects[WRW], effectName) == 0) {
			delete effect; effect = new NoiseMatrixLedEffect(matrix, random(10, 30), wrwPalette_p, random(5, 50));
			if (effect != NULL) effect->setId(noiseEffects[WRW]);
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
