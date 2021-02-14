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
		if (x >= getWidth()) x = getWidth() - 1;
		if (y >= getHeight()) y = getHeight() - 1;

		return (x * getHeight()) + ((x % 2 == 0) ? y : (getHeight() - 1 - y));
	}
};

#endif
