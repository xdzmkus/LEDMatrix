/*
* NoiseMatrixLedEffect.cpp
*
*/

#include "NoiseMatrixLedEffect.h"

const char* const NoiseMatrixLedEffect::name = "NOISE";

NoiseMatrixLedEffect::NoiseMatrixLedEffect(const IMatrixToLineConverter* converter, CRGB leds[], uint16_t count, uint16_t Hz, const CRGBPalette16& palette, uint8_t zoom)
	: LedEffect(leds, count, Hz), converter(converter), currentPalette(palette), dataSmoothing((Hz < 50) ? (200 - Hz * 4) : 0), scale(zoom)
{
	noise = new uint8_t*[converter->getWidth()];

	for (uint8_t x = 0; x < converter->getWidth(); x++)
	{
		noise[x] = new uint8_t[converter->getHeight()];
	}

    init();
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

    clearAllLeds();
}

bool NoiseMatrixLedEffect::paint()
{
    if (!isReady())
        return false;

    for (uint8_t x = 0; x < converter->getWidth(); x++)
    {
        for (uint8_t y = 0; y < converter->getHeight(); y++)
        {
            uint8_t data = inoise8(nX + scale * x, nY + scale * y, nZ);
            data = qsub8(data, 16);
            data = qadd8(data, scale8(data, 39));

            if (dataSmoothing > 0)
                noise[x][y] = scale8(noise[x][y], dataSmoothing) + scale8(data, 256 - dataSmoothing);
            else
                noise[x][y] = data;

            uint8_t index = noise[x][y] + ihue;

            // brighten up, as the color palette itself often contains the light/dark dynamic range desired
            uint8_t bri = (noise[x][y] > 127) ? 255 : dim8_raw(noise[x][y] * 2);

            ledLine[converter->getPixelNumber(x, y)] = ColorFromPalette(currentPalette, index, bri);
        }
    }

    // apply slow drift to X and Y, just for visual variation.
    nZ += speed;
    nX += speed / 8;
    nY -= speed / 16;
    ihue++;

    return true;
}
