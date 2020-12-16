#include "data_sensitive.h"

#define LED_PIN D1    // D1 leds pin (connected to D5 on my NodeMCU 1.0 !!!)
#define BTN_PIN D6    // D6 button pin

/*********** WiFi Access Point **************/
#include <ESP8266WiFi.h>

#define WLAN_SSID       _WLAN_SSID_
#define WLAN_PASS       _WLAN_PASS_
#define WLAN_HOSTNAME   _WLAN_HOSTNAME_

/*********** MQTT Server ********************/
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

#define MQTT_SERVER      _MQTT_SERVER_
#define MQTT_SERVERPORT  _MQTT_SERVERPORT_
#define MQTT_USERNAME    _MQTT_USERNAME_
#define MQTT_KEY         _MQTT_KEY_

#define MQTT_TOPIC_PUB MQTT_USERNAME"/current/state"
#define MQTT_TOPIC_SUB1 MQTT_USERNAME"/new/effect"
#define MQTT_TOPIC_SUB2 MQTT_USERNAME"/new/onoff"

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
//WiFiClientSecure client;  // use WiFiClientSecure for SSL

// Setup the MQTT client class by passing in the WiFi client
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_SERVERPORT, MQTT_USERNAME, MQTT_KEY);
//Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_SERVERPORT);

Adafruit_MQTT_Publish girlandState = Adafruit_MQTT_Publish(&mqtt, MQTT_TOPIC_PUB);
Adafruit_MQTT_Subscribe girlandEffect = Adafruit_MQTT_Subscribe(&mqtt, MQTT_TOPIC_SUB1, MQTT_QOS_1);
Adafruit_MQTT_Subscribe girlandOnOff = Adafruit_MQTT_Subscribe(&mqtt, MQTT_TOPIC_SUB2, MQTT_QOS_1);

/********** Touch button module *************/
#include <Denel_Button.h>
Denel_Button btn(BTN_PIN, BUTTON_CONNECTED::VCC, BUTTON_NORMAL::OPEN);

/*********** WS2812B leds *******************/
#include <FastLED.h>
#define MATRIX_H 11
#define MATRIX_W 36
#define NUM_LEDS (MATRIX_H * MATRIX_W)
#define CURRENT_LIMIT 16000
#define MAX_BRIGHTNESS 255
#define MIN_BRIGHTNESS 20

uint16_t brightness = MAX_BRIGHTNESS / 2;

CRGB leds[NUM_LEDS];

#include "LEDMatrix.h"
LEDMatrix ledMatrix(leds, NUM_LEDS);

#include <Ticker.h>
#define EFFECT_DURATION_SEC 45
Ticker tickerEffects;
volatile boolean f_publishState = true;

void handleTimer()
{
    if (ledMatrix.isRunning()) ledMatrix.setNextEffect();
    f_publishState = true;
}

void handleButtonEvent(const Denel_Button* button, BUTTON_EVENT eventType)
{
    switch (eventType)
    {
    case BUTTON_EVENT::Clicked:
        f_publishState = true;
        break;
    case BUTTON_EVENT::DoubleClicked:
        ledMatrix.setNextEffect();
        Serial.print(F("NEXT: ")); Serial.println(ledMatrix.getEffectName());
        break;
    case BUTTON_EVENT::RepeatClicked:
        brightness += MIN_BRIGHTNESS;
        if (brightness > MAX_BRIGHTNESS + MIN_BRIGHTNESS * 2) brightness = 0;
        FastLED.setBrightness(constrain(brightness, MIN_BRIGHTNESS, MAX_BRIGHTNESS));
        Serial.print(F("BRIGHTNESS: ")); Serial.println(brightness);
        break;
    case BUTTON_EVENT::LongPressed:
        ledMatrix.pause();
        FastLED.clear(true);
        Serial.println(F("OFF"));
        break;
    default:
        break;
    }
}

void onoff_callback(uint32_t x)
{
    Serial.print(F("on/off with code: "));
    Serial.println(x);

    switch (x)
    {
    case OFF_CODE:
        FastLED.clear(true);
        ledMatrix.pause();
        break;
    case PAUSE_CODE:
        ledMatrix.pause();
        break;
    case ON_CODE:
        ledMatrix.resume();
        break;
    default:
        f_publishState = true;
        break;
    }
}

void newEffect_callback(char* data, uint16_t len)
{
    Serial.print(F("new effect requested: "));
    Serial.println(data);

    ledMatrix.setEffectByName(data);
}

void publishState()
{
    auto currentEffect = (ledMatrix.getEffectName() == nullptr || !ledMatrix.isRunning()) ? "OFF" : ledMatrix.getEffectName();

    Serial.print(F("Publish message: ")); Serial.println(currentEffect);

    if (!girlandState.publish(currentEffect))
    {
        Serial.println(F("Publish Message Failed"));
    }
}

void setup()
{
    pinMode(BUILTIN_LED, OUTPUT);       // Initialize the BUILTIN_LED pin as an output

    Serial.begin(115200);

    setup_LED();

    setup_WiFi();

    setup_MQTT();

    tickerEffects.attach(EFFECT_DURATION_SEC, handleTimer);

    btn.setEventHandler(handleButtonEvent);

    ledMatrix.setNextEffect();
}

void setup_WiFi()
{
    Serial.println();
    Serial.print(F("Connecting to ")); Serial.println(WLAN_SSID);

    WiFi.mode(WIFI_STA);                  // Set the ESP8266 to be a WiFi-client
    WiFi.hostname(WLAN_HOSTNAME);
    WiFi.begin(WLAN_SSID, WLAN_PASS);

    while (WiFi.status() != WL_CONNECTED)
    {
        digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level but actually the LED is on; this is because it is active low on the ESP-01)
        delay(500);
        Serial.print(".");
        digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
        delay(200);
    }

    Serial.println("");
    Serial.println(F("WiFi connected"));
    Serial.print(F("IP address: "));
    Serial.println(WiFi.localIP());
}

void setup_MQTT()
{
    girlandEffect.setCallback(newEffect_callback);
    mqtt.subscribe(&girlandEffect);

    girlandOnOff.setCallback(onoff_callback);
    mqtt.subscribe(&girlandOnOff);
}

void setup_LED()
{
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
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

    if (f_publishState)
    {
        f_publishState = false;

        publishState();
    }

    if (ledMatrix.paint())
    {
        FastLED.show();
    }

    mqtt_loop();
}