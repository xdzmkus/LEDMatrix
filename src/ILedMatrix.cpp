/*
* ILedMatrix.cpp
*
*/

#include "ILedMatrix.h"

ILedMatrix::ILedMatrix(CRGB leds[], uint8_t width, uint8_t height)
	: ILedLine(leds, width * height), width(width), height(height)
{
}

ILedMatrix::~ILedMatrix()
{
}

uint8_t ILedMatrix::getWidth() const
{
	return width;
}

uint8_t ILedMatrix::getHeight() const
{
	return height;
}

CRGB* ILedMatrix::getAllPixels()
{
	return ledLine;
}

CRGB& ILedMatrix::getPixel(uint8_t x, uint8_t y)
{
	return ledLine[getPixelNumber(x,y)];
}

