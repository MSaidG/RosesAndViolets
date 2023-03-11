#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <GameInput.h>

struct bitmap_buffer
{
    BITMAPINFO bitmapInfo;
    void *memory;
    int height;
    int width;
    int pitch;
    int bytesPerPixel = 4;
};

struct window_dimension
{
    int width;
    int height;
};

static void InitDIB(bitmap_buffer *buffer, int Width, int Height);
static void RenderGraphic(bitmap_buffer *buffer, int xOffset, int yOffset);
static void DisplayBufferOnWindow(HWND windowHandle,bitmap_buffer *buffer, HDC deviceContext);
static window_dimension GetWindowDimension(HWND window);


LRESULT Wndproc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);
WNDCLASSEXA RegisterWindow(HINSTANCE instance);


void PollGamePadInput();