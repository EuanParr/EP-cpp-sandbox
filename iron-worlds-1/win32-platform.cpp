// win32-dependent component

#include <windows.h>

bool bRunning = true;

LRESULT CALLBACK windowCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;
    switch (uMsg)
    {
    case WM_CLOSE:
    case WM_DESTROY:
        {
            bRunning = false;
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

        // render
    }
    return 0;
}
