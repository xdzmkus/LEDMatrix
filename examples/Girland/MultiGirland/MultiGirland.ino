#if defined(ESP32) || defined(ESP8266)
#define LED_PIN D3   // leds pin
#define BTN_PIN D0   // button pin
#else
#define LED_PIN 9   // leds pin
#define BTN_PIN 10  // button pin
#endif

#define TOUCH_BUTTON
#include <GyverButton.h>
#if defined(TOUCH_BUTTON)
GButton touch(BTN_PIN, LOW_PULL, NORM_OPEN);
#else
GButton touch(BTN_PIN, HIGH_PULL, NORM_OPEN);
#endif

#define CURRENT_LIMIT 16000

#define MATRIX_H 8
#define MATRIX_W1 10
#define MATRIX_W2 12
#define MATRIX_W3 10

#define NUM_LEDS (MATRIX_H * (MATRIX_W1+MATRIX_W2+MATRIX_W3))

#define MAX_BRIGHTNESS 300
#define MIN_BRIGHTNESS 30

#define EEPROM_ADDRESS_EFFECT 0

#include <FastLED.h>

#include "LEDMatrix.h"
#include "StarfallMatrixLedEffect.h"
#include "BottomLeftCorner.h"

CRGB leds[NUM_LEDS];

#define NUM_EFFECTS 3
LedEffect* effects[NUM_EFFECTS] =
{
	new StarfallMatrixLedEffect(new BottomLeftCorner<MATRIX_W1, MATRIX_H>, leds, (MATRIX_W1 * MATRIX_H), 10, CRGB::White),
	new StarfallMatrixLedEffect(new BottomLeftCorner<MATRIX_W2, MATRIX_H>, leds+(MATRIX_W1 * MATRIX_H), (MATRIX_W2 * MATRIX_H), 12, 0xFF2400),
	new StarfallMatrixLedEffect(new BottomLeftCorner<MATRIX_W3, MATRIX_H>, leds+(MATRIX_W1 * MATRIX_H)+(MATRIX_W2 * MATRIX_H), (MATRIX_W3 * MATRIX_H), 10, CRGB::White)
};

LEDMatrix ledMatrix(effects, NUM_EFFECTS);

uint16_t brightness = MIN_BRIGHTNESS;

void setup()
{
	randomSeed(analogRead(0));

	FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
	FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
	FastLED.setBrightness(brightness);
	FastLED.clear(true);

	ledMatrix.resume();

	touch.setTimeout(300);
	touch.setStepTimeout(50);

	Serial.begin(115200);
	Serial.println(F("LEDMatrix"));
}

void loop()
{
	touch.tick();

	if (touch.hasClicks())
	{
		switch (touch.getClicks())
		{
		case 1:
			ledMatrix.resume();
			FastLED.show();
			break;
		case 2:
			ledMatrix.pause();
			FastLED.clear(true);
			break;
		case 3:
			ledMatrix.pause();
			break;
		default:
			break;
		}
	}
	if (touch.isStep())
	{
		brightness -= 5;

		if (brightness > MAX_BRIGHTNESS)
			brightness = MAX_BRIGHTNESS;

		FastLED.setBrightness(constrain(brightness, MIN_BRIGHTNESS, 255));
		FastLED.show();
	}

	if (ledMatrix.isChanged())
	{
		FastLED.show();
	}
}
