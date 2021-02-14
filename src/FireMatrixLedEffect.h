/* 
* FireMatrixLedEffect.h
*
*/

#ifndef __FIREMATRIXLEDEFFECT_H__
#define __FIREMATRIXLEDEFFECT_H__

#define MATRIX_HEIGHT 8
#define MATRIX_WIDTH 9

#include <ILedEffect.h>
#include "ILedMatrix.h"

class FireMatrixLedEffect : public ILedEffect
{
public:

	static const char* const name;

	static const uint8_t valueMask[MATRIX_HEIGHT][MATRIX_WIDTH] PROGMEM;

	static const uint8_t hueMask[MATRIX_HEIGHT][MATRIX_WIDTH] PROGMEM;

private:

	ILedMatrix* matrix;

protected:

	uint8_t matrixValue[MATRIX_HEIGHT][MATRIX_WIDTH];
	uint8_t line[MATRIX_WIDTH];
	uint8_t pcnt = 0;


public:

	FireMatrixLedEffect(ILedMatrix* converter, uint16_t Hz);
	~FireMatrixLedEffect();

	void reset() override;
	bool paint() override;

	operator const char* () const { return name; }

private:

	FireMatrixLedEffect(const FireMatrixLedEffect&) = delete;
	FireMatrixLedEffect& operator=(const FireMatrixLedEffect&) = delete;
};

#endif
