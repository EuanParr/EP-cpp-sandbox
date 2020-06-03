#include "renderer.h"

namespace renderer{
    void renderBackground(WindowRect windowRect)
    {
        unsigned int* pixel = (unsigned int*)windowRect.buffer;
        for (int y = 0; y < windowRect.height; y++)
        {
            for (int x = 0; x < windowRect.width; x++)
            {
                *pixel++ = 0xff00ff * x + 0x00ff00 * y;
            }
        }
    }

    void fillScreen(WindowRect windowRect, unsigned int colour)
    {
        unsigned int* pixel = (unsigned int*)windowRect.buffer;
        for (int y = 0; y < windowRect.height; y++)
        {
            for (int x = 0; x < windowRect.width; x++)
            {
                *pixel++ = colour;
            }
        }
    }
}
