#if defined(ESP32) || defined(ESP8266)
#define LED_PIN D3    // D1 leds pin (connected to D5 on my NodeMCU1.0 !!!)
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

#include "BottomLeftCorner.h"
#include "NoiseMatrixLedEffect.h"

BottomLeftCorner<MATRIX_W, MATRIX_H> matrix;
NoiseMatrixLedEffect* effect;

#include <Ticker.h>
#define EFFECT_DURATION_SEC 30
Ticker tickerEffects;
volatile boolean f_changeEffect = false;

void handleTimer()
{
	f_changeEffect = true;
}

void changeEffect()
{
	uint8_t zoom = random(5, 100);
	uint8_t speed = random(5, 60);

	Serial.print("SCALE: "); Serial.print(zoom); Serial.print("\tSPEED: "); Serial.println(speed);

	delete effect;

	effect = new NoiseMatrixLedEffect(new BottomLeftCorner<MATRIX_W, MATRIX_H>, leds, NUM_LEDS, speed, ForestColors_p, zoom);
}

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

	Serial.println(F("Forest NoiseMatrix effect"));

	setupLED();

	tickerEffects.attach(EFFECT_DURATION_SEC, handleTimer);

	effect = new NoiseMatrixLedEffect(new BottomLeftCorner<MATRIX_W, MATRIX_H>, leds, NUM_LEDS, 15, ForestColors_p);
}


void loop()
{
	if (f_changeEffect)
	{
		f_changeEffect = false;

		changeEffect();
	}

	if (effect->paint())
	{
		FastLED.show();
	}
}
