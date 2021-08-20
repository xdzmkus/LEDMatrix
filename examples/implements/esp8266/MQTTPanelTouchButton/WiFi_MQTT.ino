#if true //__has_include("my_data_sensitive.h")
#include "my_data_sensitive.h"
#else
#pragma message "default credentials are used"

#define WLAN_SSID           "AP wifi name"
#define WLAN_PASS           "and password"
#define WLAN_HOSTNAME       "set hostname"

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

/*********** WiFi ***************************/
#include <WiFiManager.h>

/*********** MQTT Server ********************/
#include <WiFiClient.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;  // use WiFiClientSecure for SSL

// Setup the MQTT client class by passing in the WiFi client
Adafruit_MQTT_Client    mqtt(&client, MQTT_SERVER, MQTT_SERVERPORT, MQTT_USERNAME, MQTT_KEY);

// Setup MQTT topics
Adafruit_MQTT_Publish   garlandState(&mqtt, MQTT_TOPIC_PUB);
Adafruit_MQTT_Subscribe garlandEffect(&mqtt, MQTT_TOPIC_SUB1, MQTT_QOS_1);
Adafruit_MQTT_Subscribe garlandAction(&mqtt, MQTT_TOPIC_SUB2, MQTT_QOS_1);
Adafruit_MQTT_Subscribe garlandRunningString(&mqtt, MQTT_TOPIC_SUB3, MQTT_QOS_1);

#include <Ticker.h>
Ticker mqttTicker; 

volatile boolean f_pingMQTT = false;

volatile boolean f_publishState = true;

void pingMQTT_callback()
{
	f_pingMQTT = true;
}

void setAction_callback(uint32_t x)
{
	Serial.print(F("new action requested: "));
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
		break;
	}
}

void setEffect_callback(char* data, uint16_t len)
{
	Serial.print(F("new effect requested: "));
	Serial.println(data);

	setEffect(data);
}

void newRunningString_callback(char* data, uint16_t len)
{
	Serial.print(F("new running string received: "));
	Serial.println(data);

	setRunningString(data, len);
}

void configure_WiFi()
{
	WiFiManager wm;

	wm.setConfigPortalTimeout(180);

	wm.setConfigPortalBlocking(true);
	if (wm.startConfigPortal(WLAN_SSID, WLAN_PASS))
	{
		Serial.println(F("WiFi Reconfigured! Rebooting..."));
		delay(5000);
		ESP.restart();
	}
}

void connect_WiFi()
{
	WiFiManager wm;

	WiFi.mode(WIFI_STA);                // Set the ESP8266 to be a WiFi-client
	WiFi.hostname(WLAN_HOSTNAME);

	Serial.println(WLAN_HOSTNAME);

	if (!wm.autoConnect(WLAN_SSID, WLAN_PASS))
	{
		Serial.println(F("Connection Failed! Rebooting..."));
		delay(5000);
		ESP.restart();
	}

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

	mqttTicker.attach(60.0, pingMQTT_callback);
}

bool keepAliveMQTT()
{
	if (!mqtt.connected())
	{
		Serial.println(F("Connecting to MQTT... "));

		uint8_t ret = mqtt.connect();

		if (ret != 0)
		{
			Serial.println(mqtt.connectErrorString(ret));
			Serial.println(F("Retry MQTT connection ..."));
			mqtt.disconnect();
			return false;
		}
		else
		{
			Serial.println(F("MQTT Connected!"));
			return true;
		}
	}

	if (f_pingMQTT)
	{
		f_pingMQTT = false;

		Serial.println(F("Ping MQTT... "));

		return mqtt.ping();
	}

	return true;
}

void processMQTT()
{
	if (!keepAliveMQTT()) return;

	mqtt.processPackets(10);

	if (f_publishState)
	{
		auto currentEffect = getEffect();
		if (currentEffect == nullptr) currentEffect = "OFF";

		Serial.print(F("Publish message: "));
		Serial.println(currentEffect);

		if (garlandState.publish(currentEffect))
		{
			f_publishState = false;
		}
		else
		{
			Serial.println(F("Publish Message Failed"));
		}
	}
}

void publishState()
{
	f_publishState = true;
}
