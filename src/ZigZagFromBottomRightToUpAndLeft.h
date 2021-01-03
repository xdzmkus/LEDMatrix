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

#include "IMatrixToLineConverter.h"

template <uint8_t WIDTH, uint8_t HEIGHT>
class ZigZagFromBottomRightToUpAndLeft : public IMatrixToLineConverter
{
public:
	uint8_t getWidth() const { return WIDTH; }
	uint8_t getHeight() const { return HEIGHT; }
	uint16_t getPixelNumber(uint8_t x, uint8_t y) const
	{
		if (x >= WIDTH) x = WIDTH - 1;
		if (y >= HEIGHT) y = HEIGHT - 1;

		return ((WIDTH - 1 - x) * HEIGHT) + (((WIDTH - x) % 2 == 1) ? y : (HEIGHT - 1 - y));
	}
};

#endif