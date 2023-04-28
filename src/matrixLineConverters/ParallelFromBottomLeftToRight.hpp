/**
Y	 ^
3    |  3   7   11  15
2    |  2   6   10  14
1    |  1   5   9   13
0    |  0   4   8   12
	 0------------------>
		0   1   2   3   X
*/

#ifndef __ParallelFromBottomLeftToRight_HPP__
#define __ParallelFromBottomLeftToRight_HPP__

#include "ILedMatrix.hpp"

template <CRGB* ledLine, uint8_t width, uint8_t height>
class ParallelFromBottomLeftToRight : public ILedMatrix<ledLine, width, height>
{
protected:

	uint16_t getPixelNumber(uint8_t x, uint8_t y) const
	{
		if (x >= width) x = width - 1;
		if (y >= height) y = height - 1;

		return x * height + y;
	}
};

#endif