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
#define BRIGHTNESS 30

CRGB leds[NUM_LEDS];

#include "MatrixLineConverters.h"
#include "UniversalLEDMatrixEffects.h"

NoiseMatrixLedEffect<ZigZagFromTopLeftToBottomAndRight, leds, MATRIX_W, MATRIX_H>* effect;

#include <ClockTimer.hpp>
#define EFFECT_DURATION_SEC 30
MillisTimer tickerEffects(EFFECT_DURATION_SEC* MillisTimer::CLOCKS_IN_SEC);

void changeEffect()
{
	uint8_t zoom = random(5, 100);
	uint8_t speed = random(5, 60);

	Serial.print("SCALE: "); Serial.print(zoom); Serial.print("\tSPEED: "); Serial.println(speed);

	delete effect;

	effect = new NoiseMatrixLedEffect<ZigZagFromTopLeftToBottomAndRight, leds, MATRIX_W, MATRIX_H>(speed, RainbowColors_p, zoom);

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

	Serial.println(F("Rainbow Noise effect"));

	setupLED();

	effect = new NoiseMatrixLedEffect<ZigZagFromTopLeftToBottomAndRight, leds, MATRIX_W, MATRIX_H>(15, RainbowColors_p);

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
