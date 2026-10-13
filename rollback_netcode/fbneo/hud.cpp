#include "burnint.h"
#include "hud.h"
#include "speedrun.h"
#include "img_hash.h"
#include "overlay.h"
#include "fbneo_host.h"
#include "../core/ggpo_bridge.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <string.h>

#define zbd8e7ca   0xF8F8F2u

static int z9126c4e = 1;

static double    zf189423 = 0.0;
static long long z43931b1 = 0;
static long long z387cb2a = 0;
static int       zac752a9 = 0;

static long long zfbe0196(void)
{
	LARGE_INTEGER now;
	if (!z43931b1) {
		LARGE_INTEGER f;
		if (!QueryPerformanceFrequency(&f) || f.QuadPart == 0) return 0;
		z43931b1 = f.QuadPart;
	}
	if (!QueryPerformanceCounter(&now)) return 0;
	return now.QuadPart;
}

static void z14073fa(void)
{
	const long long now = zfbe0196();
	if (!now) return;

	if (!z387cb2a) { z387cb2a = now; zac752a9 = 0; return; }

	zac752a9++;
	const long long elapsed = now - z387cb2a;
	if (elapsed >= z43931b1 / 2) {
		zf189423 = (double)zac752a9 * (double)z43931b1 / (double)elapsed;
		z387cb2a = now;
		zac752a9 = 0;
	}
}

static int  zd030f08 = -1;
static int  z59e1b11 = -1;
static int  zac282f1 = -1;
static int  z806cc0e = -1;
static int  z5cc2cfc = 0;

static int       zccf0ffb = -1;
static int       ze7b9ce5   = 0;
static long long zca9dd5e   = 0;

static void zd7bfa8b(int ping)
{
	const long long now = zfbe0196();
	if (!now || !z43931b1) return;
	if (zca9dd5e && now - zca9dd5e < z43931b1) return;
	zca9dd5e = now;

	if (zccf0ffb >= 0) {
		int d = ping - zccf0ffb;
		if (d < 0) d = -d;
		ze7b9ce5 += d - ze7b9ce5 / 4;
		z59e1b11 = (ze7b9ce5 + 2) / 4;
	} else {
		z59e1b11 = 0;
	}
	zccf0ffb = ping;
}

static void sample(void)
{
	if (!zb71d2fc() || !ze41ba77()) {
		zd030f08 = z59e1b11 = zac282f1 = z806cc0e = -1;
		z5cc2cfc = 0;
		zccf0ffb = -1; ze7b9ce5 = 0; zca9dd5e = 0;
		return;
	}

	z5cc2cfc = zab20c35() >= 0;

	zac282f1    = zcb8e9fd();
	z806cc0e = z57e44c7();

	zc3f6a60 s;
	zd030f08 = (z70d4a8a(&s) == z30e34bc && s.valid) ? s.zd534a2f : -1;
	if (zd030f08 >= 0) zd7bfa8b(zd030f08);
}

static void num(char* dst, size_t cap, const char* fmt, int v)
{
	if (v < 0) snprintf(dst, cap, "--");
	else       snprintf(dst, cap, fmt, v);
}

#define zbdede75 3000
static long long ze1d07e8 = 0;
static int z2fa674c = -1;

void z34d19c6(void)
{
	const long long now = zfbe0196();
	if (!now || !z43931b1) return;
	z2fa674c = -1;
	ze1d07e8 = now + (z43931b1 * zbdede75) / 1000;
}

void z0d8fbe5(int v, int ms)
{
	const long long now = zfbe0196();
	if (!now || !z43931b1) return;
	z2fa674c = v;
	ze1d07e8 = now + (z43931b1 * ms) / 1000;
}

static int z8d3b69b(void)
{
	if (!ze1d07e8) return 0;
	if (zfbe0196() < ze1d07e8) return 1;
	ze1d07e8 = 0;
	return 0;
}

void z8b4c8e3(void)
{
	z14073fa();
	sample();
}

void z06e3af7(void)
{
	z9126c4e = z9126c4e ? 0 : 1;
}

int z2eabea4(void) { return z9126c4e; }

int z6100f60(z888fe22* out)
{
	if (!out) return 0;
	const int z7c2ac87 = z8d3b69b();
	if (z9126c4e == 0 && !z7c2ac87) return 0;
	memset(out, 0, sizeof(*out));

	out->nMode   = z9126c4e;
	out->zd4c6117 = zbd8e7ca;

	if (z9126c4e >= 1) {
		char z389a2ee[16], z3498d5c[16];
		num(z389a2ee, sizeof(z389a2ee), "%df", z806cc0e);
		num(z3498d5c, sizeof(z3498d5c), "%dms", zd030f08);
		snprintf(out->z0f6d336, sizeof(out->z0f6d336), "%srollback %s | ping %s",
		         z5cc2cfc ? "DESYNC | " : "", z389a2ee, z3498d5c);
	}

	if (z7c2ac87) {
		int ra = (bRunAhead > 0 && !(BurnDrvGetFlags() & BDF_RUNAHEAD_DISABLED)) ? bRunAhead : 0;
		if (ra > 2) ra = 2;
		snprintf(out->z30b14bc, sizeof(out->z30b14bc), "%d", z2fa674c >= 0 ? z2fa674c : ra);
	}

	return 1;
}

static void zf6624e3(unsigned char* img, int w, int h, int bpp, int pitch,
                       int x, int y, const char* s, unsigned int rgb)
{
	z11f9d81(img, w, h, bpp, pitch, x + 1, y + 1, s, 0x101014u, 2);
	z11f9d81(img, w, h, bpp, pitch, x,     y,     s, rgb,       2);
}

static void zc639fea(unsigned char* img, int w, int h, int bpp, int pitch)
{
	if (!zf22a9a5()) zf7949f6(img, w, h, bpp, pitch);

	if (z9534659()) return;
	if (!img || w <= 0 || h <= 0 || pitch <= 0) return;
	if (bpp != 2 && bpp != 3 && bpp != 4) return;

	z888fe22 L;
	if (!z6100f60(&L)) return;

	z81ef24e();

	int lh = 0;
	zf92b87e("0", 2, &lh);
	if (lh <= 0) return;

	const int right = w - 2;
	int y = 1;

	int z7a04050 = 0, zf4478a4 = 0;
	zcf14204(&z7a04050, &zf4478a4);

	if (L.nMode >= 1) {
		const int w1 = zf92b87e(L.z0f6d336, 2, NULL);
		if (zf4478a4 > 0 && right - w1 < z7a04050 + 6) y = zf4478a4 + 1;
		zf6624e3(img, w, h, bpp, pitch, right - w1, y, L.z0f6d336, L.zd4c6117);
		y += lh;
	}
	if (L.nMode >= 2) {
		const int w2 = zf92b87e(L.z6af1ebd, 2, NULL);
		zf6624e3(img, w, h, bpp, pitch, right - w2, y, L.z6af1ebd, L.zd4c6117);
		y += lh;
	}
	if (L.z30b14bc[0]) {
		const int wf = zf92b87e(L.z30b14bc, 2, NULL);
		zf6624e3(img, w, h, bpp, pitch, right - wf, y, L.z30b14bc, L.zd4c6117);
	}
}

void zdc4b33f(unsigned char* img, int w, int h, int bpp, int pitch)
{
	zc639fea(img, w, h, bpp, pitch);
	za50e763(img, w, h, bpp, pitch);
}
