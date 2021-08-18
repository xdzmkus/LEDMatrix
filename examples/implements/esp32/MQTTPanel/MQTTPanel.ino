//#define LED_BUILTIN 2

#define BTN_PIN 5  // button pin
#define ENC1_PIN 19 // encoder S1 pin
#define ENC2_PIN 18	// encoder S2 pin
#define UNPINNED_ANALOG_PIN 35 // not connected analog pin

/********** Encoder button module ***********/
#include <ArduinoDebounceButton.h>
ArduinoDebounceButton btn(BTN_PIN, BUTTON_CONNECTED::GND, BUTTON_NORMAL::OPEN);

#include <ArduinoRotaryEncoder.h>
ArduinoRotaryEncoder encoder(ENC2_PIN, ENC1_PIN);

#include <Ticker.h>
Ticker ledTicker;

/********************************************/

IRAM_ATTR void catchEncoderTicks()
{
    encoder.catchTicks();
}

void handleEncoderEvent(const RotaryEncoder* enc, ENCODER_EVENT eventType)
{
    switch (eventType)
    {
    case ENCODER_EVENT::LEFT:
        adjustBrightnessFromISR(-5);
        break;
    case ENCODER_EVENT::RIGHT:
        adjustBrightnessFromISR(5);
        break;
    default:
        break;
    }
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
        break;
    }
}

void blinkLED()
{
    //toggle LED state
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

void setup_Button()
{
    btn.initPin();

    btn.setEventHandler(handleButtonEvent);
}

void setup_RotaryEncoder()
{
    encoder.initPins();
    encoder.setEventHandler(handleEncoderEvent);

    attachInterrupt(digitalPinToInterrupt(ENC1_PIN), catchEncoderTicks, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENC2_PIN), catchEncoderTicks, CHANGE);
}

void setup()
{
    Serial.begin(115200);

    randomSeed(analogRead(UNPINNED_ANALOG_PIN));

    pinMode(LED_BUILTIN, OUTPUT);        // Initialize the BUILTIN_LED pin as an output
    digitalWrite(LED_BUILTIN, LOW);      // Turn the LED on by making the voltage LOW

    ledTicker.attach_ms(500, blinkLED);  // Blink led while setup

    setup_Button();

    connect_WiFi();

    setup_MQTT();

    setup_LED();

    setup_RotaryEncoder();

    ledTicker.detach();
    digitalWrite(LED_BUILTIN, HIGH);    // Turn the LED off by making the voltage HIGH
}

void loop()
{
    btn.check();
}
