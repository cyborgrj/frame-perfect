#include "burnint.h"
#include "ram_probe.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define z0393b07       64
#define zd6683c9      2
#define z988262b   (128u * 1024u * 1024u)

static const struct { const char* szName; unsigned int zc7cb907; int bSwap; const char* z32e28a8; unsigned int z5ec0ac4; } zb2f6748[] = {
	{ "68K RAM",  0x100000,   1, NULL, 0 },
	{ "CpsRamFF", 0xFF0000,   1, NULL, 0 },
	{ "All RAM",  0x01000000, 0, "MIDWAY Wolf-Unit", 0x80000 },
	{ "Main RAM", 0x02000000, 3, "CPS-3", 0x80000 },
	{ "All Ram",  0x00000000, 0, "drv:ddragon", 0x1000 },
};

static const UINT8* z774f5ca        = NULL;
static UINT32       z8bb81d5     = 0;
static UINT32       z2892c5a    = 0;
static int          z6eabc60       = 0;
static int          z2136f91    = 0;
static int          z78a1f18 = 0;

static FILE*  z22f2a7a        = NULL;
static UINT8* z69306aa      = NULL;
static int    z2d3ac20  = 30;
static int    z737d45f = 0;
static UINT32 ze9bc394     = 0;
static UINT32 z4aada97   = 0;
static UINT32 ze24b71c   = 0;
static void  (*zf2fc3fb)(const char*) = NULL;

static void z17945bb(void (*pfn)(const char*), const char* fmt, ...)
{
	if (!pfn) return;
	char buf[512];
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	buf[sizeof(buf) - 1] = '\0';
	pfn(buf);
}

static char         z328b625[512];
static const UINT8* z1d21b01;
static UINT32       z48db153;
static UINT32       zbe22120;
static int          z698d41e;

static INT32 __cdecl z225a3a9(struct BurnArea* pba)
{
	if (!pba || !pba->szName || !pba->Data || pba->nLen == 0) return 0;

	size_t used = strlen(z328b625);
	if (used + strlen(pba->szName) + 3 < sizeof(z328b625)) {
		if (used) strcat(z328b625, ", ");
		strcat(z328b625, pba->szName);
	}

	if (z1d21b01) return 0;
	for (unsigned i = 0; i < sizeof(zb2f6748) / sizeof(zb2f6748[0]); i++) {
		if (strcmp(pba->szName, zb2f6748[i].szName) == 0) {
			if (zb2f6748[i].z32e28a8 && strncmp(zb2f6748[i].z32e28a8, "drv:", 4) == 0) {
				const char* want = zb2f6748[i].z32e28a8 + 4;
				const char* name = BurnDrvGetTextA(DRV_NAME);
				const char* parent = BurnDrvGetTextA(DRV_PARENT);
				if (!(name && strcmp(name, want) == 0) && !(parent && strcmp(parent, want) == 0)) continue;
			} else if (zb2f6748[i].z32e28a8) {
				const char* sys = BurnDrvGetTextA(DRV_SYSTEM);
				if (!sys || strcmp(sys, zb2f6748[i].z32e28a8) != 0) continue;
			}
			z1d21b01     = (const UINT8*)pba->Data;
			z48db153  = pba->nLen;
			if (zb2f6748[i].z5ec0ac4 && z48db153 > zb2f6748[i].z5ec0ac4) z48db153 = zb2f6748[i].z5ec0ac4;
			zbe22120 = zb2f6748[i].zc7cb907;
			z698d41e = zb2f6748[i].bSwap;
			break;
		}
	}
	return 0;
}

int zfc8fae7(void (*z930c6f2)(const char*))
{
	if (z774f5ca) return z7e634bf;
	if (z78a1f18) return z87693a2;
	z78a1f18 = 1;

	z1d21b01 = NULL; z48db153 = 0; zbe22120 = 0; z698d41e = 0; z328b625[0] = '\0';

	INT32 (__cdecl *pPrev)(struct BurnArea*) = BurnAcb;
	BurnAcb = z225a3a9;
	BurnAreaScan(ACB_MEMORY_RAM | ACB_READ, NULL);
	BurnAcb = pPrev;

	if (!z1d21b01) {
		z17945bb(z930c6f2, "ram probe: no work RAM I recognise. areas seen: %s",
		          z328b625[0] ? z328b625 : "(none)");
		return z87693a2;
	}

	z774f5ca     = z1d21b01;
	z8bb81d5  = z48db153;
	z2892c5a = zbe22120;
	z6eabc60    = z698d41e;
	z2136f91 = (zbe22120 == 0x01000000) ? 1 : 0;
	z17945bb(z930c6f2, "ram probe: work RAM %u bytes at cpu 0x%06X", z8bb81d5, z2892c5a);
	return z7e634bf;
}

const unsigned char* zbc4b3c3(unsigned int* pnLen, unsigned int* z79baa7d)
{
	if (pnLen)     *pnLen     = z8bb81d5;
	if (z79baa7d) *z79baa7d = z2892c5a;
	return z774f5ca;
}

unsigned char zaefff6f(unsigned int zf335dba)
{
	if (!z774f5ca || z8bb81d5 == 0) return 0;
	unsigned int off = zf335dba - z2892c5a;
	if (z2136f91) off >>= 3;
	off &= (z8bb81d5 - 1);
	off ^= (unsigned int)z6eabc60;
	return z774f5ca[off];
}

static void z2030794(FILE* f, UINT8 v) { fputc(v, f); }

static void zbffa3d7(FILE* f, UINT32 v)
{
	fputc((int)( v        & 0xFF), f);
	fputc((int)((v >>  8) & 0xFF), f);
	fputc((int)((v >> 16) & 0xFF), f);
	fputc((int)((v >> 24) & 0xFF), f);
}

int zb8ed435(int nInterval, void (*z930c6f2)(const char*))
{
	if (z22f2a7a) return z7e634bf;

	int rc = zfc8fae7(z930c6f2);
	if (rc != z7e634bf) return rc;

	z69306aa = (UINT8*)malloc(z8bb81d5);
	if (!z69306aa) return zd893ffb;

	const char* szGame = BurnDrvGetTextA(DRV_NAME);
	if (!szGame) szGame = "game";

	char szPath[256];
	time_t t = time(NULL);
	struct tm* lt = localtime(&t);
	if (lt) {
		snprintf(szPath, sizeof(szPath), "rbf-probe-%s-%04d%02d%02d-%02d%02d%02d.rbfp",
		         szGame, lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday,
		         lt->tm_hour, lt->tm_min, lt->tm_sec);
	} else {
		snprintf(szPath, sizeof(szPath), "rbf-probe-%s.rbfp", szGame);
	}

	z22f2a7a = fopen(szPath, "wb");
	if (!z22f2a7a) {
		free(z69306aa); z69306aa = NULL;
		z17945bb(z930c6f2, "ram probe: could not open %s for writing", szPath);
		return z9dba26e;
	}

	z2d3ac20  = (nInterval > 0) ? nInterval : 30;
	z737d45f = 0;
	ze9bc394     = 0;
	z4aada97   = 0;
	ze24b71c   = 0;
	zf2fc3fb       = z930c6f2;

	char z4af555c[32]; memset(z4af555c, 0, sizeof(z4af555c));
	strncpy(z4af555c, szGame, sizeof(z4af555c) - 1);
	char z6719ceb[28];     memset(z6719ceb, 0, sizeof(z6719ceb));

	fwrite("RBFPROBE", 1, 8, z22f2a7a);
	zbffa3d7(z22f2a7a, zd6683c9);
	zbffa3d7(z22f2a7a, z8bb81d5);
	zbffa3d7(z22f2a7a, z2892c5a);
	zbffa3d7(z22f2a7a, z0393b07);
	zbffa3d7(z22f2a7a, (UINT32)z2d3ac20);
	fwrite(z4af555c, 1, sizeof(z4af555c), z22f2a7a);
	zbffa3d7(z22f2a7a, (UINT32)z6eabc60);
	fwrite(z6719ceb,     1, sizeof(z6719ceb),     z22f2a7a);

	z17945bb(z930c6f2, "ram probe: recording %s every %d frames", szPath, z2d3ac20);
	return z7e634bf;
}

static void za9b8e6d(void)
{
	static int z9b9472d = 0;
	static int s_n = 0;
	static unsigned int z00f5299[4], zcf700fd[4];
	if (!z9b9472d) {
		z9b9472d = 1;
		char v[128] = "";
		if (GetEnvironmentVariableA("RBF_TESTE_VIDA", v, sizeof(v))) {
			char* p = v;
			while (*p && s_n < 4) {
				char* end = NULL;
				z00f5299[s_n] = (unsigned int)strtoul(p, &end, 16);
				zcf700fd[s_n] = 1;
				if (end && *end == '=') zcf700fd[s_n] = (unsigned int)strtoul(end + 1, &end, 10);
				if (z00f5299[s_n]) s_n++;
				p = end;
				while (p && *p == ',') p++;
				if (!p) break;
			}
		}
		if (s_n && zfc8fae7(NULL) != z7e634bf) s_n = 0;
	}
	if (!s_n || !z774f5ca) return;
	static long long z98cd887 = -1, zb2b9b69 = 0;
	if (z98cd887 < 0) {
		char v[32] = "";
		z98cd887 = GetEnvironmentVariableA("RBF_TESTE_VIDA_DESDE", v, sizeof(v)) ? atoll(v) : 0;
	}
	if (++zb2b9b69 < z98cd887) return;
	UINT8* ram = (UINT8*)z774f5ca;
	for (int i = 0; i < s_n; i++) {
		unsigned int off = z00f5299[i] - z2892c5a;
		if (z2136f91) off >>= 3;
		if (off + 1 >= z8bb81d5) continue;
		const unsigned int cur = ram[off] | (ram[off + 1] << 8);
		static int z2e059c0[4] = { 0, 0, 0, 0 };
		static unsigned int s_last[4] = { 0, 0, 0, 0 };
		z2e059c0[i] = (cur == s_last[i]) ? z2e059c0[i] + 1 : 0;
		s_last[i] = cur;
		if (z2e059c0[i] < 30) continue;
		if (cur >= 1 && cur <= 166 && cur != zcf700fd[i]) {
			ram[off] = (UINT8)(zcf700fd[i] & 0xFF);
			ram[off + 1] = (UINT8)(zcf700fd[i] >> 8);
		}
	}
}

static void zac88549(void)
{
	static int z9b9472d = 0, s_n = 0;
	static unsigned int zf819f71[4], z632cfad[4];
	static long long z3da9584 = 0, z3dbf348 = 0, zb2b9b69 = 0;
	if (!z9b9472d) {
		z9b9472d = 1;
		char v[128] = "";
		if (GetEnvironmentVariableA("RBF_TESTE_BYTE", v, sizeof(v))) {
			char* p = v;
			while (*p && s_n < 4) {
				char* end = NULL;
				zf819f71[s_n] = (unsigned int)strtoul(p, &end, 16);
				z632cfad[s_n] = 0;
				if (end && *end == '=') z632cfad[s_n] = (unsigned int)strtoul(end + 1, &end, 10);
				if (zf819f71[s_n]) s_n++;
				p = end;
				while (p && *p == ',') p++;
				if (!p) break;
			}
			char d[32] = "";
			if (GetEnvironmentVariableA("RBF_TESTE_BYTE_DE", d, sizeof(d))) z3da9584 = atoll(d);
			if (GetEnvironmentVariableA("RBF_TESTE_BYTE_ATE", d, sizeof(d))) z3dbf348 = atoll(d);
		}
		if (s_n && zfc8fae7(NULL) != z7e634bf) s_n = 0;
	}
	if (!s_n || !z774f5ca) return;
	++zb2b9b69;
	if (zb2b9b69 < z3da9584 || (z3dbf348 > 0 && zb2b9b69 > z3dbf348)) return;
	for (int i = 0; i < s_n; i++) {
		unsigned int off = zf819f71[i] - z2892c5a;
		if (z2136f91) off >>= 3;
		off &= (z8bb81d5 - 1);
		off ^= (unsigned int)z6eabc60;
		((UINT8*)z774f5ca)[off] = (UINT8)z632cfad[i];
	}
}

void z1907e87(void)
{
	za9b8e6d();
	zac88549();
	if (!z22f2a7a) return;

	ze9bc394++;
	if (z737d45f > 0) { z737d45f--; return; }
	z737d45f = z2d3ac20 - 1;

	const UINT32 z5f9d2a3 = (z8bb81d5 + z0393b07 - 1) / z0393b07;

	if (z4aada97 == 0) {
		z2030794(z22f2a7a, 1);
		zbffa3d7(z22f2a7a, ze9bc394);
		fwrite(z774f5ca, 1, z8bb81d5, z22f2a7a);
		ze24b71c += 5 + z8bb81d5;
	} else {
		UINT32 z9df25b7 = 0;
		for (UINT32 c = 0; c < z5f9d2a3; c++) {
			UINT32 off = c * z0393b07;
			UINT32 len = (off + z0393b07 <= z8bb81d5) ? z0393b07 : (z8bb81d5 - off);
			if (memcmp(z774f5ca + off, z69306aa + off, len) != 0) z9df25b7++;
		}

		if (z9df25b7 * (4 + z0393b07) >= z8bb81d5) {
			z2030794(z22f2a7a, 1);
			zbffa3d7(z22f2a7a, ze9bc394);
			fwrite(z774f5ca, 1, z8bb81d5, z22f2a7a);
			ze24b71c += 5 + z8bb81d5;
		} else {
			z2030794(z22f2a7a, 2);
			zbffa3d7(z22f2a7a, ze9bc394);
			zbffa3d7(z22f2a7a, z9df25b7);
			for (UINT32 c = 0; c < z5f9d2a3; c++) {
				UINT32 off = c * z0393b07;
				UINT32 len = (off + z0393b07 <= z8bb81d5) ? z0393b07 : (z8bb81d5 - off);
				if (memcmp(z774f5ca + off, z69306aa + off, len) == 0) continue;
				zbffa3d7(z22f2a7a, c);
				fwrite(z774f5ca + off, 1, len, z22f2a7a);
				for (UINT32 k = len; k < z0393b07; k++) fputc(0, z22f2a7a);
			}
			ze24b71c += 9 + z9df25b7 * (4 + z0393b07);
		}
	}

	memcpy(z69306aa, z774f5ca, z8bb81d5);
	z4aada97++;

	if (ze24b71c >= z988262b) {
		z17945bb(zf2fc3fb, "ram probe: %u MB recorded - stopping before this fills "
		                 "the disk. Far more than one match needs.",
		          ze24b71c / (1024u * 1024u));
		z2702846();
	}
}

void z2702846(void)
{
	if (!z22f2a7a) return;

	z2030794(z22f2a7a, 0);
	fclose(z22f2a7a);
	z22f2a7a = NULL;

	free(z69306aa);
	z69306aa = NULL;

	z17945bb(zf2fc3fb, "ram probe: %u samples over %u frames, %u KB.",
	          z4aada97, ze9bc394, (ze24b71c + 1023) / 1024);
	zf2fc3fb = NULL;
}

int z307837a(void) { return z22f2a7a != NULL; }
