/**
	 ^
Y    |  3   4   11  12
2    |  2   5   10  13
1    |  1   6   9   14
0    |  0   7   8   15
	 0------------------>
		0   1   2   X
*/

#ifndef __ZigZagFromBottomLeftToUpAndRight_HPP__
#define __ZigZagFromBottomLeftToUpAndRight_HPP__

#include "ILedMatrix.hpp"

template <CRGB* ledLine, uint8_t width, uint8_t height>
class ZigZagFromBottomLeftToUpAndRight : public ILedMatrix<ledLine, width, height>
{
protected:

	uint16_t getPixelNumber(uint8_t x, uint8_t y) const
	{
		if (x >= width) x = width - 1;
		if (y >= height) y = height - 1;

		return (x * height) + ((x % 2 == 0) ? y : (height - 1 - y));
	}
};

#endif
