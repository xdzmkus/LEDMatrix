/**
	 ^
Y    |  15  8   7   0
2    |  14  9   6   1
1    |  13  10  5   2
0    |  12  11  4   3
	 0------------------>
		0   1   2   X
*/

#ifndef __ZigZagFromTopRightToBottomAndLeft_HPP__
#define __ZigZagFromTopRightToBottomAndLeft_HPP__

#include "ILedMatrix.hpp"

template <CRGB* const ledLine, const uint8_t width, const uint8_t height>
class ZigZagFromTopRightToBottomAndLeft : public ILedMatrix<ledLine, width, height>
{
protected:

	uint16_t getPixelNumber(uint8_t x, uint8_t y) const
	{
		if (x >= width) x = width - 1;
		if (y >= height) y = height - 1;

		return ((width - 1 - x) * height) + (((width - x) % 2 == 1) ? (height - 1 - y) : y);
	}
};

#endif
