#include "test.h"

static u_int isProgramRunning = false;
IGameInput* gamePadInput = 0;
IGameInputDevice* gamePad = 0;
IGameInputReading *inputReading;
GameInputGamepadState gamePadState;


int WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, PSTR cmdLine, int cmdShow)
{


    //////////////////GAME INPUT INIT/////////////////////////////////
    GameInputCreate(&gamePadInput);
    //////////////////////////////////////////////////



    ///////////////GRAPHICS INIT////////////////////
    bitmap_buffer frameBuffer = {};
    InitDIB(&frameBuffer, 1288, 728);
    /////////////////////////////////////////////////



    //////////////////WINDOW INIT///////////////////////////////
    MSG message;
    WNDCLASSEXA windowClass = RegisterWindow(instance);
    if (!RegisterClassExA(&windowClass))
    {
        MessageBoxExA(0, (LPCSTR)"Window Registration Failed!", (LPCSTR)"Error!", MB_OK | MB_ICONEXCLAMATION, 0);
        return 0;
    }

    HWND windowHandle = CreateWindowExA(0, windowClass.lpszClassName, (LPCSTR)"Tutorial",
                                        WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1200, 700, 0,
                                        0, instance, 0);
    if (!windowHandle)
    {
        MessageBoxExA(0, (LPCSTR)"Window Creation Failed!", (LPCSTR)"Error!", MB_OK | MB_ICONEXCLAMATION, 0);
        return 0;
    }
    ShowWindow(windowHandle, cmdShow);
    UpdateWindow(windowHandle);
    /////////////////////////////////////////////////////////////////


    isProgramRunning = true;
    int time = 0;
    float xOffset;    
    float yOffset; 
    while (isProgramRunning)
    {

        while (PeekMessageA(&message, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }


        /////////////////////////////////////////////GAME INPUT/////////////////////////////////////////////////////////
        PollGamePadInput();
        
        if (gamePadState.buttons == GameInputGamepadA)
        {
            xOffset += 5;
            yOffset += 5;   
        }

        float xSpeed = gamePadState.leftThumbstickX*2;
        printf("%f\n", xSpeed);
        if (xSpeed > 0.3f || xSpeed < -0.3f)
        {
            xOffset += xSpeed;
        }
        
        ///////////////////////////////////////////////GAME INPUT END///////////////////////////////////////////////////////


           
        /////////////////////////////////////////////ANIMATION/////////////////////////////////////////////////////////
        if (time >= 10)
        {
            xOffset += 1;
            yOffset += 1;    
            time = 0;
        }
        time += 1;
        //////////////////////////////////////////////ANIMATION END///////////////////////////////////////////////////////////



        /////////////////////////////////////////////GRAPHICS//////////////////////////////////////////////////////////
        HDC deviceContext = GetDC(windowHandle);   

        RenderGraphic(&frameBuffer, xOffset, yOffset);
        DisplayBufferOnWindow(windowHandle, &frameBuffer, deviceContext); 

        ReleaseDC(windowHandle, deviceContext);
        ////////////////////////////////////////////GRAPHICS END///////////////////////////////////////////////////////////////
    }

    return 0;
    
}

WNDCLASSEXA RegisterWindow(HINSTANCE instance)
{
    WNDCLASSEXA windowClass = {};
    HICON iconHandle = (HICON) LoadImageA(0, MAKEINTRESOURCE(32649), IMAGE_CURSOR, 0, 0, LR_SHARED);

    windowClass.cbSize = sizeof(windowClass);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = Wndproc;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = DLGWINDOWEXTRA;
    windowClass.hInstance = instance;
    windowClass.hIcon = iconHandle;
    windowClass.hCursor = 0;
    windowClass.hbrBackground = 0;
    windowClass.lpszClassName = (LPCSTR)"Win32_API";
    windowClass.hIconSm = iconHandle;

    return windowClass;
}



LRESULT Wndproc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT result;

    switch (message)
    {

        case WM_CLOSE:
        {
            PostQuitMessage(0);
            isProgramRunning = false;
        }break;

        case WM_DESTROY:
        {   
            PostQuitMessage(0);
            isProgramRunning = false;
        }break;

        case WM_PAINT:
        {
            PAINTSTRUCT paintStruct;
            HDC deviceContextHandle = BeginPaint(windowHandle, &paintStruct);

            EndPaint(windowHandle, &paintStruct);

        }break;

        default:
            result = DefWindowProc(windowHandle, message, wParam, lParam);
            break;


    }

    return result;
}

static window_dimension GetWindowDimension(HWND window)
{
    window_dimension result;

    RECT ClientRect;
    GetClientRect(window, &ClientRect);
    result.height = ClientRect.bottom - ClientRect.top;
    result.width = ClientRect.right - ClientRect.left;

    return result;
}


static void InitDIB(bitmap_buffer *buffer, int Width, int Height)
{

    // TODO: bullet proof this.

    if (buffer->memory)
    {
        VirtualFree(buffer->memory, 0, MEM_RELEASE);
    }

    buffer->height = Height;
    buffer->width = Width;

    // NOTE: Negative Height is to make bitmap top-down and left to right!
    buffer->bitmapInfo.bmiHeader.biSize = sizeof(buffer->bitmapInfo.bmiHeader);
    buffer->bitmapInfo.bmiHeader.biWidth = buffer->width;
    buffer->bitmapInfo.bmiHeader.biHeight = -buffer->height;
    buffer->bitmapInfo.bmiHeader.biPlanes = 1;
    buffer->bitmapInfo.bmiHeader.biBitCount = 32; 
    buffer->bitmapInfo.bmiHeader.biCompression = BI_RGB;
    
    // NOTE: May just allocate this ourselves?
    //BitmapHandle = CreateDIBSection(BitmapDeviceContext, &BitmapInfo, DIB_RGB_COLORS, &BitmapMemory, 0, 0);

    
    int BitmapMemorySize = (buffer->width * buffer->height) * buffer->bytesPerPixel;
    buffer->memory = VirtualAlloc(0, BitmapMemorySize, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
    buffer->pitch = Width * buffer->bytesPerPixel;

    // NOTE: Clear to black


}

static void DisplayBufferOnWindow(HWND windowHandle,bitmap_buffer *buffer, HDC deviceContext)
{
    // NOTE: Aspect ratio corection
    window_dimension dimension = GetWindowDimension(windowHandle);
    StretchDIBits(deviceContext, 0, 0, dimension.width, dimension.height, 0, 0, buffer->width, buffer->height, buffer->memory, &buffer->bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
    //BitBlt(deviceContext, 0, 0, dimension.width, dimension.height, );
}

static void RenderGraphic(bitmap_buffer *buffer, int xOffset, int yOffset)
{
    int width = buffer->width;     
    int height = buffer->height;
    

    uint8_t *row  = (uint8_t *)buffer->memory;

    for (int y = 0; y < height; y++)
    {
        uint32_t *pixel = (uint32_t *) row;
        for (int x = 0; x < width; x++)
        {
            uint8_t blue = (x + xOffset);
            uint8_t green = (y + yOffset);

            *pixel++ = ((green << 16) | blue);
        }

        row = (uint8_t *) pixel;

    }
}


void PollGamePadInput()
{
    if (SUCCEEDED(gamePadInput->GetCurrentReading(GameInputKindGamepad, gamePad, &inputReading)))
    {
        if (!gamePadInput)
        {
            inputReading->GetDevice(&gamePad);
        }

        inputReading->GetGamepadState(&gamePadState);
        inputReading->Release();
    }
    else if (gamePad)
    {
        gamePad->Release();
        gamePad = 0;
    }

}


