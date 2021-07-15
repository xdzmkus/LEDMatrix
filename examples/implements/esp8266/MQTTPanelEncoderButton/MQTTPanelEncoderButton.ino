
#define BTN_PIN D0  // 16(pulldown) - button pin
#define ENC1_PIN D1 // encoder S1 pin
#define ENC2_PIN D2	// encoder S2 pin

#define UNPINNED_ANALOG_PIN A0 // not connected analog pin

/********** Encoder button module ***********/
#include <ArduinoDebounceButton.h>
ArduinoDebounceButton btn(BTN_PIN, BUTTON_CONNECTED::GND, BUTTON_NORMAL::OPEN);

#include <ArduinoRotaryEncoder.h>
ArduinoRotaryEncoder encoder(ENC2_PIN, ENC1_PIN);

#include <EventsQueue.hpp>
EventsQueue<ENCODER_EVENT, 10> queue;

#include <Ticker.h>
Ticker ledTicker;

/********************************************/

IRAM_ATTR void catchEncoderTicks()
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
	case BUTTON_EVENT::RepeatClicked:
		adjustBrightness(-10);
		break;
	case BUTTON_EVENT::LongPressed:
		turnOffLeds();
		break;
	default:
		return;
	}

	publishState();
}

void blinkLED()
{
	//toggle LED state
	digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

void setup()
{
	Serial.begin(115200);

	randomSeed(analogRead(UNPINNED_ANALOG_PIN));

	setup_LED();

	pinMode(LED_BUILTIN, OUTPUT);        // Initialize the BUILTIN_LED pin as an output
	digitalWrite(LED_BUILTIN, LOW);      // Turn the LED on by making the voltage LOW

	btn.initPin();

	delay(5000);

	ledTicker.attach_ms(500, blinkLED);  // Blink led while setup

	setup_WiFi();
	setup_MQTT();

	encoder.initPins();
	encoder.setEventHandler(handleEncoderEvent);

	attachInterrupt(digitalPinToInterrupt(ENC1_PIN), catchEncoderTicks, CHANGE);
	attachInterrupt(digitalPinToInterrupt(ENC2_PIN), catchEncoderTicks, CHANGE);

	btn.setEventHandler(handleButtonEvent);

	ledTicker.detach();
	digitalWrite(LED_BUILTIN, HIGH);    // Turn the LED off by making the voltage HIGH

	turnOnLeds();
}

void loop()
{
	btn.check();

	processEncoder();

	processMQTT();

	processLED();
}
