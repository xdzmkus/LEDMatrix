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

void ILedMatrix::operator>>(uint8_t startX)
{
	// shift right columns
	for (uint8_t x = width - 1; x > startX; x--)
	{
		for (uint8_t y = 0; y < height; y++)
		{
			getPixel(x, y) = getPixel(x - 1, y);
		}
	}
}

void ILedMatrix::operator<<(uint8_t startX)
{
	uint8_t rightLimit = startX >= width ? width - 1 : startX;

	// shift left columns
	for (uint8_t x = 0; x < startX; x++)
	{
		for (uint8_t y = 0; y < height; y++)
		{
			getPixel(x, y) = getPixel(x + 1, y);
		}
	}
}

void ILedMatrix::clearColumn(uint8_t col)
{
	if (col < width)
	{
		for (uint8_t y = 0; y < height; y++)
		{
			getPixel(col, y) = CRGB::Black;
		}
	}
}

void ILedMatrix::clearLine(uint8_t line)
{
	if (line < height)
	{
		for (uint8_t x = 0; x < width; x++)
		{
			getPixel(x, line) = CRGB::Black;
		}
	}
}

