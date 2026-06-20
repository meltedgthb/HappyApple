#include <windows.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <mmsystem.h>
#include <shlobj.h>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "shell32.lib")

struct Particle {
    float x, y;
    float vx, vy;
    float alpha;
    COLORREF color;
};

std::vector<Particle> particles;

int W = 800, H = 600;

HBITMAP hBitmap = NULL;
HDC hdcMem = NULL;

std::wstring GetDownloadsPath() {
    PWSTR path = NULL;
    std::wstring result;

    if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Downloads, 0, NULL, &path))) {
        result = path;
        CoTaskMemFree(path);
    }

    return result;
}

void StartMusic() {
    std::wstring downloads = GetDownloadsPath();
    std::wstring mp3 = downloads + L"\\nyan.mp3";

    std::wstring cmd = L"open \"" + mp3 + L"\" type mpegvideo alias bgm";
    mciSendStringW(cmd.c_str(), NULL, 0, NULL);
    mciSendStringW(L"play bgm repeat", NULL, 0, NULL);
}

void StopMusic() {
    mciSendStringW(L"stop bgm", NULL, 0, NULL);
    mciSendStringW(L"close bgm", NULL, 0, NULL);
}

void ResizeBuffer(HWND hwnd) {
    if (hdcMem) DeleteDC(hdcMem);
    if (hBitmap) DeleteObject(hBitmap);

    HDC hdc = GetDC(hwnd);
    RECT r;
    GetClientRect(hwnd, &r);

    W = r.right;
    H = r.bottom;

    hdcMem = CreateCompatibleDC(hdc);
    hBitmap = CreateCompatibleBitmap(hdc, W, H);
    SelectObject(hdcMem, hBitmap);

    ReleaseDC(hwnd, hdc);
}

void Spawn() {
    Particle p;
    p.x = rand() % W;
    p.y = rand() % H;
    p.vx = (rand() % 100 - 50) / 10.0f;
    p.vy = (rand() % 100 - 50) / 10.0f;
    p.alpha = 255;
    p.color = RGB(rand() % 255, rand() % 255, rand() % 255);
    particles.push_back(p);
}

void Update() {
    HBRUSH bg = CreateSolidBrush(RGB(0, 0, 0));
    RECT r = { 0,0,W,H };
    FillRect(hdcMem, &r, bg);
    DeleteObject(bg);

    for (size_t i = 0; i < particles.size();) {
        auto &p = particles[i];

        p.x += p.vx;
        p.y += p.vy;
        p.alpha -= 2;

        if (p.alpha <= 0) {
            particles.erase(particles.begin() + i);
            continue;
        }

        SetPixel(hdcMem, (int)p.x, (int)p.y, p.color);
        i++;
    }
}

void Render(HWND hwnd) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    BitBlt(hdc, 0, 0, W, H, hdcMem, 0, 0, SRCCOPY);

    EndPaint(hwnd, &ps);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {

    case WM_CREATE:
        ResizeBuffer(hwnd);
        SetTimer(hwnd, 1, 16, NULL);
        StartMusic();
        return 0;

    case WM_SIZE:
        ResizeBuffer(hwnd);
        return 0;

    case WM_TIMER:
        Spawn();
        Spawn();
        Spawn();

        Update();
        InvalidateRect(hwnd, NULL, FALSE);
        return 0;

    case WM_PAINT:
        Render(hwnd);
        return 0;

    case WM_DESTROY:
        KillTimer(hwnd, 1);
        StopMusic();
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int) {
    srand((unsigned)time(0));

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = "iPhoneAnniversaryWindow";

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        "iPhoneAnniversaryWindow",
        "iPhone Anniversary",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        900, 700,
        NULL, NULL,
        hInst, NULL
    );

    ShowWindow(hwnd, SW_SHOW);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}