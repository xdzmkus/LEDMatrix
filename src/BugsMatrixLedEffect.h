/* 
* BugsMatrixLedEffect.h
*
*/

#ifndef __BUGSMATRIXLEDEFFECT_H__
#define __BUGSMATRIXLEDEFFECT_H__

#include "ILedEffect.h"
#include "ILedMatrix.h"

class BugsMatrixLedEffect : public ILedEffect
{
public:

	static const char* const name;

protected:

	ILedMatrix* matrix;

	const uint8_t BUGS_MAX_SPEED = 20;

	const uint8_t numBugs;

	struct BUGS
	{
		CRGB color;
		int8_t speed;
		uint8_t xPos;
		uint8_t yPos;
		bool hvDir;
	}
	*bugs = nullptr;

public:
	BugsMatrixLedEffect(ILedMatrix* converter, uint16_t Hz, uint8_t bugs = 1);
	~BugsMatrixLedEffect();
	
	void reset() override;
	bool paint() override;

	operator const char* () const {	return name; }

private:
	BugsMatrixLedEffect(const BugsMatrixLedEffect&) = delete;
	BugsMatrixLedEffect& operator=(const BugsMatrixLedEffect&) = delete;

};

#endif
