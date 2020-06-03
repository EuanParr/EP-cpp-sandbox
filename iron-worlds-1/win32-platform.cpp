// win32-dependent component

#include <windows.h>

bool bRunning = true; // do game loop

void* bufferMemory; // window buffer memory
int bufferWidth;
int bufferHeight;
BITMAPINFO bufferBitmapInfo;

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
            bufferWidth = rect.right - rect.left; // find new dimensions
            bufferHeight = rect.bottom - rect.top;

            // make buffer
            int bufferSize = bufferWidth * bufferHeight * sizeof(unsigned int);

            if (bufferMemory) // memory already allocated
            {
                VirtualFree(bufferMemory, 0, MEM_RELEASE);
            }

            bufferMemory = VirtualAlloc(0, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

            bufferBitmapInfo.bmiHeader.biSize = sizeof(bufferBitmapInfo.bmiHeader);
            bufferBitmapInfo.bmiHeader.biWidth = bufferWidth;
            bufferBitmapInfo.bmiHeader.biHeight = bufferHeight;
            bufferBitmapInfo.bmiHeader.biPlanes = 1;
            bufferBitmapInfo.bmiHeader.biBitCount = 32;
            bufferBitmapInfo.bmiHeader.biCompression = BI_RGB;
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

    while (bRunning)
    {
        // get input
        MSG message;
        while (PeekMessage(&message, window, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }

        // simulate
    unsigned int* pixel = (unsigned int*)bufferMemory;
        for (int y = 0; y < bufferHeight; y++)
        {
            for (int x = 0; x < bufferWidth; x++)
            {
                *pixel++ = 0x808080;
            }
        }

        // render
        StretchDIBits(hdc, 0, 0, bufferWidth, bufferHeight, 0, 0, bufferWidth, bufferHeight, bufferMemory, &bufferBitmapInfo, DIB_RGB_COLORS, SRCCOPY);
    }
    return 0;
}
