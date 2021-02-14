/**
	 ^
Y    |  12  11  4   3
2    |  13  10  5   2
1    |  14  9   6   1
0    |  15  8   7   0
	 0------------------>
		0   1   2   X
*/

#ifndef __ZigZagFromBottomRightToUpAndLeft_H__
#define __ZigZagFromBottomRightToUpAndLeft_H__

#include "ILedMatrix.h"

class ZigZagFromBottomRightToUpAndLeft : public ILedMatrix
{
public:

	ZigZagFromBottomRightToUpAndLeft(CRGB leds[], uint8_t width, uint8_t height)
		: ILedMatrix(leds, width, height)
	{
	};

	~ZigZagFromBottomRightToUpAndLeft()
	{
	};


protected:

	uint16_t getPixelNumber(uint8_t x, uint8_t y) const
	{
		if (x >= getWidth()) x = getWidth() - 1;
		if (y >= getHeight()) y = getHeight() - 1;

		return ((getWidth() - 1 - x) * getHeight()) + (((getWidth() - x) % 2 == 1) ? y : (getHeight() - 1 - y));
	}
};

#endif
