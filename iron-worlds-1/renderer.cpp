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

    int rangeRestrict(int val, int lower, int upper)
    {
        if (val < lower)
        {
            return lower;
        }
        else if (val > upper)
        {
            return upper;
        }
        else
        {
            return val;
        }
    }

    void drawRect(WindowRect windowRect, int left, int right, int bottom, int top, unsigned int colour)
    {
        left = rangeRestrict(left, 0, windowRect.width);
        right = rangeRestrict(right, 0, windowRect.width);
        bottom = rangeRestrict(bottom, 0, windowRect.height);
        top = rangeRestrict(top, 0, windowRect.height);

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
