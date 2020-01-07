/*
* LEDMatrix.cpp
*/

#include <EEPROM.h>

#include "LEDMatrix.h"
#include "MatrixEffect.h"

LEDMatrix::LEDMatrix(CRGB leds[], uint8_t width, uint8_t height)
	: LEDStrip(leds, width*height), width(width), height(height)
{
}

// default destructor
LEDMatrix::~LEDMatrix()
{
}

void LEDMatrix::save(int memIdx)
{
#if defined(ESP32) || defined(ESP8266)
	EEPROM.write(memIdx, static_cast<uint8_t>(matrixState));
	EEPROM.commit();
#else
	EEPROM.update(memIdx, static_cast<uint8_t>(matrixState));
#endif
}

void LEDMatrix::load(int memIdx)
{
	switch (static_cast<MATRIX_STATE>(EEPROM.read(memIdx)))
	{
	case BUGS:
		matrixState = BUGS;
		turnBugs();
		break;
	case GLOWWORM:
		matrixState = GLOWWORM;
		turnGlowworm();
		break;
	case COLORS:
		matrixState = COLORS;
		turnColors();
		break;
	case RAINBOW:
		matrixState = RAINBOW;
		turnRainbow();
		break;
	case SPARKLES:
		matrixState = SPARKLES;
		turnSparkles();
		break;
	case FLAME:
		matrixState = FLAME;
		turnFlame();
		break;
	case FLASHES:
		matrixState = FLASHES;
		turnFlashes();
		break;
	case ON:
		matrixState = ON;
		turnON();
		break;
	case MATRIX:
		matrixState = MATRIX;
		turnMatrix();
		break;
	default:
		matrixState = OFF;
		turnOFF();
		break;
	}

	effectTimer.reset();
}

void LEDMatrix::nextMode()
{
	switch (matrixState)
	{
	case BUGS:
		matrixState = GLOWWORM;
		turnGlowworm();
		break;
	case GLOWWORM:
		matrixState = COLORS;
		turnColors();
		break;
	case COLORS:
		matrixState = RAINBOW;
		turnRainbow();
		break;
	case RAINBOW:
		matrixState = SPARKLES;
		turnSparkles();
		break;
	case SPARKLES:
		matrixState = FLAME;
		turnFlame();
		break;
	case FLAME:
		matrixState = FLASHES;
		turnFlashes();
		break;
	case FLASHES:
		matrixState = MATRIX;
		turnMatrix();
		break;
	default:
		matrixState = BUGS;
		turnBugs();
		break;
	}

	effectTimer.reset();
}

void LEDMatrix::turnMatrix()
{
	if (effect != NULL) delete effect;
	effect = new MatrixEffect(leds, width, height);
}
