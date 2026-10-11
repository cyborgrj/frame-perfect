#include <windows.h>
#include <string.h>
#include <wchar.h>
#include <math.h>

#include "fbneo_host.h"
#include "ggpo_bridge.h"
#include "overlay.h"
#include <stdio.h>

static void z69dc1cf(const char* fmt, int a, int b)
{
	char line[96];
	snprintf(line, sizeof(line), fmt, a, b);
	z5d90bae(line);
}

#define z1eeca0f  200
#define z97ebd85       24
#define zebac260     1500
#define zcc03446    240
#define zc0ed3c1    2200
#define zb567096 10000
#define zbe5776f   1800

static const wchar_t zd137d22[] =
	L"Este \x00E9 o chat da partida. T ou F1 exibe/oculta o chat. F2 desativa o chat nesta partida.";

struct z9bcd5c1 {
	int     mine, id, read;
	wchar_t text[z1eeca0f + 1];
};

static int      z910e658, z314f466, zcca8952, zb7ea71a, z97ff77b, zae54566;
static int      zb421fde, z648aeaa, z73e1c41;
static float    z96fa54e;
static wchar_t  z3ba2b20[z1eeca0f + 1];
static z9bcd5c1 z25621e0[z97ebd85];
static wchar_t  zdf20384[40];
static wchar_t  ze37be3c[96];
static DWORD    z6d8bb6b, z047008c, z3778cd4, zc98579d;

static void zfdd3284(int mine, int id, const wchar_t* t)
{
	if (zae54566 == z97ebd85) {
		memmove(z25621e0, z25621e0 + 1, sizeof(z9bcd5c1) * (z97ebd85 - 1));
		zae54566--;
	}
	z9bcd5c1& L = z25621e0[zae54566++];
	L.mine = mine;
	L.id = id;
	L.read = 0;
	wcsncpy(L.text, t, z1eeca0f);
	L.text[z1eeca0f] = 0;
}

static void hint(const wchar_t* t, DWORD ms)
{
	wcsncpy(ze37be3c, t, 95);
	ze37be3c[95] = 0;
	z047008c = GetTickCount() + ms;
}

static float zf22247e(DWORD now)
{
	const float goal = z73e1c41 ? 1.0f : 0.0f;
	if (!z3778cd4) return goal;
	const DWORD t = now - z3778cd4;
	if (t >= zcc03446) { z3778cd4 = 0; return goal; }
	float x = (float)t / zcc03446;
	x = 1.0f - (1.0f - x) * (1.0f - x) * (1.0f - x);
	return z96fa54e + (goal - z96fa54e) * x;
}

static void zd1751a0(int open, DWORD z8e3ddfa)
{
	const DWORD now = GetTickCount();
	if (open != z73e1c41) {
		z96fa54e = zf22247e(now);
		z73e1c41 = open;
		z3778cd4 = now ? now : 1;
	}
	zc98579d = (open && z8e3ddfa) ? now + z8e3ddfa : 0;
	if (!zc98579d && open && z8e3ddfa) zc98579d = 1;
}

static void zec97fc8()
{
	zcca8952 = 0;
	int top = 0;
	for (int i = 0; i < zae54566; i++)
		if (!z25621e0[i].mine && z25621e0[i].id > top) top = z25621e0[i].id;
	if (top > z97ff77b) {
		z97ff77b = top;
		z018147a(top);
	}
}

static void zf272a87()
{
	z314f466 = 0;
	zd1751a0(0, 0);
	z648aeaa = 0;
}

static void flash(const wchar_t* t)
{
	hint(t, zbe5776f);
	zd1751a0(1, zbe5776f);
}

static void z900294f()
{
	int a = 0, b = zb7ea71a;
	while (a < b && z3ba2b20[a] == L' ') a++;
	while (b > a && z3ba2b20[b - 1] == L' ') b--;
	if (a == b) { zf272a87(); return; }
	const DWORD now = GetTickCount();
	if (z6d8bb6b && now - z6d8bb6b < zebac260) { hint(L"Espere um instante", zc0ed3c1); return; }
	char utf8[z67a7ed4];
	const int n = WideCharToMultiByte(CP_UTF8, 0, z3ba2b20 + a, b - a, utf8, sizeof(utf8), NULL, NULL);
	const int id = n > 0 ? z77d22b7(utf8, n) : 0;
	if (!id) { hint(L"N\x00E3o deu para enviar agora", zc0ed3c1); return; }
	wchar_t t[z1eeca0f + 1];
	wcsncpy(t, z3ba2b20 + a, b - a);
	t[b - a] = 0;
	zfdd3284(1, id, t);
	z69dc1cf("chat: enviada a mensagem %d (%d bytes)", id, n);
	z6d8bb6b = now;
	zb7ea71a = 0;
	z3ba2b20[0] = 0;
	zf272a87();
}

static void z6a78f87(int kind, int id, const char* text, int len, void*  )
{
	if (kind == 0) {
		wchar_t w[z1eeca0f + 1];
		int n = MultiByteToWideChar(CP_UTF8, 0, text, len, w, z1eeca0f);
		if (n <= 0) return;
		w[n] = 0;
		for (int i = 0; i < n; i++) if (w[i] < 0x20 || w[i] == 0x7F) w[i] = L' ';
		zfdd3284(0, id, w);
		z69dc1cf("chat: chegou a mensagem %d (%d bytes)", id, len);
		if (!zb421fde && z73e1c41) zec97fc8();
		else zcca8952++;
	} else if (kind == 1) {
		z69dc1cf("chat: o outro lado recebeu a mensagem %d", id, 0);
	} else if (kind == 2) {
		z69dc1cf("chat: o outro lado leu ate a mensagem %d", id, 0);
		for (int i = 0; i < zae54566; i++)
			if (z25621e0[i].mine && z25621e0[i].id <= id) z25621e0[i].read = 1;
	}
}

void z2e5f4c9(int bOn, const char* zd8f5856)
{
	z910e658 = bOn ? 1 : 0;
	z314f466 = zcca8952 = zb7ea71a = z97ff77b = zae54566 = 0;
	zb421fde = 0;
	z3ba2b20[0] = 0;
	z6d8bb6b = z047008c = z3778cd4 = zc98579d = 0;
	z73e1c41 = 0;
	z96fa54e = 0.0f;
	zdf20384[0] = 0;
	if (zd8f5856 && zd8f5856[0]) {
		char raw[80];
		int k = 0;
		for (int i = 0; zd8f5856[i] && k < 79; i++) {
			const char c = zd8f5856[i];
			int hi = -1, lo = -1;
			if (c == '%' && zd8f5856[i + 1] && zd8f5856[i + 2]) {
				const char a = zd8f5856[i + 1], b = zd8f5856[i + 2];
				hi = (a >= '0' && a <= '9') ? a - '0' : (a >= 'A' && a <= 'F') ? a - 'A' + 10 : (a >= 'a' && a <= 'f') ? a - 'a' + 10 : -1;
				lo = (b >= '0' && b <= '9') ? b - '0' : (b >= 'A' && b <= 'F') ? b - 'A' + 10 : (b >= 'a' && b <= 'f') ? b - 'a' + 10 : -1;
			}
			if (hi >= 0 && lo >= 0) { raw[k++] = (char)(hi * 16 + lo); i += 2; }
			else raw[k++] = c;
		}
		raw[k] = 0;
		int n = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, raw, -1, zdf20384, 39);
		if (n <= 0) MultiByteToWideChar(CP_ACP, 0, raw, -1, zdf20384, 39);
		zdf20384[39] = 0;
	}
	if (!zdf20384[0]) wcscpy(zdf20384, L"Advers\x00E1rio");
	z648aeaa = z910e658;
	if (z910e658) zd1751a0(1, zb567096);
}

void ze31c6f0(void)
{
	z910e658 = z314f466 = 0;
}

void zfc2ac67(void)
{
	if (z910e658) z9949c17(z6a78f87, NULL);
}

int z60692a4(void) { return z910e658; }
int z806b0c0(void) { return z910e658 && z314f466; }

int z2b7bad9(unsigned int vk)
{
	if (!z910e658) return 0;
	if (z314f466) {
		if (vk == VK_RETURN) { z900294f(); return 1; }
		if (vk == VK_ESCAPE || vk == VK_F1) { zf272a87(); return 1; }
		if (vk == VK_BACK) { if (zb7ea71a > 0) z3ba2b20[--zb7ea71a] = 0; return 1; }
		return 0;
	}
	if (vk == VK_F2) {
		zb421fde = !zb421fde;
		z648aeaa = 0;
		z69dc1cf("chat: F2 neste lado - ativado %d%.0d", !zb421fde, 0);
		flash(zb421fde ? L"Chat desativado nesta partida. F2 ativa de novo."
		                 : L"Chat ativado. T escreve, F1 mostra ou esconde.");
		return 1;
	}
	if (zb421fde && (vk == 'T' || vk == VK_F1)) {
		flash(L"Chat desativado nesta partida. F2 ativa de novo.");
		return 1;
	}
	if (vk == 'T') {
		z314f466 = 1;
		zd1751a0(1, 0);
		zec97fc8();
		return 1;
	}
	if (vk == VK_F1) {
		if (z73e1c41) zf272a87();
		else { zd1751a0(1, 0); zec97fc8(); }
		return 1;
	}
	return 0;
}

void z5fa0f19(unsigned int ch)
{
	if (!z910e658 || !z314f466) return;
	if (ch < 0x20 || ch == 0x7F || (ch >= 0xD800 && ch <= 0xDFFF)) return;
	if (zb7ea71a >= z1eeca0f) { hint(L"No m\x00E1ximo 200 caracteres", zc0ed3c1); return; }
	z3ba2b20[zb7ea71a++] = (wchar_t)ch;
	z3ba2b20[zb7ea71a] = 0;
}

int z8ad8d82(z1ea8be6* v)
{
	if (!v || !z910e658) return 0;
	memset(v, 0, sizeof(*v));
	const DWORD now = GetTickCount();
	if (zc98579d && !z314f466 && (LONG)(now - zc98579d) >= 0) { zc98579d = 0; zd1751a0(0, 0); z648aeaa = 0; }
	v->fOpen = zf22247e(now);
	v->zcf8a8e5 = z314f466;
	v->z92c652b = zb421fde;
	v->z822f0cf = zcca8952;
	v->z1e5d86a = z3ba2b20;
	v->nInput = zb7ea71a;
	v->nMax = z1eeca0f;
	v->zd8f5856 = zdf20384;
	if (now < z047008c) v->ze6affeb = ze37be3c;
	int room = z0452cf9;
	if (z648aeaa) {
		z9bafc5a& W = v->lines[v->z19ef79a++];
		W.sys = 1;
		W.text = zd137d22;
		room--;
	}
	const int from = zae54566 > room ? zae54566 - room : 0;
	for (int i = from; i < zae54566; i++) {
		z9bafc5a& L = v->lines[v->z19ef79a++];
		L.mine = z25621e0[i].mine;
		L.read = z25621e0[i].read;
		L.text = z25621e0[i].text;
	}
	return 1;
}

void z8eca64b(zdcfcee4 fn, void* ctx, int x, int y, int w, int h, int r, unsigned int rgb, int alpha)
{
	if (w <= 0 || h <= 0 || alpha <= 0) return;
	if (r * 2 > h) r = h / 2;
	if (r * 2 > w) r = w / 2;
	if (r < 1) { fn(ctx, x, y, x + w, y + h, rgb, alpha); return; }
	for (int i = 0; i < r; i++) {
		const double dy = r - i - 0.5;
		const int in = (int)(r - sqrt((double)r * r - dy * dy) + 0.5);
		fn(ctx, x + in, y + i, x + w - in, y + i + 1, rgb, alpha);
		fn(ctx, x + in, y + h - 1 - i, x + w - in, y + h - i, rgb, alpha);
	}
	if (h - 2 * r > 0) fn(ctx, x, y + r, x + w, y + h - r, rgb, alpha);
}

static void disk(zdcfcee4 fn, void* ctx, int cx, int cy, int r, unsigned int rgb, int alpha)
{
	for (int dy = -r; dy < r; dy++) {
		const double yy = dy + 0.5;
		const int hw = (int)(sqrt((double)r * r - yy * yy) + 0.5);
		if (hw > 0) fn(ctx, cx - hw, cy + dy, cx + hw, cy + dy + 1, rgb, alpha);
	}
}

static void forbidden(zdcfcee4 fn, void* ctx, int cx, int cy, int r, int t, unsigned int rgb, int alpha)
{
	const int ri = r - t;
	for (int dy = -r; dy < r; dy++) {
		const double yy = dy + 0.5;
		const int ho = (int)(sqrt((double)r * r - yy * yy) + 0.5);
		const int hi = (ri > 0 && yy * yy < (double)ri * ri) ? (int)(sqrt((double)ri * ri - yy * yy) + 0.5) : 0;
		if (hi <= 0) { fn(ctx, cx - ho, cy + dy, cx + ho, cy + dy + 1, rgb, alpha); continue; }
		fn(ctx, cx - ho, cy + dy, cx - hi, cy + dy + 1, rgb, alpha);
		fn(ctx, cx + hi, cy + dy, cx + ho, cy + dy + 1, rgb, alpha);
		int a = cx + dy - t / 2 - 1, b = cx + dy + t / 2 + 1;
		if (a < cx - hi) a = cx - hi;
		if (b > cx + hi) b = cx + hi;
		if (b > a) fn(ctx, a, cy + dy, b, cy + dy + 1, rgb, alpha);
	}
}

void zb52af7e(zdcfcee4 fn, void* ctx, int x, int y, int w, int h, int alpha,
                    int z92c652b, int z822f0cf, int* z924dd86, int* ze97eb52, int* pbr)
{
	if (z924dd86) *z924dd86 = 0;
	if (ze97eb52) *ze97eb52 = 0;
	if (pbr) *pbr = 0;
	if (w <= 0 || h <= 0 || alpha <= 0) return;
	z8eca64b(fn, ctx, x, y, w, h, h / 2, 0x2E3440, alpha);
	z8eca64b(fn, ctx, x + 1, y + 1, w - 2, h - 2, (h - 2) / 2, 0x1B1F27, alpha);
	const int fg = alpha * 2 > 255 ? 255 : alpha * 2;
	const int badge = !z92c652b && z822f0cf > 0;
	const int bw = h * 62 / 100, bh = h * 44 / 100;
	const int z7127108 = badge ? x + w * 34 / 100 : x + w / 2;
	const int bx = z7127108 - bw / 2, by = y + (h - bh) / 2 - h / 14;
	z8eca64b(fn, ctx, bx, by, bw, bh, bh / 3 > 1 ? bh / 3 : 1, 0xE7EAF0, fg);
	const int tail = bh / 3 > 1 ? bh / 3 : 1;
	for (int i = 0; i < tail; i++)
		fn(ctx, bx + bw / 5, by + bh + i, bx + bw / 5 + tail - i, by + bh + i + 1, 0xE7EAF0, fg);
	if (z92c652b) {
		const int r = h * 40 / 100;
		forbidden(fn, ctx, z7127108, y + h / 2, r, r / 4 > 1 ? r / 4 : 1, 0xFF5555, fg);
	} else if (badge) {
		const int r = h * 30 / 100 > 2 ? h * 30 / 100 : 2;
		const int cx = x + w * 72 / 100, cy = y + h / 2;
		disk(fn, ctx, cx, cy, r, 0xB0A7FF, fg);
		if (z924dd86) *z924dd86 = cx;
		if (ze97eb52) *ze97eb52 = cy;
		if (pbr) *pbr = r;
	}
}
