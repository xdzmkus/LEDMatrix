/**
	 ^
Y    |  12  11  4   3
2    |  13  10  5   2
1    |  14  9   6   1
0    |  15  8   7   0
	 0------------------>
		0   1   2   X
*/

#ifndef __ZigZagFromBottomRightToUpAndLeft_HPP__
#define __ZigZagFromBottomRightToUpAndLeft_HPP__

#include "ILedMatrix.hpp"

template <CRGB* ledLine, uint8_t width, uint8_t height>
class ZigZagFromBottomRightToUpAndLeft : public ILedMatrix<ledLine, width, height>
{
protected:

	uint16_t getPixelNumber(uint8_t x, uint8_t y) const
	{
		if (x >= width) x = width - 1;
		if (y >= height) y = height - 1;

		return ((width - 1 - x) * height) + (((width - x) % 2 == 1) ? y : (height - 1 - y));
	}
};

#endif
