// win32-dependent component

#include <windows.h>
#include <math.h>

#include "renderer.h"

bool bRunning = true; // game loop continuation flag

struct RenderState // global variables
{
    renderer::WindowRect windowRect;
    BITMAPINFO bitmapInfo;
};

RenderState renderState; // instantiate the struct

LRESULT CALLBACK windowCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{ // called on window events
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
        { // must remake pixel buffer
            // find new dimensions
            RECT rect;
            GetClientRect(hwnd, &rect);
            renderState.windowRect.width = rect.right - rect.left;
            renderState.windowRect.height = rect.bottom - rect.top;

            // make buffer
            int bufferSize = renderState.windowRect.width * renderState.windowRect.height * sizeof(unsigned int);

            if (renderState.windowRect.buffer) // memory already allocated
            { // release the existing buffer
                VirtualFree(renderState.windowRect.buffer, 0, MEM_RELEASE);
            }

            // allocate new buffer
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
    HWND window = CreateWindow(WindowClass.lpszClassName, "Gravity", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
    HDC hdc = GetDC(window); // get device context - reference for drawing to window

    // declare game-loop variables
    float xVel = 2;
    float thrust = 0.01;
    float yVel = 0;
    float xPos = 640;
    float yPos = 160;
    int xInt = xPos;
    int yInt = yPos;
    bool moveRight = false;
    bool moveLeft = false;
    bool moveUp = false;
    bool moveDown = false;

    float gStrength = 1000;
    int gravX = 640;
    int gravY = 360;
    float squaredDist;
    float gravForce;

    // game-loop
    while (bRunning)
    {
        // get input
        MSG message;
        while (PeekMessage(&message, window, 0, 0, PM_REMOVE))
        {
            switch (message.message)
            {
            case (WM_KEYUP): // is a key event
            case (WM_KEYDOWN):
                {
                    unsigned int keyOrd = (unsigned int)message.wParam;
                    bool bKeyDown = (((message.lParam & (1 << 31)) == 0)); // gets flag for determining up/down
                    switch (message.wParam)
                    { // simple key handling, will need redoing later
                    case (VK_RIGHT):
                        moveRight = bKeyDown;
                        break;
                    case (VK_LEFT):
                        moveLeft = bKeyDown;
                        break;
                    case (VK_UP):
                        moveUp = bKeyDown;
                        break;
                    case (VK_DOWN):
                        moveDown = bKeyDown;
                        break;
                    }
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

        // factor in acceleration of gravity
        squaredDist = (gravX - xPos) * (gravX - xPos) + (gravY - yPos) * (gravY - yPos);
        gravForce = gStrength * pow(squaredDist, -1.5);
        xVel += gravForce * (gravX - xPos);
        yVel += gravForce * (gravY - yPos);

        // factor in acceleration of thrusters
        xVel += thrust * (moveRight - moveLeft);
        yVel += thrust * (moveUp - moveDown);

        // move craft
        xPos += xVel;
        yPos += yVel;
        xInt = xPos;
        yInt = yPos;

        // draw
        renderer::fillScreen(renderState.windowRect, 0x000020);
        renderer::drawRect(renderState.windowRect, xInt - 10, xInt + 10, yInt - 10, yInt + 10, 0xff0000); // draw craft
        renderer::drawRect(renderState.windowRect, gravX - 10, gravX + 10, gravY - 10, gravY + 10, 0x00ff00); // draw planet

        // render - put window buffer to screen
        StretchDIBits(hdc, 0, 0, renderState.windowRect.width, renderState.windowRect.height, 0, 0, renderState.windowRect.width, renderState.windowRect.height, renderState.windowRect.buffer, &(renderState.bitmapInfo), DIB_RGB_COLORS, SRCCOPY);

        // It does not need to process all the time - doing so causes unusual power consumption.
        Sleep(8);
    }
    return 0;
}
