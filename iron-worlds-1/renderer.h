#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED

namespace renderer
{
    struct WindowRect
    {
        int width, height;
        void* buffer;
    };
    void fillScreen(WindowRect windowRect, unsigned int colour);
    void drawRect(WindowRect windowRect, int left, int right, int bottom, int top, unsigned int colour);
}

#endif // RENDERER_H_INCLUDED
