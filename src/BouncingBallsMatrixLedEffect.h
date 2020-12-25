/* 
* BouncingBallsMatrixLedEffect.h
*
*/

#ifndef __BOUNSINGBALLSMATRIXLEDEFFECT_H__
#define __BOUNSINGBALLSMATRIXLEDEFFECT_H__

#include <LedEffect.h>
#include "IMatrixConverter.h"

class BouncingBallsMatrixLedEffect : public LedEffect
{
private:

	const float Gravity = 9.8;
	const float MaxVelocity;

	const IMatrixConverter* converter;

public:

	static const char* const name;

protected:

	struct BOUNCING_COLUMN
	{
		struct BOUNCING_BALL
		{
			CRGB  color;
			int   position;
			float height;
			float velocity;
			byte  dampingPercentage;
			long  startTime;
		}
		*balls;

		uint8_t	numBalls;
	}
	*bouncingColumns;

public:
	BouncingBallsMatrixLedEffect(const IMatrixConverter* converter, CRGB leds[], uint16_t count, uint16_t Hz, uint8_t maxBallsCount = 1);
	~BouncingBallsMatrixLedEffect();
	
	void init() override;
	bool paint() override;

	operator const char* () const {	return name; }

private:
	BouncingBallsMatrixLedEffect(const BouncingBallsMatrixLedEffect&) = delete;
	BouncingBallsMatrixLedEffect& operator=(const BouncingBallsMatrixLedEffect&) = delete;

};

#endif

