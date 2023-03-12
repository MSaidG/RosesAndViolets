#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <GameInput.h>
#include <Objbase.h>
#include <xaudio2.h>
#include <mmreg.h>

#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT  ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'

//#define fourccRIFF (DWORD)'FFIR'
//#define fourccDATA (DWORD)"atad"
//#define fourccFMT  (DWORD)"tmf"
//#define fourccWAVE (DWORD)"EVAW"
//#define fourccXWMA (DWORD)"AMWX"
//#define fourccDPDS (DWORD)"sdpd"


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

HRESULT ReadChunkData(HANDLE hFile, void *buffer, DWORD buffersize, DWORD bufferoffset);
HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD &dwChunkSize, DWORD &dwChunkDataPosition);




