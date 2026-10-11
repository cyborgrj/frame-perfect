#include "burnint.h"
#include "ficha.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

enum { z584d921 = 0, zdfc107b, z6a30044, zaa0734c, z3ffc86e };

static const struct { const char* szGame; unsigned int nAddr; } zd47805f[] = {
	{ "sfa2",   0xFF8076 },
	{ "sfa3",   0xFF8076 },
	{ "vsavj",  0xFF8076 },
	{ "mvsc",   0xFF4499 },
	{ "msh",    0xFF4C09 },
	{ "mshvsf", 0xFF4C75 },
	{ "xmvsf",  0xFF545F },
	{ "ssf2xj", 0xFF82E9 },
	{ "sf2ce",  0xFF82D9 },
};

#define z0c24531   0x7CE0
#define z7a857a0   0x354
#define zfa670f3      0x362

static int            z910e658    = 0;
static int            za065e40  = z584d921;
static unsigned char* z7e75214   = NULL;
static unsigned int   zf2d7d46   = 0;
static unsigned int   zf5ee0a8   = 0;

static const char*    z1d94425  = NULL;
static unsigned char* z1d21b01 = NULL;
static unsigned int   s_len   = 0;

static void z3c2dd92(void (*pfn)(const char*), const char* fmt, ...)
{
	if (!pfn) return;
	char buf[256];
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	buf[sizeof(buf) - 1] = '\0';
	pfn(buf);
}

static INT32 __cdecl z9a12344(struct BurnArea* pba)
{
	if (z1d21b01 || !pba || !pba->szName || !pba->Data || pba->nLen == 0) return 0;
	if (strcmp(pba->szName, z1d94425) == 0) {
		z1d21b01 = (unsigned char*)pba->Data;
		s_len   = pba->nLen;
	}
	return 0;
}

static unsigned char* zd164f58(const char* szName, int nAction, unsigned int* pnLen)
{
	z1d94425 = szName; z1d21b01 = NULL; s_len = 0;
	INT32 (__cdecl *pPrev)(struct BurnArea*) = BurnAcb;
	BurnAcb = z9a12344;
	BurnAreaScan(nAction | ACB_READ, NULL);
	BurnAcb = pPrev;
	*pnLen = s_len;
	return z1d21b01;
}

static int z1abe89d(const char* z245c249)
{
	const char* name = BurnDrvGetTextA(DRV_NAME);
	return name && strcmp(name, z245c249) == 0;
}

static int attach(void (*z930c6f2)(const char*))
{
	za065e40 = z584d921; z7e75214 = NULL; zf2d7d46 = 0; zf5ee0a8 = 0;
	const char* sys = BurnDrvGetTextA(DRV_SYSTEM);

	if (sys && strstr(sys, "Neo Geo")) {
		unsigned int n = 0;
		unsigned char* p = zd164f58("NVRAM", ACB_NVRAM, &n);
		if (!p || n < 0x40) { z3c2dd92(z930c6f2, "ficha: Neo Geo sem a RAM de backup (placa AES) - fica como esta"); return 0; }
		za065e40 = zdfc107b; z7e75214 = p; zf2d7d46 = n;
		return 1;
	}
	for (unsigned i = 0; i < sizeof(zd47805f) / sizeof(zd47805f[0]); i++) {
		if (!z1abe89d(zd47805f[i].szGame)) continue;
		unsigned int n = 0;
		unsigned char* p = zd164f58("CpsRamFF", ACB_MEMORY_RAM, &n);
		const unsigned int off = (zd47805f[i].nAddr - 0xFF0000) ^ 1;
		if (!p || off >= n) break;
		za065e40 = z6a30044; z7e75214 = p; zf2d7d46 = n; zf5ee0a8 = off;
		return 1;
	}
	if (z1abe89d("sfiii3")) {
		unsigned int n = 0;
		unsigned char* p = zd164f58("Main RAM", ACB_MEMORY_RAM, &n);
		if (p && (z0c24531 ^ 3) < n) { za065e40 = zaa0734c; z7e75214 = p; zf2d7d46 = n; zf5ee0a8 = z0c24531 ^ 3; return 1; }
	}
	if (z1abe89d("umk3")) {
		unsigned int n = 0;
		unsigned char* p = zd164f58("NV RAM", ACB_NVRAM, &n);
		if (p && zfa670f3 < n) { za065e40 = z3ffc86e; z7e75214 = p; zf2d7d46 = n; return 1; }
	}
	return 0;
}

int z3faa1df(int bOn, void (*z930c6f2)(const char*))
{
	z910e658 = 0;
	if (!bOn) return 0;
	if (!attach(z930c6f2)) {
		z3c2dd92(z930c6f2, "ficha: %s sem o credito mapeado - a ficha fica como esta", BurnDrvGetTextA(DRV_NAME));
		return 0;
	}
	z910e658 = 1;
	z3c2dd92(z930c6f2, "ficha: infinita (%s), o botao de ficha nao chega ao jogo",
	          za065e40 == zdfc107b ? "Neo Geo, 99" : za065e40 == z3ffc86e ? "UMK3, 99" : "Capcom, 9");
	return 1;
}

int z6ef378d(void) { return z910e658; }

void z474b480(void)
{
	if (!z910e658 || !z7e75214) return;
	switch (za065e40) {
		case zdfc107b:
			z7e75214[0x34 ^ 1] = 0x99;
			z7e75214[0x36 ^ 1] = 0x99;
			break;
		case z6a30044:
		case zaa0734c:
			z7e75214[zf5ee0a8] = 0x09;
			break;
		case z3ffc86e: {
			const unsigned char was = z7e75214[z7a857a0];
			if (was != 0x63) {
				z7e75214[zfa670f3]    = (unsigned char)(z7e75214[zfa670f3] - (0x63 - was));
				z7e75214[z7a857a0] = 0x63;
			}
			break;
		}
	}
}
