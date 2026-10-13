#include <windows.h>
#include <stdlib.h>
#include <wchar.h>

#include "training_notice.h"
#include "lang.h"

#define z21fcd44      L"RbfTrainingNotice"
#define zfec9376  1
#define ze2a5418  2

#define z74d6d86      RGB(0x12, 0x14, 0x1A)
#define zab5ebb2    RGB(0x4D, 0xD0, 0xE1)
#define za4ba6b0    RGB(0xFF, 0xFF, 0xFF)
#define z749c406    RGB(0xC9, 0xCF, 0xDA)
#define z3d3aae5     RGB(0x8E, 0x97, 0xA8)
#define ze4c26b4    RGB(0x3A, 0x41, 0x50)

enum { z90131bd = 0, zda39eaf = 1 };

static struct {
	int     kind;
	int     dpi;
	double  scale;
	int     left;
	int     interacted;
	int     focus;
	int     nButtons;
	const wchar_t* label[2];
	int     action[2];
	RECT    z212301c[2];
	int     result;
	z953c9b9 pad;
	int     z5aa7ca5;
	int     zc692425;
	POINT   z018b98d;
	DWORD   zb1fdfa2;
} s;

static int S(int px)
{
	return (int)(MulDiv(px, s.dpi, 96) * s.scale + 0.5);
}

static HFONT z30ceb2d(int px, int weight)
{
	return CreateFontW(-S(px), 0, 0, 0, weight, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
	                   OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
	                   DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
}

static void z9e18520(HDC dc, HFONT f, COLORREF c, const wchar_t* t, RECT r, UINT fmt)
{
	HGDIOBJ old = SelectObject(dc, f);
	SetTextColor(dc, c);
	DrawTextW(dc, t, -1, &r, fmt | DT_NOPREFIX);
	SelectObject(dc, old);
}

static int textHeight(HDC dc, HFONT f, const wchar_t* t, int width)
{
	RECT r = { 0, 0, width, 0 };
	HGDIOBJ old = SelectObject(dc, f);
	DrawTextW(dc, t, -1, &r, DT_CALCRECT | DT_WORDBREAK | DT_NOPREFIX);
	SelectObject(dc, old);
	return r.bottom;
}

static void frame(HDC dc, int w, int h)
{
	RECT rc = { 0, 0, w, h };
	HBRUSH bg = CreateSolidBrush(z74d6d86);
	FillRect(dc, &rc, bg);
	DeleteObject(bg);

	HPEN pen = CreatePen(PS_SOLID, S(3), zab5ebb2);
	HGDIOBJ z3142160 = SelectObject(dc, pen);
	HGDIOBJ z546cbdc = SelectObject(dc, GetStockObject(NULL_BRUSH));
	RoundRect(dc, S(2), S(2), w - S(2), h - S(2), S(24), S(24));
	SelectObject(dc, z546cbdc);
	SelectObject(dc, z3142160);
	DeleteObject(pen);
}

static void buttons(HDC dc, int w, int top)
{
	const int bw = S(190), bh = S(52), gap = S(18);
	const int total = s.nButtons * bw + (s.nButtons - 1) * gap;
	int x = (w - total) / 2;
	HFONT f = z30ceb2d(22, FW_SEMIBOLD);

	for (int i = 0; i < s.nButtons; i++) {
		RECT r = { x, top, x + bw, top + bh };
		s.z212301c[i] = r;
		const int on = (s.focus == i);
		HBRUSH br = CreateSolidBrush(on ? zab5ebb2 : z74d6d86);
		HPEN pen = CreatePen(PS_SOLID, S(2), on ? zab5ebb2 : ze4c26b4);
		HGDIOBJ z546cbdc = SelectObject(dc, br);
		HGDIOBJ z3142160 = SelectObject(dc, pen);
		RoundRect(dc, r.left, r.top, r.right, r.bottom, S(14), S(14));
		SelectObject(dc, z3142160);
		SelectObject(dc, z546cbdc);
		DeleteObject(pen);
		DeleteObject(br);
		z9e18520(dc, f, on ? z74d6d86 : za4ba6b0, z6f2968c(s.label[i]), r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		x += bw + gap;
	}
	DeleteObject(f);
}

static void z82cf2cc(HDC dc, int w, int h)
{
	frame(dc, w, h);

	HFONT z023821e = z30ceb2d(15, FW_BOLD);
	HFONT z5b4a22e  = z30ceb2d(40, FW_BOLD);
	HFONT z0e77c4c   = z30ceb2d(22, FW_NORMAL);
	HFONT fCount = z30ceb2d(18, FW_SEMIBOLD);

	int y = S(26);
	RECT r = { S(28), y, w - S(28), y + S(22) };
	SetTextCharacterExtra(dc, S(3));
	z9e18520(dc, z023821e, zab5ebb2, z6f2968c(L"TREINO"), r, DT_CENTER | DT_SINGLELINE);
	SetTextCharacterExtra(dc, 0);
	y += S(22) + S(8);

	r.top = y; r.bottom = y + S(56);
	z9e18520(dc, z5b4a22e, za4ba6b0, z6f2968c(L"Pressione F6 para o treino avançado"), r,
	       DT_CENTER | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);
	y += S(56);

	r.top = y; r.bottom = y + S(32);
	z9e18520(dc, z0e77c4c, z749c406, z6f2968c(L"hitboxes, gravação do adversário e menu de opções"), r,
	       DT_CENTER | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);
	y += S(32) + S(24);

	buttons(dc, w, y);
	y += S(52) + S(12);

	if (s.left > 0 && !s.interacted) {
		wchar_t t[64];
		_snwprintf(t, 63, z6f2968c(L"fecha sozinho em %d"), s.left);
		t[63] = 0;
		r.top = y; r.bottom = y + S(26);
		z9e18520(dc, fCount, zab5ebb2, t, r, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	}

	DeleteObject(z023821e);
	DeleteObject(z5b4a22e);
	DeleteObject(z0e77c4c);
	DeleteObject(fCount);
}

struct zdb32abc { const wchar_t* key; const wchar_t* text; };
struct z2dc75c0 { const wchar_t* title; const zdb32abc* rows; int n; const wchar_t* note; };

static const zdb32abc zb2097c2[] = {
	{ L"F6",         L"liga o treino avançado; com ele ligado, abre e fecha o menu de opções" },
	{ L"F7",         L"liga e desliga os hitboxes (começam desligados)" },
	{ L"F3",         L"volta ao começo do treino" },
	{ L"F10 / F9",   L"salva / carrega o seu ponto do treino" },
	{ L"Esc",        L"sai do treino" },
};
static const zdb32abc z038a4b4[] = {
	{ L"1 toque",    L"repete a gravação" },
	{ L"2 toques",   L"começa e termina uma gravação" },
	{ L"3 toques",   L"troca de lado: você controla o P2, para gravar o que ele faz" },
	{ L"4 toques",   L"(ou segurar) abre o menu" },
};
static const zdb32abc zb0c749e[] = {
	{ L"← →",   L"escolhe a opção" },
	{ L"Botão A",    L"confirma" },
	{ L"Botão B",    L"explica a opção escolhida" },
	{ L"Botão C",    L"volta" },
	{ L"Coin",           L"volta ao jogo" },
};
static const zdb32abc zf368387[] = {
	{ L"Básico",     L"posição do HUD, tempo entre os toques do Coin, atraso das entradas, estado salvo, direção que o P2 segura, hitboxes" },
	{ L"Jogadores",      L"vida e barra do P1 e do P2: recarregar sempre, depois do combo ou nunca, e o valor máximo" },
	{ L"Gravação", L"slot, repetir em loop, ordem e início aleatórios, cortar o começo e o fim, quem é gravado, tocar ao apanhar ou ao carregar o estado, virar sozinho, salvar e carregar, editor" },
};

static const z2dc75c0 z122491e[] = {
	{ L"TECLAS", zb2097c2, sizeof(zb2097c2) / sizeof(zb2097c2[0]), NULL },
	{ L"COIN, COM O TREINO AVANÇADO LIGADO", z038a4b4, sizeof(z038a4b4) / sizeof(z038a4b4[0]), NULL },
};
static const z2dc75c0 z647342c[] = {
	{ L"NO MENU", zb0c749e, sizeof(zb0c749e) / sizeof(zb0c749e[0]),
	  L"Nos jogos da Capcom, A, B e C são o soco fraco, o médio e o forte." },
	{ L"OPÇÕES DO MENU", zf368387, sizeof(zf368387) / sizeof(zf368387[0]), NULL },
};

static int column(HDC dc, const z2dc75c0* sec, int nSec, int x, int y, int width, int bDraw)
{
	HFONT z5b4a22e = z30ceb2d(15, FW_BOLD);
	HFONT zfd65895  = z30ceb2d(19, FW_SEMIBOLD);
	HFONT fText = z30ceb2d(18, FW_NORMAL);
	HFONT z2e53e83 = z30ceb2d(16, FW_NORMAL);
	const int z5d13cbd = S(118), zc59ed7d = S(9), top = y;

	for (int i = 0; i < nSec; i++) {
		if (i > 0) y += S(22);
		RECT r = { x, y, x + width, y + S(22) };
		if (bDraw) {
			SetTextCharacterExtra(dc, S(2));
			z9e18520(dc, z5b4a22e, zab5ebb2, z6f2968c(sec[i].title), r, DT_LEFT | DT_SINGLELINE);
			SetTextCharacterExtra(dc, 0);
		}
		y += S(22) + S(8);

		for (int j = 0; j < sec[i].n; j++) {
			const int tw = width - z5d13cbd;
			const int th = textHeight(dc, fText, z6f2968c(sec[i].rows[j].text), tw);
			const int kh = S(26);
			const int rh = (th > kh) ? th : kh;
			if (bDraw) {
				RECT rk = { x, y, x + z5d13cbd - S(10), y + kh };
				z9e18520(dc, zfd65895, za4ba6b0, z6f2968c(sec[i].rows[j].key), rk, DT_LEFT | DT_SINGLELINE | DT_END_ELLIPSIS);
				RECT rt = { x + z5d13cbd, y + S(1), x + width, y + rh + S(1) };
				z9e18520(dc, fText, z749c406, z6f2968c(sec[i].rows[j].text), rt, DT_LEFT | DT_WORDBREAK);
			}
			y += rh + zc59ed7d;
		}
		if (sec[i].note) {
			const int nh = textHeight(dc, z2e53e83, z6f2968c(sec[i].note), width);
			if (bDraw) {
				RECT rn = { x, y, x + width, y + nh };
				z9e18520(dc, z2e53e83, z3d3aae5, z6f2968c(sec[i].note), rn, DT_LEFT | DT_WORDBREAK);
			}
			y += nh + zc59ed7d;
		}
	}

	DeleteObject(z5b4a22e);
	DeleteObject(zfd65895);
	DeleteObject(fText);
	DeleteObject(z2e53e83);
	return y - top;
}

static void zb223a2a(HDC dc, int w, int h)
{
	frame(dc, w, h);

	HFONT z6581d70 = z30ceb2d(32, FW_BOLD);
	HFONT z0e77c4c   = z30ceb2d(18, FW_NORMAL);

	const int pad = S(40), zbbd06e4 = S(44);
	int y = S(30);
	RECT r = { pad, y, w - pad, y + S(44) };
	z9e18520(dc, z6581d70, za4ba6b0, z6f2968c(L"Como usar o treino"), r, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
	y += S(44);
	r.top = y; r.bottom = y + S(28);
	z9e18520(dc, z0e77c4c, z3d3aae5, z6f2968c(L"O treino avançado começa desligado. Ligue no F6 quando quiser."), r,
	       DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);
	y += S(28) + S(22);

	const int zf11efbe = (w - 2 * pad - zbbd06e4) / 2;
	column(dc, z122491e, 2, pad, y, zf11efbe, 1);
	column(dc, z647342c, 2, pad + zf11efbe + zbbd06e4, y, zf11efbe, 1);

	buttons(dc, w, h - S(28) - S(52));

	DeleteObject(z6581d70);
	DeleteObject(z0e77c4c);
}

static int z62f635c(HDC dc, int w)
{
	const int pad = S(40), zbbd06e4 = S(44);
	const int zf11efbe = (w - 2 * pad - zbbd06e4) / 2;
	const int a = column(dc, z122491e, 2, pad, 0, zf11efbe, 0);
	const int b = column(dc, z647342c, 2, pad + zf11efbe + zbbd06e4, 0, zf11efbe, 0);
	return S(30) + S(44) + S(28) + S(22) + (a > b ? a : b) + S(20) + S(52) + S(28);
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
	SetBkMode(mem, TRANSPARENT);

	if (s.kind == zda39eaf) zb223a2a(mem, w, hh);
	else                     z82cf2cc(mem, w, hh);

	BitBlt(dc, 0, 0, w, hh, mem, 0, 0, SRCCOPY);
	SelectObject(mem, zadadd7e);
	DeleteObject(bmp);
	DeleteDC(mem);
}

static void touched(HWND h)
{
	if (!s.interacted) {
		s.interacted = 1;
		KillTimer(h, zfec9376);
		InvalidateRect(h, NULL, FALSE);
	}
}

static void zee97c5c(HWND h, int result)
{
	s.result = result;
	DestroyWindow(h);
}

static void activate(HWND h)
{
	zee97c5c(h, (s.focus >= 0 && s.focus < s.nButtons) ? s.action[s.focus] : z29cb4a5);
}

static void z698c72e(HWND h, int dir)
{
	touched(h);
	const int fechar = s.nButtons - 1;
	if (dir == 2) {
		if (s.focus < 0) s.focus = fechar;
	} else if (dir == -2) {
		if (s.kind == z90131bd) s.focus = -1;
	} else if (s.focus < 0) {
		s.focus = fechar;
	} else {
		s.focus += dir;
		if (s.focus < 0) s.focus = 0;
		if (s.focus >= s.nButtons) s.focus = s.nButtons - 1;
	}
	InvalidateRect(h, NULL, FALSE);
}

static int zb6db9a6(int x, int y)
{
	for (int i = 0; i < s.nButtons; i++) {
		POINT p = { x, y };
		if (PtInRect(&s.z212301c[i], p)) return i;
	}
	return -1;
}

static void za91ecac(HWND h)
{
	if (!s.pad) return;
	const int now = s.pad();
	s.zc692425 &= now;
	const int hit = now & ~s.z5aa7ca5 & ~s.zc692425;
	s.z5aa7ca5 = now;
	if (!hit) return;

	if (hit & z8c6eb8d)    { touched(h); activate(h); return; }
	if (hit & zdafc2d5)  { zee97c5c(h, z29cb4a5); return; }
	if (hit & z5e6ee0d)  z698c72e(h, 2);
	if (hit & z204ed0c)    z698c72e(h, -2);
	if (hit & z539e065)  z698c72e(h, -1);
	if (hit & z769d967) z698c72e(h, +1);
}

static LRESULT CALLBACK z311a70a(HWND h, UINT m, WPARAM wp, LPARAM lp)
{
	switch (m) {
	case WM_TIMER:
		if (wp == zfec9376) {
			if (--s.left <= 0) { zee97c5c(h, z29cb4a5); return 0; }
			InvalidateRect(h, NULL, FALSE);
		} else if (wp == ze2a5418) {
			za91ecac(h);
		}
		return 0;
	case WM_KEYDOWN:
		if (lp & 0x40000000) return 0;
		if (GetTickCount() - s.zb1fdfa2 < 300) return 0;
		switch (wp) {
		case VK_F6:                          zee97c5c(h, z1966f60); break;
		case VK_ESCAPE: case VK_BACK:        zee97c5c(h, z29cb4a5); break;
		case VK_RETURN: case VK_SPACE:       touched(h); activate(h); break;
		case VK_DOWN:                        z698c72e(h, 2); break;
		case VK_UP:                          z698c72e(h, -2); break;
		case VK_LEFT:                        z698c72e(h, -1); break;
		case VK_RIGHT: case VK_TAB:          z698c72e(h, +1); break;
		}
		return 0;
	case WM_MOUSEMOVE: {
		POINT p;
		GetCursorPos(&p);
		if (abs(p.x - s.z018b98d.x) + abs(p.y - s.z018b98d.y) > 6) {
			touched(h);
			const int b = zb6db9a6((short)LOWORD(lp), (short)HIWORD(lp));
			if (b >= 0 && b != s.focus) { s.focus = b; InvalidateRect(h, NULL, FALSE); }
		}
		return 0;
	}
	case WM_LBUTTONUP: {
		const int b = zb6db9a6((short)LOWORD(lp), (short)HIWORD(lp));
		if (b >= 0) { s.focus = b; touched(h); activate(h); }
		return 0;
	}
	case WM_SETCURSOR:
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		return TRUE;
	case WM_ERASEBKGND:
		return 1;
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC dc = BeginPaint(h, &ps);
		paint(h, dc);
		EndPaint(h, &ps);
		return 0;
	}
	case WM_DESTROY:
		KillTimer(h, zfec9376);
		KillTimer(h, ze2a5418);
		return 0;
	}
	return DefWindowProcW(h, m, wp, lp);
}

static void z28d322b(void)
{
	static const int keys[] = { VK_ESCAPE, VK_RETURN, VK_SPACE, VK_F6, VK_BACK };
	const DWORD t0 = GetTickCount();
	while (GetTickCount() - t0 < 500) {
		int held = s.pad ? s.pad() : 0;
		for (int i = 0; i < (int)(sizeof(keys) / sizeof(keys[0])); i++)
			if (GetAsyncKeyState(keys[i]) & 0x8000) held = 1;
		if (!held) break;
		Sleep(15);
	}
	MSG msg;
	while (PeekMessageW(&msg, NULL, WM_KEYFIRST, WM_KEYLAST, PM_REMOVE)) {}
}

static int run(HWND game, int kind, int nSeconds, z953c9b9 pad)
{
	memset(&s, 0, sizeof(s));
	s.kind   = kind;
	s.scale  = 1.0;
	s.focus  = -1;
	s.pad    = pad;
	s.result = z29cb4a5;
	s.left   = nSeconds > 0 ? nSeconds : 0;
	if (kind == zda39eaf) {
		s.nButtons = 1;
		s.label[0] = L"Fechar";  s.action[0] = z29cb4a5;
		s.focus = 0;
	} else {
		s.nButtons = 2;
		s.label[0] = L"Ver mais"; s.action[0] = z8c60dd0;
		s.label[1] = L"Fechar";   s.action[1] = z29cb4a5;
	}

	HINSTANCE inst = GetModuleHandleW(NULL);
	static int registered = 0;
	if (!registered) {
		WNDCLASSW wc;
		memset(&wc, 0, sizeof(wc));
		wc.lpfnWndProc   = z311a70a;
		wc.hInstance     = inst;
		wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
		wc.lpszClassName = z21fcd44;
		registered = RegisterClassW(&wc) != 0;
	}

	typedef HANDLE (WINAPI *z0e74931)(HANDLE);
	typedef UINT   (WINAPI *za1a7f1e)(HWND);
	HMODULE u32 = GetModuleHandleW(L"user32.dll");
	z0e74931 z368437a = u32 ? (z0e74931)GetProcAddress(u32, "SetThreadDpiAwarenessContext") : NULL;
	za1a7f1e zf19a91e = u32 ? (za1a7f1e)GetProcAddress(u32, "GetDpiForWindow") : NULL;
	HANDLE ze30c210 = z368437a ? z368437a((HANDLE)(INT_PTR)-4) : NULL;

	if (!game || !IsWindow(game)) game = GetForegroundWindow();
	HMONITOR mon = MonitorFromWindow(game ? game : GetDesktopWindow(), MONITOR_DEFAULTTOPRIMARY);
	MONITORINFO mi;
	memset(&mi, 0, sizeof(mi));
	mi.cbSize = sizeof(mi);
	GetMonitorInfoW(mon, &mi);

	HWND h = CreateWindowExW(WS_EX_TOPMOST | WS_EX_TOOLWINDOW, z21fcd44, L"Frame Perfect", WS_POPUP,
	                         mi.rcMonitor.left, mi.rcMonitor.top, 10, 10, NULL, NULL, inst, NULL);
	if (h) {
		s.dpi = zf19a91e ? (int)zf19a91e(h) : 96;
		if (s.dpi < 96) s.dpi = 96;
		const int z9409772 = mi.rcMonitor.right - mi.rcMonitor.left;
		const int z952ba09 = mi.rcMonitor.bottom - mi.rcMonitor.top;
		int w, hh;
		if (kind == zda39eaf) {
			w = S(1120);
			if (z9409772 > 0 && w > z9409772 * 94 / 100) w = z9409772 * 94 / 100;
			HDC dc = GetDC(h);
			hh = z62f635c(dc, w);
			for (int i = 0; i < 6 && z952ba09 > 0 && hh > z952ba09 * 92 / 100; i++) {
				s.scale *= 0.9;
				hh = z62f635c(dc, w);
			}
			ReleaseDC(h, dc);
		} else {
			w = S(900);
			if (z9409772 > 0 && w > z9409772 * 9 / 10) w = z9409772 * 9 / 10;
			hh = S(26 + 22 + 8 + 56 + 32 + 24 + 52 + 12 + 26 + 22);
		}
		if (z952ba09 > 0 && hh > z952ba09 * 94 / 100) hh = z952ba09 * 94 / 100;
		SetWindowPos(h, HWND_TOPMOST, mi.rcMonitor.left + (z9409772 - w) / 2, mi.rcMonitor.top + (z952ba09 - hh) / 2,
		             w, hh, SWP_SHOWWINDOW);
		SetForegroundWindow(h);
		if (s.left > 0) SetTimer(h, zfec9376, 1000, NULL);
		SetTimer(h, ze2a5418, 33, NULL);
		GetCursorPos(&s.z018b98d);
		s.zc692425 = pad ? pad() : 0;
		s.z5aa7ca5 = s.zc692425;
		s.zb1fdfa2 = GetTickCount();
	}
	if (z368437a && ze30c210) z368437a(ze30c210);
	if (!h) return z29cb4a5;

	int nShow = 0;
	while (ShowCursor(TRUE) < 0 && nShow < 16) nShow++;
	nShow++;

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

	while (nShow-- > 0) ShowCursor(FALSE);
	z28d322b();
	if (game && IsWindow(game)) SetForegroundWindow(game);
	if (quit) PostQuitMessage((int)z4d48448);
	return s.result;
}

int z95d8da8(void* zf7534e9, int nSeconds, z953c9b9 pad)
{
	if (nSeconds < 1) nSeconds = 1;
	if (nSeconds > 30) nSeconds = 30;
	return run((HWND)zf7534e9, z90131bd, nSeconds, pad);
}

int z2378d3a(void* zf7534e9, z953c9b9 pad)
{
	const int r = run((HWND)zf7534e9, zda39eaf, 0, pad);
	return (r == z1966f60) ? z1966f60 : z29cb4a5;
}
