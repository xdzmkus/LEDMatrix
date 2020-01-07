/*
* LEDMatrix.h
*/


#ifndef __LEDMATRIX_H__
#define __LEDMATRIX_H__

#include <LEDStrip.h>

class LEDMatrix : public LEDStrip
{
private:

	uint8_t width;
	uint8_t height;

	enum MATRIX_STATE
	{
		OFF,
		ON,
		BUGS,
		GLOWWORM,
		COLORS,
		RAINBOW,
		SPARKLES,
		FLAME,
		FLASHES,
		MATRIX
	}
	matrixState = OFF;

public:

	LEDMatrix(CRGB leds[], uint8_t width, uint8_t height);
	~LEDMatrix();

	uint8_t getWidth() const { return width; }
	uint8_t getHeight() const { return height; }

	virtual void save(int memIdx) override;
	virtual void load(int memIdx) override;

	virtual void nextMode() override;

	virtual void turnMatrix();

};

#endif //__LEDMATRIX_H__
