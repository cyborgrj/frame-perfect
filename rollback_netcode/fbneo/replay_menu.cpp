#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string.h>
#include <wchar.h>
#include <stdlib.h>

#include "replay_menu.h"

#define z0ba2714  L"RbfReplayMenu"
#define zf21aff9  L"RbfReplayBadge"

#define z24a3442   RGB(0x01, 0x02, 0x03)
#define z74d6d86    RGB(0x12, 0x14, 0x1A)
#define zab5ebb2  RGB(0x4D, 0xD0, 0xE1)
#define za4ba6b0  RGB(0xFF, 0xFF, 0xFF)
#define z3d3aae5   RGB(0x9A, 0xA3, 0xB5)
#define ze4c26b4  RGB(0x3A, 0x41, 0x50)
#define zef0b2db RGB(0x4D, 0xD0, 0xE1)
#define zd683b61 RGB(0x0B, 0x0D, 0x14)

#define za0d7924 160

static HWND    z0da889e = NULL;
static HWND    z51076ca = NULL;
static int     z6f699e2 = 96;
static wchar_t ze85f91f[128] = L"";
static wchar_t z383bc1b[256] = L"";
static wchar_t* zfdaa8e2[za0d7924];
static wchar_t* z5b45830[za0d7924];
static int     z88507af = 0;
static int     z12900a4 = 0;
static int     zac1fa47 = 0;
static wchar_t zbd6a90f[64] = L"";

static int S(int px) { return MulDiv(px, z6f699e2, 96); }

static HFONT font(int px, int weight)
{
	return CreateFontW(-S(px), 0, 0, 0, weight, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
	                   OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
	                   DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
}

static void z8ef0707(HDC dc, const RECT* r, COLORREF fill, COLORREF edge, int pen, int radius)
{
	HBRUSH br = CreateSolidBrush(fill);
	HPEN pn = pen > 0 ? CreatePen(PS_SOLID, pen, edge) : NULL;
	HGDIOBJ z546cbdc = SelectObject(dc, br);
	HGDIOBJ z3142160 = SelectObject(dc, pn ? (HGDIOBJ)pn : GetStockObject(NULL_PEN));
	RoundRect(dc, r->left, r->top, r->right + (pn ? 0 : 1), r->bottom + (pn ? 0 : 1), radius, radius);
	SelectObject(dc, z3142160);
	SelectObject(dc, z546cbdc);
	if (pn) DeleteObject(pn);
	DeleteObject(br);
}

static void text(HDC dc, HFONT f, COLORREF col, const wchar_t* s, RECT r, UINT fmt)
{
	HGDIOBJ old = SelectObject(dc, f);
	SetTextColor(dc, col);
	DrawTextW(dc, s, -1, &r, fmt | DT_NOPREFIX);
	SelectObject(dc, old);
}

static void z84826f8(HWND h, HDC dc)
{
	RECT rc;
	GetClientRect(h, &rc);
	const int W = rc.right, H = rc.bottom;
	if (W <= 0 || H <= 0) return;

	HDC mem = CreateCompatibleDC(dc);
	HBITMAP bmp = CreateCompatibleBitmap(dc, W, H);
	HGDIOBJ zadadd7e = SelectObject(mem, bmp);
	HBRUSH key = CreateSolidBrush(z24a3442);
	FillRect(mem, &rc, key);
	DeleteObject(key);
	SetBkMode(mem, TRANSPARENT);

	const int z7a61baa  = S(54);
	const int z88624c0 = S(78);
	const int z3b8420f = z383bc1b[0] ? S(54) : S(22);
	int pw = S(640);
	if (pw > W * 9 / 10) pw = W * 9 / 10;
	int z9f7bc52 = (H * 8 / 10 - z88624c0 - z3b8420f) / z7a61baa;
	if (z9f7bc52 < 3) z9f7bc52 = 3;
	const int rows = z88507af < z9f7bc52 ? z88507af : z9f7bc52;
	const int ph = z88624c0 + rows * z7a61baa + z3b8420f + S(12);

	if (z12900a4 < zac1fa47) zac1fa47 = z12900a4;
	if (z12900a4 >= zac1fa47 + rows) zac1fa47 = z12900a4 - rows + 1;
	if (zac1fa47 > z88507af - rows) zac1fa47 = z88507af - rows;
	if (zac1fa47 < 0) zac1fa47 = 0;

	const RECT p = { (W - pw) / 2, (H - ph) / 2, (W - pw) / 2 + pw, (H - ph) / 2 + ph };
	z8ef0707(mem, &p, z74d6d86, zab5ebb2, S(3), S(24));

	HFONT z6581d70 = font(28, FW_SEMIBOLD);
	HFONT zdf9ed90  = font(21, FW_SEMIBOLD);
	HFONT z0e77c4c   = font(14, FW_NORMAL);
	HFONT z7c27689  = font(14, FW_NORMAL);

	RECT t = { p.left + S(30), p.top + S(18), p.right - S(30), p.top + z88624c0 - S(8) };
	text(mem, z6581d70, za4ba6b0, ze85f91f, t, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);

	const int scroll = z88507af > rows;
	const int left = p.left + S(18), right = p.right - S(scroll ? 30 : 18);
	for (int i = 0; i < rows; i++) {
		const int k = zac1fa47 + i;
		RECT r = { left, p.top + z88624c0 + i * z7a61baa, right, p.top + z88624c0 + (i + 1) * z7a61baa - S(6) };
		const int sel = (k == z12900a4);
		if (sel) z8ef0707(mem, &r, zef0b2db, zef0b2db, 0, S(14));
		const int z03bd248 = z5b45830[k] && z5b45830[k][0];
		RECT ri = { r.left + S(16), r.top, r.right - S(12), z03bd248 ? r.top + (r.bottom - r.top) * 6 / 10 : r.bottom };
		text(mem, zdf9ed90, sel ? zd683b61 : za4ba6b0, zfdaa8e2[k] ? zfdaa8e2[k] : L"", ri,
		     DT_LEFT | (z03bd248 ? DT_BOTTOM : DT_VCENTER) | DT_SINGLELINE | DT_END_ELLIPSIS);
		if (z03bd248) {
			RECT rs = { r.left + S(16), ri.bottom, r.right - S(12), r.bottom };
			text(mem, z0e77c4c, sel ? zd683b61 : z3d3aae5, z5b45830[k], rs, DT_LEFT | DT_TOP | DT_SINGLELINE | DT_END_ELLIPSIS);
		}
	}

	if (scroll) {
		const RECT tr = { p.right - S(22), p.top + z88624c0, p.right - S(16), p.top + z88624c0 + rows * z7a61baa - S(6) };
		z8ef0707(mem, &tr, ze4c26b4, ze4c26b4, 0, S(6));
		const int th = tr.bottom - tr.top;
		int kh = th * rows / z88507af;
		if (kh < S(24)) kh = S(24);
		const int z21ab708 = tr.top + (th - kh) * zac1fa47 / (z88507af - rows);
		const RECT kr = { tr.left, z21ab708, tr.right, z21ab708 + kh };
		z8ef0707(mem, &kr, zab5ebb2, zab5ebb2, 0, S(6));
	}

	if (z383bc1b[0]) {
		RECT f = { p.left + S(30), p.bottom - z3b8420f, p.right - S(30), p.bottom - S(10) };
		text(mem, z7c27689, z3d3aae5, z383bc1b, f, DT_LEFT | DT_VCENTER | DT_WORDBREAK);
	}

	DeleteObject(z6581d70);
	DeleteObject(zdf9ed90);
	DeleteObject(z0e77c4c);
	DeleteObject(z7c27689);

	BitBlt(dc, 0, 0, W, H, mem, 0, 0, SRCCOPY);
	SelectObject(mem, zadadd7e);
	DeleteObject(bmp);
	DeleteDC(mem);
}

static void zefe3839(HWND h, HDC dc)
{
	RECT rc;
	GetClientRect(h, &rc);
	HDC mem = CreateCompatibleDC(dc);
	HBITMAP bmp = CreateCompatibleBitmap(dc, rc.right, rc.bottom);
	HGDIOBJ zadadd7e = SelectObject(mem, bmp);
	HBRUSH key = CreateSolidBrush(z24a3442);
	FillRect(mem, &rc, key);
	DeleteObject(key);
	SetBkMode(mem, TRANSPARENT);
	z8ef0707(mem, &rc, z74d6d86, zab5ebb2, S(2), S(16));
	HFONT f = font(20, FW_BOLD);
	text(mem, f, za4ba6b0, zbd6a90f, rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	DeleteObject(f);
	BitBlt(dc, 0, 0, rc.right, rc.bottom, mem, 0, 0, SRCCOPY);
	SelectObject(mem, zadadd7e);
	DeleteObject(bmp);
	DeleteDC(mem);
}

static LRESULT CALLBACK zeb7e92f(HWND h, UINT m, WPARAM wp, LPARAM lp)
{
	switch (m) {
		case WM_PAINT: {
			PAINTSTRUCT ps;
			HDC dc = BeginPaint(h, &ps);
			if (h == z51076ca) zefe3839(h, dc); else z84826f8(h, dc);
			EndPaint(h, &ps);
			return 0;
		}
		case WM_ERASEBKGND:
			return 1;
		case WM_MOUSEACTIVATE:
			return MA_NOACTIVATE;
		case WM_NCHITTEST:
			return HTTRANSPARENT;
	}
	return DefWindowProcW(h, m, wp, lp);
}

static HWND z45ddf71(const wchar_t* cls, HWND game, const RECT* zda4085b, BYTE alpha)
{
	HINSTANCE inst = GetModuleHandleW(NULL);
	static int registered = 0;
	if (!registered) {
		WNDCLASSW wc;
		memset(&wc, 0, sizeof(wc));
		wc.lpfnWndProc   = zeb7e92f;
		wc.hInstance     = inst;
		wc.lpszClassName = z0ba2714;
		RegisterClassW(&wc);
		wc.lpszClassName = zf21aff9;
		RegisterClassW(&wc);
		registered = 1;
	}

	typedef HANDLE (WINAPI *z0e74931)(HANDLE);
	typedef UINT   (WINAPI *za1a7f1e)(HWND);
	HMODULE u32 = GetModuleHandleW(L"user32.dll");
	z0e74931 z368437a = u32 ? (z0e74931)GetProcAddress(u32, "SetThreadDpiAwarenessContext") : NULL;
	za1a7f1e zf19a91e = u32 ? (za1a7f1e)GetProcAddress(u32, "GetDpiForWindow") : NULL;
	HANDLE ze30c210 = z368437a ? z368437a((HANDLE)(INT_PTR)-4) : NULL;

	RECT r = *zda4085b;
	HWND w = CreateWindowExW(WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE | WS_EX_LAYERED | WS_EX_TRANSPARENT,
	                         cls, L"Frame Perfect", WS_POPUP, r.left, r.top, r.right - r.left, r.bottom - r.top,
	                         NULL, NULL, inst, NULL);
	if (w) {
		z6f699e2 = zf19a91e ? (int)zf19a91e(w) : 96;
		if (z6f699e2 < 96) z6f699e2 = 96;
		SetLayeredWindowAttributes(w, z24a3442, alpha, LWA_COLORKEY | LWA_ALPHA);
	}
	if (z368437a && ze30c210) z368437a(ze30c210);
	return w;
}

static void za615c45(HWND game, RECT* r)
{
	if (!game || !IsWindow(game) || !GetWindowRect(game, r) || r->right <= r->left) {
		HMONITOR mon = MonitorFromWindow(game ? game : GetDesktopWindow(), MONITOR_DEFAULTTOPRIMARY);
		MONITORINFO mi;
		memset(&mi, 0, sizeof(mi));
		mi.cbSize = sizeof(mi);
		GetMonitorInfoW(mon, &mi);
		*r = mi.rcMonitor;
	}
}

void zdd07d90(void* zf7534e9)
{
	if (z0da889e) return;
	RECT r;
	za615c45((HWND)zf7534e9, &r);
	z0da889e = z45ddf71(z0ba2714, (HWND)zf7534e9, &r, 240);
	if (z0da889e) {
		SetWindowPos(z0da889e, HWND_TOPMOST, r.left, r.top, r.right - r.left, r.bottom - r.top,
		             SWP_SHOWWINDOW | SWP_NOACTIVATE);
		UpdateWindow(z0da889e);
	}
}

void zde9f9af(void)
{
	if (!z0da889e) return;
	DestroyWindow(z0da889e);
	z0da889e = NULL;
}

int z16194cb(void) { return z0da889e != NULL; }

static wchar_t* dup(const wchar_t* s)
{
	if (!s) return NULL;
	size_t n = wcslen(s);
	wchar_t* d = (wchar_t*)malloc((n + 1) * sizeof(wchar_t));
	if (d) memcpy(d, s, (n + 1) * sizeof(wchar_t));
	return d;
}

void zec06d2b(const wchar_t* szTitle, const wchar_t* const* zf1c8326, const wchar_t* const* z9a20f86,
                    int nItems, int nSel, const wchar_t* zaefe29f)
{
	for (int i = 0; i < z88507af; i++) {
		free(zfdaa8e2[i]); zfdaa8e2[i] = NULL;
		free(z5b45830[i]);  z5b45830[i] = NULL;
	}
	if (nItems > za0d7924) nItems = za0d7924;
	if (nItems < 0) nItems = 0;
	for (int i = 0; i < nItems; i++) {
		zfdaa8e2[i] = dup(zf1c8326 ? zf1c8326[i] : L"");
		z5b45830[i]  = dup(z9a20f86 ? z9a20f86[i] : NULL);
	}
	z88507af = nItems;
	z12900a4 = nSel < 0 ? -1 : (nSel >= nItems ? nItems - 1 : nSel);
	wcsncpy(ze85f91f, szTitle ? szTitle : L"", 127);
	ze85f91f[127] = 0;
	wcsncpy(z383bc1b, zaefe29f ? zaefe29f : L"", 255);
	z383bc1b[255] = 0;
	if (z0da889e) {
		InvalidateRect(z0da889e, NULL, FALSE);
		UpdateWindow(z0da889e);
	}
}

void zc0b38b8(void* zf7534e9, const wchar_t* szText)
{
	if (!szText || !szText[0]) {
		if (z51076ca) { DestroyWindow(z51076ca); z51076ca = NULL; }
		zbd6a90f[0] = 0;
		return;
	}
	if (z51076ca && wcscmp(zbd6a90f, szText) == 0) return;
	wcsncpy(zbd6a90f, szText, 63);
	zbd6a90f[63] = 0;
	if (!z51076ca) {
		RECT gr;
		za615c45((HWND)zf7534e9, &gr);
		RECT r = gr;
		z51076ca = z45ddf71(zf21aff9, (HWND)zf7534e9, &r, 230);
		if (!z51076ca) return;
		const int w = S(190), h = S(46), m = S(24);
		SetWindowPos(z51076ca, HWND_TOPMOST, gr.right - w - m, gr.top + m + S(40), w, h,
		             SWP_SHOWWINDOW | SWP_NOACTIVATE);
	}
	InvalidateRect(z51076ca, NULL, FALSE);
	UpdateWindow(z51076ca);
}
