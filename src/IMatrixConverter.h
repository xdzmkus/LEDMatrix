#ifndef __IMATRIXCONVERTER_H__
#define __IMATRIXCONVERTER_H__

class IMatrixConverter
{
public:
	virtual uint8_t getWidth() const = 0;
	virtual uint8_t getHeight() const = 0;
	virtual uint16_t getPixelNumber(uint8_t x, uint8_t y) const = 0;
};

#endif
