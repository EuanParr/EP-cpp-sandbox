// win32-dependent component

#include <windows.h>

bool bRunning = true; // do game loop

struct RenderState
{
    int height, width;
    void* memory; // window buffer memory
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
            renderState.width = rect.right - rect.left; // find new dimensions
            renderState.height = rect.bottom - rect.top;

            // make buffer
            int bufferSize = renderState.width * renderState.height * sizeof(unsigned int);

            if (renderState.memory) // memory already allocated
            {
                VirtualFree(renderState.memory, 0, MEM_RELEASE);
            }

            renderState.memory = VirtualAlloc(0, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

            renderState.bitmapInfo.bmiHeader.biSize = sizeof(renderState.bitmapInfo.bmiHeader);
            renderState.bitmapInfo.bmiHeader.biWidth = renderState.width;
            renderState.bitmapInfo.bmiHeader.biHeight = renderState.height;
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
    unsigned int* pixel = (unsigned int*)renderState.memory;
        for (int y = 0; y < renderState.height; y++)
        {
            for (int x = 0; x < renderState.width; x++)
            {
                *pixel++ = 0xff00ff * x + 0x00ff00 * y;
            }
        }

        // render
        StretchDIBits(hdc, 0, 0, renderState.width, renderState.height, 0, 0, renderState.width, renderState.height, renderState.memory, &(renderState.bitmapInfo), DIB_RGB_COLORS, SRCCOPY);
    }
    return 0;
}
