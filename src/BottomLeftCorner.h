#ifndef __BOTTOMLEFTCORNER_H__
#define __BOTTOMLEFTCORNER_H__

#include "IMatrixConverter.h"

template <uint8_t WIDTH, uint8_t HEIGHT>
class BottomLeftCorner : public IMatrixConverter
{
public:
	uint8_t getWidth() const { return WIDTH; }
	uint8_t getHeight() const { return HEIGHT; }
	uint16_t getPixelNumber(uint8_t x, uint8_t y) const
	{
		if (x > WIDTH - 1) x = WIDTH - 1;
		if (y > HEIGHT - 1) y = HEIGHT - 1;

		return (x * HEIGHT) + ((x % 2 == 0) ? y : (HEIGHT - 1 - y));
	}
};

#endif
