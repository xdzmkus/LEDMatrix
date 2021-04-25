#if defined(ESP8266)
#define LED_PIN D3  // D1 leds pin (connected to D5 on my NodeMCU1.0 !!!)
#else
#define LED_PIN 9   // leds pin
#endif

#include <FastLED.h>
#define MATRIX_H 8
#define MATRIX_W1 10
#define MATRIX_W2 12
#define MATRIX_W3 10
#define NUM_LEDS (MATRIX_H * (MATRIX_W1 + MATRIX_W2 + MATRIX_W3))
#define CURRENT_LIMIT 8000
#define BRIGHTNESS 50

CRGB leds[NUM_LEDS];

#include <ILedEffect.h>
#include "ZigZagFromBottomRightToUpAndLeft.h"
#include "StarfallMatrixLedEffect.h"

#define NUM_EFFECTS 3
ILedEffect* effects[NUM_EFFECTS];

ZigZagFromBottomRightToUpAndLeft matrix1(leds, MATRIX_W1, MATRIX_H);
ZigZagFromBottomRightToUpAndLeft matrix2(leds + (MATRIX_W1 * MATRIX_H), MATRIX_W2, MATRIX_H);
ZigZagFromBottomRightToUpAndLeft matrix3(leds + (MATRIX_W1 * MATRIX_H) + (MATRIX_W2 * MATRIX_H), MATRIX_W3, MATRIX_H);

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

	Serial.println(F("Union of Starfall effects"));

	setupLED();

	effects[0] = new StarfallMatrixLedEffect(&matrix1, 10, CRGB::White);
	effects[1] = new StarfallMatrixLedEffect(&matrix2, 10, 0xFF2400);
	effects[2] = new StarfallMatrixLedEffect(&matrix3, 10, CRGB::White);

	for (uint8_t i = 0; i < NUM_EFFECTS; i++)
	{
		effects[i]->start();
	}
}


void loop()
{
	bool isChanged = false;

	for (uint8_t i = 0; i < NUM_EFFECTS; i++)
	{
		if (effects[i]->isReady())
		{
			isChanged = true;
			effects[i]->paint();
		}
	}

	if (isChanged)
	{
		FastLED.show();
	}
}
