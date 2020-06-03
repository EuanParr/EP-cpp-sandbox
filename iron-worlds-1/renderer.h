#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED

namespace renderer
{
    struct WindowRect
    {
        int width, height;
        void* buffer;
    };
    void renderBackground(WindowRect windowRect);
    void fillScreen(WindowRect windowRect, unsigned int colour);
}

#endif // RENDERER_H_INCLUDED
