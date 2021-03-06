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
#define BRIGHTNESS 50

CRGB leds[NUM_LEDS];

#include "ZigZagFromBottomRightToUpAndLeft.h"
#include "FireMatrixLedEffect.h"

ZigZagFromBottomRightToUpAndLeft matrix(leds, MATRIX_W, MATRIX_H);
FireMatrixLedEffect* effect;

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

	Serial.println(F("FireMatrix effect"));

	setupLED();

	effect = new FireMatrixLedEffect(&matrix, 10);

	effect->start();
}


void loop()
{
	if (effect->isReady())
	{
		effect->paint();
		FastLED.show();
	}
}
