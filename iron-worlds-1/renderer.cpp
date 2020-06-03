#include "renderer.h"

namespace renderer{
    void fillScreen(WindowRect windowRect, unsigned int colour)
    {
        unsigned int* pixel = (unsigned int*)windowRect.buffer;
        for (int y = 0; y < windowRect.height; y++)
        {
            for (int x = 0; x < windowRect.width; x++)
            {
                *pixel++ = colour; // sets the value at the pixel pointer to colour then increments the pointer
            }
        }
    }

    void drawRect(WindowRect windowRect, int left, int right, int bottom, int top, unsigned int colour)
    {
        for (int y = bottom; y < top; y++)
        {
            unsigned int* pixel = (unsigned int*)windowRect.buffer + left + y * windowRect.width;
            for (int x = left; x < right; x++)
            {
                *pixel++ = colour;
            }
        }
    }
}
