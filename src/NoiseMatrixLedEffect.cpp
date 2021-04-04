/*
* NoiseMatrixLedEffect.cpp
*
*/

#include "NoiseMatrixLedEffect.h"

const char* const NoiseMatrixLedEffect::name = "NOISE";

NoiseMatrixLedEffect::NoiseMatrixLedEffect(ILedMatrix* converter, uint16_t Hz, const CRGBPalette16& palette, uint8_t zoom)
	: ILedEffect(Hz), matrix(converter), currentPalette(palette), dataSmoothing((Hz < 50) ? (200 - Hz * 4) : 0), scale(zoom)
{
	noise = new uint8_t*[matrix->getWidth()];

	for (uint8_t x = 0; x < matrix->getWidth(); x++)
	{
		noise[x] = new uint8_t[matrix->getHeight()];
	}

    reset();
}

NoiseMatrixLedEffect::~NoiseMatrixLedEffect()
{
	for (uint8_t x = 0; x < matrix->getWidth(); x++)
	{
		delete[] noise[x];
	}

    delete[] noise;
    noise = nullptr;
}

void NoiseMatrixLedEffect::reset()
{
    ILedEffect::reset();

    nX = random16();
    nY = random16();
    nZ = random16();
    ihue = 0;

    matrix->clearAllLeds();
}

void NoiseMatrixLedEffect::paint()
{
    for (uint8_t x = 0; x < matrix->getWidth(); x++)
    {
        for (uint8_t y = 0; y < matrix->getHeight(); y++)
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

           
           matrix->getPixel(x, y) = ColorFromPalette(currentPalette, index, bri);
        }
    }

    // apply slow drift to X and Y, just for visual variation.
    nZ += speed;
    nX += speed / 8;
    nY -= speed / 16;
    ihue++;
}
