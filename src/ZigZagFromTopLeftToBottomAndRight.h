/**
	 ^
Y    |  0   7   8   15
2    |  1   6   9   14
1    |  2   5   10  13
0    |  3   4   11  12
	 0------------------>
		0   1   2   X
*/

#ifndef __ZigZagFromTopLeftToBottomAndRight_H__
#define __ZigZagFromTopLeftToBottomAndRight_H__

#include "ILedMatrix.h"

class ZigZagFromTopLeftToBottomAndRight : public ILedMatrix
{
public:

	ZigZagFromTopLeftToBottomAndRight(CRGB leds[], uint8_t width, uint8_t height)
		: ILedMatrix(leds, width, height)
	{
	};

	~ZigZagFromTopLeftToBottomAndRight()
	{
	};

protected:

	uint16_t getPixelNumber(uint8_t x, uint8_t y) const
	{
		if (x >= width) x = width - 1;
		if (y >= height) y = height - 1;

		return (x * height) + ((x % 2 == 0) ? (height - 1 - y) : y);
	}
};

#endif
