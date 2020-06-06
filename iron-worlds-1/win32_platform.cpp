// win32-dependent component

#include <windows.h>

#include "renderer.h"

bool bRunning = true; // do game loop

struct RenderState
{
    renderer::WindowRect windowRect;
    BITMAPINFO bitmapInfo;
};

RenderState renderState;

LRESULT CALLBACK windowCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;
    switch (uMsg)
    {
    case WM_CLOSE: // attempted to close program
    case WM_DESTROY:
        {
            bRunning = false;
        }
        break;
    case WM_SIZE: // window size changed
        {
            RECT rect;
            GetClientRect(hwnd, &rect);
            renderState.windowRect.width = rect.right - rect.left; // find new dimensions
            renderState.windowRect.height = rect.bottom - rect.top;

            // make buffer
            int bufferSize = renderState.windowRect.width * renderState.windowRect.height * sizeof(unsigned int);

            if (renderState.windowRect.buffer) // memory already allocated
            {
                VirtualFree(renderState.windowRect.buffer, 0, MEM_RELEASE);
            }

            renderState.windowRect.buffer = VirtualAlloc(0, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

            renderState.bitmapInfo.bmiHeader.biSize = sizeof(renderState.bitmapInfo.bmiHeader);
            renderState.bitmapInfo.bmiHeader.biWidth = renderState.windowRect.width;
            renderState.bitmapInfo.bmiHeader.biHeight = renderState.windowRect.height;
            renderState.bitmapInfo.bmiHeader.biPlanes = 1;
            renderState.bitmapInfo.bmiHeader.biBitCount = 32;
            renderState.bitmapInfo.bmiHeader.biCompression = BI_RGB;
        }
        break;
    default:
        {
            result = DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }
    return result;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    // create window class
    WNDCLASS WindowClass = {};
    WindowClass.style = CS_HREDRAW | CS_VREDRAW;
    WindowClass.lpszClassName = "Game Window Class";
    WindowClass.lpfnWndProc = windowCallback;

    // register the window class
    RegisterClass(&WindowClass);

    // create the window
    HWND window = CreateWindow(WindowClass.lpszClassName, "Iron Worlds 1", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
    HDC hdc = GetDC(window); // get device context - reference for drawing to window

    float xVel = 1;
    float yVel = 0;
    float xPos = 0;
    int xInt = xPos;
    int yPos = 340;
    bool doMove = false;

    while (bRunning)
    {
        // get input
        MSG message;
        while (PeekMessage(&message, window, 0, 0, PM_REMOVE))
        {
            switch (message.message)
            {
            case (WM_KEYUP):
            case (WM_KEYDOWN):
                {
                    unsigned int keyOrd = (unsigned int)message.wParam;
                    bool bKeyDown = (((message.lParam & (1 << 31)) == 0)); // gets flag for determining up/down
                    doMove = bKeyDown;
                }
                break;
            default:
                {
                    TranslateMessage(&message);
                    DispatchMessage(&message);
                }
            }
        }

        // simulate
        if (doMove)
        {
            xVel += 0;
            xPos += xVel;
        }
        xInt = xPos;
        renderer::fillScreen(renderState.windowRect, 0x808080);
        renderer::drawRect(renderState.windowRect, xInt, xInt + 20, yPos, yPos + 20, 0x0000ff);

        // render - put window buffer to screen
        StretchDIBits(hdc, 0, 0, renderState.windowRect.width, renderState.windowRect.height, 0, 0, renderState.windowRect.width, renderState.windowRect.height, renderState.windowRect.buffer, &(renderState.bitmapInfo), DIB_RGB_COLORS, SRCCOPY);
    }
    return 0;
}
