#if true // && __has_include("my_data_sensitive.h")
#include "my_data_sensitive.h"
#else

#define WLAN_SSID		    "your wifi name"
#define WLAN_PASS		    "and password"
#define WLAN_HOSTNAME	    "connect with hostname"

#define MQTT_SERVER		    "127.0.0.1"
#define MQTT_SERVERPORT	    1883
#define MQTT_USERNAME	    "your mqtt username"
#define MQTT_KEY		    "and password"

#define MQTT_TOPIC_PUB      MQTT_USERNAME"/current/state"
#define MQTT_TOPIC_SUB1     MQTT_USERNAME"/new/effect"
#define MQTT_TOPIC_SUB2     MQTT_USERNAME"/new/onoff"

#define ON_CODE		        6735
#define OFF_CODE	        2344
#define PAUSE_CODE	        2747

#endif

#define LED_PIN D3    // D1 leds pin (connected to D5 on my NodeMCU 1.0 !!!)

/*********** WS2812B leds *******************/
#include <FastLED.h>
#define MATRIX_H 8
#define MATRIX_W 32
#define NUM_LEDS (MATRIX_H * MATRIX_W)
#define CURRENT_LIMIT 8000
#define MAX_BRIGHTNESS 255
#define MIN_BRIGHTNESS 20

uint16_t brightness = MIN_BRIGHTNESS;

CRGB leds[NUM_LEDS];

/*********** LED Matrix Effects *************/
#include "LEDMatrix.h"
LEDMatrix ledMatrix(leds, NUM_LEDS);

#include <Ticker.h>
#define EFFECT_DURATION_SEC 45
Ticker tickerEffects;
volatile boolean f_publishState = true;

/*********** WiFi Client ********************/
#include <ESP8266WiFi.h>

/*********** MQTT Server ********************/
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
//WiFiClientSecure client;  // use WiFiClientSecure for SSL

// Setup the MQTT client class by passing in the WiFi client
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_SERVERPORT, MQTT_USERNAME, MQTT_KEY);
//Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_SERVERPORT);

Adafruit_MQTT_Publish girlandState = Adafruit_MQTT_Publish(&mqtt, MQTT_TOPIC_PUB);
Adafruit_MQTT_Subscribe girlandEffect = Adafruit_MQTT_Subscribe(&mqtt, MQTT_TOPIC_SUB1, MQTT_QOS_1);
Adafruit_MQTT_Subscribe girlandOnOff = Adafruit_MQTT_Subscribe(&mqtt, MQTT_TOPIC_SUB2, MQTT_QOS_1);

/********************************************/

void handleTimer()
{
    if (ledMatrix.isRunning()) ledMatrix.setNextEffect();
    f_publishState = true;
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

    ledMatrix.resume();

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
    mqtt_loop();

    if (f_publishState)
    {
        f_publishState = false;

        publishState();
    }

    if (ledMatrix.paint())
    {
        FastLED.show();
    }
}
