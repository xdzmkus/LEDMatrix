#if defined(ESP8266)
#define LED_PIN D5  // leds pin
#define BTN_PIN D0  // 16(pulldown) - button pin
#define ENC1_PIN D1 // encoder S1 pin
#define ENC2_PIN D2	// encoder S2 pin

#else
#define LED_PIN  9  // leds pin
#define BTN_PIN  4  // button pin
#define ENC1_PIN 3  // encoder S1 pin
#define ENC2_PIN 2	// encoder S2 pin

#endif

#define UNPINNED_ANALOG_PIN A0 // not connected analog pin

#include <ArduinoDebounceButton.h>
ArduinoDebounceButton btn(BTN_PIN, BUTTON_CONNECTED::GND, BUTTON_NORMAL::OPEN);

#include <ArduinoRotaryEncoder.h>
ArduinoRotaryEncoder encoder(ENC2_PIN, ENC1_PIN);

#include <EventsQueue.hpp>
EventsQueue<ENCODER_EVENT, 8> queue;

#include <FastLED.h>
#define MATRIX_H 8

#if defined(ESP8266)
#define MATRIX_W 32
#else
#define MATRIX_W 16
#endif

#define CURRENT_LIMIT 8000

uint8_t brightness = 100;

CRGB leds[(MATRIX_H * MATRIX_W)];

#include "LEDMatrix.h"
#include "ZigZagFromTopLeftToBottomAndRight.h"
ZigZagFromTopLeftToBottomAndRight matrix(leds, MATRIX_W, MATRIX_H);
LEDMatrix ledMatrix(&matrix);

#if defined(ESP8266)
IRAM_ATTR
#endif
void catchEncoderTicks()
{
	encoder.catchTicks();
}

void handleEncoderEvent(const RotaryEncoder* enc, ENCODER_EVENT eventType)
{
	queue.push(eventType);
}

void processEncoder()
{
	bool processEncEvent;
	ENCODER_EVENT encEvent;

	do
	{
		noInterrupts();

		processEncEvent = queue.length();

		if (processEncEvent)
		{
			encEvent = queue.pop();
		}

		interrupts();

		if (processEncEvent)
		{
			switch (encEvent)
			{
			case ENCODER_EVENT::LEFT:
				adjustBrightness(-5);
				break;
			case ENCODER_EVENT::RIGHT:
				adjustBrightness(5);
				break;
			default:
				break;
			}
		}
	} while (processEncEvent);
}

void turnOnLeds()
{
	ledMatrix.setEffectByIdx(0);
	ledMatrix.turnOn();
}

void changeEffect()
{
	ledMatrix.setNextEffect();
}

void turnOffLeds()
{
	ledMatrix.turnOff();
	FastLED.clear(true);
}

void adjustBrightness(int8_t delta)
{
	brightness += delta;
	FastLED.setBrightness(brightness);
}

void handleButtonEvent(const DebounceButton* button, BUTTON_EVENT eventType)
{
	switch (eventType)
	{
	case BUTTON_EVENT::Clicked:
		changeEffect();
		break;
	case BUTTON_EVENT::DoubleClicked:
		turnOnLeds();
		break;
	case BUTTON_EVENT::LongPressed:
		turnOffLeds();
		break;
	default:
		break;
	}
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
	//Serial.begin(115200);

	randomSeed(analogRead(UNPINNED_ANALOG_PIN));

	setupLED();

	btn.initPin();

	btn.setEventHandler(handleButtonEvent);

	encoder.initPins();

	encoder.setEventHandler(handleEncoderEvent);

	attachInterrupt(digitalPinToInterrupt(ENC1_PIN), catchEncoderTicks, CHANGE);
	attachInterrupt(digitalPinToInterrupt(ENC2_PIN), catchEncoderTicks, CHANGE);

	turnOnLeds();
}

void loop()
{
	btn.check();

	processEncoder();

	if (ledMatrix.refresh())
	{
		FastLED.show();
	}
}
