#include <windows.h>
#include <wchar.h>

#include "end_notice.h"
#include "lang.h"
#include "img_hash.h"

#define z5944ea4  L"RbfEndNotice"
#define zdb55ed0  1

static const wchar_t* z1ae5640;
static const wchar_t* z465a5ee;
static int  z70f7193;
static int  z3110ed4;
static int  ze97fd94 = 96;
static DWORD z52ffb8f;
static int  z916e639;

static int S(int px) { return MulDiv(px, ze97fd94, 96); }

static void z18ef5e4(wchar_t* out, size_t n)
{
	int left = z3110ed4 < 1 ? 1 : z3110ed4;
	_snwprintf(out, n, z6f2968c(L"voltando para o lobby em %d..."), left);
	out[n - 1] = 0;
}

static HFONT z30ceb2d(int px, int weight)
{
	return CreateFontW(-S(px), 0, 0, 0, weight, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
	                   OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
	                   DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
}

static void line(HDC dc, HFONT f, COLORREF c, const wchar_t* text, int top, int height, int width)
{
	RECT r = { S(24), top, width - S(24), top + height };
	HGDIOBJ old = SelectObject(dc, f);
	SetTextColor(dc, c);
	DrawTextW(dc, text, -1, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX | DT_END_ELLIPSIS);
	SelectObject(dc, old);
}

#define z17549fd  48
#define z7edb86e 40
#define zb56bf8a 30

static int zcf2c43f(HDC dc, HFONT f, const wchar_t* text, int width)
{
	if (!text || !text[0]) return 0;
	RECT r = { 0, 0, width - 2 * S(z17549fd), 0 };
	HGDIOBJ old = SelectObject(dc, f);
	DrawTextW(dc, text, -1, &r, DT_CENTER | DT_WORDBREAK | DT_NOPREFIX | DT_CALCRECT);
	SelectObject(dc, old);
	return r.bottom - r.top;
}

static void z52b8b2e(HDC dc, HFONT f, COLORREF c, const wchar_t* text, int top, int height, int width)
{
	RECT r = { S(z17549fd), top, width - S(z17549fd), top + height };
	HGDIOBJ old = SelectObject(dc, f);
	SetTextColor(dc, c);
	DrawTextW(dc, text, -1, &r, DT_CENTER | DT_WORDBREAK | DT_NOPREFIX);
	SelectObject(dc, old);
}

static void z2a94674(HDC dc, int width, int* pHead, int* ze8c0695, int* pCount)
{
	HFONT z5b4a22e   = z30ceb2d(z7edb86e, FW_BOLD);
	HFONT zf611102 = z30ceb2d(zb56bf8a, FW_SEMIBOLD);
	*pHead   = zcf2c43f(dc, z5b4a22e, z1ae5640, width);
	*ze8c0695 = z465a5ee ? zcf2c43f(dc, zf611102, z465a5ee, width) : 0;
	*pCount  = S(46);
	DeleteObject(z5b4a22e);
	DeleteObject(zf611102);
}

static int z49b5402(int head, int detail, int count)
{
	return head + (detail ? S(16) + detail : 0) + S(20) + count;
}

static void paint(HWND h, HDC dc)
{
	RECT rc;
	GetClientRect(h, &rc);
	const int w = rc.right, hh = rc.bottom;
	if (w <= 0 || hh <= 0) return;

	HDC mem = CreateCompatibleDC(dc);
	HBITMAP bmp = CreateCompatibleBitmap(dc, w, hh);
	HGDIOBJ zadadd7e = SelectObject(mem, bmp);

	HBRUSH bg = CreateSolidBrush(RGB(0x12, 0x14, 0x1A));
	FillRect(mem, &rc, bg);
	DeleteObject(bg);

	HPEN pen = CreatePen(PS_SOLID, S(3), RGB(0x4D, 0xD0, 0xE1));
	HGDIOBJ z3142160 = SelectObject(mem, pen);
	HGDIOBJ z546cbdc = SelectObject(mem, GetStockObject(NULL_BRUSH));
	RoundRect(mem, S(2), S(2), w - S(2), hh - S(2), S(24), S(24));
	SelectObject(mem, z546cbdc);
	SelectObject(mem, z3142160);
	DeleteObject(pen);

	SetBkMode(mem, TRANSPARENT);
	HFONT z5b4a22e   = z30ceb2d(z916e639 ? z7edb86e : 60, FW_BOLD);
	HFONT zf611102 = z30ceb2d(z916e639 ? zb56bf8a : 34, FW_SEMIBOLD);
	HFONT fCount  = z30ceb2d(30, FW_SEMIBOLD);

	wchar_t count[96];
	z18ef5e4(count, sizeof(count) / sizeof(count[0]));

	if (z916e639) {
		int z88624c0, za954317, z5dc7df5;
		z2a94674(mem, w, &z88624c0, &za954317, &z5dc7df5);
		int y = (hh - z49b5402(z88624c0, za954317, z5dc7df5)) / 2;
		z52b8b2e(mem, z5b4a22e, RGB(0xFF, 0xFF, 0xFF), z1ae5640 ? z1ae5640 : L"", y, z88624c0, w);
		y += z88624c0;
		if (z465a5ee) {
			y += S(16);
			z52b8b2e(mem, zf611102, RGB(0xE7, 0xEA, 0xF0), z465a5ee, y, za954317, w);
			y += za954317;
		}
		y += S(20);
		line(mem, fCount, RGB(0x4D, 0xD0, 0xE1), count, y, z5dc7df5, w);
	} else {
		const int z88624c0 = S(84), za954317 = S(50), z5dc7df5 = S(46);
		int total = z88624c0 + z5dc7df5 + S(14) + (z465a5ee ? za954317 + S(4) : 0);
		int y = (hh - total) / 2;
		line(mem, z5b4a22e, RGB(0xFF, 0xFF, 0xFF), z1ae5640 ? z1ae5640 : L"", y, z88624c0, w);
		y += z88624c0;
		if (z465a5ee) {
			y += S(4);
			line(mem, zf611102, RGB(0xE7, 0xEA, 0xF0), z465a5ee, y, za954317, w);
			y += za954317;
		}
		y += S(14);
		line(mem, fCount, RGB(0x4D, 0xD0, 0xE1), count, y, z5dc7df5, w);
	}

	DeleteObject(z5b4a22e);
	DeleteObject(zf611102);
	DeleteObject(fCount);

	BitBlt(dc, 0, 0, w, hh, mem, 0, 0, SRCCOPY);
	SelectObject(mem, zadadd7e);
	DeleteObject(bmp);
	DeleteDC(mem);
}

static LRESULT CALLBACK z3fc4aa9(HWND h, UINT m, WPARAM wp, LPARAM lp)
{
	switch (m) {
	case WM_TIMER:
		if (wp == zdb55ed0) {
			if (--z3110ed4 <= 0) { DestroyWindow(h); return 0; }
			InvalidateRect(h, NULL, FALSE);
		}
		return 0;
	case WM_KEYDOWN:
		if ((wp == VK_RETURN || wp == VK_ESCAPE || wp == VK_SPACE) &&
		    !(lp & 0x40000000) && GetTickCount() - z52ffb8f > 500)
			DestroyWindow(h);
		return 0;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		DestroyWindow(h);
		return 0;
	case WM_ERASEBKGND:
		return 1;
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC dc = BeginPaint(h, &ps);
		paint(h, dc);
		EndPaint(h, &ps);
		return 0;
	}
	case WM_PRINTCLIENT:
		paint(h, (HDC)wp);
		return 0;
	case WM_DESTROY:
		KillTimer(h, zdb55ed0);
		return 0;
	}
	return DefWindowProcW(h, m, wp, lp);
}

void z22ec6fe(const char* sz, const wchar_t* szFallback, wchar_t* out, int n)
{
	if (!out || n < 1) return;
	if (!sz || !sz[0] || MultiByteToWideChar(CP_ACP, 0, sz, -1, out, n) <= 0)
		wcsncpy(out, szFallback ? szFallback : L"", n);
	out[n - 1] = 0;
}

static void show(const wchar_t* zaa888a7, const wchar_t* z6922496, int nSeconds, int zeab096a)
{
	if (nSeconds < 1) nSeconds = 1;
	if (nSeconds > 9) nSeconds = 9;

	z1ae5640   = zaa888a7 ? zaa888a7 : L"";
	z465a5ee = (z6922496 && z6922496[0]) ? z6922496 : NULL;
	z70f7193  = z3110ed4 = nSeconds;
	z916e639   = zeab096a;
	const int za56ce87 = zfe62411();

	HINSTANCE inst = GetModuleHandleW(NULL);
	static int registered = 0;
	if (!registered) {
		WNDCLASSW wc;
		memset(&wc, 0, sizeof(wc));
		wc.lpfnWndProc   = z3fc4aa9;
		wc.hInstance     = inst;
		wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
		wc.lpszClassName = z5944ea4;
		registered = RegisterClassW(&wc) != 0;
	}

	typedef HANDLE (WINAPI *z0e74931)(HANDLE);
	typedef UINT   (WINAPI *za1a7f1e)(HWND);
	HMODULE u32 = GetModuleHandleW(L"user32.dll");
	z0e74931 z368437a = u32 ? (z0e74931)GetProcAddress(u32, "SetThreadDpiAwarenessContext") : NULL;
	za1a7f1e zf19a91e = u32 ? (za1a7f1e)GetProcAddress(u32, "GetDpiForWindow") : NULL;
	HANDLE ze30c210 = z368437a ? z368437a((HANDLE)(INT_PTR)-4) : NULL;

	HWND fg = GetForegroundWindow();
	HMONITOR mon = MonitorFromWindow(fg ? fg : GetDesktopWindow(), MONITOR_DEFAULTTOPRIMARY);
	MONITORINFO mi;
	memset(&mi, 0, sizeof(mi));
	mi.cbSize = sizeof(mi);
	GetMonitorInfoW(mon, &mi);

	HWND h = CreateWindowExW(za56ce87 ? (WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE) : (WS_EX_TOPMOST | WS_EX_TOOLWINDOW),
	                         z5944ea4, L"Frame Perfect", WS_POPUP,
	                         za56ce87 ? -8000 : mi.rcMonitor.left, za56ce87 ? -8000 : mi.rcMonitor.top, 10, 10,
	                         NULL, NULL, inst, NULL);
	if (h) {
		ze97fd94 = zf19a91e ? (int)zf19a91e(h) : 96;
		if (ze97fd94 < 96) ze97fd94 = 96;
		const int z9409772 = mi.rcMonitor.right - mi.rcMonitor.left;
		const int z952ba09 = mi.rcMonitor.bottom - mi.rcMonitor.top;
		int w = S(z916e639 ? 900 : 820);
		if (z9409772 > 0 && w > z9409772 * 9 / 10) w = z9409772 * 9 / 10;
		int hh = S(z465a5ee ? 280 : 228);
		if (z916e639) {
			HDC dc = GetDC(h);
			int z88624c0, za954317, z5dc7df5;
			z2a94674(dc, w, &z88624c0, &za954317, &z5dc7df5);
			ReleaseDC(h, dc);
			hh = z49b5402(z88624c0, za954317, z5dc7df5) + 2 * S(40);
		}
		if (z952ba09 > 0 && hh > z952ba09 * 9 / 10) hh = z952ba09 * 9 / 10;
		if (za56ce87) {
			SetWindowPos(h, NULL, -8000, -8000, w, hh, SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOZORDER);
		} else {
			SetWindowPos(h, HWND_TOPMOST, mi.rcMonitor.left + (z9409772 - w) / 2, mi.rcMonitor.top + (z952ba09 - hh) / 2,
			             w, hh, SWP_SHOWWINDOW);
			SetForegroundWindow(h);
		}
		SetTimer(h, zdb55ed0, 1000, NULL);
		z52ffb8f = GetTickCount();
	}
	if (z368437a && ze30c210) z368437a(ze30c210);
	if (!h) { z1ae5640 = z465a5ee = NULL; z916e639 = 0; return; }

	MSG msg;
	int quit = 0;
	WPARAM z4d48448 = 0;
	while (IsWindow(h)) {
		BOOL r = GetMessageW(&msg, NULL, 0, 0);
		if (r == 0) { quit = 1; z4d48448 = msg.wParam; break; }
		if (r < 0) break;
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	if (IsWindow(h)) DestroyWindow(h);
	if (quit) PostQuitMessage((int)z4d48448);

	z1ae5640 = z465a5ee = NULL;
	z916e639 = 0;
}

void z639f64f(const wchar_t* zaa888a7, const wchar_t* z6922496, int nSeconds)
{
	show(zaa888a7, z6922496, nSeconds, 0);
}

void zd0938e8(const wchar_t* zaa888a7, const wchar_t* z6922496, int nSeconds)
{
	show(zaa888a7, z6922496, nSeconds, 1);
}
