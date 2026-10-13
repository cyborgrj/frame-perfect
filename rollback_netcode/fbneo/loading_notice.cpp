#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string.h>
#include <wchar.h>

#include "loading_notice.h"
#include "img_hash.h"

#define z80a93f8  L"RbfLoadingNotice"

#define z3925190 RGB(0x00, 0x00, 0x00)
#define z74d6d86    RGB(0x12, 0x14, 0x1A)
#define zab5ebb2  RGB(0x4D, 0xD0, 0xE1)
#define za4ba6b0  RGB(0xFF, 0xFF, 0xFF)
#define ze4c26b4  RGB(0x3A, 0x41, 0x50)

static HWND    zaff28a1 = NULL;
static int     z6f699e2 = 96;
static double  z185839a = 0.0;
static wchar_t z18507cf[96] = L"";
static unsigned long z32cace1 = 0;

static int S(int px)
{
	return MulDiv(px, z6f699e2, 96);
}

static void paint(HWND h, HDC dc)
{
	RECT rc;
	GetClientRect(h, &rc);
	const int W = rc.right, H = rc.bottom;
	if (W <= 0 || H <= 0) return;

	HDC mem = CreateCompatibleDC(dc);
	HBITMAP bmp = CreateCompatibleBitmap(dc, W, H);
	HGDIOBJ zadadd7e = SelectObject(mem, bmp);

	HBRUSH black = CreateSolidBrush(z3925190);
	FillRect(mem, &rc, black);
	DeleteObject(black);

	int pw = S(620);
	if (pw > W * 9 / 10) pw = W * 9 / 10;
	const int ph = S(190);
	const RECT p = { (W - pw) / 2, (H - ph) / 2, (W - pw) / 2 + pw, (H - ph) / 2 + ph };

	HBRUSH bg = CreateSolidBrush(z74d6d86);
	HPEN pen = CreatePen(PS_SOLID, S(3), zab5ebb2);
	HGDIOBJ z546cbdc = SelectObject(mem, bg);
	HGDIOBJ z3142160 = SelectObject(mem, pen);
	RoundRect(mem, p.left, p.top, p.right, p.bottom, S(24), S(24));
	SelectObject(mem, z3142160);
	SelectObject(mem, z546cbdc);
	DeleteObject(pen);
	DeleteObject(bg);

	HFONT font = CreateFontW(-S(34), 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
	                         OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
	                         DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
	HGDIOBJ ze9bdf74 = SelectObject(mem, font);
	SetBkMode(mem, TRANSPARENT);
	SetTextColor(mem, za4ba6b0);
	RECT t = { p.left, p.top + S(34), p.right, p.top + S(34) + S(50) };
	DrawTextW(mem, z18507cf, -1, &t, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
	SelectObject(mem, ze9bdf74);
	DeleteObject(font);

	const RECT track = { p.left + S(56), p.top + S(118), p.right - S(56), p.top + S(118) + S(18) };
	HBRUSH edge = CreateSolidBrush(ze4c26b4);
	HGDIOBJ zd4d8941 = SelectObject(mem, edge);
	HGDIOBJ z87f0081 = SelectObject(mem, GetStockObject(NULL_PEN));
	RoundRect(mem, track.left, track.top, track.right + 1, track.bottom + 1, S(18), S(18));
	double f = z185839a < 0 ? 0 : z185839a > 1 ? 1 : z185839a;
	const int fill = (int)((track.right - track.left) * f + 0.5);
	if (fill > 0) {
		HBRUSH cyan = CreateSolidBrush(zab5ebb2);
		SelectObject(mem, cyan);
		RoundRect(mem, track.left, track.top, track.left + fill + 1, track.bottom + 1, S(18), S(18));
		SelectObject(mem, edge);
		DeleteObject(cyan);
	}
	SelectObject(mem, z87f0081);
	SelectObject(mem, zd4d8941);
	DeleteObject(edge);

	BitBlt(dc, 0, 0, W, H, mem, 0, 0, SRCCOPY);
	SelectObject(mem, zadadd7e);
	DeleteObject(bmp);
	DeleteDC(mem);
}

static LRESULT CALLBACK z6f26fb8(HWND h, UINT m, WPARAM wp, LPARAM lp)
{
	switch (m) {
		case WM_PAINT: {
			PAINTSTRUCT ps;
			HDC dc = BeginPaint(h, &ps);
			paint(h, dc);
			EndPaint(h, &ps);
			return 0;
		}
		case WM_ERASEBKGND:
			return 1;
		case WM_MOUSEACTIVATE:
			return MA_NOACTIVATE;
	}
	return DefWindowProcW(h, m, wp, lp);
}

void zbd03061(void* zf7534e9, const wchar_t* zaa888a7)
{
	if (zaff28a1) return;
	wcsncpy(z18507cf, zaa888a7 ? zaa888a7 : L"", 95);
	z18507cf[95] = 0;
	z185839a = 0.0;

	HINSTANCE inst = GetModuleHandleW(NULL);
	static int registered = 0;
	if (!registered) {
		WNDCLASSW wc;
		memset(&wc, 0, sizeof(wc));
		wc.lpfnWndProc   = z6f26fb8;
		wc.hInstance     = inst;
		wc.hCursor       = NULL;
		wc.lpszClassName = z80a93f8;
		registered = RegisterClassW(&wc) != 0;
		if (!registered) z32cace1 = GetLastError();
	}

	typedef HANDLE (WINAPI *z0e74931)(HANDLE);
	typedef UINT   (WINAPI *za1a7f1e)(HWND);
	HMODULE u32 = GetModuleHandleW(L"user32.dll");
	z0e74931 z368437a = u32 ? (z0e74931)GetProcAddress(u32, "SetThreadDpiAwarenessContext") : NULL;
	za1a7f1e zf19a91e = u32 ? (za1a7f1e)GetProcAddress(u32, "GetDpiForWindow") : NULL;
	HANDLE ze30c210 = z368437a ? z368437a((HANDLE)(INT_PTR)-4) : NULL;

	HWND game = (HWND)zf7534e9;
	RECT r;
	if (!game || !IsWindow(game) || !GetWindowRect(game, &r) || r.right <= r.left) {
		HMONITOR mon = MonitorFromWindow(game ? game : GetDesktopWindow(), MONITOR_DEFAULTTOPRIMARY);
		MONITORINFO mi;
		memset(&mi, 0, sizeof(mi));
		mi.cbSize = sizeof(mi);
		GetMonitorInfoW(mon, &mi);
		r = mi.rcMonitor;
	}
	if (zfe62411()) OffsetRect(&r, -8000 - r.left, -8000 - r.top);

	zaff28a1 = CreateWindowExW(WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE, z80a93f8, L"Frame Perfect",
	                        WS_POPUP, r.left, r.top, r.right - r.left, r.bottom - r.top,
	                        NULL, NULL, inst, NULL);
	if (!zaff28a1) z32cace1 = GetLastError();
	if (zaff28a1) {
		z6f699e2 = zf19a91e ? (int)zf19a91e(zaff28a1) : 96;
		if (z6f699e2 < 96) z6f699e2 = 96;
		SetWindowPos(zaff28a1, HWND_TOPMOST, r.left, r.top, r.right - r.left, r.bottom - r.top,
		             SWP_SHOWWINDOW | SWP_NOACTIVATE);
		UpdateWindow(zaff28a1);
	}
	if (z368437a && ze30c210) z368437a(ze30c210);
}

void zea13d64(double p)
{
	if (!zaff28a1) return;
	z185839a = p;
	InvalidateRect(zaff28a1, NULL, FALSE);
	UpdateWindow(zaff28a1);
}

void zba9d2b1(const wchar_t* zaa888a7)
{
	wcsncpy(z18507cf, zaa888a7 ? zaa888a7 : L"", 95);
	z18507cf[95] = 0;
	if (!zaff28a1) return;
	InvalidateRect(zaff28a1, NULL, FALSE);
	UpdateWindow(zaff28a1);
}

void z0b86b9d(void)
{
	if (!zaff28a1) return;
	DestroyWindow(zaff28a1);
	zaff28a1 = NULL;
}

int zef1a6d1(void) { return zaff28a1 != NULL; }
unsigned long zb56cb39(void) { return z32cace1; }
