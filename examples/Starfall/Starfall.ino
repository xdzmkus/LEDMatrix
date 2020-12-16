#if defined(ESP32) || defined(ESP8266)
#define LED_PIN D1    // D1 leds pin (connected to D5 on my NodeMCU1.0 !!!)
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

#include "BottomLeftCorner.h"
#include "StarfallMatrixLedEffect.h"

BottomLeftCorner<MATRIX_W, MATRIX_H> matrix;
StarfallMatrixLedEffect* effect;

void setupLED()
{
	FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
	FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
	FastLED.setBrightness(BRIGHTNESS);
	FastLED.clear(true);
}

void setup()
{
	Serial.begin(115200);

	Serial.println(F("Starfall effect"));

	setupLED();

	effect = new StarfallMatrixLedEffect(&matrix, leds, NUM_LEDS, 10);
}


void loop()
{
	if (effect->paint())
	{
		FastLED.show();
	}
}