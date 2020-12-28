#ifndef __ZigZagFromBottomLeftToUpAndRight_H__
#define __ZigZagFromBottomLeftToUpAndRight_H__

#include "IMatrixToLineConverter.h"

template <uint8_t WIDTH, uint8_t HEIGHT>
class ZigZagFromBottomLeftToUpAndRight : public IMatrixToLineConverter
{
public:
	uint8_t getWidth() const { return WIDTH; }
	uint8_t getHeight() const { return HEIGHT; }
	uint16_t getPixelNumber(uint8_t x, uint8_t y) const
	{
		if (x >= WIDTH) x = WIDTH - 1;
		if (y >= HEIGHT) y = HEIGHT - 1;

		return (x * HEIGHT) + ((x % 2 == 0) ? y : (HEIGHT - 1 - y));
	}
};

#endif
