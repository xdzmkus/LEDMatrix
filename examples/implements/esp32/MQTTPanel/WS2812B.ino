/*********** WS2812B leds *******************/
#define LED_PIN 16  // leds pin

#include <FastLED.h>
#define MATRIX_H 8
#define MATRIX_W 32
#define CURRENT_LIMIT 8000

#define EFFECT_DURATION_SEC 45

volatile uint8_t brightness = 10;

CRGB leds[(MATRIX_H * MATRIX_W)];

/*********** LED Matrix Effects *************/
#include "LEDMatrixEx.h"
#include "ZigZagFromTopLeftToBottomAndRight.h"
ZigZagFromTopLeftToBottomAndRight matrix(leds, MATRIX_W, MATRIX_H);
LEDMatrixEx ledMatrix(&matrix);

#include <Ticker.h>
Ticker effectsTicker;

TaskHandle_t xLEDHandle = NULL;
SemaphoreHandle_t xLEDSemaphore = NULL;

const TickType_t xMaxBlockTime = pdMS_TO_TICKS(100);

void setup_LED()
{
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, (MATRIX_H * MATRIX_W)).setCorrection(TypicalSMD5050);
    FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
    FastLED.setBrightness(brightness);
    FastLED.clear(true);

	xLEDSemaphore = xSemaphoreCreateMutex();

	if (xLEDSemaphore == NULL)
	{
		Serial.println(F("There was insufficient FreeRTOS heap available for the LED semaphore to be created successfully."));
		return;
	}

	/* Create the task, storing the handle. */
	BaseType_t xReturned = xTaskCreatePinnedToCore(
		vTaskLED,        /* Function that implements the task. */
		"processLED",    /* Text name for the task. */
		8192,            /* Stack size in words, not bytes. */
		NULL,            /* Parameter passed into the task. */
		1,				 /* Priority at which the task is created. */
		&xLEDHandle,     /* Used to pass out the created task's handle. */
		1);

	if (xReturned != pdPASS)
	{
		Serial.println(F("Error creating LED task!"));
		return;
	}
}

void vTaskLED(void* pvParameters)
{
	turnOnLeds();

	uint32_t ulNotifiedValue;

	while (true)
	{
		/* Wait to be notified of brightness. */
		BaseType_t xResult = xTaskNotifyWait(
			pdFALSE,			/* Don't clear bits on entry. */
			ULONG_MAX,			/* Clear all bits on exit. */
			&ulNotifiedValue,	/* Stores the notified value. */
			0);

		if (xResult == pdPASS)
		{
			/* A notification was received. */
			if (ulNotifiedValue < 256)
			{
				FastLED.setBrightness(static_cast<uint8_t>(ulNotifiedValue));
			}
		}

		if (xSemaphoreTake(xLEDSemaphore, 0) == pdTRUE)
		{
			if (ledMatrix.refresh())
			{
				FastLED.show();
			}
			/* We have finished accessing the shared resource.  Release the	semaphore. */
			xSemaphoreGive(xLEDSemaphore);
		}
	}
}

void turnOnLeds()
{
	if (xSemaphoreTake(xLEDSemaphore, xMaxBlockTime) == pdTRUE)
	{
		effectsTicker.detach();

		ledMatrix.setEffectByIdx(0);
		ledMatrix.turnOn();

		effectsTicker.attach(EFFECT_DURATION_SEC, changeEffect);

		xSemaphoreGive(xLEDSemaphore);
	}

	publishState();
}

void turnOffLeds()
{
	if (xSemaphoreTake(xLEDSemaphore, xMaxBlockTime) == pdTRUE)
	{
		effectsTicker.detach();

		ledMatrix.turnOff();

		FastLED.clear(true);

		xSemaphoreGive(xLEDSemaphore);
	}

	publishState();
}

void changeEffect()
{
	if (xSemaphoreTake(xLEDSemaphore, xMaxBlockTime) == pdTRUE)
	{
		effectsTicker.detach();

		ledMatrix.setNextEffect();

		effectsTicker.attach(EFFECT_DURATION_SEC, changeEffect);

		xSemaphoreGive(xLEDSemaphore);
	}

	publishState();
}

void setEffect(const char* data)
{
	if (xSemaphoreTake(xLEDSemaphore, xMaxBlockTime) == pdTRUE)
	{
		if (ledMatrix.setEffectByName(data))
		{
			effectsTicker.detach();
		}
		xSemaphoreGive(xLEDSemaphore);
	}

	publishState();
}

const char* getEffect()
{
	const char* effectName = nullptr;

	if (xSemaphoreTake(xLEDSemaphore, xMaxBlockTime) == pdTRUE)
	{
		effectName = !ledMatrix.isOn() ? nullptr : ledMatrix.getEffectName();

		xSemaphoreGive(xLEDSemaphore);
	}

	return effectName;
}

void setRunningString(char* data, uint16_t len)
{
	if (xSemaphoreTake(xLEDSemaphore, xMaxBlockTime) == pdTRUE)
	{
		ledMatrix.setRunningString(data, len);

		xSemaphoreGive(xLEDSemaphore);
	}
}

void adjustBrightness(int8_t delta)
{
	brightness += delta;
	xTaskNotify(xLEDHandle, brightness, eSetValueWithOverwrite);
}

void adjustBrightnessFromISR(int8_t delta)
{
	brightness += delta;

	BaseType_t xHigherPriorityTaskWoken;

	/* xHigherPriorityTaskWoken must be initialised to pdFALSE.  If calling
	xTaskNotifyFromISR() unblocks the handling task, and the priority of
	the handling task is higher than the priority of the currently running task,
	then xHigherPriorityTaskWoken will automatically get set to pdTRUE. */
	xHigherPriorityTaskWoken = pdFALSE;

	/* Unblock the handling task so the task can perform any processing necessitated
	by the interrupt.  xHandlingTask is the task's handle, which was obtained
	when the task was created.  The handling task's 0th notification value
	is bitwise ORed with the interrupt status - ensuring bits that are already
	set are not overwritten. */
	xTaskNotifyFromISR(xLEDHandle, brightness, eSetValueWithOverwrite, &xHigherPriorityTaskWoken);

	/* Force a context switch. */
	portYIELD_FROM_ISR();
}
