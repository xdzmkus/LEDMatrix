#if __has_include("my_data_sensitive.h")
#include "my_data_sensitive.h"
#else

#define WLAN_SSID           "your wifi name"
#define WLAN_PASS           "and password"
#define WLAN_HOSTNAME       "connect with hostname"

#define MQTT_SERVER         "127.0.0.1"
#define MQTT_SERVERPORT     1883
#define MQTT_USERNAME       "your mqtt username"
#define MQTT_KEY            "and password"

#define MQTT_TOPIC_PUB      MQTT_USERNAME"/get/state"
#define MQTT_TOPIC_SUB1     MQTT_USERNAME"/set/effect"
#define MQTT_TOPIC_SUB2     MQTT_USERNAME"/set/action"
#define MQTT_TOPIC_SUB3     MQTT_USERNAME"/set/runningString"

#define ON_CODE             6735
#define OFF_CODE            2344
#define NEXT_CODE           2747

#endif

#define LED_PIN D3    // D1 leds pin (connected to D5 on my NodeMCU 1.0 !!!)
#define BTN_PIN 16    // D0 button pin

#define UNPINNED_ANALOG_PIN A0 // not connected analog pin

/*********** WS2812B leds *******************/
#include <FastLED.h>
#define MATRIX_H 11
#define MATRIX_W 36
#define CURRENT_LIMIT 16000
#define MAX_BRIGHTNESS 255
#define MIN_BRIGHTNESS 120

#define EFFECT_DURATION_SEC 45

uint16_t brightness = MAX_BRIGHTNESS / 3;

CRGB leds[(MATRIX_H * MATRIX_W)];

/*********** LED Matrix Effects *************/
#include "LEDMatrixEx.h"
#include "ZigZagFromBottomRightToUpAndLeft.h"
ZigZagFromBottomRightToUpAndLeft matrix(leds, MATRIX_W, MATRIX_H);
LEDMatrixEx ledMatrix(&matrix);

#include <Ticker.h>
Ticker tickerEffects;

volatile boolean f_publishState = true;

/********** Touch button module *************/
#include <ArduinoDebounceButton.h>
ArduinoDebounceButton btn(BTN_PIN, BUTTON_CONNECTED::VCC, BUTTON_NORMAL::OPEN);

/*********** WiFi Client ********************/
#include <ESP8266WiFi.h>

/*********** MQTT Server ********************/
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;  // use WiFiClientSecure for SSL

// Setup the MQTT client class by passing in the WiFi client
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_SERVERPORT, MQTT_USERNAME, MQTT_KEY);

Adafruit_MQTT_Publish garlandState = Adafruit_MQTT_Publish(&mqtt, MQTT_TOPIC_PUB);

Adafruit_MQTT_Subscribe garlandEffect = Adafruit_MQTT_Subscribe(&mqtt, MQTT_TOPIC_SUB1, MQTT_QOS_1);
Adafruit_MQTT_Subscribe garlandAction = Adafruit_MQTT_Subscribe(&mqtt, MQTT_TOPIC_SUB2, MQTT_QOS_1);
Adafruit_MQTT_Subscribe garlandRunningString = Adafruit_MQTT_Subscribe(&mqtt, MQTT_TOPIC_SUB3, MQTT_QOS_1);

/********************************************/
void turnOnLeds()
{
    tickerEffects.attach(EFFECT_DURATION_SEC, changeEffect);

    ledMatrix.setEffectByIdx(0);
    ledMatrix.turnOn();

    f_publishState = true;
}

void turnOffLeds()
{
    tickerEffects.detach();

    ledMatrix.turnOff();

    f_publishState = true;

    FastLED.clear(true);
}

void changeEffect()
{
    tickerEffects.detach();

    ledMatrix.setNextEffect();

    tickerEffects.attach(EFFECT_DURATION_SEC, changeEffect);

    f_publishState = true;
}

void adjustBrightness()
{
    brightness += MIN_BRIGHTNESS;
    if (brightness > MAX_BRIGHTNESS + MIN_BRIGHTNESS * 2)
    {
        brightness = 0;
    }
    FastLED.setBrightness(constrain(brightness, MIN_BRIGHTNESS, MAX_BRIGHTNESS));
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
        adjustBrightness();
        break;
    case BUTTON_EVENT::LongPressed:
        turnOffLeds();
        break;
    default:
        break;
    }
}
void setAction_callback(uint32_t x)
{
    Serial.print(F("on/off with code: "));
    Serial.println(x);

    switch (x)
    {
    case ON_CODE:
        turnOnLeds();
        break;
    case OFF_CODE:
        turnOffLeds();
        break;
    case NEXT_CODE:
        changeEffect();
        break;
    default:
        f_publishState = true;
        break;
    }
}

void setEffect_callback(char* data, uint16_t len)
{
    Serial.print(F("new effect requested: "));
    Serial.println(data);

    if (ledMatrix.setEffectByName(data))
    {
        tickerEffects.detach();
    }
}

void newRunningString_callback(char* data, uint16_t len)
{
    Serial.print(F("new running string received: "));
    Serial.println(data);

    ledMatrix.setRunningString(data, len);
}

void publishState()
{
    auto currentEffect = (ledMatrix.getEffectName() == nullptr || !ledMatrix.isOn()) ? "OFF" : ledMatrix.getEffectName();

    Serial.print(F("Publish message: "));
    Serial.println(currentEffect);

    if (!garlandState.publish(currentEffect))
    {
        Serial.println(F("Publish Message Failed"));
    }
}

void setup()
{
    randomSeed(analogRead(UNPINNED_ANALOG_PIN));

    pinMode(LED_BUILTIN, OUTPUT);       // Initialize the LED_BUILTIN pin as an output

#if defined(ESP8266) && (BTN_PIN == 16)
    pinMode(BTN_PIN, INPUT_PULLDOWN_16);
#else
    btn.initPin();
#endif

    btn.setEventHandler(handleButtonEvent);

    Serial.begin(115200);

    setup_LED();

    setup_WiFi();

    setup_MQTT();

    turnOnLeds();
}

void setup_WiFi()
{
    Serial.println();
    Serial.print(F("Connecting to "));
    Serial.println(WLAN_SSID);

    WiFi.mode(WIFI_STA);                  // Set the ESP8266 to be a WiFi-client
    WiFi.hostname(WLAN_HOSTNAME);
    WiFi.begin(WLAN_SSID, WLAN_PASS);

    for (uint8_t s = 0; (WiFi.status() != WL_CONNECTED) && (s < 20); s++)
    {
        digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level but actually the LED is on; this is because it is active low on the ESP-01)
        delay(500);
        Serial.print(".");
        digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
        delay(200);
    }
    if (!WiFi.isConnected())
    {
        Serial.println("Connection Failed! Rebooting...");
        ESP.restart();
    }

    Serial.println("");
    Serial.println(F("WiFi connected"));
    Serial.print(F("IP address: "));
    Serial.println(WiFi.localIP());
}

void setup_MQTT()
{
    garlandEffect.setCallback(setEffect_callback);
    mqtt.subscribe(&garlandEffect);

    garlandAction.setCallback(setAction_callback);
    mqtt.subscribe(&garlandAction);

    garlandRunningString.setCallback(newRunningString_callback);
    mqtt.subscribe(&garlandRunningString);
}

void setup_LED()
{
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, (MATRIX_H * MATRIX_W));
    FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
    FastLED.setBrightness(constrain(brightness, MIN_BRIGHTNESS, MAX_BRIGHTNESS));
    FastLED.clear(true);
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void mqtt_loop()
{
    if (!mqtt.connected())
    {
        uint8_t ret(mqtt.connect());

        Serial.println(F("Connecting to MQTT... "));
        if (ret != 0)
        {
            Serial.println(mqtt.connectErrorString(ret));
            Serial.println(F("Retry MQTT connection ..."));
            mqtt.disconnect();
            return;
        }
        else
        {
            Serial.println(F("MQTT Connected!"));
        }
    }

    mqtt.processPackets(10);
}

void loop()
{
    btn.check();

    mqtt_loop();

    if (f_publishState)
    {
        f_publishState = false;

        publishState();
    }

    if (ledMatrix.refresh())
    {
        FastLED.show();
    }
}
