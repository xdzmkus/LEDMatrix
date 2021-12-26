#if defined(ESP8266)
#define FASTLED_FORCE_SOFTWARE_SPI
#define FASTLED_FORCE_SOFTWARE_PINS
#define LED_PIN D5  // leds pin
#elif defined(ESP32)
#define LED_PIN  16 // leds pin
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

#include "ZigZagFromTopLeftToBottomAndRight.hpp"
#include "UniversalLEDMatrixEffects.h"

FireMatrixLedEffect<ZigZagFromTopLeftToBottomAndRight, leds, MATRIX_W, MATRIX_H> effect(20);

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

	Serial.print(F("Effect: "));
	Serial.println(effect);

	setupLED();

	effect.start();
}


void loop()
{
	if (effect.isReady())
	{
		effect.paint();
		FastLED.show();
	}
}
