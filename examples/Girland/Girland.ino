#define LED_PIN 9   // led pin
#define BTN_PIN 10  // button pin

#define MATRIX_H 8
#define MATRIX_W 32
#define NUM_LEDS (MATRIX_H * MATRIX_W)

#define CURRENT_LIMIT 8000
#define MAX_BRIGHTNESS 300
#define MIN_BRIGHTNESS 30

#define EEPROM_ADDRESS_EFFECT 0

#include <FastLED.h>

#include <LEDLine.h>
#include <BugsLedEffect.h>
#include <GlowwormLedEffect.h>
#include <ColorsLedEffect.h>
#include <RainbowLedEffect.h>
#include <SparklesLedEffect.h>
#include <FlashLedEffect.h>
#include <FlagLedEffect.h>

#include "MatrixLedEffect.h"
#include "BottomLeftCorner.h"

#include <GyverButton.h>

GButton touch(BTN_PIN, LOW_PULL, NORM_OPEN);

CRGB leds[NUM_LEDS];

BottomLeftCorner<MATRIX_W, MATRIX_H> converter;

#define NUM_EFFECTS 8
ILedEffect* effects[NUM_EFFECTS] =
{
	new MatrixLedEffect(&converter, leds, NUM_LEDS, 8, CRGB::Blue),
	new BugsLedEffect(leds, NUM_LEDS, 20),
	new GlowwormLedEffect(leds, NUM_LEDS, 30),
	new ColorsLedEffect(leds, NUM_LEDS, 10),
	new RainbowLedEffect(leds, NUM_LEDS, 10),
	new SparklesLedEffect(leds, NUM_LEDS, 10),
	new FlashLedEffect(leds, NUM_LEDS, 1, CRGB::Orange),
	new FlagLedEffect(leds, NUM_LEDS, 4, { CRGB::White, 2, CRGB::Red, 4, CRGB::White, 2 })
};

LEDLine ledMatrix(effects, NUM_EFFECTS);

uint16_t brightness = MIN_BRIGHTNESS;

void setup()
{
	randomSeed(analogRead(0));

	FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
	FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
	FastLED.setBrightness(brightness);
	FastLED.clear(true);

	ledMatrix.loadState(EEPROM_ADDRESS_EFFECT);
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
			ledMatrix.nextState();
			break;
		case 2:
			ledMatrix.pause();
			FastLED.clear(true);
			break;
		case 3:
			ledMatrix.pause();
			ledMatrix.saveState(EEPROM_ADDRESS_EFFECT);
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
