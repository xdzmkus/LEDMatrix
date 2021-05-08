/**
	 ^
Y    |  3   4   11  12
2    |  2   5   10  13
1    |  1   6   9   14
0    |  0   7   8   15
	 0------------------>
		0   1   2   X
*/

#ifndef __ZigZagFromBottomLeftToUpAndRight_H__
#define __ZigZagFromBottomLeftToUpAndRight_H__

#include "ILedMatrix.h"

class ZigZagFromBottomLeftToUpAndRight : public ILedMatrix
{
public:

	ZigZagFromBottomLeftToUpAndRight(CRGB leds[], uint8_t width, uint8_t height)
		: ILedMatrix(leds, width, height)
	{
	};

	~ZigZagFromBottomLeftToUpAndRight()
	{
	};

protected:

	uint16_t getPixelNumber(uint8_t x, uint8_t y) const
	{
		if (x >= width) x = width - 1;
		if (y >= height) y = height - 1;

		return (x * height) + ((x % 2 == 0) ? y : (height - 1 - y));
	}
};

#endif
