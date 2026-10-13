#include "anticheat.h"

#include <stdio.h>
#include <string.h>

extern "C" unsigned int z3859adb(int nPlayer);
extern "C" const char*  z536c3a8(int nPlayer, int nBit);

#define za93f539            32
#define zaaccb82   180
#define zabe097c   4
#define zb9f56f1     75
#define z7beb09f 3
#define z3721e04 12
#define zf5d831e          48
#define z167a794   10800
#define zf721a60            32
#define zdd0e180      15
#define z13f07a9         8
#define z2aa44c0         8
#define z3cc7728   15
#define z4cc94c6   20
#define zb420ee2     30
#define z9f0e446     70
#define z860e5cd  60

typedef struct {
	int       zd9f1dae;
	long long z5c1952b;
	int       z83cba3a;
	int       zc3fdd29;
	int       z7581f58;
	int       z4f56a75;
	int       zc29f119;
	int       z0d7e8b1;
	int       zeb5da3b;
	long long z1fb5cfe;
	int       z0ab7e7e;
} zaaf91da;

typedef struct { unsigned int z6ec48e5; int nTotal; int z6f56010; int zc29f119; } zfec72eb;

typedef struct {
	int          zee1e43f;
	unsigned int z69ddd90, z3d694bb, zdc304fe, zad258ad;
	zaaf91da     z080e948[za93f539];
	unsigned int nState;
	long long    zb79905b;
	unsigned int zd99a84f[z3721e04];
	int          z4f56a75;
	unsigned int z0dddf14[zf721a60];
	zfec72eb      z74a50b3[zf5d831e];
	int          z721dfb5, z4e20614;
	int          z1000ed9, zbf865ca;
	int          z3a28dbf;
	int          z0761967, zcc80597;
	long long    z12c098f;
	int          zeedc6e3;
} zffe827f;

static int       z910e658 = 0;
static int       z5a44a65 = 0;
static void    (*g_log)(const char*) = NULL;
static long long ze9bc394 = 0;
static long long z1bfdc76 = 0;
static zffe827f    zcd602dd[2];

static void z4f31939(const char* fmt, ...);

static int has(const char* s, const char* w) { return s && strstr(s, w) != NULL; }

static void zf6b696c(int p, zffe827f* s)
{
	s->zee1e43f = 1;
	for (int b = 0; b < za93f539; b++) {
		const char* n = z536c3a8(p, b);
		if (!n) continue;
		if (has(n, "Coin") || has(n, "Start") || has(n, "Service") || has(n, "Reset") ||
		    has(n, "Diag") || has(n, "Test") || has(n, "Select") || has(n, "Dip")) continue;
		const unsigned int m = 1u << b;
		if (has(n, " Up") || has(n, " Down")) { s->z69ddd90 |= m; continue; }
		if (has(n, " Left"))  { s->z69ddd90 |= m; s->zdc304fe  |= m; continue; }
		if (has(n, " Right")) { s->z69ddd90 |= m; s->zad258ad |= m; continue; }
		s->z3d694bb |= m;
	}
	z4f31939("anticheat: P%d - direcoes 0x%X, botoes 0x%X (do mapa de comandos)", p + 1, s->z69ddd90, s->z3d694bb);
}

static void notice(zffe827f* s, int kind)
{
	s->z12c098f = ze9bc394 + zaaccb82;
	s->zeedc6e3  = kind;
}

static void zc3379de(int p, zffe827f* s, zaaf91da* k, int b)
{
	s->z1000ed9++;
	if (k->z83cba3a != 1) { k->z0d7e8b1 = 0; k->zeb5da3b = 0; k->z0ab7e7e = 0; return; }
	s->zbf865ca++;
	const long long gap = k->z5c1952b - k->z1fb5cfe;
	if (k->z0d7e8b1 > 0 && gap <= z2aa44c0) k->z0d7e8b1++;
	else k->z0d7e8b1 = 1;
	if (k->zeb5da3b > 0 && gap <= z4cc94c6) { k->zeb5da3b++; s->z3a28dbf++; }
	else { k->zeb5da3b = 1; k->z0ab7e7e = 0; }
	k->z1fb5cfe = k->z5c1952b;
	if (k->z0d7e8b1 >= z13f07a9 || k->zeb5da3b >= z3cc7728) {
		if (!k->z0ab7e7e && k->zc29f119) k->z0ab7e7e = 1;
		if (!k->z0ab7e7e) {
			k->z0ab7e7e = 1;
			s->z721dfb5++;
			s->zcc80597 = 1;
			const char* n = z536c3a8(p, b);
			if (k->z0d7e8b1 >= z13f07a9)
				z4f31939("anticheat: f%lld possivel TURBO do P%d - %s, %d apertos seguidos de 1 quadro",
				      ze9bc394, p + 1, n ? n : "?", k->z0d7e8b1);
			else
				z4f31939("anticheat: f%lld possivel TURBO do P%d - %s, %d apertos seguidos de 1 quadro (ate %d quadros entre eles)",
				      ze9bc394, p + 1, n ? n : "?", k->zeb5da3b, z4cc94c6);
		}
		notice(s, zc4d6e9b);
	}
	if (!s->z0761967 && s->z1000ed9 >= zb420ee2 &&
	    s->zbf865ca * 100 >= s->z1000ed9 * z9f0e446) {
		s->z0761967 = 1;
		if (s->z3a28dbf * 100 >= s->zbf865ca * z860e5cd) {
			if (!s->zcc80597) {
				s->zcc80597 = 1;
				s->z721dfb5++;
				notice(s, zc4d6e9b);
				z4f31939("anticheat: f%lld possivel TURBO do P%d - %d de %d apertos da janela com exatamente 1 quadro, %d repetindo o botao",
				      ze9bc394, p + 1, s->zbf865ca, s->z1000ed9, s->z3a28dbf);
			}
		} else {
			s->z4e20614++;
			notice(s, z0f4913a);
			z4f31939("anticheat: f%lld possivel MACRO do P%d - %d de %d apertos da janela com exatamente 1 quadro",
			      ze9bc394, p + 1, s->zbf865ca, s->z1000ed9);
		}
	}
}

static void za2b1a23(int p, zffe827f* s, unsigned int bits)
{
	for (int b = 0; b < za93f539; b++) {
		if (!(s->z3d694bb & (1u << b))) continue;
		zaaf91da* k = &s->z080e948[b];
		const int on = (bits >> b) & 1;
		if (on && !k->zd9f1dae) {
			if (k->z5c1952b > 0) {
				const int period = (int)(ze9bc394 - k->z5c1952b);
				if (period == k->zc3fdd29 && k->z83cba3a == k->z7581f58) {
					k->z4f56a75++;
				} else {
					k->zc3fdd29 = period; k->z7581f58 = k->z83cba3a; k->z4f56a75 = 1; k->zc29f119 = 0;
				}
				const int need = period <= 2 ? 5 : period == 3 ? 4 : period <= 8 ? 14 : 0;
				if (need && k->z4f56a75 >= need) {
					if (!k->zc29f119 && k->z0ab7e7e) k->zc29f119 = 1;
					if (!k->zc29f119) {
						k->zc29f119 = 1;
						s->z721dfb5++;
						s->zcc80597 = 1;
						const char* n = z536c3a8(p, b);
						z4f31939("anticheat: f%lld possivel TURBO do P%d - %s, %d apertos seguidos a cada %d quadros, %d apertado",
						      ze9bc394, p + 1, n ? n : "?", k->z4f56a75 + 1, period, k->z7581f58);
					}
					notice(s, zc4d6e9b);
				}
			}
			k->z5c1952b = ze9bc394;
		} else if (!on && k->zd9f1dae) {
			k->z83cba3a = (int)(ze9bc394 - k->z5c1952b);
			zc3379de(p, s, k, b);
		}
		k->zd9f1dae = on;
	}
}

static unsigned int mirror(const zffe827f* s, unsigned int st)
{
	const unsigned int l = st & s->zdc304fe, r = st & s->zad258ad;
	st &= ~(s->zdc304fe | s->zad258ad);
	if (l) st |= s->zad258ad;
	if (r) st |= s->zdc304fe;
	return st;
}

static unsigned int fnv(const unsigned int* a, int n, unsigned int z212301c)
{
	unsigned int h = 2166136261u;
	for (int i = 0; i < n; i++)
		for (int k = 0; k < 4; k++) { h ^= (a[i] >> (k * 8)) & 0xFF; h *= 16777619u; }
	for (int k = 0; k < 4; k++) { h ^= (z212301c >> (k * 8)) & 0xFF; h *= 16777619u; }
	return h ^ (unsigned int)n;
}

static unsigned int zc39788b(const zffe827f* s, const unsigned int* dirs, int n, unsigned int z212301c)
{
	unsigned int alt[z3721e04];
	for (int i = 0; i < n; i++) alt[i] = mirror(s, dirs[i]);
	const unsigned int h1 = fnv(dirs, n, z212301c), h2 = fnv(alt, n, z212301c);
	return h1 < h2 ? h1 : h2;
}

static zfec72eb* z2d20561(zffe827f* s, unsigned int key)
{
	zfec72eb* low = &s->z74a50b3[0];
	for (int i = 0; i < zf5d831e; i++) {
		if (s->z74a50b3[i].nTotal && s->z74a50b3[i].z6ec48e5 == key) return &s->z74a50b3[i];
		if (s->z74a50b3[i].nTotal < low->nTotal) low = &s->z74a50b3[i];
	}
	memset(low, 0, sizeof(*low));
	low->z6ec48e5 = key;
	return low;
}

static void za790078(zffe827f* s, unsigned int st, unsigned int zd55d93a)
{
	unsigned int dirs[z3721e04];
	int n = 0;
	unsigned int d = st & s->z69ddd90;
	if (!d) return;
	dirs[n++] = d;
	for (int k = 1; k <= zdd0e180 && n < z3721e04 && ze9bc394 - k >= 1; k++) {
		const unsigned int pd = s->z0dddf14[(ze9bc394 - k) % zf721a60] & s->z69ddd90;
		if (!pd) break;
		if (pd != dirs[n - 1]) dirs[n++] = pd;
	}
	if (n < 2) return;
	for (int i = 0; i < n / 2; i++) { unsigned int t = dirs[i]; dirs[i] = dirs[n - 1 - i]; dirs[n - 1 - i] = t; }
	z2d20561(s, zc39788b(s, dirs, n, zd55d93a))->nTotal++;
}

static void z36ce24b(int p, zffe827f* s)
{
	const int n = s->z4f56a75;
	if (n < z7beb09f || n > z3721e04) return;
	const unsigned int last = s->zd99a84f[n - 1] & s->z3d694bb;
	if (!last) return;
	if (s->nState & last) return;
	unsigned int dirs[z3721e04];
	int nd = 0;
	for (int i = 0; i < n; i++) {
		if (i < n - 1 && (s->zd99a84f[i] & s->z3d694bb)) return;
		for (int j = 0; j < i; j++) if (s->zd99a84f[j] == s->zd99a84f[i]) return;
		const unsigned int d = s->zd99a84f[i] & s->z69ddd90;
		if (d && (nd == 0 || dirs[nd - 1] != d)) dirs[nd++] = d;
	}
	if (nd < 2) return;

	zfec72eb* sh = z2d20561(s, zc39788b(s, dirs, nd, last));
	sh->z6f56010++;
	if (sh->nTotal < sh->z6f56010) sh->nTotal = sh->z6f56010;
	if (sh->z6f56010 >= zabe097c && sh->z6f56010 * 100 >= sh->nTotal * zb9f56f1) {
		if (!sh->zc29f119) {
			sh->zc29f119 = 1;
			s->z4e20614++;
			z4f31939("anticheat: f%lld possivel MACRO do P%d - o mesmo comando limpo (%d passos de 1 quadro) %d de %d vezes na janela",
			      ze9bc394, p + 1, n, sh->z6f56010, sh->nTotal);
		}
		notice(s, z0f4913a);
	}
}

static void z3fd2935(int p, zffe827f* s, unsigned int bits)
{
	const unsigned int st = bits & (s->z69ddd90 | s->z3d694bb);
	const unsigned int prev = s->z0dddf14[(ze9bc394 - 1) % zf721a60];
	const unsigned int zd55d93a = st & s->z3d694bb & ~prev;
	if (zd55d93a) za790078(s, st, zd55d93a);
	if (st != s->nState) {
		const long long dur = ze9bc394 - s->zb79905b;
		if (dur == 1 && s->nState != 0) {
			if (s->z4f56a75 < z3721e04) s->zd99a84f[s->z4f56a75] = s->nState;
			if (s->z4f56a75 <= z3721e04) s->z4f56a75++;
		} else {
			s->z4f56a75 = 0;
		}
		s->nState = st;
		s->zb79905b = ze9bc394;
	} else if (ze9bc394 - s->zb79905b == 2 && s->z4f56a75 > 0) {
		z36ce24b(p, s);
		s->z4f56a75 = 0;
	}
	s->z0dddf14[ze9bc394 % zf721a60] = st;
}

void z47f386e(int bOn, int z980d9ce, void (*z930c6f2)(const char*))
{
	z910e658 = bOn ? 1 : 0;
	z5a44a65 = z980d9ce ? 1 : 0;
	g_log = z930c6f2;
	ze9bc394 = 0;
	z1bfdc76 = 0;
	memset(zcd602dd, 0, sizeof(zcd602dd));
}

static void z7f3a8dd(const unsigned int* bits)
{
	ze9bc394++;
	if (ze9bc394 - z1bfdc76 >= z167a794) {
		z1bfdc76 = ze9bc394;
		for (int p = 0; p < 2; p++) {
			memset(zcd602dd[p].z74a50b3, 0, sizeof(zcd602dd[p].z74a50b3));
			zcd602dd[p].z1000ed9 = zcd602dd[p].zbf865ca = zcd602dd[p].z3a28dbf = 0;
			zcd602dd[p].z0761967 = zcd602dd[p].zcc80597 = 0;
		}
	}
	for (int p = 0; p < 2; p++) {
		zffe827f* s = &zcd602dd[p];
		if (!s->zee1e43f) zf6b696c(p, s);
		za2b1a23(p, s, bits[p]);
		z3fd2935(p, s, bits[p]);
	}
}

void z22b4a5e(void)
{
	if (!z910e658 || z5a44a65) return;
	const unsigned int bits[2] = { z3859adb(0), z3859adb(1) };
	z7f3a8dd(bits);
}

void zd3804c8(unsigned int z2a7085d, unsigned int zb7a1f8c)
{
	if (!z910e658 || !z5a44a65) return;
	const unsigned int bits[2] = { z2a7085d, zb7a1f8c };
	z7f3a8dd(bits);
}

int z709ab7c(int nSide, int z2642b88)
{
	if (nSide < 1 || nSide > 2) return 0;
	return z2642b88 == zc4d6e9b ? zcd602dd[nSide - 1].z721dfb5 :
	       z2642b88 == z0f4913a ? zcd602dd[nSide - 1].z4e20614 : 0;
}

int z8486cb4(int* pSide, int* zd49fdcf)
{
	if (!z910e658) return 0;
	int best = -1;
	for (int p = 0; p < 2; p++)
		if (zcd602dd[p].z12c098f > ze9bc394 &&
		    (best < 0 || zcd602dd[p].z12c098f > zcd602dd[best].z12c098f)) best = p;
	if (best < 0) return 0;
	if (pSide) *pSide = best + 1;
	if (zd49fdcf) *zd49fdcf = zcd602dd[best].zeedc6e3;
	return 1;
}

#include <stdarg.h>
static void z4f31939(const char* fmt, ...)
{
	if (!g_log) return;
	char buf[256];
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	buf[sizeof(buf) - 1] = '\0';
	g_log(buf);
}
