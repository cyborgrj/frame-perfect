#include <windows.h>
#include <string.h>
#include <wchar.h>
#include <math.h>

#include "fbneo_host.h"
#include "ggpo_bridge.h"
#include "overlay.h"
#include "lang.h"
#include <stdio.h>
#include <stdlib.h>

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
static int      zb421fde, z648aeaa, z73e1c41, z3fe3b91;
static int      z6ca83a6;
static float    z96fa54e;
static wchar_t  z3ba2b20[z1eeca0f + 1];
static z9bcd5c1 z25621e0[z97ebd85];
static wchar_t  zdf20384[40];
static wchar_t  ze37be3c[96];
static DWORD    z6d8bb6b, z047008c, z3778cd4, zc98579d;

#define zca5cc97    0x52424631u
#define z5a3fd25    4
#define z4fef28d    2048
#define za44be31  8000
#define z2389a06  2500
#define z8f977be  360
#define z574b01f    0xF5C542u
#define z0bf0bb5      0x1B1606u

struct z4bff1f0 {
	int       has;
	char      id[64];
	int       ze607124, ping, z76fba51, ft, total;
	wchar_t   zde4da44[48];
	ULONGLONG deadline;
	int       zcf8dc2e, zeb6e153, zfe95bb1;
	int       confirmado;
	ULONGLONG zfbbec47;
	HWND      launcher;
};
static z4bff1f0    z088e12c;
static int       z0aab915;
static int       z63a2de8;
static int       zd12bcf2;
static ULONGLONG z365ad42;
static int       z7ccb18c;
static char      zf18bccf[z5a3fd25][z4fef28d + 1];
static int       z68250a9[z5a3fd25];
static HWND      z25be773[z5a3fd25];
static int       zc31d562, z4ad9100;
static HWND      zdf08deb;
static WNDPROC   z07efa8b;
static volatile LONG zba9cab4 = -1;
static char        z87a4840[256];
static const char* z7afcbd7;

static int  zd5c72f9(void) { return z088e12c.has && !z088e12c.zcf8dc2e && !z088e12c.zfe95bb1; }
static int  z4452880(void) { return z088e12c.has && (!z910e658 || zb421fde); }
static int  zd8b4d78(void) { return z73e1c41 && z63a2de8 && zd5c72f9(); }
static void z7e88af5(int accept);

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
	z63a2de8 = 0;
	zd12bcf2 = 0;
}

static void flash(const wchar_t* t)
{
	hint(t, zbe5776f);
	zd1751a0(1, zbe5776f);
}

static void z883dbc9(int how)
{
	zd12bcf2 = (how == 2);
	if (z4452880()) {
		z314f466 = 0;
		z63a2de8 = zd5c72f9() ? 1 : 0;
		zd1751a0(1, 0);
		return;
	}
	zd1751a0(1, 0);
	zec97fc8();
	if (how == 2) {
		z314f466 = 0;
		z63a2de8 = zd5c72f9() ? 1 : 0;
		return;
	}
	z63a2de8 = 0;
	if (how == 0 || zd5c72f9()) z314f466 = 1;
}

static void z900294f()
{
	int a = 0, b = zb7ea71a;
	while (a < b && z3ba2b20[a] == L' ') a++;
	while (b > a && z3ba2b20[b - 1] == L' ') b--;
	if (a == b) { zf272a87(); return; }
	const DWORD now = GetTickCount();
	if (z6d8bb6b && now - z6d8bb6b < zebac260) { hint(z6f2968c(L"Espere um instante"), zc0ed3c1); return; }
	char utf8[z67a7ed4];
	const int n = WideCharToMultiByte(CP_UTF8, 0, z3ba2b20 + a, b - a, utf8, sizeof(utf8), NULL, NULL);
	const int id = n > 0 ? z77d22b7(utf8, n) : 0;
	if (!id) { hint(z6f2968c(L"N\x00E3o deu para enviar agora"), zc0ed3c1); return; }
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
	if (kind == 0 && z7675262(text, len)) return;
	if (kind == 0 && !z910e658) return;
	if (kind == 0 && z3fe3b91) return;
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

extern HWND hScrnWnd;

static void zb393acd(const char* fmt, const char* id, int a, int b)
{
	char line[192];
	snprintf(line, sizeof(line), fmt, id ? id : "-", a, b);
	z5d90bae(line);
}

static LRESULT CALLBACK ze050a0f(HWND h, UINT m, WPARAM w, LPARAM l)
{
	if (m == WM_COPYDATA && l) {
		const COPYDATASTRUCT* cd = (const COPYDATASTRUCT*)l;
		if (cd->dwData == zca5cc97) {
			const DWORD n = cd->cbData;
			if (!z0aab915 || !cd->lpData || n == 0 || n > z4fef28d || zc31d562 >= z5a3fd25) { z4ad9100++; return FALSE; }
			memcpy(zf18bccf[zc31d562], cd->lpData, n);
			zf18bccf[zc31d562][n] = 0;
			z68250a9[zc31d562] = (int)n;
			z25be773[zc31d562] = (HWND)w;
			zc31d562++;
			return TRUE;
		}
	}
	const WNDPROC old = z07efa8b;
	if (m == WM_NCDESTROY && h == zdf08deb) zdf08deb = NULL;
	return old ? CallWindowProcW(old, h, m, w, l) : DefWindowProcW(h, m, w, l);
}

static void ze675b85(void)
{
	HWND h = hScrnWnd;
	if (!h || h == zdf08deb || !IsWindow(h)) return;
	DWORD pid = 0;
	GetWindowThreadProcessId(h, &pid);
	if (pid != GetCurrentProcessId()) return;
	zdf08deb = h;
	SetLastError(0);
	const LONG_PTR old = SetWindowLongPtrW(h, GWLP_WNDPROC, (LONG_PTR)ze050a0f);
	if (!old) {
		zb393acd("proxima: nao deu para ouvir a janela do jogo (erro %s%d)", "", (int)GetLastError(), 0);
		return;
	}
	z07efa8b = (WNDPROC)old;
	typedef BOOL (WINAPI *z308841b)(HWND, UINT, DWORD, void*);
	z308841b f = (z308841b)(void*)GetProcAddress(GetModuleHandleA("user32.dll"), "ChangeWindowMessageFilterEx");
	if (f) f(h, WM_COPYDATA, 1  , NULL);
	zb393acd("proxima: ouvindo o launcher na janela do jogo%s", "", 0, 0);
}

static int za70a238(const char* v, int lo, int hi, int* out)
{
	if (!v || !*v || strlen(v) > 9) return 0;
	int n = 0;
	for (const char* p = v; *p; p++) {
		if (*p < '0' || *p > '9') return 0;
		n = n * 10 + (*p - '0');
	}
	if (n < lo || n > hi) return 0;
	*out = n;
	return 1;
}

static int z9b18de0(const char* v)
{
	const size_t n = v ? strlen(v) : 0;
	if (n < 1 || n > 63) return 0;
	for (size_t i = 0; i < n; i++) {
		const char c = v[i];
		if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
		      c == '-' || c == '_' || c == '.' || c == ':')) return 0;
	}
	return 1;
}

static int zde40c1c(const char* v)
{
	const size_t n = v ? strlen(v) : 0;
	if (n < 1 || n > 32) return 0;
	for (size_t i = 0; i < n; i++) {
		const char c = v[i];
		if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || c == '_' || c == ':')) return 0;
	}
	return 1;
}

static int z00ea91c(const char* v, HWND* out)
{
	if (!v || !*v || strlen(v) > 20) return 0;
	unsigned long long n = 0;
	for (const char* p = v; *p; p++) {
		if (*p < '0' || *p > '9') return 0;
		const unsigned long long d = (unsigned long long)(*p - '0');
		if (n > (~0ULL - d) / 10) return 0;
		n = n * 10 + d;
	}
	if (!n || n != (unsigned long long)(UINT_PTR)n) return 0;
	*out = (HWND)(UINT_PTR)n;
	return 1;
}

static int z21822d5(HWND h)
{
	if (!h || !IsWindow(h)) return 0;
	DWORD pid = 0;
	GetWindowThreadProcessId(h, &pid);
	return pid != 0 && pid != GetCurrentProcessId();
}

static int z4f0c4ae(const char* v, wchar_t* out, int cap)
{
	if (!v || !*v || strlen(v) > 160) return 0;
	wchar_t w[200];
	const int n = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, v, (int)strlen(v), w, 199);
	if (n <= 0) return 0;
	w[n] = 0;
	for (int i = 0; i < n; i++) if (w[i] < 0x20 || (w[i] >= 0x7F && w[i] < 0xA0)) w[i] = L' ';
	int a = 0, b = n;
	while (a < b && w[a] == L' ') a++;
	while (b > a && w[b - 1] == L' ') b--;
	if (a == b) return 0;
	int k = b - a;
	if (k > cap - 1) k = cap - 1;
	if (k > 40) k = 40;
	wcsncpy(out, w + a, k);
	out[k] = 0;
	return 1;
}

struct z6463bd2 { HWND to, from; int n; char text[128]; };

static DWORD WINAPI z09ae9f7(LPVOID p)
{
	z6463bd2* r = (z6463bd2*)p;
	COPYDATASTRUCT cd;
	cd.dwData = zca5cc97;
	cd.cbData = (DWORD)r->n;
	cd.lpData = r->text;
	DWORD_PTR res = 0;
	const LRESULT ok = SendMessageTimeoutW(r->to, WM_COPYDATA, (WPARAM)r->from, (LPARAM)&cd,
	                                       SMTO_ABORTIFHUNG | SMTO_NORMAL, 3000, &res);
	InterlockedExchange(&zba9cab4, ok ? 1 : 0);
	free(r);
	return 0;
}

static void za11c932(int accept)
{
	if (!z088e12c.launcher) {
		zb393acd("teste: a resposta do pedido %s seria aceitar=%d (sem o launcher)", z088e12c.id, accept, 0);
		return;
	}
	z088e12c.zfbbec47 = GetTickCount64();
	z6463bd2* r = (z6463bd2*)calloc(1, sizeof(z6463bd2));
	if (!r) return;
	r->to = z088e12c.launcher;
	r->from = zdf08deb ? zdf08deb : hScrnWnd;
	r->n = snprintf(r->text, sizeof(r->text), "tipo=resposta\nid=%s\naceitar=%d", z088e12c.id, accept ? 1 : 0);
	if (r->n <= 0 || r->n >= (int)sizeof(r->text)) { free(r); return; }
	HANDLE t = CreateThread(NULL, 64 * 1024, z09ae9f7, r, 0, NULL);
	if (t) { CloseHandle(t); return; }
	z09ae9f7(r);
}

static void z8f6a2f5(void)
{
	z63a2de8 = 0;
	if (z73e1c41 && (zd12bcf2 || z4452880() || !z314f466)) zd1751a0(1, z2389a06);
}

static void z7e88af5(int accept)
{
	if (!zd5c72f9()) return;
	z088e12c.zcf8dc2e = 1;
	z088e12c.zeb6e153 = accept ? 1 : 0;
	zb393acd("proxima: respondido o pedido %s - aceitar=%d%.0d", z088e12c.id, z088e12c.zeb6e153, 0);
	za11c932(z088e12c.zeb6e153);
	z8f6a2f5();
}

static void zbf75b8e(char* s, int n, HWND from, int bTest)
{
	if (!s || n <= 0) return;
	if ((int)strlen(s) != n) { zb393acd("proxima: mensagem com um NUL no meio (%s%d bytes)", "", n, 0); return; }
	const char *zdb78ff0 = NULL, *id = NULL, *jogo = NULL, *zde4da44 = NULL, *ze607124 = NULL, *ping = NULL,
	           *z76fba51 = NULL, *ft = NULL, *zbbfa04a = NULL, *resp = NULL, *zeb6e153 = NULL, *hwnd = NULL;
	for (char* line = s; line; ) {
		char* nl = strchr(line, '\n');
		if (nl) *nl = 0;
		size_t L = strlen(line);
		if (L && line[L - 1] == '\r') line[--L] = 0;
		if (L) {
			char* eq = strchr(line, '=');
			if (!eq || eq == line) { zb393acd("proxima: mensagem fora do formato (%s%d bytes)", "", n, 0); return; }
			*eq = 0;
			const char* k = line;
			const char* v = eq + 1;
			if      (!strcmp(k, "tipo"))       zdb78ff0 = v;
			else if (!strcmp(k, "id"))         id = v;
			else if (!strcmp(k, "jogo"))       jogo = v;
			else if (!strcmp(k, "nome"))       zde4da44 = v;
			else if (!strcmp(k, "estacao"))    ze607124 = v;
			else if (!strcmp(k, "ping"))       ping = v;
			else if (!strcmp(k, "pontos"))     z76fba51 = v;
			else if (!strcmp(k, "ft"))         ft = v;
			else if (!strcmp(k, "segundos"))   zbbfa04a = v;
			else if (!strcmp(k, "respondido")) resp = v;
			else if (!strcmp(k, "aceito"))     zeb6e153 = v;
			else if (!strcmp(k, "hwnd"))       hwnd = v;
		}
		line = nl ? nl + 1 : NULL;
	}
	if (!zdb78ff0 || !z9b18de0(id)) { zb393acd("proxima: mensagem sem tipo ou id valido (%s%d bytes)", "", n, 0); return; }

	if (!strcmp(zdb78ff0, "fecha")) {
		if (!z088e12c.has || strcmp(z088e12c.id, id)) return;
		const int was = zd8b4d78() || (z73e1c41 && z4452880());
		z088e12c.has = 0;
		z63a2de8 = 0;
		if (was) zf272a87();
		zb393acd("proxima: o launcher fechou o pedido %s%.0d%.0d", id, 0, 0);
		return;
	}
	if (strcmp(zdb78ff0, "proxima")) return;

	z4bff1f0 c;
	memset(&c, 0, sizeof(c));
	int seg = 0, r = 0, a = 0;
	if (!za70a238(ze607124, 1, 9999, &c.ze607124) || !z4f0c4ae(zde4da44, c.zde4da44, 48) || !za70a238(zbbfa04a, 0, 3600, &seg)) {
		zb393acd("proxima: pedido %s incompleto ou fora do formato%.0d%.0d", id, 0, 0);
		return;
	}
	c.ping = -1; c.z76fba51 = -1; c.ft = 0;
	if (ping && !za70a238(ping, 0, 9999, &c.ping)) return;
	if (z76fba51 && !za70a238(z76fba51, 0, 9999999, &c.z76fba51)) return;
	if (ft && !za70a238(ft, 1, 99, &c.ft)) return;
	if (resp && !za70a238(resp, 0, 1, &r)) return;
	if (zeb6e153 && !za70a238(zeb6e153, 0, 1, &a)) return;
	if (jogo && !zde40c1c(jogo)) return;
	if (!bTest) {
		HWND hw = NULL;
		if (hwnd && !z00ea91c(hwnd, &hw)) return;
		if (!hw) hw = from;
		if (!z21822d5(hw)) { zb393acd("proxima: pedido %s sem a janela do launcher%.0d%.0d", id, 0, 0); return; }
		c.launcher = hw;
	}

	const ULONGLONG now = GetTickCount64();
	const int same = z088e12c.has && !strcmp(z088e12c.id, id);
	strcpy(c.id, id);
	c.deadline = now + (ULONGLONG)seg * 1000;
	c.total = z8f977be > seg ? z8f977be : seg;
	if (same && z088e12c.total > c.total) c.total = z088e12c.total;
	int z8668aa5 = 0;
	if (r) {
		c.zcf8dc2e = 1;
		c.zeb6e153 = a;
		c.confirmado = 1;
	} else if (same && (z088e12c.zcf8dc2e || z088e12c.zfe95bb1)) {
		c.zcf8dc2e = z088e12c.zcf8dc2e;
		c.zeb6e153 = z088e12c.zeb6e153;
		c.zfe95bb1 = z088e12c.zfe95bb1;
		c.zfbbec47 = z088e12c.zfbbec47;
		z8668aa5 = z088e12c.zcf8dc2e && c.launcher && now - z088e12c.zfbbec47 >= 3000;
	}
	if (!same) {
		z365ad42 = 0;
		z7ccb18c = !c.zcf8dc2e;
	}
	const int z482c42e = zd8b4d78();
	const int zd95d470 = same && z088e12c.confirmado;
	z088e12c = c;
	z088e12c.has = 1;
	if (!same) {
		zb393acd("proxima: chegou o pedido %s (estacao %d, %d s)", id, c.ze607124, seg);
		if (z482c42e && zd5c72f9()) z63a2de8 = 1;
	} else if (r && !zd95d470) {
		zb393acd("proxima: o lobby registrou a resposta do pedido %s (aceito %d)%.0d", id, a, 0);
	}
	if (z8668aa5) {
		zb393acd("proxima: o launcher ainda nao tem a resposta do pedido %s - enviando de novo (aceitar=%d)%.0d", id, z088e12c.zeb6e153, 0);
		za11c932(z088e12c.zeb6e153);
	}
	if (z482c42e && !zd5c72f9()) z8f6a2f5();
	if (jogo && !same) {
		char line[128];
		snprintf(line, sizeof(line), "proxima: pedido %s do jogo %s", id, jogo);
		z5d90bae(line);
	}
}

static void z68bbfb3(void)
{
	if (z4ad9100) {
		zb393acd("proxima: %s%d mensagem(ns) do launcher ignorada(s) (fora da partida, grande demais ou fila cheia)", "", z4ad9100, 0);
		z4ad9100 = 0;
	}
	const int n = zc31d562;
	for (int i = 0; i < n; i++) zbf75b8e(zf18bccf[i], z68250a9[i], z25be773[i], 0);
	zc31d562 = 0;
}

static void z07da0a6(void)
{
	if (!zd5c72f9()) return;
	if (GetTickCount64() < z088e12c.deadline) return;
	const int z482c42e = zd8b4d78();
	z088e12c.zfe95bb1 = 1;
	zb393acd("proxima: o tempo do pedido %s acabou sem resposta (= nao)%.0d%.0d", z088e12c.id, 0, 0);
	if (z482c42e) z8f6a2f5();
}

static void za983a8d(int resp, int acc, int seg)
{
	char m[320];
	const int n = snprintf(m, sizeof(m),
	                       "tipo=proxima\nid=teste-1\njogo=kof98\nestacao=3\nnome=[KOF]Kyo_99\nping=32\n"
	                       "pontos=1084\nft=5\nsegundos=%d\nrespondido=%d\naceito=%d\nhwnd=0",
	                       seg, resp, acc);
	if (n > 0 && n < (int)sizeof(m)) zbf75b8e(m, n, NULL, 1);
}

static void z69cdc08(void)
{
	if (!z7afcbd7 || !*z7afcbd7) return;
	const long long f = zca74129();
	char item[48];
	const char* end = strchr(z7afcbd7, ';');
	size_t len = end ? (size_t)(end - z7afcbd7) : strlen(z7afcbd7);
	if (len >= sizeof(item)) { z7afcbd7 = NULL; return; }
	memcpy(item, z7afcbd7, len);
	item[len] = 0;
	long long at = atoll(item);
	if (f < at) return;
	z7afcbd7 = end ? end + 1 : NULL;
	const char* colon = strchr(item, ':');
	if (!colon) {
		const char* sp = strchr(item, 's');
		const int seg = sp ? atoi(sp + 1) : 216;
		zb393acd("teste: RBF_TESTE_PROXIMA - o pedido simulado chega (%s%d s, quadro %d)", "", seg, (int)f);
		za983a8d(0, 0, seg > 0 ? seg : 216);
		return;
	}
	const char* a = colon + 1;
	zb393acd("teste: RBF_TESTE_PROXIMA - %s (quadro %d)%.0d", a, (int)f, 0);
	static const struct { const char* n; unsigned vk; } z2514565[] = {
		{ "t", 'T' }, { "f1", VK_F1 }, { "f2", VK_F2 }, { "up", VK_UP }, { "down", VK_DOWN },
		{ "left", VK_LEFT }, { "right", VK_RIGHT }, { "enter", VK_RETURN }, { "esc", VK_ESCAPE },
	};
	for (size_t i = 0; i < sizeof(z2514565) / sizeof(z2514565[0]); i++)
		if (!strcmp(a, z2514565[i].n)) { z2b7bad9(z2514565[i].vk); return; }
	if (!strcmp(a, "pad")) { z2ae00bb(); return; }
	static const struct { const char* n; unsigned e; } zc64fac5[] = {
		{ "pu", z1eb4eb1 }, { "pd", zbd4b563 }, { "pl", z6a08570 },
		{ "pr", z9e5f2af }, { "pa", z32e3848 }, { "pb", z884daca },
	};
	for (size_t i = 0; i < sizeof(zc64fac5) / sizeof(zc64fac5[0]); i++)
		if (!strcmp(a, zc64fac5[i].n)) { z52fc988(zc64fac5[i].e); return; }
	const ULONGLONG now = GetTickCount64();
	const int left = (z088e12c.has && z088e12c.deadline > now) ? (int)((z088e12c.deadline - now) / 1000) : 0;
	if (!strcmp(a, "ok1") || !strcmp(a, "ok0")) { za983a8d(1, a[2] == '1', left); return; }
	if (!strcmp(a, "denovo")) { za983a8d(0, 0, left); return; }
	if (!strcmp(a, "fecha")) {
		char m[64];
		const int n = snprintf(m, sizeof(m), "tipo=fecha\nid=teste-1");
		zbf75b8e(m, n, NULL, 1);
	}
}

void z2e5f4c9(int bOn, const char* zd8f5856, int zb568fa4, int z64249f9)
{
	z910e658 = bOn ? 1 : 0;
	z3fe3b91 = zb568fa4 ? 1 : 0;
	char tc[16];
	z6ca83a6 = (z83650cd() && GetEnvironmentVariableA("RBF_TESTE_CHAT", tc, sizeof(tc))) ? atoi(tc) : 0;
	if (z3fe3b91) z69dc1cf("chat: mudo deste lado (%d)", 1, 0);
	z314f466 = zcca8952 = zb7ea71a = z97ff77b = zae54566 = 0;
	zb421fde = 0;
	z3ba2b20[0] = 0;
	z6d8bb6b = z047008c = z3778cd4 = zc98579d = 0;
	z73e1c41 = 0;
	z96fa54e = 0.0f;
	zdf20384[0] = 0;
	z0aab915 = z64249f9 ? 1 : 0;
	z63a2de8 = zd12bcf2 = 0;
	z7afcbd7 = NULL;
	if (z0aab915 && z83650cd() && GetEnvironmentVariableA("RBF_TESTE_PROXIMA", z87a4840, sizeof(z87a4840)) && z87a4840[0]) {
		z7afcbd7 = z87a4840;
		zb393acd("teste: RBF_TESTE_PROXIMA=%s%.0d%.0d", z87a4840, 0, 0);
	}
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
	if (!zdf20384[0]) wcscpy(zdf20384, z6f2968c(L"Advers\x00E1rio"));
	z648aeaa = z910e658;
	if (z910e658) zd1751a0(1, zb567096);
	if (z0aab915) ze675b85();
}

void ze31c6f0(void)
{
	z910e658 = z314f466 = 0;
	z0aab915 = 0;
	z63a2de8 = 0;
}

void zfc2ac67(void)
{
	if (z910e658 || z38fea27()) z9949c17(z6a78f87, NULL);
	if (z910e658 && z6ca83a6 > 0 && --z6ca83a6 == 0)
		z69dc1cf("teste: RBF_TESTE_CHAT mandou a mensagem %d (%d bytes)", z77d22b7("teste", 5), 5);
	if (z0aab915) {
		ze675b85();
		z68bbfb3();
		z69cdc08();
		z07da0a6();
		const LONG sent = InterlockedExchange(&zba9cab4, -1);
		if (sent >= 0)
			zb393acd(sent ? "proxima: a resposta do pedido %s chegou ao launcher%.0d%.0d"
			           : "proxima: a resposta do pedido %s nao chegou ao launcher (sem resposta em 3 s)%.0d%.0d",
			      z088e12c.id, 0, 0);
	}
}

int z60692a4(void) { return z910e658 || z088e12c.has; }
int z806b0c0(void) { return (z910e658 && z314f466) || zd8b4d78(); }

static int zb059b6d(unsigned int vk)
{
	switch (vk) {
	case VK_LEFT:   z63a2de8 = 1; break;
	case VK_RIGHT:  z63a2de8 = 2; break;
	case VK_RETURN: z7e88af5(z63a2de8 == 2); break;
	case VK_DOWN:
		if (z910e658 && !z4452880()) { z63a2de8 = 0; z314f466 = 1; zd12bcf2 = 0; }
		break;
	case VK_ESCAPE:
	case VK_F1:     zf272a87(); break;
	case 'T':       if (z4452880()) zf272a87(); break;
	default:        break;
	}
	return 1;
}

int z2b7bad9(unsigned int vk)
{
	if (!z910e658 && !z088e12c.has) return 0;
	if (zd8b4d78()) return zb059b6d(vk);
	if (z314f466) {
		if (vk == VK_UP && zd5c72f9()) { z63a2de8 = 1; return 1; }
		if (vk == VK_RETURN) { z900294f(); return 1; }
		if (vk == VK_ESCAPE || vk == VK_F1) { zf272a87(); return 1; }
		if (vk == VK_BACK) { if (zb7ea71a > 0) z3ba2b20[--zb7ea71a] = 0; return 1; }
		return 0;
	}
	if (vk == VK_F2) {
		if (!z910e658) return 0;
		zb421fde = !zb421fde;
		z648aeaa = 0;
		z63a2de8 = 0;
		z69dc1cf("chat: F2 neste lado - ativado %d%.0d", !zb421fde, 0);
		flash(zb421fde ? z6f2968c(L"Chat desativado nesta partida. F2 ativa de novo.")
		                 : z6f2968c(L"Chat ativado. T escreve, F1 mostra ou esconde."));
		return 1;
	}
	if (vk != 'T' && vk != VK_F1) return 0;
	if (zb421fde && !z088e12c.has) {
		flash(z6f2968c(L"Chat desativado nesta partida. F2 ativa de novo."));
		return 1;
	}
	if (z73e1c41 && (vk == VK_F1 || z4452880())) { zf272a87(); return 1; }
	z883dbc9(vk == 'T' ? 0 : 1);
	return 1;
}

void z5fa0f19(unsigned int ch)
{
	if (!z910e658 || !z314f466 || z63a2de8) return;
	if (ch < 0x20 || ch == 0x7F || (ch >= 0xD800 && ch <= 0xDFFF)) return;
	if (zb7ea71a >= z1eeca0f) { hint(z6f2968c(L"No m\x00E1ximo 200 caracteres"), zc0ed3c1); return; }
	z3ba2b20[zb7ea71a++] = (wchar_t)ch;
	z3ba2b20[zb7ea71a] = 0;
}

int z6f4ded3(void) { return zd8b4d78(); }

void zcf8c9b2(int z3cbb6bc)
{
	if (!z910e658 || zb421fde) return;
	wchar_t t[96];
	_snwprintf(t, 95, z6f2968c(L"FT encerrada: o chat fica aberto por %d s."), z3cbb6bc);
	t[95] = 0;
	hint(t, (DWORD)z3cbb6bc * 1000);
	zd1751a0(1, 0);
}

int z2ae00bb(void)
{
	if (!z910e658 && !z088e12c.has) return 0;
	if (z73e1c41) { zf272a87(); return 1; }
	if (zb421fde && !z088e12c.has) {
		flash(z6f2968c(L"Chat desativado nesta partida. F2 ativa de novo."));
		return 1;
	}
	z883dbc9(2);
	return 1;
}

void z52fc988(unsigned int e)
{
	if (!z73e1c41 || !zd12bcf2 || !e) return;
	if (!zd8b4d78()) {
		if ((e & z1eb4eb1) && zd5c72f9()) z63a2de8 = 1;
		return;
	}
	if (e & z6a08570) z63a2de8 = 1;
	if (e & z9e5f2af) z63a2de8 = 2;
	if (e & z32e3848) { z7e88af5(z63a2de8 == 2); return; }
	if (e & (z884daca | zbd4b563)) zf272a87();
}

int z8ad8d82(z1ea8be6* v)
{
	if (!v || (!z910e658 && !z088e12c.has)) return 0;
	memset(v, 0, sizeof(*v));
	const DWORD now = GetTickCount();
	if (zc98579d && !z314f466 && !zd8b4d78() && (LONG)(now - zc98579d) >= 0) { zc98579d = 0; zd1751a0(0, 0); z648aeaa = 0; zd12bcf2 = 0; }
	v->fOpen = zf22247e(now);
	v->zcf8a8e5 = z314f466;
	v->z92c652b = zb421fde;
	v->z822f0cf = zcca8952;
	v->z1e5d86a = z3ba2b20;
	v->nInput = zb7ea71a;
	v->nMax = z1eeca0f;
	v->zd8f5856 = zdf20384;
	if (now < z047008c) v->ze6affeb = ze37be3c;
	v->z7a57a1d = z088e12c.has;
	v->z24baf09 = z4452880();
	v->zcc658c2 = zd8b4d78();
	v->z5ad0488 = zd5c72f9();
	if (v->z5ad0488 && z7ccb18c && v->fOpen <= 0.001f) { z7ccb18c = 0; z365ad42 = GetTickCount64() + za44be31; }
	if (v->z5ad0488 && GetTickCount64() < z365ad42) v->z1868977 = z6f2968c(L"Pr\x00F3xima luta? T, F1 ou Start+Select");
	if (!z910e658 && v->fOpen <= 0.001f && !v->z5ad0488) return 0;
	int room = z0452cf9;
	if (z648aeaa && z910e658) {
		z9bafc5a& W = v->lines[v->z19ef79a++];
		W.sys = 1;
		W.text = z6f2968c(zd137d22);
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

static int zb877b9f(const z5509597* P, const wchar_t* s, int font)
{
	return (s && *s) ? P->measure(P->ctx, s, font, NULL) : 0;
}

static int zd6e3647(const z5509597* P, int bDraw, const wchar_t* s, int x, int y, unsigned int rgb, int font)
{
	if (!s || !*s) return 0;
	return bDraw ? P->text(P->ctx, s, x, y, rgb, font) : zb877b9f(P, s, font);
}

static void ze17451f(const z5509597* P, wchar_t* s, int cap, int font, int maxW)
{
	if (zb877b9f(P, s, font) <= maxW) return;
	int n = (int)wcslen(s);
	if (n > cap - 4) n = cap - 4;
	if (n > 120) n = 120;
	wchar_t t[128];
	for (; n > 0; n--) {
		wcsncpy(t, s, n);
		t[n] = 0;
		wcscat(t, L"...");
		if (zb877b9f(P, t, font) <= maxW) break;
	}
	if (n <= 0) wcscpy(t, L"...");
	wcsncpy(s, t, cap - 1);
	s[cap - 1] = 0;
}

static int z27097ae(const z5509597* P, int bDraw, const wchar_t* s, int x, int y, int maxW, int lh,
                  unsigned int rgb, int font)
{
	wchar_t line[256], z6cbe879[256];
	int lines = 0;
	line[0] = 0;
	const wchar_t* p = s;
	while (p && *p) {
		while (*p == L' ') p++;
		if (!*p) break;
		const wchar_t* e = p;
		while (*e && *e != L' ') e++;
		int wl = (int)(e - p);
		if (wl > 120) wl = 120;
		const int cur = (int)wcslen(line);
		if (cur + 1 + wl >= 255) break;
		wcscpy(z6cbe879, line);
		if (cur) wcscat(z6cbe879, L" ");
		wcsncat(z6cbe879, p, wl);
		if (cur && zb877b9f(P, z6cbe879, font) > maxW) {
			if (bDraw) P->text(P->ctx, line, x, y + lines * lh, rgb, font);
			lines++;
			wcsncpy(line, p, wl);
			line[wl] = 0;
		} else {
			wcscpy(line, z6cbe879);
		}
		p = e;
	}
	if (line[0]) {
		ze17451f(P, line, 256, font, maxW);
		if (bDraw) P->text(P->ctx, line, x, y + lines * lh, rgb, font);
		lines++;
	}
	return lines;
}

static void z65d0b6e(wchar_t* out, int cap, int n)
{
	wchar_t d[16];
	_snwprintf(d, 15, L"%d", n < 0 ? 0 : n);
	d[15] = 0;
	const int len = (int)wcslen(d);
	int k = 0;
	for (int i = 0; i < len && k < cap - 2; i++) {
		if (i && (len - i) % 3 == 0) out[k++] = L'.';
		out[k++] = d[i];
	}
	out[k] = 0;
}

static void z5525f3e(const z5509597* P, int x, int y, int w, int h, const wchar_t* label, int bAccept, int zf1e38aa)
{
	const int r = h / 4 > 1 ? h / 4 : 1;
	const int b = P->unit >= 18 ? 2 : 1;
	if (zf1e38aa) {
		const int g = b + 1;
		z8eca64b(P->rect, P->ctx, x - g, y - g, w + g * 2, h + g * 2, r + g, 0x3CC8DC, 0x90);
		z8eca64b(P->rect, P->ctx, x, y, w, h, r, 0xFFFFFF, 0xFF);
	} else {
		z8eca64b(P->rect, P->ctx, x, y, w, h, r, bAccept ? 0x2F6B3A : 0x4A505A, 0xFF);
	}
	z8eca64b(P->rect, P->ctx, x + b, y + b, w - b * 2, h - b * 2, r > b ? r - b : 1, bAccept ? 0x1F3B26 : 0x2A2F37, 0xFF);
	const int tw = zb877b9f(P, label, 0);
	P->text(P->ctx, label, x + (w - tw) / 2, y + (h - P->unit) / 2, bAccept ? 0x7EE787 : 0xF0F3F6, 0);
}

static int z5bf702d(const z5509597* P, int x, int y, int w, int bDraw)
{
	const int u = P->unit > 0 ? P->unit : 10, s = P->small > 0 ? P->small : u;
	const int pad = u / 3 > 2 ? u / 3 : 2, gap = u / 6 > 1 ? u / 6 : 1;
	int cy = y;

	{
		wchar_t tag[48];
		_snwprintf(tag, 47, z6f2968c(L"ESTA\x00C7\x00C3O %d"), z088e12c.ze607124);
		tag[47] = 0;
		const int tp = s / 3 > 2 ? s / 3 : 2;
		const int z92ad069 = zb877b9f(P, tag, 1) + tp * 2, z4314d39 = s + 2;
		const int z7a61baa = u > z4314d39 ? u : z4314d39;
		if (bDraw) {
			z8eca64b(P->rect, P->ctx, x, cy + (z7a61baa - z4314d39) / 2, z92ad069, z4314d39, z4314d39 / 5 > 1 ? z4314d39 / 5 : 1, z574b01f, 0xFF);
			P->text(P->ctx, tag, x + tp, cy + (z7a61baa - z4314d39) / 2 + 1, z0bf0bb5, 1);
			wchar_t title[80];
			wcsncpy(title, z6f2968c(L"Continuar na esta\x00E7\x00E3o?"), 79);
			title[79] = 0;
			ze17451f(P, title, 80, 0, w - z92ad069 - pad);
			P->text(P->ctx, title, x + z92ad069 + pad, cy + (z7a61baa - u) / 2, z574b01f, 0);
		}
		cy += z7a61baa + gap * 2;
	}

	{
		const wchar_t* lab = z6f2968c(L"Na fila:");
		int sp = zb877b9f(P, L"a a", 0) - zb877b9f(P, L"aa", 0);
		if (sp < 2) sp = u / 4 > 2 ? u / 4 : 2;
		int xx = x + zd6e3647(P, bDraw, lab, x, cy, 0xC9D1D9, 0) + sp;
		wchar_t nm[48];
		wcsncpy(nm, z088e12c.zde4da44, 47);
		nm[47] = 0;
		ze17451f(P, nm, 48, 0, x + w - xx);
		int tl = 0;
		if (nm[0] == L'[')
			for (int i = 1; nm[i] && i < 12; i++)
				if (nm[i] == L']') { tl = nm[i + 1] ? i + 1 : 0; break; }
		if (bDraw) {
			if (tl) {
				wchar_t t[16];
				wcsncpy(t, nm, tl);
				t[tl] = 0;
				xx += P->text(P->ctx, t, xx, cy, z574b01f, 0);
			}
			P->text(P->ctx, nm + tl, xx, cy, 0xFFFFFF, 0);
		}
		cy += u + gap;
	}

	{
		wchar_t info[128], part[48];
		info[0] = 0;
		if (z088e12c.ping >= 0) {
			_snwprintf(part, 47, L"%d ms", z088e12c.ping);
			part[47] = 0;
			wcscat(info, part);
		}
		if (z088e12c.z76fba51 >= 0) {
			wchar_t pts[24];
			z65d0b6e(pts, 24, z088e12c.z76fba51);
			if (info[0]) wcscat(info, L" \x00B7 ");
			wcscat(info, L"Ranking Manual ");
			wcscat(info, pts);
		}
		if (z088e12c.ft > 0) {
			_snwprintf(part, 47, L"FT%d", z088e12c.ft);
			part[47] = 0;
			if (info[0]) wcscat(info, L" \x00B7 ");
			wcscat(info, part);
		}
		int xx = x;
		if (z088e12c.ping >= 0) {
			const int bw = s / 5 > 2 ? s / 5 : 2, bg = bw / 2 > 1 ? bw / 2 : 1;
			const int lit = z088e12c.ping <= 60 ? 3 : z088e12c.ping <= 120 ? 2 : 1;
			const unsigned int col = lit == 3 ? 0x3FB950 : lit == 2 ? z574b01f : 0xFF5555;
			if (bDraw)
				for (int i = 0; i < 3; i++) {
					const int bh = s * (4 + i * 2) / 10 > 1 ? s * (4 + i * 2) / 10 : 1;
					const int bx = xx + i * (bw + bg), base = cy + s - s / 6;
					P->rect(P->ctx, bx, base - bh, bx + bw, base, i < lit ? col : 0x5A5340, 0xFF);
				}
			xx += 3 * (bw + bg) + bg * 2;
		}
		if (info[0]) {
			ze17451f(P, info, 128, 1, x + w - xx);
			zd6e3647(P, bDraw, info, xx, cy, 0xC9BF9F, 1);
		}
		cy += s + gap;
	}

	if (zd5c72f9()) {
		const ULONGLONG now = GetTickCount64();
		const ULONGLONG left = z088e12c.deadline > now ? z088e12c.deadline - now : 0;
		const int z404a75f = u / 6 > 2 ? u / 6 : 2;
		cy += gap * 2;
		if (bDraw) {
			const long long total = (long long)(z088e12c.total > 0 ? z088e12c.total : z8f977be) * 1000;
			long long fill = (long long)w * (long long)left / total;
			if (fill > w) fill = w;
			z8eca64b(P->rect, P->ctx, x, cy, w, z404a75f, z404a75f / 2, 0x3D3313, 0xFF);
			if (fill > 0) z8eca64b(P->rect, P->ctx, x, cy, (int)fill, z404a75f, z404a75f / 2, z574b01f, 0xFF);
		}
		cy += z404a75f + gap;
		wchar_t t[96];
		_snwprintf(t, 95, z6f2968c(L"fecha em %d s \x00B7 sem resposta = n\x00E3o"), (int)((left + 999) / 1000));
		t[95] = 0;
		const int tw = zb877b9f(P, t, 1);
		zd6e3647(P, bDraw, t, x + w - tw, cy, 0xC9BF9F, 1);
		cy += s + gap * 2;
		const int bh = u + u / 2 + 2, zfd30726 = pad + gap;
		const int bw = (w - zfd30726) / 2;
		if (bDraw) {
			const int zcd1214d = zd8b4d78() ? z63a2de8 : 0;
			z5525f3e(P, x, cy, bw, bh, z6f2968c(L"Recusar"), 0, zcd1214d == 1);
			z5525f3e(P, x + bw + zfd30726, cy, w - bw - zfd30726, bh, z6f2968c(L"Aceitar"), 1, zcd1214d == 2);
		}
		cy += bh + gap * 3;
		const int only = z4452880() || !z314f466;
		const wchar_t* h = P->ansi
			? (only ? L"Esq./Dir.: escolhe - Enter ou A: confirma"
			        : L"Cima: bot\x00F5" L"es - Esq./Dir.: escolhe - Enter ou A: confirma")
			: (only ? L"\x2190 \x2192 escolhe \x00B7 Enter ou A confirma"
			        : L"\x2191 vai para os bot\x00F5" L"es \x00B7 \x2190 \x2192 escolhe \x00B7 Enter ou A confirma");
		cy += s * z27097ae(P, bDraw, z6f2968c(h), x, cy, w, s, 0x9AA3AD, 1);
	} else {
		const wchar_t* t = (z088e12c.zcf8dc2e && z088e12c.zeb6e153)
			? L"Voc\x00EA aceitou: voc\x00EA continua na esta\x00E7\x00E3o depois desta partida."
			: z088e12c.zcf8dc2e ? L"Voc\x00EA recusou: depois desta partida voc\x00EA sai da esta\x00E7\x00E3o."
			                  : L"Sem resposta: depois desta partida voc\x00EA sai da esta\x00E7\x00E3o.";
		cy += gap * 2;
		cy += u * z27097ae(P, bDraw, z6f2968c(t), x, cy, w, u, (z088e12c.zcf8dc2e && z088e12c.zeb6e153) ? 0x7EE787 : 0xFFB86C, 0);
	}
	return cy - y;
}

int z07cecca(const z5509597* P, int x, int y, int w, int bDraw)
{
	if (!P || !P->rect || !P->text || !P->measure || !z088e12c.has || w <= 20) return 0;
	const int u = P->unit > 0 ? P->unit : 10;
	const int pad = u / 2 > 3 ? u / 2 : 3;
	const int b = u >= 18 ? 2 : 1;
	const int r = u / 3 > 2 ? u / 3 : 2;
	const int iw = w - (pad + b) * 2;
	const int h = z5bf702d(P, 0, 0, iw, 0) + (pad + b) * 2;
	if (!bDraw) return h;
	z8eca64b(P->rect, P->ctx, x, y, w, h, r, z574b01f, 0xFF);
	z8eca64b(P->rect, P->ctx, x + b, y + b, w - b * 2, h - b * 2, r > b ? r - b : 1, 0x2A2310, 0xF5);
	z5bf702d(P, x + b + pad, y + b + pad, iw, 1);
	return h;
}

void zb1eff89(const z5509597* P, int cx, int bottom, int h, const wchar_t* szText)
{
	if (!P || !P->rect || !P->text || !P->measure || h <= 4) return;
	const int s = P->small > 0 ? P->small : h / 2;
	const int bw = h * 62 / 100, bh = h * 44 / 100;
	const int dr = h * 30 / 100 > 2 ? h * 30 / 100 : 2;
	const int pad = h / 3 > 2 ? h / 3 : 2, gap = h / 4 > 1 ? h / 4 : 1;
	const int tw = szText ? zb877b9f(P, szText, 1) : 0;
	const int w = szText ? pad + bw + gap + tw + gap + dr * 2 + pad : h * 2;
	const int x = cx - w / 2, y = bottom - h;
	const int b = h >= 24 ? 2 : 1;
	z8eca64b(P->rect, P->ctx, x - 2, y - 2, w + 4, h + 4, h / 2 + 2, z574b01f, 0x40);
	z8eca64b(P->rect, P->ctx, x, y, w, h, h / 2, z574b01f, 0xFF);
	z8eca64b(P->rect, P->ctx, x + b, y + b, w - b * 2, h - b * 2, (h - b * 2) / 2, 0x14181E, 0xD0);
	const int z7127108 = szText ? x + pad + bw / 2 : x + w * 34 / 100;
	const int bx = z7127108 - bw / 2, by = y + (h - bh) / 2 - h / 14;
	z8eca64b(P->rect, P->ctx, bx, by, bw, bh, bh / 3 > 1 ? bh / 3 : 1, z574b01f, 0xFF);
	const int tail = bh / 3 > 1 ? bh / 3 : 1;
	for (int i = 0; i < tail; i++)
		P->rect(P->ctx, bx + bw / 5, by + bh + i, bx + bw / 5 + tail - i, by + bh + i + 1, z574b01f, 0xFF);
	if (szText) P->text(P->ctx, szText, bx + bw + gap, y + (h - s) / 2, z574b01f, 1);
	const int zf8c6de4 = szText ? x + w - pad - dr : x + w * 72 / 100, dcy = y + h / 2;
	disk(P->rect, P->ctx, zf8c6de4, dcy, dr, z574b01f, 0xFF);
	const int ow = zb877b9f(P, L"1", 1);
	P->text(P->ctx, L"1", zf8c6de4 - ow / 2, dcy - s / 2, z0bf0bb5, 1);
}
