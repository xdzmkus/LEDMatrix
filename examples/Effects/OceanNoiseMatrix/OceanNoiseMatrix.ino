#if defined(ESP8266)
#define LED_PIN D3  // D1 leds pin (connected to D5 on my NodeMCU1.0 !!!)
#else
#define LED_PIN 9   // leds pin
#endif

#include <FastLED.h>
#define MATRIX_H 8
#define MATRIX_W 32
#define NUM_LEDS (MATRIX_H * MATRIX_W)
#define CURRENT_LIMIT 8000
#define BRIGHTNESS 30

CRGB leds[NUM_LEDS];

#include "ZigZagFromBottomRightToUpAndLeft.h"
#include "NoiseMatrixLedEffect.h"

ZigZagFromBottomRightToUpAndLeft matrix(leds, MATRIX_W, MATRIX_H);
NoiseMatrixLedEffect* effect;

#include <EffectTimer.h>
#define EFFECT_DURATION_SEC 30
MillisTimer tickerEffects(EFFECT_DURATION_SEC* MillisTimer::CLOCKS_IN_SEC);

void changeEffect()
{
	uint8_t zoom = random(5, 100);
	uint8_t speed = random(5, 60);

	Serial.print("SCALE: "); Serial.print(zoom); Serial.print("\tSPEED: "); Serial.println(speed);

	delete effect;

	effect = new NoiseMatrixLedEffect(&matrix, speed, OceanColors_p, zoom);

	effect->start();
}

void setupLED()
{
	FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
	FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
	FastLED.setBrightness(BRIGHTNESS);
	FastLED.clear(true);
}

void setup()
{
	Serial.begin(115200);

	Serial.println(F("Ocean Noise effect"));

	setupLED();

	effect = new NoiseMatrixLedEffect(&matrix, 15, OceanColors_p);

	effect->start();

	tickerEffects.start();
}

void loop()
{
	if (tickerEffects.isReady())
	{
		changeEffect();
	}

	if (effect->isReady())
	{
		effect->paint();
		FastLED.show();
	}
}
