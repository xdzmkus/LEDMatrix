/*
* NoiseMatrixLedEffect.cpp
*
*/

#include "NoiseMatrixLedEffect.h"

const char* const NoiseMatrixLedEffect::name = "NOISE";

NoiseMatrixLedEffect::NoiseMatrixLedEffect(const IMatrixConverter* converter, CRGB leds[], uint16_t count, uint16_t Hz, const CRGBPalette16& palette)
	: LedEffect(leds, count, Hz), converter(converter), currentPalette(palette)
{
	init();

	noise = new uint8_t*[converter->getWidth()];

	for (uint8_t x = 0; x < converter->getWidth(); x++)
	{
		noise[x] = new uint8_t[converter->getHeight()];
	}
}

NoiseMatrixLedEffect::~NoiseMatrixLedEffect()
{
	for (uint8_t x = 0; x < converter->getWidth(); x++)
	{
		delete[] noise[x];
	}

    delete[] noise;
    noise = nullptr;
}

void NoiseMatrixLedEffect::init()
{
    nX = random16();
    nY = random16();
    nZ = random16();
    ihue = 0;

    LedEffect::init();
}

bool NoiseMatrixLedEffect::paint()
{
	if (!isReady())
		return false;

    for (int x = 0; x < converter->getWidth(); x++)
    {
        int xOffset = scale * x;
        for (int y = 0; y < converter->getHeight(); y++)
        {
            int yOffset = scale * y;

            uint8_t data = inoise8(nX + xOffset, nY + yOffset, nZ);
            data = qsub8(data, 16);
            data = qadd8(data, scale8(data, 39));

            noise[x][y] = scale8(noise[x][y], dataSmoothing) + scale8(data, 256 - dataSmoothing);
        }
    }

    // apply slow drift to X and Y, just for visual variation.
    nZ += speed;
    nX += speed / 8;
    nY -= speed / 16;
    
    for (int x = 0; x < converter->getWidth(); x++)
    {
        for (int y = 0; y < converter->getHeight(); y++)
        {
            uint8_t index = noise[x][y] + ihue;

            // brighten up, as the color palette itself often contains the light/dark dynamic range desired
            uint8_t bri = (noise[x][y] > 127) ? 255 : dim8_raw(noise[x][y] * 2);
           
            ledLine[converter->getPixelNumber(x, y)] = ColorFromPalette(currentPalette, index, bri);
        }
    }

    ihue++;

	return true;
}
