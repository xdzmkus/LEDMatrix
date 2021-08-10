/*
* ILedMatrix.hpp
*
*         ^
* HEIGHT  |  x   x   x   x
* 2       |  x   x   x   x
* 1       |  x   x   x   x
* 0       |  x   x   x   x
*         0------------------->
*            0   1   2  WIDTH
*/

#ifndef _ILEDMATRIX_HPP_
#define _ILEDMATRIX_HPP_

#include <ILedLine.hpp>

template <CRGB* const ledLine, const uint8_t width, const uint8_t height>
class ILedMatrix : public ILedLine<ledLine, width * height>
{
public:

	ILedMatrix()
	{
	};

	virtual ~ILedMatrix()
	{
	};

	uint8_t getWidth() const
	{
		return width;
	};

	uint8_t getHeight() const
	{
		return height;
	};

	CRGB* getAllPixels()
	{
		return ledLine;
	};

	CRGB& getPixel(uint8_t x, uint8_t y)
	{
		return ledLine[getPixelNumber(x, y)];
	};

	void operator>>(uint8_t startX)
	{
		// shift right columns
		for (uint8_t x = width - 1; x > startX; x--)
		{
			for (uint8_t y = 0; y < height; y++)
			{
				getPixel(x, y) = getPixel(x - 1, y);
			}
		}
	};
	
	void operator<<(uint8_t startX)
	{
		uint8_t rightLimit = startX >= width ? width - 1 : startX;

		// shift left columns
		for (uint8_t x = 0; x < startX; x++)
		{
			for (uint8_t y = 0; y < height; y++)
			{
				getPixel(x, y) = getPixel(x + 1, y);
			}
		}
	};

	void clearColumn(uint8_t column)
	{
		if (column < width)
		{
			for (uint8_t y = 0; y < height; y++)
			{
				getPixel(column, y) = CRGB::Black;
			}
		}
	};

	void clearLine(uint8_t line)
	{
		if (line < height)
		{
			for (uint8_t x = 0; x < width; x++)
			{
				getPixel(x, line) = CRGB::Black;
			}
		}
	};

protected:

	virtual uint16_t getPixelNumber(uint8_t x, uint8_t y) const = 0;

private:

	ILedMatrix(const ILedMatrix&) = delete;
	ILedMatrix& operator=(const ILedMatrix&) = delete;

};

#endif
