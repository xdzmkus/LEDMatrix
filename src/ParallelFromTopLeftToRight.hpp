/**
Y	 ^
3    |  0   4   8   12
2    |  1   5   9   13
1    |  2   6   10  14
0    |  3   7   11  15
	 0------------------>
		0   1   2   3   X
*/

#ifndef __ParallelFromTopLeftToRightt_HPP__
#define __ParallelFromTopLeftToRight_HPP__

#include "ILedMatrix.hpp"

template <CRGB* ledLine, uint8_t width, uint8_t height>
class ParallelFromTopLeftToRight : public ILedMatrix<ledLine, width, height>
{
protected:

	uint16_t getPixelNumber(uint8_t x, uint8_t y) const
	{
		if (x >= width) x = width - 1;
		if (y >= height) y = height - 1;

		return x * height + (height - 1 - y);
	}
};

#endif