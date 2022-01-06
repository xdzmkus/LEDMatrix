#if defined(ESP8266)
#define FASTLED_FORCE_SOFTWARE_SPI
#define FASTLED_FORCE_SOFTWARE_PINS
#define LED_PIN D5  // leds pin
#define UNPINNED_ANALOG_PIN A0 // not connected analog pin
#elif defined(ESP32)
#define LED_PIN  16 // leds pin
#define UNPINNED_ANALOG_PIN 35 // not connected analog pin
#else
#define LED_PIN 9   // leds pin
#define UNPINNED_ANALOG_PIN A0 // not connected analog pin
#endif

#include <FastLED.h>

#define MATRIX_H 8
#define MATRIX_W 32

#define CURRENT_LIMIT 8000

uint8_t brightness = 100;

CRGB leds[(MATRIX_H * MATRIX_W)];

#include "MatrixLineConverters.h"
#include "StaticLEDMatrix.hpp"
StaticLEDMatrix<ZigZagFromTopLeftToBottomAndRight, leds, MATRIX_W, MATRIX_H> ledMatrix;

#include "EffectTimer.hpp"

#define EFFECT_DURATION_SEC 60
MillisTimer timerEffects(EFFECT_DURATION_SEC* MillisTimer::CLOCKS_IN_SEC);

void changeEffect()
{
	ledMatrix.setNextEffect();

	Serial.print(F("EFFECT: "));
	Serial.println(ledMatrix.getEffectName());
}

void setupLED()
{
	FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, (MATRIX_H * MATRIX_W)).setCorrection(TypicalSMD5050);
	FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
	FastLED.setBrightness(brightness);
	FastLED.clear(true);
}

void setup()
{
	randomSeed(analogRead(UNPINNED_ANALOG_PIN));

	Serial.begin(115200);

	Serial.println(F("LEDMatrix EFFECTS:"));
	for (uint8_t var = 0; var < ledMatrix.howManyEffects(); var++)
	{
		Serial.println(ledMatrix.getAllEffectsNames()[var]);
	}

	setupLED();

	ledMatrix.turnOn();

	Serial.print(F("EFFECT: "));
	Serial.println(ledMatrix.getEffectName());

	timerEffects.start();
}

void loop()
{
	if (timerEffects.isReady())
		changeEffect();

	if (ledMatrix.refresh())
	{
		FastLED.show();
	}
}
