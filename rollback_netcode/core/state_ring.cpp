#include "burnint.h"
#include "state_ring.h"

#include <stdlib.h>
#include <string.h>

#define z3188900 16

#define ze19ce35 (ACB_FULLSCAN | ACB_RUNAHEAD)

#define z6d50e51 0x811c9dc5u

static UINT8*  z83456b9         = NULL;
static INT32   z882e322     = 0;
static INT32   za3a8bc3        = 0;
static INT32   z6a22f8f     = 0;
static INT32*  zf9beb62    = NULL;
static UINT32* zc4fa861 = NULL;
static UINT8*  z5eac49e        = NULL;
static bool    z4b89ca5        = false;
static bool    z8224778       = false;

static UINT8*  z89cbd0a      = NULL;
static UINT8*  z5d44e30   = NULL;
static UINT32  z3719527     = z6d50e51;
static INT32   zc27b02a    = 0;
static bool    za73dc62 = false;
static bool    z2e9bc2a = false;

#define z7ebbe29 (-1)

static void z66ddb4b(const UINT8* p, INT32 n)
{
	UINT32 h = z3719527;
	INT32 i = 0;
	for (; i + 4 <= n; i += 4) {
		UINT32 w;
		memcpy(&w, p + i, 4);
		h = (h ^ w) * 0x01000193u;
		h = (h << 13) | (h >> 19);
	}
	for (; i < n; i++) {
		h = (h ^ p[i]) * 0x01000193u;
	}
	z3719527 = h;
}

static INT32 __cdecl z9480df5(struct BurnArea* pba)
{
	zc27b02a += pba->nLen;
	return 0;
}

static bool zbb60e78(const char* szName)
{
	if (!szName || !szName[0]) return false;

	if (strncmp(szName, "op->", 4) == 0) return true;
	if (strncmp(szName, "YMPSG[", 6) == 0) return true;
	if (strncmp(szName, "nBurnCurrentYM2151Register", 26) == 0) return true;
	if (strcmp(szName, "chanout") == 0) return true;
	if (strcmp(szName, "m2") == 0) return true;
	if (strcmp(szName, "c1") == 0) return true;
	if (strcmp(szName, "c2") == 0) return true;
	if (strcmp(szName, "mem") == 0) return true;

	if (strncmp(szName, "YM2610", 6) == 0) return true;
	if (strncmp(szName, "nYM2610Position", 15) == 0) return true;

	if (strncmp(szName, "YM2608", 6) == 0) return true;
	if (strncmp(szName, "YM2203", 6) == 0) return true;
	if (strncmp(szName, "YM2612", 6) == 0) return true;
	if (strncmp(szName, "YM2413", 6) == 0) return true;
	if (strncmp(szName, "YM3812", 6) == 0) return true;
	if (strncmp(szName, "YM3526", 6) == 0) return true;
	if (strncmp(szName, "Y8950", 5) == 0) return true;

	if (strncmp(szName, "AY8910", 6) == 0) return true;
	if (strncmp(szName, "nAY8910Position", 15) == 0) return true;

	if (strncmp(szName, "MSM6295", 7) == 0) return true;
	if (strncmp(szName, "nMSM6295Status", 14) == 0) return true;

	if (strncmp(szName, "QSound", 6) == 0) return true;

	if (strncmp(szName, "Z80 #", 5) == 0) return true;
	if (strncmp(szName, "nZetCycles", 10) == 0) return true;
	if (strncmp(szName, "ZetCPUContext", 13) == 0) return true;
	if (strcmp(szName, "nQsndCyclesExtra") == 0) return true;
	if (strcmp(szName, "nQsndZBank") == 0) return true;
	if (strcmp(szName, "QscCmd") == 0) return true;
	if (strcmp(szName, "CpsZRamC0") == 0) return true;
	if (strcmp(szName, "CpsZRamF0") == 0) return true;
	if (strcmp(szName, "Z80 RAM") == 0) return true;

	if (strcmp(szName, "nTicksDone") == 0) return true;
	if (strcmp(szName, "nTimerCount") == 0) return true;
	if (strcmp(szName, "nTimerStart") == 0) return true;
	if (strcmp(szName, "dTime") == 0) return true;

	if (strncmp(szName, "chip->voice", 11) == 0) return true;
	if (strncmp(szName, "chip->key", 9) == 0) return true;

	if (strcmp(szName, "Chips") == 0) return true;
	if (strncmp(szName, "k053260 ", 8) == 0) return true;
	if (strncmp(szName, "sChip->", 7) == 0) return true;

	return false;
}

#define zc2e62ca 2048
typedef struct {
	char szName[48];
	INT32 nOffset;
	INT32 nLen;
	bool  z10fad86;
} z95493ea;
static z95493ea z6e1d9f5[zc2e62ca];
static INT32    zcaee2e8     = 0;
static INT32    zf08260d = 0;
static INT32    zcf1792d   = 0;
static INT32    z661a4bf        = 0;

static INT32 __cdecl z5cd4e4e(struct BurnArea* pba)
{
	if (z89cbd0a + pba->nLen > z5d44e30) { za73dc62 = true; return 1; }
	memcpy(z89cbd0a, pba->Data, pba->nLen);
	const bool z9d7f1b5 = (zcf1792d < zcaee2e8)
		? z6e1d9f5[zcf1792d].z10fad86
		: zbb60e78(pba->szName);
	if (z2e9bc2a && !z9d7f1b5) z66ddb4b(z89cbd0a, pba->nLen);
	zcf1792d++;
	z89cbd0a   += pba->nLen;
	zc27b02a += pba->nLen;
	return 0;
}

static INT32 __cdecl z06c1d5c(struct BurnArea* pba)
{
	if (z89cbd0a + pba->nLen > z5d44e30) { za73dc62 = true; return 1; }
	memcpy(pba->Data, z89cbd0a, pba->nLen);
	z89cbd0a   += pba->nLen;
	zc27b02a += pba->nLen;
	return 0;
}

static INT32 __cdecl z058a9a6(struct BurnArea* pba)
{
	z661a4bf++;
	if (zcaee2e8 < zc2e62ca) {
		z95493ea* a = &z6e1d9f5[zcaee2e8++];
		strncpy(a->szName, pba->szName ? pba->szName : "?", sizeof(a->szName) - 1);
		a->szName[sizeof(a->szName) - 1] = 0;
		a->nOffset   = zc27b02a;
		a->nLen      = pba->nLen;
		a->z10fad86 = zbb60e78(a->szName);
		if (a->z10fad86) zf08260d++;
	}
	zc27b02a += pba->nLen;
	return 0;
}

static INT32 z7802801(void)
{
	zc27b02a         = 0;
	zcaee2e8     = 0;
	zf08260d = 0;
	z661a4bf        = 0;
	BurnAcb = z058a9a6;
	BurnAreaScan(ze19ce35 | ACB_READ, NULL);
	z4b89ca5 = (z661a4bf == zcaee2e8);
	return zc27b02a;
}

int z22742b3(const void* pA, const void* pB, int nLen,
                          void (*z930c6f2)(const char*), int z7234cc7)
{
	if (!pA || !pB || zcaee2e8 == 0 || nLen != z882e322) return -1;
	const UINT8* a = (const UINT8*)pA;
	const UINT8* b = (const UINT8*)pB;

	int nDiff = 0;
	for (INT32 i = 0; i < zcaee2e8; i++) {
		const z95493ea* r = &z6e1d9f5[i];
		if (r->z10fad86) continue;
		if (r->nOffset + r->nLen > nLen) break;
		if (memcmp(a + r->nOffset, b + r->nOffset, r->nLen) == 0) continue;

		INT32 first = 0, count = 0;
		for (INT32 k = 0; k < r->nLen; k++) {
			if (a[r->nOffset + k] != b[r->nOffset + k]) {
				if (count == 0) first = k;
				count++;
			}
		}
		if (z930c6f2 && nDiff < z7234cc7) {
			char line[200];
			_snprintf(line, sizeof(line) - 1,
			          "  [CHECKSUM-FAIL] state area #%d '%s' (%d bytes): %d byte(s) differ, first at +%d (%02X vs %02X)",
			          i, r->szName, r->nLen, count, first, a[r->nOffset + first], b[r->nOffset + first]);
			line[sizeof(line) - 1] = 0;
			z930c6f2(line);
		}
		nDiff++;
	}

	for (INT32 i = 0; i < zcaee2e8; i++) {
		const z95493ea* r = &z6e1d9f5[i];
		if (!r->z10fad86) continue;
		if (r->nOffset + r->nLen > nLen) break;
		if (memcmp(a + r->nOffset, b + r->nOffset, r->nLen) == 0) continue;

		INT32 first = 0, count = 0;
		for (INT32 k = 0; k < r->nLen; k++) {
			if (a[r->nOffset + k] != b[r->nOffset + k]) {
				if (count == 0) first = k;
				count++;
			}
		}
		if (z930c6f2 && nDiff < z7234cc7) {
			char line[200];
			_snprintf(line, sizeof(line) - 1,
			          "  [sound-excluded] state area #%d '%s' (%d bytes): %d byte(s) differ, first at +%d (%02X vs %02X)",
			          i, r->szName, r->nLen, count, first, a[r->nOffset + first], b[r->nOffset + first]);
			line[sizeof(line) - 1] = 0;
			z930c6f2(line);
		}
		nDiff++;
	}
	return nDiff;
}

int zc8e48ed(int z2eef737)
{
	if (z8224778) zbf15f5b();

	const INT32 measured = z7802801();
	if (measured <= 0) return z2230627;

	INT32 want = (z2eef737 > 0) ? z2eef737 : z3188900;
	INT32 pow2 = 1;
	while (pow2 < want) pow2 <<= 1;

	z882e322 = measured;
	za3a8bc3    = pow2;
	z6a22f8f = pow2 - 1;

	z83456b9         = (UINT8*)malloc((size_t)z882e322 * (size_t)za3a8bc3);
	zf9beb62    = (INT32*)malloc(sizeof(INT32) * (size_t)za3a8bc3);
	zc4fa861 = (UINT32*)malloc(sizeof(UINT32) * (size_t)za3a8bc3);
	z5eac49e        = (UINT8*)malloc((size_t)za3a8bc3);
	if (!z83456b9 || !zf9beb62 || !zc4fa861 || !z5eac49e) {
		zbf15f5b();
		return zd0bac66;
	}

	z7119772();
	z8224778 = true;

	bprintf(0, _T(" ** StateRing: %d slots x $%x bytes = %d KB pinned.\n"),
	        za3a8bc3, z882e322, (INT32)(((long long)z882e322 * za3a8bc3) / 1024));
	return z20fb1dd;
}

void zbf15f5b(void)
{
	if (z83456b9)         free(z83456b9);
	if (zf9beb62)    free(zf9beb62);
	if (zc4fa861) free(zc4fa861);
	if (z5eac49e)        free(z5eac49e);
	z83456b9 = NULL;
	zf9beb62 = NULL;
	zc4fa861 = NULL;
	z5eac49e = NULL;
	z882e322 = za3a8bc3 = z6a22f8f = 0;
	z8224778 = false;
}

void z7119772(void)
{
	for (INT32 i = 0; i < za3a8bc3; i++) {
		zf9beb62[i]    = z7ebbe29;
		zc4fa861[i] = 0;
		z5eac49e[i]        = 0;
	}
}

int z2726b67(int nFrame, void** ppBuf, int* pnLen, unsigned int* z044e348)
{
	if (!z8224778)   return z04f824f;
	if (nFrame < 0)  return zf93968a;

	const INT32 slot = nFrame & z6a22f8f;
	UINT8* dst = z83456b9 + (size_t)slot * (size_t)z882e322;

	z89cbd0a         = dst;
	z5d44e30      = dst + z882e322;
	z3719527        = z6d50e51;
	zc27b02a       = 0;
	za73dc62    = false;
	z2e9bc2a    = (z044e348 != NULL) || !z4b89ca5;
	zcf1792d = 0;

	BurnAcb = z5cd4e4e;
	BurnAreaScan(ze19ce35 | ACB_READ, NULL);

	if (za73dc62)              return z909baab;
	if (zc27b02a != z882e322)   return zcbdd653;

	zf9beb62[slot]    = nFrame;
	zc4fa861[slot] = z2e9bc2a ? z3719527 : 0;
	z5eac49e[slot]        = z2e9bc2a ? 1 : 0;

	if (ppBuf)      *ppBuf      = dst;
	if (pnLen)      *pnLen      = z882e322;
	if (z044e348) *z044e348 = z3719527;
	return z20fb1dd;
}

int z4dac140(int nFrame, const void* pBuf, int nLen)
{
	if (!z8224778) return z04f824f;

	const UINT8* src;
	if (pBuf != NULL) {
		if (nLen != z882e322) return zcbdd653;
		src = (const UINT8*)pBuf;
	} else {
		if (nFrame < 0) return zf93968a;
		const INT32 slot = nFrame & z6a22f8f;
		if (zf9beb62[slot] != nFrame) return zaab425a;
		src = z83456b9 + (size_t)slot * (size_t)z882e322;
	}

	z89cbd0a      = (UINT8*)src;
	z5d44e30   = (UINT8*)src + z882e322;
	zc27b02a    = 0;
	za73dc62 = false;

	BurnAcb = z06c1d5c;
	BurnAreaScan(ze19ce35 | ACB_WRITE, NULL);

	if (za73dc62)            return z909baab;
	if (zc27b02a != z882e322) return zcbdd653;
	return z20fb1dd;
}

int z27fcc4a(const void* pBuf, int nLen)
{
	if (!z8224778) return z04f824f;
	if (pBuf == NULL || nLen <= 0) return zf93968a;

	z89cbd0a      = (UINT8*)pBuf;
	z5d44e30   = (UINT8*)pBuf + nLen;
	zc27b02a    = 0;
	za73dc62 = false;

	BurnAcb = z06c1d5c;
	BurnAreaScan(ze19ce35 | ACB_WRITE, NULL);

	if (za73dc62)      return z909baab;
	if (zc27b02a != nLen) return zcbdd653;

	const int rc = zc8e48ed(za3a8bc3);
	if (rc != z20fb1dd) return rc;
	return (z882e322 == nLen) ? z20fb1dd : zcbdd653;
}

void zaf99fbe(void* pBuf)
{
	(void)pBuf;
}

int          zb9193bb(void)        { return z8224778 ? 1 : 0; }
int          z9e7e754(void)        { return z8224778 ? z882e322 : 0; }
int          zecd0b95(void)       { return z8224778 ? za3a8bc3 : 0; }
long long    z957052b(void)       { return z8224778 ? (long long)z882e322 * za3a8bc3 : 0; }

int z4adf996(int nFrame)
{
	if (!z8224778 || nFrame < 0) return 0;
	return (zf9beb62[nFrame & z6a22f8f] == nFrame) ? 1 : 0;
}

int zf580c77(const void* pBuf)
{
	if (!z8224778 || !pBuf || z882e322 <= 0) return -1;
	const UINT8* p = (const UINT8*)pBuf;
	if (p < z83456b9 || p >= z83456b9 + (size_t)z882e322 * (size_t)za3a8bc3) return -1;
	const size_t off = (size_t)(p - z83456b9);
	if (off % (size_t)z882e322) return -1;
	return zf9beb62[off / (size_t)z882e322];
}

const void* z8483030(int nFrame, int* pnLen)
{
	if (!z4adf996(nFrame)) return NULL;
	if (pnLen) *pnLen = z882e322;
	return z83456b9 + (size_t)(nFrame & z6a22f8f) * (size_t)z882e322;
}

unsigned int z3afde3a(int nFrame)
{
	if (!z4adf996(nFrame)) return 0;
	const INT32 slot = nFrame & z6a22f8f;
	if (!z5eac49e[slot]) {
		const UINT8* src = z83456b9 + (size_t)slot * (size_t)z882e322;
		z3719527 = z6d50e51;
		for (INT32 i = 0; i < zcaee2e8; i++)
			if (!z6e1d9f5[i].z10fad86) z66ddb4b(src + z6e1d9f5[i].nOffset, z6e1d9f5[i].nLen);
		zc4fa861[slot] = z3719527;
		z5eac49e[slot]        = 1;
	}
	return zc4fa861[slot];
}

int z9976ce3(void)
{
	return z8224778 ? zcaee2e8 : 0;
}

int zdb4ae3e(void)
{
	return z8224778 ? zf08260d : 0;
}
