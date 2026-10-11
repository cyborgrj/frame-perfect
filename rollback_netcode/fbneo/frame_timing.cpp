#include "frame_timing.h"
#include "burnint.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

extern "C" void z5d90bae(const char* s);
extern unsigned int nVidSelect;

#define z45bab31   30.0
#define z934071a     250.0
#define z2691e29     0.25
#define z3b3f254       400

struct zdd8cd8b {
	long long n;
	double    sum, zab820d4, mn, mx;
	unsigned  hist[z3b3f254];
	long long z94925ed;
	long long z6a8c37a, zb3e9574;

	void reset() { memset(this, 0, sizeof(*this)); mn = 1e9; }

	void add(double ms, double period)
	{
		n++;
		sum += ms; zab820d4 += ms * ms;
		if (ms < mn) mn = ms;
		if (ms > mx) mx = ms;
		int b = (int)(ms / z2691e29);
		if (b < 0) b = 0;
		if (b >= z3b3f254) b = z3b3f254 - 1;
		hist[b]++;
		if (period > 0) {
			if (fabs(ms - period) > 2.0) z94925ed++;
			if (ms < period * 0.5) z6a8c37a++;
			if (ms > period * 1.5) zb3e9574++;
		}
	}

	double mean() const { return n ? sum / (double)n : 0.0; }
	double sd() const
	{
		if (n < 2) return 0.0;
		const double m = mean();
		const double v = zab820d4 / (double)n - m * m;
		return v > 0 ? sqrt(v) : 0.0;
	}
	double pct(double p) const
	{
		if (!n) return 0.0;
		const long long want = (long long)ceil(p * (double)n);
		long long acc = 0;
		for (int i = 0; i < z3b3f254; i++) {
			acc += hist[i];
			if (acc >= want) return (i + 0.5) * z2691e29;
		}
		return z3b3f254 * z2691e29;
	}
};

static int        z08b6939    = 0;
static int        z9dd5725  = 0;
static FILE*      z7e589f2      = NULL;
static int        zee12ad6  = 0;
static double     z2f1766e    = 0.0;
static long long  zfffea48       = 0;
static long long  z2e5240f  = 0, z905337f = 0, z1cce77d = 0;
static long long  z9ebd558 = 0;
static unsigned   ze267bd7 = 0;
static double     zacff564 = -1.0, z1b624f1 = -1.0;
static long long  zae94da7   = 0;
static long long  z1491418   = 0;
static long long  za6591eb = 0;
static int        z01e3441 = 0;

static zdd8cd8b g_win[4], z67b2509[4];
enum { ze3211d9 = 0, za0b0d8c = 1, z6714d5b = 2, z17f4406 = 3 };

static long long now()
{
	LARGE_INTEGER t;
	QueryPerformanceCounter(&t);
	return t.QuadPart;
}

static double z12d4ead()
{
	return nBurnFPS > 0 ? 100000.0 / (double)nBurnFPS : 0.0;
}

static const char* z496ba9b()
{
	switch (nVidSelect) {
		case 0: return "DirectDraw";
		case 1: return "Direct3D 7";
		case 2: return "DirectDraw SoftFX";
		case 3: return "DirectX 9";
		case 4: return "DirectX 9 Alt";
	}
	return "?";
}

static void z6a9d6f9(char* szOut, size_t cap)
{
	szOut[0] = 0;
	if (!GetModuleFileNameA(NULL, szOut, (DWORD)cap)) { szOut[0] = 0; return; }
	char* p = strrchr(szOut, '\\');
	if (p) p[1] = 0; else szOut[0] = 0;
}

static void z0823a98()
{
	if (z08b6939) return;
	z08b6939 = 1;

	LARGE_INTEGER f;
	QueryPerformanceFrequency(&f);
	z2f1766e = f.QuadPart ? 1000.0 / (double)f.QuadPart : 0.0;

	for (int i = 0; i < 4; i++) { g_win[i].reset(); z67b2509[i].reset(); }

	DEVMODEA dm;
	memset(&dm, 0, sizeof(dm));
	dm.dmSize = sizeof(dm);
	if (EnumDisplaySettingsA(NULL, ENUM_CURRENT_SETTINGS, &dm)) z01e3441 = (int)dm.dmDisplayFrequency;

	atexit(z42b2a72);
}

static void zaed3d3c()
{
	if (!z9dd5725 || z7e589f2) return;
	char szDir[MAX_PATH], szPath[MAX_PATH + 64];
	z6a9d6f9(szDir, sizeof(szDir));
	time_t t = time(NULL);
	struct tm* lt = localtime(&t);
	if (lt) {
		snprintf(szPath, sizeof(szPath), "%srbf-quadros-%04d%02d%02d-%02d%02d%02d.csv", szDir,
		         lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);
	} else {
		snprintf(szPath, sizeof(szPath), "%srbf-quadros.csv", szDir);
	}
	z7e589f2 = fopen(szPath, "w");
	if (!z7e589f2) { z9dd5725 = 0; return; }
	fprintf(z7e589f2, "# %s, jogo %.3f quadros/s (%.3f ms), monitor %d Hz\n",
	        z496ba9b(), nBurnFPS / 100.0, z12d4ead(), z01e3441);
	fprintf(z7e589f2, "quadro;t_ms;entrega_ms;present_ms;emu_ms;tela_ms\n");
	char zde61a7d[MAX_PATH + 64];
	snprintf(zde61a7d, sizeof(zde61a7d), "quadros: gravando %s", szPath);
	z5d90bae(zde61a7d);
}

static void ze88acb8(const char* z0a0dc81, zdd8cd8b* s, double seconds)
{
	const zdd8cd8b& e = s[ze3211d9];
	if (e.n < 10) return;
	const double period = z12d4ead();
	char z35b762b[160] = "tela -";
	if (s[z17f4406].n >= 10) {
		const zdd8cd8b& d = s[z17f4406];
		snprintf(z35b762b, sizeof(z35b762b), "tela media %.2f desvio %.2f p99 %.2f max %.2f ms, fora de +-2 ms %.0f%%",
		         d.mean(), d.sd(), d.pct(0.99), d.mx, 100.0 * (double)d.z94925ed / (double)d.n);
	}
	char zde61a7d[640];
	snprintf(zde61a7d, sizeof(zde61a7d),
	         "quadros (%s, %.0f s, %s, jogo %.2f/s = %.2f ms, monitor %d Hz): "
	         "entrega %lld, media %.2f ms, desvio %.2f, p50 %.2f p95 %.2f p99 %.2f max %.2f, "
	         "fora de +-2 ms %.0f%%, curtos %lld, longos %lld | Present media %.2f max %.2f ms | "
	         "emu media %.2f desvio %.2f | %s | pausas %lld",
	         z0a0dc81, seconds, z496ba9b(), nBurnFPS / 100.0, period, z01e3441,
	         e.n, e.mean(), e.sd(), e.pct(0.50), e.pct(0.95), e.pct(0.99), e.mx,
	         100.0 * (double)e.z94925ed / (double)e.n, e.z6a8c37a, e.zb3e9574,
	         s[z6714d5b].mean(), s[z6714d5b].mx,
	         s[za0b0d8c].mean(), s[za0b0d8c].sd(), z35b762b, z1491418);
	z5d90bae(zde61a7d);
}

static void z62fbaae(long long t)
{
	if (!za6591eb) { za6591eb = t; return; }
	const double s = (double)(t - za6591eb) * z2f1766e / 1000.0;
	if (s < z45bab31) return;
	ze88acb8("parcial", g_win, s);
	for (int i = 0; i < 4; i++) g_win[i].reset();
	za6591eb = t;
	if (z7e589f2) fflush(z7e589f2);
}

static void zd27afef(int k, double ms)
{
	const double period = (k == z6714d5b) ? 0.0 : z12d4ead();
	g_win[k].add(ms, period);
	z67b2509[k].add(ms, period);
}

void zff3d0f4(int z5e74bc4)
{
	z0823a98();
	z9dd5725 = z5e74bc4 ? 1 : 0;
}

void z4e4ccd5(void)
{
	z0823a98();
	const long long t = now();
	if (!zfffea48) zfffea48 = t;
	if (z2e5240f) {
		const double ms = (double)(t - z2e5240f) * z2f1766e;
		if (ms <= z934071a) { zd27afef(za0b0d8c, ms); zacff564 = ms; }
		else zacff564 = -1.0;
	}
	z2e5240f = t;
}

void z3b4888a(void)
{
	z0823a98();
	z1cce77d = now();
	if (!zfffea48) zfffea48 = z1cce77d;
}

void z5504f30(void)
{
	if (!z1cce77d) return;
	const long long t = now();
	const double zaa494ac = (double)(t - z1cce77d) * z2f1766e;
	double z8d520c8 = -1.0;

	if (z905337f) {
		z8d520c8 = (double)(z1cce77d - z905337f) * z2f1766e;
		if (z8d520c8 <= z934071a) zd27afef(ze3211d9, z8d520c8);
		else { z1491418++; z8d520c8 = -1.0; }
	}
	zd27afef(z6714d5b, zaa494ac);
	z905337f = z1cce77d;
	zae94da7++;

	if (z9dd5725 && !z7e589f2) zaed3d3c();
	if (z7e589f2) {
		const double z424d435 = (double)(z1cce77d - zfffea48) * z2f1766e;
		char z374e4a5[24] = "", zd315bdb[24] = "", z50d200e[24] = "";
		if (z8d520c8 >= 0) snprintf(z374e4a5, sizeof(z374e4a5), "%.3f", z8d520c8);
		if (zacff564 >= 0) snprintf(zd315bdb, sizeof(zd315bdb), "%.3f", zacff564);
		if (z1b624f1 >= 0) snprintf(z50d200e, sizeof(z50d200e), "%.3f", z1b624f1);
		fprintf(z7e589f2, "%lld;%.3f;%s;%.3f;%s;%s\n", zae94da7, z424d435, z374e4a5, zaa494ac, zd315bdb, z50d200e);
	}
	z1b624f1 = -1.0;

	z62fbaae(t);
}

void z4ccf091(unsigned int z6e6fe6a, unsigned int z4c1eb79, long long zdb2baae)
{
	(void)z4c1eb79;
	z0823a98();
	if (z9ebd558 && z6e6fe6a == ze267bd7 + 1 && zdb2baae > z9ebd558) {
		const double ms = (double)(zdb2baae - z9ebd558) * z2f1766e;
		if (ms <= z934071a) { zd27afef(z17f4406, ms); z1b624f1 = ms; }
	}
	if (z6e6fe6a != ze267bd7) {
		ze267bd7 = z6e6fe6a;
		z9ebd558 = zdb2baae;
	}
}

void z42b2a72(void)
{
	if (zee12ad6 || !z08b6939) return;
	zee12ad6 = 1;
	if (zfffea48) {
		const double s = (double)(now() - zfffea48) * z2f1766e / 1000.0;
		ze88acb8("total", z67b2509, s);
	}
	if (z7e589f2) { fclose(z7e589f2); z7e589f2 = NULL; }
}
