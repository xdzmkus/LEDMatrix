#if defined(ESP8266)
#define BTN_PIN  D0 // button pin
#define ENC1_PIN D1 // encoder S1 pin
#define ENC2_PIN D2	// encoder S2 pin
#define UNPINNED_ANALOG_PIN A0 // not connected analog pin
#elif defined(ESP32)
#define BTN_PIN  5  // button pin
#define ENC1_PIN 19 // encoder S1 pin
#define ENC2_PIN 18	// encoder S2 pin
#define UNPINNED_ANALOG_PIN 35 // not connected analog pin
#endif

/********** Touch button module ***********/
#include <ArduinoDebounceButton.h>
ArduinoDebounceButton btn(BTN_PIN, BUTTON_CONNECTED::GND, BUTTON_NORMAL::OPEN);

#include <EventsQueue.hpp>
EventsQueue<BUTTON_EVENT, 10> queue;

#include <Ticker.h>
Ticker builtinLedTicker;
Ticker btnTicker;

/********************************************/
void handleButtonEvent(const DebounceButton* button, BUTTON_EVENT eventType)
{
	queue.push(eventType);
}

void check_button()
{
	btn.check();
}

void processBtn()
{
	bool processBtnEvent;
	BUTTON_EVENT btnEvent;

	do
	{
		noInterrupts();

		processBtnEvent = queue.length();

		if (processBtnEvent)
		{
			btnEvent = queue.pop();
		}

		interrupts();

		if (processBtnEvent)
		{
			switch (btnEvent)
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
				break;
			}
		}

	} while (processBtnEvent);
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

	builtinLedTicker.attach_ms(500, blinkLED);  // Blink led while setup

	connect_WiFi();
	setup_MQTT();

	btn.setEventHandler(handleButtonEvent);

	btnTicker.attach_ms(btn.delayDebounce >> 1, check_button);

	builtinLedTicker.detach();
	digitalWrite(LED_BUILTIN, HIGH);    // Turn the LED off by making the voltage HIGH

	turnOnLeds();
}

void loop()
{
	processBtn();

	processMQTT();

	processLED();
}
