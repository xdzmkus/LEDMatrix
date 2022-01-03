/**
Y	 ^
3    |  15  11  7   3
2    |  14  10  6   2
1    |  13  9   5   1
0    |  12  8   4   0
	 0------------------>
		0   1   2   3   X
*/

#ifndef __ParallelFromBottomRightToLeft_HPP__
#define __ParallelFromBottomRightToLeft_HPP__

#include "ILedMatrix.hpp"

template <CRGB* ledLine, uint8_t width, uint8_t height>
class ParallelFromBottomRightToLeft : public ILedMatrix<ledLine, width, height>
{
protected:

	uint16_t getPixelNumber(uint8_t x, uint8_t y) const
	{
		if (x >= width) x = width - 1;
		if (y >= height) y = height - 1;

		return (width - 1 - x) * height + y;
	}
};

#endif