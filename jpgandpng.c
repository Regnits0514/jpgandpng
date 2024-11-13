#include <windows.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")

using namespace Gdiplus;

const wchar_t* IMAGE_PATH = L"jcshim.jpg";  // 확장자 변경 가능
int imageWidth = 0;
int imageHeight = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_PAINT)
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        Graphics graphics(hdc);
        Image image(IMAGE_PATH);  // 여기서 이미지 파일 경로에 관계없이 자동으로 처리
        graphics.DrawImage(&image, 0, 0);
        EndPaint(hWnd, &ps);
    }
    else if (message == WM_DESTROY)
    {
        PostQuitMessage(0);
    }
    else
    {
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // 이미지 크기 얻기
    Image image(IMAGE_PATH);
    imageWidth = image.GetWidth();
    imageHeight = image.GetHeight();

    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"ImageViewer";
    RegisterClass(&wc);

    // 윈도우 스타일에 따른 추가 여백 계산
    RECT rect = { 0, 0, imageWidth, imageHeight };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
    int windowWidth = rect.right - rect.left;
    int windowHeight = rect.bottom - rect.top;

    HWND hwnd = CreateWindow(L"ImageViewer", L"Image Viewer", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, windowWidth, windowHeight, NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, nCmdShow);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);
    return 0;
}