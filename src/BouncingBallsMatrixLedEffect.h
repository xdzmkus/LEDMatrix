/*
* BouncingBallsMatrixLedEffect.h
*
*/

#ifndef __BOUNSINGBALLSMATRIXLEDEFFECT_H__
#define __BOUNSINGBALLSMATRIXLEDEFFECT_H__

#include <ILedEffect.h>
#include "ILedMatrix.h"

class BouncingBallsMatrixLedEffect : public ILedEffect
{
private:

	ILedMatrix* matrix;

	const float Gravity = 9.8;
	const float MaxVelocity;

public:

	static const char* const name;

protected:

	struct BOUNCING_COLUMN
	{
		struct BOUNCING_BALL
		{
			CRGB	color;
			uint8_t position;
			float	height;
			float	velocity;
			uint8_t dampingPercentage;
			unsigned long startTime;
		}
		*balls = nullptr;

		uint8_t numBalls;
	}
	*bouncingColumns = nullptr;

public:
	BouncingBallsMatrixLedEffect(ILedMatrix* converter, uint16_t Hz, uint8_t maxBallsCount = 1);
	~BouncingBallsMatrixLedEffect();

	void reset() override;
	bool paint() override;

	operator const char* () const { return name; }

private:
	BouncingBallsMatrixLedEffect(const BouncingBallsMatrixLedEffect&) = delete;
	BouncingBallsMatrixLedEffect& operator=(const BouncingBallsMatrixLedEffect&) = delete;
};

#endif

