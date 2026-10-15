#include "burnint.h"

#include "fbneo_host.h"
#include "../core/state_ring.h"
#include "../core/ggpo_bridge.h"
#include "../core/nat_punch.h"
#include "../core/relay.h"
#include "../core/port_map.h"
#include "match_score.h"
#include "stream_files.h"
#include "ram_probe.h"
#include "overlay.h"
#include "end_notice.h"
#include "anticheat.h"
#include "ficha.h"
#include "input_display.h"
#include "loading_notice.h"
#include "replay_menu.h"
#include "speedrun.h"
#include "img_hash.h"
#include "lang.h"
#include "hud.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

extern int bDrvOkay;

extern INT32  VidFrame();
extern INT32  VidRedraw();

static void z5ee653e(void)
{
	static unsigned char* keep = NULL;
	static int z7e0ab32 = 0;
	void* p = NULL;
	int n = 0;
	int z9dab7e8 = 0;
	if (z9e7e754() > 0 && z2726b67(0, &p, &n, NULL) == z20fb1dd && p && n > 0) {
		if (z7e0ab32 != n) {
			free(keep);
			keep = (unsigned char*)malloc((size_t)n);
			z7e0ab32 = keep ? n : 0;
		}
		if (keep) { memcpy(keep, p, (size_t)n); z9dab7e8 = 1; }
	}
	VidRedraw();
	if (z9dab7e8) z4dac140(0, keep, z7e0ab32);
	if (pBurnSoundOut && nBurnSoundLen > 0) memset(pBurnSoundOut, 0, (size_t)nBurnSoundLen * 4);
}
extern UINT8* pVidImage;
extern UINT32 nCurrentFrame;
extern INT32  bRunAhead;
extern INT32  bBurnRunAheadFrame;
void StateRunAheadSave();
void StateRunAheadLoad();

extern HWND hScrnWnd;

extern int z40e51f6;
void z1706dba();

#define z193a6a8    8
#define z24955bb    0x01
#define zbda0044 0x02
#define za026c16  0x04
#define zca0bcba  0x08

static int zc300cc9 = 1;

static const char* zda80ec2 = "rbf-netplay.log";

static void z503808b(const char* fmt, ...)
{
	FILE* f = fopen(zda80ec2, "a");
	if (!f) return;
	time_t t = time(NULL);
	struct tm* lt = localtime(&t);
	if (lt) fprintf(f, "[%02d:%02d:%02d] ", lt->tm_hour, lt->tm_min, lt->tm_sec);
	va_list ap;
	va_start(ap, fmt);
	vfprintf(f, fmt, ap);
	va_end(ap);
	fputc('\n', f);
	fclose(f);
}

static void z6cc50c1(const char* s) { z503808b("%s", s ? s : ""); }

void z5d90bae(const char* s) { z6cc50c1(s); }

static void zea852f9(const char* szLine)
{
	char szPath[MAX_PATH] = { 0 };
	if (GetModuleFileNameA(NULL, szPath, MAX_PATH - 16)) {
		char* p = strrchr(szPath, '\\');
		if (p) p[1] = 0; else szPath[0] = 0;
	}
	strcat(szPath, "rbf-video.txt");

	FILE* f = fopen(szPath, "a");
	if (!f) return;
	fprintf(f, "%s\n", szLine);
	fclose(f);
}

void z5379fd8(int zbfa47cd)
{
	char sz[64];
	snprintf(sz, sizeof(sz), "fallback=%d", zbfa47cd);
	zea852f9(sz);
}

void z142cb0f(const char* zc786f38)
{
	char sz[96];
	snprintf(sz, sizeof(sz), "fx=%s", zc786f38 ? zc786f38 : "");
	zea852f9(sz);
}

void z2184d23(int z5cddbe5)
{
	char sz[32];
	snprintf(sz, sizeof(sz), "vol=%d", z5cddbe5);
	zea852f9(sz);
}

void z2c08b33(int z659a54f)
{
	static int bDone = 0;
	if (bDone) return;
	bDone = 1;
	char sz[64];
	snprintf(sz, sizeof(sz), "start=%d", z659a54f);
	zea852f9(sz);
}

#define z2777e28 180

void z74d9120(int z659a54f)
{
	static int nFrames = 0;
	static int bDone   = 0;
	if (bDone) return;
	if (++nFrames < z2777e28) return;

	bDone = 1;
	char sz[64];
	snprintf(sz, sizeof(sz), "ok=%d", z659a54f);
	zea852f9(sz);
}

#define z2d35323 (z102275f * 8)

typedef struct {
	UINT8* pVal[z2d35323];
	int    nBits;
} z11d88ad;

static z11d88ad  z3c35b6f[z897cf46];
static const char*   zde7202f[z897cf46][z2d35323];
static int           z854395e[z897cf46];
static int           z3332e9c    = 0;
static int           z378679b = 0;
static int           zd82262d = 1;
static int           z05f35c5 = 1;
static int           zcef8663      = 0;
static int           z2fc19ce   = 0;
static int           z69d3fdc    = 0;
static int           z5aca318 = 0;
static const char*   zb702c05   = "closed";
static int           z1cd003b  = 0;
static int           z6a2ac86     = 0;
static int           z6a3da4b     = 0;
static int           z59322ff = 0;
static unsigned long long zfeed50d = 0;
static long long          z065abf3 = 0;
#define z7cc0a06 20000
static int           z73638f5 = 0;
static z54dec38 z94aaa38;

static int           z5aa079a      = 0;
static long long     z16a2db6 = 0;
static int           zcc7a50c  = 0;
static long long     zcbf2334 = 0;

static int           z2cf405c   = 0;
static char          z0bdc044[MAX_PATH] = "";
static int           z607dc74   = 0;

static int zec3f3b5(void)
{
	char v[16] = "";
	const int n = GetEnvironmentVariableA("RBF_TESTE_CONFERE", v, sizeof(v)) ? atoi(v) : 0;
	return n > 0 ? n : 0;
}

static void ze109f17(long long k, unsigned int chk)
{
	z503808b("conferencia: quadro %lld estado %08X", k, chk);
}

static int           z90c98a2     = 0;
static int           z7a81a0d   = 0;
static int           z71317ab = 0;

static int           z7618f45        = 0;
static int           zd5a1a8b     = 0;
static char          z47ca634[40];
static int           zdd50e3c   = 0;
static int           za391f79 = 0;
#define z185d636 7200
static int            zeb07121       = 0;
static long long      z1846391    = z185d636;
static long long      z3c39947     = 0;
static long long      z05fec26  = 0;
static unsigned char* z43a5fe9    = NULL;
static int            z0e8c3f0 = 0;
static int            z67cf06e  = 0;
static long long      z48296e9  = 0;
static int            za0279de = 1;
static long long     zec0344b  = 0;
static int       zd4afa4e   = 0;
#define zd1d8fe5  12
#define zc37a608    60
#define z108238d  180
#define z82fc0ec 40
static int       z6b3941a   = 0;
static long long zdf61b15  = 0;
static double    zdd2f6bc = 0;
static int       z1fa1f56   = 0;
static int       z98879fd    = 0;
static int       zd3b974b   = 0;
static char      z579f7ed[260] = "";
static long long z9e1660b   = 0;
static long long z6db68d6 = 0;
static int       z4698017 = 5;
static int       zc67fd07(void);
static int  z2862e37(void);
static void z2466609(const char* z733a844, const char* zd5e0939, const char* szGame);

static void z256ef6d(char* s)
{
	if (!s) return;
	char* o = s;
	for (const char* i = s; *i; i++) {
		int hi = -1, lo = -1;
		if (i[0] == '%' && i[1] && i[2]) {
			hi = i[1]; lo = i[2];
			hi = (hi >= '0' && hi <= '9') ? hi - '0' : ((hi | 32) >= 'a' && (hi | 32) <= 'f') ? (hi | 32) - 'a' + 10 : -1;
			lo = (lo >= '0' && lo <= '9') ? lo - '0' : ((lo | 32) >= 'a' && (lo | 32) <= 'f') ? (lo | 32) - 'a' + 10 : -1;
		}
		if (hi >= 0 && lo >= 0) { *o++ = (char)((hi << 4) | lo); i += 2; }
		else *o++ = *i;
	}
	*o = '\0';
}
static void z4ec6bf1(long long f);
static void z2861a61(long long f);
static int  z698ca60(long long target);
static zd7729d0 z6db85c1;
static int       z13cb333  = 0;

static long long z91b3ccd(void)
{
	if (z7618f45) return zec0344b;
	if (z5aa079a)  return z16a2db6;
	return za0ce261();
}

static int z57065f7(const char* szName)
{
	if (!szName) return 0;
	if ((szName[0] == 'P' || szName[0] == 'p') && szName[1] >= '1' && szName[1] <= '9')
		return szName[1] - '0';
	return 0;
}

static int    z9ecf998 = 0;
static int    zc3f80f4    = -1;
static UINT8* z1700714[32];
static int    zec17866     = 0;
static UINT8* za109f1f[64];
static int    zb22a1f2     = 0;
static int    z3bfe586 = 0;

#define z40e133b   5000
#define z79f391c  10000
#define zbc831cf 1000
static int       zc1569cf       = 0;
static ULONGLONG z165ce15    = 0;
static int       z2edaa31   = 0;
static ULONGLONG z2b2c5f4     = 0;
static ULONGLONG z94d9e98    = 0;
static int       zf63f05e     = 0;
static int       zb1c7a2d     = 0;
static int       zc607119 = 0;

static int zbb0eeed(const void* in, int p, int bit)
{
	if (!in || bit < 0 || (bit >> 3) >= z378679b) return 0;
	const unsigned char* b = (const unsigned char*)in + (size_t)p * z378679b;
	return (b[bit >> 3] >> (bit & 7)) & 1;
}

static void zf95ede8(int bOn)
{
	zc1569cf = bOn && zc3f80f4 >= 0 && z3332e9c == 2 && z3c35b6f[1].nBits <= zc3f80f4;
	z165ce15 = z2b2c5f4 = z94d9e98 = 0;
	z2edaa31 = zf63f05e = zb1c7a2d = zc607119 = 0;
}

static int z2d72c1e(const struct BurnInputInfo* bii)
{
	return (bii->szName && _stricmp(bii->szName, "Reset") == 0) ||
	       (bii->szInfo && _stricmp(bii->szInfo, "reset") == 0);
}

static int z441d635(void)
{
	memset(z3c35b6f, 0, sizeof(z3c35b6f));
	for (int p = 0; p < z897cf46; p++) z854395e[p] = -1;
	z73638f5 = 0;
	zc3f80f4 = -1;
	zec17866  = 0;
	zb22a1f2  = 0;
	UINT8* z00dcad5 = NULL;

	struct BurnInputInfo bii;
	for (UINT32 i = 0; ; i++) {
		memset(&bii, 0, sizeof(bii));
		if (BurnDrvGetInputInfo(&bii, i)) break;

		const int pl = z57065f7(bii.szName);
		if (pl == 0 && bii.nType == BIT_DIGITAL && bii.pVal) {
			if (z9ecf998 && !z00dcad5 && z2d72c1e(&bii)) z00dcad5 = bii.pVal;
			else if (zec17866 < (int)(sizeof(z1700714) / sizeof(z1700714[0]))) z1700714[zec17866++] = bii.pVal;
			continue;
		}
		if (pl > z3332e9c && bii.nType == BIT_DIGITAL && bii.pVal) {
			if (zb22a1f2 < (int)(sizeof(za109f1f) / sizeof(za109f1f[0]))) za109f1f[zb22a1f2++] = bii.pVal;
			continue;
		}
		if (pl < 1 || pl > z3332e9c) continue;

		if (bii.nType != BIT_DIGITAL) {
			if (!z73638f5) {
				bprintf(PRINT_IMPORTANT,
				        _T("[fbneo_host] WARNING: non-digital input \"%S\" is NOT rolled back (v1 digital-only).\n"),
				        bii.szName);
				z73638f5 = 1;
			}
			continue;
		}

		z11d88ad* m = &z3c35b6f[pl - 1];
		if (m->nBits < z2d35323) zde7202f[pl - 1][m->nBits] = bii.szName;
		if (m->nBits >= z2d35323) {
			bprintf(PRINT_ERROR,
			        _T("[fbneo_host] ERROR: player %d has more than %d digital inputs.\n"),
			        pl, z2d35323);
			return -1;
		}
		{
			const size_t ze2fe7c2 = bii.szName ? strlen(bii.szName) : 0;
			if (ze2fe7c2 >= 5 && strcmp(bii.szName + ze2fe7c2 - 5, " Coin") == 0 && z854395e[pl - 1] < 0) z854395e[pl - 1] = m->nBits;
		}
		m->pVal[m->nBits++] = bii.pVal;
	}

	if (z00dcad5) {
		if (z3c35b6f[0].nBits < z2d35323) {
			zc3f80f4 = z3c35b6f[0].nBits;
			z3c35b6f[0].pVal[z3c35b6f[0].nBits++] = z00dcad5;
		} else if (zec17866 < (int)(sizeof(z1700714) / sizeof(z1700714[0]))) {
			z1700714[zec17866++] = z00dcad5;
		}
	}

	int maxBits = 0;
	for (int p = 0; p < z3332e9c; p++)
		if (z3c35b6f[p].nBits > maxBits) maxBits = z3c35b6f[p].nBits;

	if (maxBits == 0) {
		bprintf(PRINT_ERROR, _T("[fbneo_host] ERROR: no digital inputs mapped for %d players.\n"), z3332e9c);
		return -1;
	}

	z378679b = (maxBits + 7) / 8;
	return 0;
}

#define zd6e234e 0xAA

static unsigned char zf1988cc[z897cf46][z102275f];
static int           z2a26285   = 0;
static int           z44d7871 = 0;

static int zddc3607(void);

void zcdce694(void)
{
	if (!zcef8663 || (z7618f45 && !z2862e37())) return;
	for (int p = 0; p < z3332e9c; p++) {
		const z11d88ad* m = &z3c35b6f[p];
		for (int b = 0; b < m->nBits; b++)
			if (m->pVal[b]) *m->pVal[b] = zd6e234e;
	}
}

void z5887a63(void)
{
	if (!zcef8663 || (z7618f45 && !z2862e37())) return;

	int mask = 0;
	memset(zf1988cc, 0, sizeof(zf1988cc));

	for (int p = 0; p < z3332e9c; p++) {
		const z11d88ad* m = &z3c35b6f[p];
		int z3cd3a64 = 0;
		for (int b = 0; b < m->nBits; b++) {
			if (!m->pVal[b]) continue;
			const unsigned char v = *m->pVal[b];
			if (v == zd6e234e) {
				*m->pVal[b] = 0;
				continue;
			}
			if (!(p == 0 && b == zc3f80f4)) z3cd3a64 = 1;
			if (v) zf1988cc[p][b >> 3] |= (unsigned char)(1 << (b & 7));
		}
		if (z3cd3a64) mask |= 1 << p;
	}

	z2a26285 = mask;

	int want = z05f35c5;
	if (mask & 1) {
		want = 1;
	} else if (mask) {
		for (int p = 0; p < z3332e9c; p++) if (mask & (1 << p)) { want = p + 1; break; }
	}

	if (!z44d7871) {
		z44d7871 = 1;
		z503808b("input: controls bound here = mask 0x%X, reading player %d (we are side %d)",
		       mask, want, zd82262d);
	}
	z05f35c5 = want;
}

static void z7c7574b(int p, unsigned char* out, int z9df8312)
{
	const int n = (z9df8312 < z102275f) ? z9df8312 : z102275f;
	memset(out, 0, z9df8312);
	if (!z2cf405c) {
		memcpy(out, zf1988cc[p], n);
		return;
	}
	const z11d88ad* m = &z3c35b6f[p];
	for (int b = 0; b < m->nBits && (b >> 3) < n; b++)
		if (m->pVal[b] && *m->pVal[b]) out[b >> 3] |= (unsigned char)(1 << (b & 7));
}

static void ze1b2699(unsigned char* out, int p, int z9df8312);

static void zf125d1e(void* out, int z9df8312, void*  )
{
	const int p = z2cf405c ? 0 : (z05f35c5 >= 1 && z05f35c5 <= z3332e9c) ? z05f35c5 - 1 : 0;
	z7c7574b(p, (unsigned char*)out, z9df8312);
	ze1b2699((unsigned char*)out, p, z9df8312);
	if (zfeed50d) { memset(out, 0, (size_t)z9df8312); return; }

	if (zc1569cf && (zc3f80f4 >> 3) < z9df8312) {
		unsigned char* o = (unsigned char*)out;
		const unsigned char m = (unsigned char)(1 << (zc3f80f4 & 7));
		const int raw = (zf1988cc[0][zc3f80f4 >> 3] & m) != 0;
		o[zc3f80f4 >> 3] &= (unsigned char)~m;
		int press = raw && !zf63f05e;
		zf63f05e = raw;
		if (z3bfe586 > 0 && !zc607119 && z91b3ccd() >= z3bfe586) {
			zc607119 = 1;
			press = 1;
			z503808b("teste: F3 deste lado no frame %lld", z91b3ccd());
		}
		const ULONGLONG now = GetTickCount64();
		if (z165ce15 && now >= z165ce15) {
			if (!z2edaa31) z2b2c5f4 = now + z79f391c;
			z165ce15 = 0;
		}
		if (press && !z165ce15 && (zb1c7a2d || now >= z2b2c5f4)) {
			z165ce15  = now + z40e133b;
			z2edaa31 = zb1c7a2d;
			z503808b("F3: %s (frame %lld)", z2edaa31 ? "aceitou o recomeco" : "pediu o recomeco", z91b3ccd());
		}
		if (z165ce15) o[zc3f80f4 >> 3] |= m;
		return;
	}

	if (z3bfe586 > 0 && zd82262d == 1 && p == 0 && zc3f80f4 >= 0 &&
	    (zc3f80f4 >> 3) < z9df8312) {
		const long long f = z91b3ccd();
		if (f >= z3bfe586 && f < z3bfe586 + 10) {
			((unsigned char*)out)[zc3f80f4 >> 3] |= (unsigned char)(1 << (zc3f80f4 & 7));
			if (f == z3bfe586) z503808b("teste: reset do anfitriao no frame %lld", f);
		}
	}
}

static void zbdcd1c5(const void* ze691b51, int z0d29982)
{
	const unsigned char* in = (const unsigned char*)ze691b51;
	for (int p = 0; p < z0d29982 && p < z3332e9c; p++) {
		const unsigned char* pin = in + (size_t)p * z378679b;
		const z11d88ad* m = &z3c35b6f[p];
		for (int b = 0; b < m->nBits; b++) {
			if (!m->pVal[b]) continue;
			*m->pVal[b] = (UINT8)((pin[b >> 3] >> (b & 7)) & 1);
		}
	}
	for (int i = 0; i < zec17866; i++) *z1700714[i] = 0;
	for (int i = 0; i < zb22a1f2; i++) *za109f1f[i] = 0;
	if (z6ef378d()) {
		for (int p = 0; p < z0d29982 && p < z3332e9c; p++)
			if (z854395e[p] >= 0 && z3c35b6f[p].pVal[z854395e[p]]) *z3c35b6f[p].pVal[z854395e[p]] = 0;
		z474b480();
	}
}

static void* zbcde670    = NULL;
static int   z5723e7c = 0;

static void z8da8b3a(const void* pState, int nState)
{
	free(zbcde670);
	zbcde670 = NULL;
	z5723e7c = 0;
	if (!pState || nState <= 0) return;
	zbcde670 = malloc((size_t)nState);
	if (!zbcde670) return;
	memcpy(zbcde670, pState, (size_t)nState);
	z5723e7c = nState;
}

static int z27451c3(const void* inputs, int z0d29982)
{
	if (zc3f80f4 < 0 || !zbcde670 || !z3c35b6f[0].pVal[zc3f80f4]) return 0;
	if (zc1569cf) {
		*z3c35b6f[0].pVal[zc3f80f4] = 0;
		if (z0d29982 < 2 || !zbb0eeed(inputs, 0, zc3f80f4) || !zbb0eeed(inputs, 1, zc3f80f4)) return 0;
		z165ce15  = 0;
		z2edaa31 = 0;
		z94d9e98  = GetTickCount64() + zbc831cf;
	} else if (!*z3c35b6f[0].pVal[zc3f80f4]) {
		return 0;
	}
	z4dac140(0, zbcde670, z5723e7c);
	z65fd730();
	zbdcd1c5(inputs, z0d29982);
	*z3c35b6f[0].pVal[zc3f80f4] = 0;
	return 1;
}

static int z03eddeb = 0;

const char* z536c3a8(int nPlayer, int nBit)
{
	if (nPlayer < 0 || nPlayer >= z3332e9c || nBit < 0 || nBit >= z3c35b6f[nPlayer].nBits || nBit >= 32) return NULL;
	return zde7202f[nPlayer][nBit];
}

unsigned int z3859adb(int nPlayer)
{
	if (nPlayer < 0 || nPlayer >= z3332e9c) return 1;
	const z11d88ad* m = &z3c35b6f[nPlayer];
	unsigned int bits = 0;
	for (int b = 0; b < m->nBits; b++)
		if (m->pVal[b] && *m->pVal[b]) bits |= 1u << (b & 31);
	return bits;
}
static void z4878edb(int bDone, int z6a2f112)
{
	if (!z6a2f112) return;
	if (!bDone) { z03eddeb = 0; return; }
	if (z03eddeb) return;
	z03eddeb = 1;
	z503808b("%s: de volta ao estado de abertura (frame %lld)", zc1569cf ? "F3 dos dois" : "F3 do anfitriao", z91b3ccd());
}

static INT16* zc1fc8ee    = NULL;
static int    z9419624 = 0;

static INT16* zb749718(void)
{
	const int want = (nBurnSoundLen > 0 ? nBurnSoundLen : 1024) * 4;
	if (want > z9419624) {
		free(zc1fc8ee);
		zc1fc8ee    = (INT16*)calloc((size_t)want * 2, sizeof(INT16));
		z9419624 = zc1fc8ee ? want : 0;
	}
	return zc1fc8ee;
}

#define z8c7847c 64
static int            zfe8da2e       = 0;
static unsigned char* z304a7bf     = NULL;
static unsigned char* zcf99bc1  = NULL;
static int            z50e15b9  = 0;
static int            z729071d[z8c7847c];
static long long      z0644a42  = 0;
static long long      zb3cc48a  = 0;
static long long      zb654f7a = 0;
static int            zc145976 = 0;

static void zf75197b(void)
{
	free(z304a7bf);    z304a7bf = NULL;
	free(zcf99bc1); zcf99bc1 = NULL;
	z50e15b9 = 0;
}

static int zfb10f62(void)
{
	zf75197b();
	z0644a42 = zb3cc48a = zb654f7a = 0;
	zc145976 = 0;
	if (!z59b4fb8()) return 0;
	z50e15b9 = z3a3ab4d();
	z304a7bf    = (unsigned char*)malloc((size_t)z8c7847c * z50e15b9);
	zcf99bc1 = (unsigned char*)malloc((size_t)z50e15b9);
	if (!z304a7bf || !zcf99bc1) {
		z503808b("speedrun: sem memoria para o cronometro acompanhar o rollback");
		zf75197b();
		return 0;
	}
	for (int i = 0; i < z8c7847c; i++) z729071d[i] = -1;
	return 1;
}

static const unsigned char* zb0895cf(int frame)
{
	if (!z304a7bf || frame < 0) return NULL;
	const int k = frame & (z8c7847c - 1);
	return z729071d[k] == frame ? z304a7bf + (size_t)k * z50e15b9 : NULL;
}

static void zd89e26b(int frame, void*  )
{
	if (!zfe8da2e || frame < 0) return;
	const int k = frame & (z8c7847c - 1);
	unsigned char* slot = z304a7bf + (size_t)k * z50e15b9;
	if (z729071d[k] != frame) {
		z66855d5(slot);
		z729071d[k] = frame;
		return;
	}
	z66855d5(zcf99bc1);
	z0644a42++;
	char what[192];
	const int d = z40c67c6(slot, zcf99bc1, what, sizeof(what));
	if (d) {
		zb3cc48a++;
		if (z90c98a2) {
			if (zb3cc48a <= 3) z503808b("synctest: o cronometro do speedrun saiu diferente no quadro %d refeito: %s", frame, what);
		} else if (d == 2 && ++zb654f7a <= 20) {
			z503808b("speedrun: o rollback corrigiu o cronometro no comeco do quadro %d: %s", frame, what);
		}
	}
	memcpy(slot, zcf99bc1, (size_t)z50e15b9);
}

static void zaabe502(int frame, void*  )
{
	if (!zfe8da2e) return;
	static int z6be3154 = -1;
	if (z6be3154 < 0) z6be3154 = GetEnvironmentVariableA("RBF_TESTE_CRONO_SEM_VOLTA", NULL, 0) ? 1 : 0;
	if (z6be3154) return;
	const unsigned char* s = zb0895cf(frame);
	if (!s) {
		if (!zc145976++) z503808b("speedrun: o rollback voltou ao quadro %d sem a foto do cronometro - ele segue como estava", frame);
		return;
	}
	z856caea(s);
}

static int z7c85c59(const void* ze691b51, int z0d29982,
                           int  , int bRollback, void*  )
{
	zbdcd1c5(ze691b51, z0d29982);
	if (zc1569cf && !bRollback) zb1c7a2d = zbb0eeed(ze691b51, zd82262d == 1 ? 1 : 0, zc3f80f4);
	z4878edb(z27451c3(ze691b51, z0d29982), !bRollback);

	if (bRollback) {
		UINT8* z73e8341  = pBurnDraw;
		INT16* z4071d6c = pBurnSoundOut;
		pBurnDraw          = NULL;
		pBurnSoundOut      = zb749718();

		BurnDrvFrame();

		pBurnDraw          = z73e8341;
		pBurnSoundOut      = z4071d6c;
		if (zfe8da2e) z2e9e89c();
		return 0;
	}

	INT16* z8057731 = pBurnSoundOut;
	const int z47ff047 = (pBurnSoundOut == NULL);
	if (z47ff047) pBurnSoundOut = zb749718();

	if (zc300cc9) {
		const int z697750b = (bRunAhead > 0) && !(BurnDrvGetFlags() & BDF_RUNAHEAD_DISABLED);
		if (!z697750b) {
			if (VidFrame()) {
				pBurnDraw = NULL;
				BurnDrvFrame();
			}
		} else {
			pBurnDraw = (BurnDrvGetFlags() & BDF_RUNAHEAD_DRAWSYNC) ? pVidImage : NULL;
			BurnDrvFrame();
			StateRunAheadSave();
			INT16* pBurnSoundOut_temp = pBurnSoundOut;
			pBurnSoundOut = zb749718();
			bBurnRunAheadFrame = 1;

			const int K = (bRunAhead > 2) ? 2 : bRunAhead;
			for (int k = 1; k < K; k++) {
				nCurrentFrame++;
				pBurnDraw = NULL;
				BurnDrvFrame();
			}

			nCurrentFrame++;
			if (VidFrame()) {
				pBurnDraw = NULL;
				BurnDrvFrame();
			}
			nCurrentFrame -= K;
			bBurnRunAheadFrame = 0;
			StateRunAheadLoad();
			pBurnSoundOut = pBurnSoundOut_temp;
		}
	} else {
		pBurnDraw = NULL;
		BurnDrvFrame();
	}

	if (z47ff047) pBurnSoundOut = z8057731;
	if (zfe8da2e) z2e9e89c();
	return 0;
}

static void z703d600(void)
{
	if (z2cf405c) return;
	int side = zf7cdc79();
	for (int s = 1; s <= 2; s++) {
		const int left = z719f6d2(s);
		if (left > 0 && left <= 60) side |= 1 << (s - 1);
	}
	if (!side && zd82262d >= 1 && zd82262d <= 2 && z26c0539(zd82262d)) {
		side = 1 << (zd82262d - 1);
		z503808b("inatividade: o outro lado saiu com o aviso de inatividade do P%d na tela.", zd82262d);
	}
	if (!side) return;
	z1cd003b = side;
	zb702c05  = "ausente";
	z3440738(side);
	z503808b("inatividade: o outro lado fechou primeiro (%s no fim da contagem).",
	       side == 3 ? "os dois" : side == 1 ? "o P1" : "o P2");
}

static void zd7529fe(const z0d60a9e* ev, void*  )
{
	const char* n = "?";
	switch (ev->code) {
	case z06e5e34:              n = "connected";     break;
	case z722c814:          n = "synchronizing"; break;
	case z6d4531c:           n = "synchronized";  break;
	case ze1dba0a:                n = "running";       break;
	case ze1cca8e:           n = "disconnected";  break;
	case z14a1d04: n = "interrupted";   break;
	case z5a7fbc9:     n = "resumed";       break;
	case z95efe8a:               n = "timesync";      break;
	case z4647cc7:                 n = "DESYNC";        break;
	}
	if (ev->code == z4647cc7) {
		z503808b("DESYNC: from frame %d this machine (%04X) and the opponent (%04X) run different games. "
		       "The screens no longer match and this match's result is not trustworthy.",
		       ev->a, ((unsigned)ev->b >> 16) & 0xFFFF, (unsigned)ev->b & 0xFFFF);
		return;
	}
	z503808b("ggpo: %s (player=%d a=%d b=%d)", n, ev->player, ev->a, ev->b);
	bprintf(PRINT_IMPORTANT, _T("[fbneo_host] ggpo: %S (player=%d a=%d b=%d)\n"), n, ev->player, ev->a, ev->b);

	if (ev->code == z14a1d04) z5aca318 = 1;
	if (ev->code == z5a7fbc9)     z5aca318 = 0;
	if (ev->code == ze1cca8e) {
		z69d3fdc  = 1;
		if (strcmp(zb702c05, "limit") != 0 && strcmp(zb702c05, "ausente") != 0)
			zb702c05 = z5aca318 ? "disconnect" : "opponent_quit";
		z503808b("opponent disconnected - ending the match here.%s",
		       z5aca318 ? "" : " (o adversario saiu da partida, sem queda de conexao antes)");
		z703d600();
	}

}

static unsigned char* z64b1c5a    = NULL;
static int            z469310d = 0;
static int            zbb8c906   = 0;

#define z757ee24 24
static int zfc16947[z757ee24], zc5912d1[z757ee24];
static int z53e1f91 = 0, z9985de6 = 0;

static void zc2062f9(int f)
{
	if (z53e1f91 > 0 && f >= zfc16947[z53e1f91 - 1] && f <= zc5912d1[z53e1f91 - 1] + 1) {
		if (f > zc5912d1[z53e1f91 - 1]) zc5912d1[z53e1f91 - 1] = f;
		return;
	}
	if (z53e1f91 < z757ee24) { zfc16947[z53e1f91] = zc5912d1[z53e1f91] = f; z53e1f91++; }
	else z9985de6++;
}

static void z526bfdc(void)
{
	if (z53e1f91 == 0) return;
	char line[600];
	int n = _snprintf(line, sizeof(line) - 1, "synctest: frames that differed: ");
	for (int i = 0; i < z53e1f91 && n > 0 && n < (int)sizeof(line) - 24; i++) {
		n += (zfc16947[i] == zc5912d1[i])
			? _snprintf(line + n, sizeof(line) - 1 - n, "%s%d", i ? ", " : "", zfc16947[i])
			: _snprintf(line + n, sizeof(line) - 1 - n, "%s%d-%d", i ? ", " : "", zfc16947[i], zc5912d1[i]);
	}
	line[sizeof(line) - 1] = 0;
	z503808b("%s%s", line, z9985de6 ? " ... (and more)" : "");
}

static void za361ecd(const char* filename, const unsigned char* buf, int len, void*  )
{
	if (!filename || !buf || len <= 0) return;

	if (strstr(filename, "replay")) {
		const char* d = strstr(filename, "state-");
		if (d) zc2062f9(atoi(d + 6));
	}
	if (zbb8c906 >= 3) return;

	if (strstr(filename, "original")) {
		if (z469310d != len) {
			free(z64b1c5a);
			z64b1c5a = (unsigned char*)malloc((size_t)len);
			z469310d = z64b1c5a ? len : 0;
		}
		if (z64b1c5a) memcpy(z64b1c5a, buf, (size_t)len);
		return;
	}

	if (strstr(filename, "replay") && z64b1c5a && z469310d == len) {
		zbb8c906++;
		z503808b("synctest: re-simulated state differs from the original (%s):", filename);
		const int n = z22742b3(z64b1c5a, buf, len, z6cc50c1, 40);
		if (n > 40) z503808b("  ... and %d more area(s)", n - 40);
		if (n == 0) z503808b("  (no area differs - the checksum itself is not stable)");
	}
}

static void z3ebe12a(int frame, const void* inputs, int nBytes, void*  )
{
	if (zd82262d == 1 && z94aaa38.z5737392[0] && z94aaa38.zc225a0e && z94aaa38.zb306bef[0])
		z59d3a98(frame, inputs, nBytes);

	if (z05fec26 && (long long)frame + 1 >= z05fec26) {
		const long long want = z05fec26;
		z05fec26 = 0;
		int n = 0;
		const void* p = ((long long)frame + 1 == want) ? z8483030((int)want, &n) : NULL;
		if (p && n > 0)
			z99502a6((int)want, p, n, z43a5fe9, z0e8c3f0, z3afde3a((int)want));
		else if (z67cf06e++ < 3)
			z503808b("ponto salvo: o quadro %lld nao estava no anel - fica para o proximo", want);
	}

	{
		unsigned int bits[2] = { 0, 0 };
		const unsigned char* in = (const unsigned char*)inputs;
		for (int p = 0; p < 2 && p < z3332e9c; p++) {
			if ((p + 1) * z378679b > nBytes) break;
			const unsigned char* pin = in + (size_t)p * z378679b;
			for (int b = 0; b < z3c35b6f[p].nBits && b < z378679b * 8; b++)
				if ((pin[b >> 3] >> (b & 7)) & 1) bits[p] |= 1u << (b & 31);
		}
		zd3804c8(bits[0], bits[1]);
	}

	static int zc2a49e7 = 0;
	if (zc3f80f4 >= 0 && (zc3f80f4 >> 3) < nBytes) {
		const int down = (((const unsigned char*)inputs)[zc3f80f4 >> 3] >> (zc3f80f4 & 7)) & 1;
		if (down && !zc2a49e7) z503808b("partida: o anfitriao reiniciou o jogo (frame %d)", frame);
		zc2a49e7 = down;
	}

	if (z607dc74 > 0 && !z5aa079a && !z90c98a2 && ((long long)frame + 1) % z607dc74 == 0) {
		const unsigned int chk = z3afde3a(frame + 1);
		if (chk) ze109f17((long long)frame + 1, chk);
		else z503808b("conferencia: quadro %d sem estado no anel", frame + 1);
	}

	if (zfe8da2e && !z5aa079a) {
		const unsigned char* s = zb0895cf(frame + 1);
		if (s) z4508746(s);
		else if (!zc145976++) z503808b("speedrun: o quadro confirmado %d sem a foto do cronometro", frame);
	}
}

static void z0687dd4(void)
{
	memset(&z6db85c1, 0, sizeof(z6db85c1));
	z6db85c1.z9ffd6e7 = zf125d1e;
	z6db85c1.z24c6e73       = z7c85c59;
	z6db85c1.on_event         = zd7529fe;
	z6db85c1.z6a769fe        = za361ecd;
	z6db85c1.z2ecdd27 = z3ebe12a;
	z6db85c1.z01e689f   = zd89e26b;
	z6db85c1.z5f96894  = zaabe502;
	z6db85c1.user             = NULL;
}

static int z909038f(const z54dec38* cfg)
{
	if (!cfg)                                                   return -1;
	if (cfg->z0d29982 < 2 || cfg->z0d29982 > z897cf46) return -1;
	if (cfg->ze08b6ae < 1 || cfg->ze08b6ae > cfg->z0d29982)   return -1;
	if (!bDrvOkay)                                             return -1;

	z94aaa38         = *cfg;
	z256ef6d(z94aaa38.zd843ce2);
	z256ef6d(z94aaa38.z7dd7ec2);
	z3332e9c    = cfg->z0d29982;
	zd82262d = cfg->ze08b6ae;
	z05f35c5 = (cfg->z7606996 >= 1 && cfg->z7606996 <= cfg->z0d29982) ? cfg->z7606996 : 1;
	z2fc19ce   = 0;
	z69d3fdc    = 0;
	z5aca318 = 0;
	zb702c05   = "closed";
	z1cd003b  = 0;
	z6a2ac86     = 0;
	z6a3da4b     = 0;
	z59322ff = 0;
	zfeed50d     = 0;
	z7618f45       = 0;
	zec0344b = 0;
	z90c98a2 = 0;
	z5aa079a        = cfg->zc5cc23a ? 1 : 0;
	z16a2db6   = 0;
	zcbf2334   = 0;
	zcc7a50c    = 0;

	za391f79 = (zd82262d == 1 && z94aaa38.z5737392[0] &&
	                  z94aaa38.zc225a0e && z94aaa38.zb306bef[0]) ? 1 : 0;

	z47f386e(1, 1, z6cc50c1);
	if (!z5aa079a) {
		z97e971e(z94aaa38.zed95901);
		z9fff242(1);
		z5ebf79e(z94aaa38.ze6bfbdc, z6cc50c1);
		z5126edb(z94aaa38.zb306bef);
		z5c0c8b7(z94aaa38.zd843ce2, z94aaa38.z7dd7ec2, z94aaa38.ze6bfbdc);
		z5c3a407(BurnDrvGetTextA(DRV_NAME), z94aaa38.zb306bef, z94aaa38.zd843ce2, z94aaa38.z7dd7ec2, z94aaa38.ze6bfbdc, 0);
	}

	const int rc = z441d635();
	zf95ede8(!z5aa079a && !z59b4fb8());
	if (zc1569cf) z503808b("F3: recomeco so com os dois (bit %d)", zc3f80f4);
	z3faa1df(!z5aa079a && !z59b4fb8() && !z94aaa38.zed95901, z6cc50c1);
	return rc;
}

static void z76013f9(z4238888* bc)
{
	memset(bc, 0, sizeof(*bc));
	strncpy(bc->z91da805, z94aaa38.z91da805, sizeof(bc->z91da805) - 1);
	bc->z0d29982       = z3332e9c;
	bc->z9df8312    = z378679b;
	bc->ze08b6ae   = zd82262d;
	bc->zb3dbf67     = z94aaa38.zb3dbf67;
	strncpy(bc->z37f80f0, z94aaa38.z37f80f0, sizeof(bc->z37f80f0) - 1);
	bc->zbf4b4be    = z94aaa38.zbf4b4be;
	bc->z3b2036b    = (z94aaa38.z3b2036b >= 0) ? z94aaa38.z3b2036b : 0;
	bc->z293ec00 = 0;
	bc->z7f20ee6    = z94aaa38.z7f20ee6;
}

#define z8b8720c       45000
#define zee8a580     1500

static int zf868484(int z1af252e, int zf13fff5)
{
	if (z1af252e >= zee8a580) {
		if (!zef1a6d1()) {
			zbd03061(hScrnWnd, z6f2968c(L"Esperando o advers\x00E1rio"));
			z503808b("nat punch: o adversario ainda nao chegou - tela de espera (ate %d s)", zf13fff5 / 1000);
		}
		zea13d64((double)z1af252e / zf13fff5);
	}
	MSG msg;
	PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
	return 0;
}

static int isLoopback(const char* s)
{
	return !s || !s[0] || strncmp(s, "127.", 4) == 0 || _stricmp(s, "localhost") == 0;
}

static int z9568fa1(const z54dec38* cfg)
{
	return isLoopback(cfg->z37f80f0) && isLoopback(cfg->z5737392) && isLoopback(cfg->z6666865);
}

const char* z83650cd(void) { return (zcef8663 && z2cf405c && z0bdc044[0]) ? z0bdc044 : NULL; }
int         zdd5bb4f(void)  { return z5aa079a; }

int z3a135ef = 0;
static int z6303ef7[2] = { 0, 0 };
static int ze3ce0f1[2]  = { 0, 0 };

static int z4febd14(void)
{
	return ze41ba77() && !z5aa079a && !z90c98a2 && z3332e9c == 2;
}

static int zedb57ac = 0;

int z38fea27(void)
{
	return ((z3a135ef & 3) || zedb57ac) && z4febd14();
}

static int z0ff5272(void)
{
	int k = bRunAhead;
	if (k < 0) k = 0;
	if (k > 2) k = 2;
	return k;
}

static int                z02d2bda = 0, z7b9eeb8 = -1, z326ef20 = 0, z12b318f = 0, zb3ae073 = 0, z6ce0d9e = 0;
static int                z145c818[5], z8c41b24 = 0, z54881ff = 0, z95e03d4 = 0, zf037b31 = 0, z73efbc8 = 0, zd0bb897 = 0, z046ac0f = -1, z446847e = 0;
static unsigned long long zf915871 = 0, z77073bd = 0, ze1a56d8 = 0;
static char               z2cf7838[256] = "";
static const char*        z7af9695 = NULL;
static const int          zc919bb8[3] = { 59, 140, 200 };

static int z9ec58c2(int d)
{
	(void)d;
	return z02d2bda;
}

static int                za66db34 = 0, z31c9081 = 0, za8a2596 = 0, za6cc0b6 = 0, z078fe31 = 0, zc6504dd = 0;
static unsigned long long z4a236b1 = 0;
static LARGE_INTEGER      zae94e00;

static void z94eceb6(double ms)
{
	if (!za66db34 || z078fe31 || bRunAhead <= 0 || z5aa079a || z7618f45 || z90c98a2 || z3332e9c != 2 || !zae94e00.QuadPart) return;
	const unsigned long long now = GetTickCount64();
	if (!z4a236b1) z4a236b1 = now;
	if (now - z4a236b1 < 15000) return;
	const double per = nBurnFPS > 0 ? 100000.0 / (double)nBurnFPS : 16.9;
	if (zc6504dd) ms = per;
	z31c9081++;
	if (ms > per * 0.85) za8a2596++;
	if (z31c9081 < 300) return;
	if (za8a2596 * 10 >= z31c9081) za6cc0b6++; else za6cc0b6 = 0;
	const int pct = za8a2596 * 100 / z31c9081;
	z31c9081 = za8a2596 = 0;
	if (za6cc0b6 < 2) return;
	z078fe31 = 1;
	z02d2bda = 0;
	bRunAhead = 0;
	z503808b("u7 %d", pct);
	zea852f9("k9=1");
}

static void z82cde07(const z54dec38* cfg)
{
	char v[8] = "";
	zedb57ac = GetEnvironmentVariableA("RBF_U", v, sizeof(v)) ? atoi(v) : 0;
	if (zedb57ac < 0 || zedb57ac > 2) zedb57ac = 0;
	z02d2bda = 0; z7b9eeb8 = -1; z326ef20 = 0; z12b318f = 0; zb3ae073 = 0; z6ce0d9e = 0;
	z8c41b24 = z54881ff = z95e03d4 = zf037b31 = z73efbc8 = zd0bb897 = z446847e = 0;
	z046ac0f = -1;
	zf915871 = z77073bd = ze1a56d8 = 0;
	z7af9695 = NULL;
	z2cf7838[0] = 0;
	v[0] = 0;
	za66db34 = GetEnvironmentVariableA("RBF_C", v, sizeof(v)) ? atoi(v) : 0;
	z31c9081 = za8a2596 = za6cc0b6 = z078fe31 = 0;
	z4a236b1 = 0;
	QueryPerformanceFrequency(&zae94e00);
	v[0] = 0;
	zc6504dd = (GetEnvironmentVariableA("RBF_TESTE_CARGA", v, sizeof(v)) && cfg && z9568fa1(cfg)) ? 1 : 0;
	if (zedb57ac && GetEnvironmentVariableA("RBF_TESTE_PING", z2cf7838, sizeof(z2cf7838)) && z2cf7838[0]) {
		if (cfg && z9568fa1(cfg)) { z7af9695 = z2cf7838; z503808b("teste: RBF_TESTE_PING=%s", z2cf7838); }
		else z2cf7838[0] = 0;
	}
}

static void z2ce5e2d(int d)
{
	if (!zedb57ac || z5aa079a || z7618f45 || z90c98a2 || z3332e9c != 2) { zedb57ac = 0; return; }
	z02d2bda = z0ff5272();
	bRunAhead = z9ec58c2(d);
	const unsigned long long now = GetTickCount64();
	zf915871 = now;
	z77073bd = now + 1000;
	ze1a56d8 = now;
	z503808b("u0 %d %d %d %d", zedb57ac, d, z0ff5272(), z7b9eeb8);
}

static void zccb4ec0(int d, int est)
{
	if (z2549f00(d) < 0) return;
	bRunAhead = z9ec58c2(d);
	if (++z73efbc8 > 250) z73efbc8 = 1;
	const char m[3] = { 0x02, (char)('a' + d), (char)z73efbc8 };
	z446847e = z77d22b7(m, 3) ? 0 : d + 1;
	z503808b("u %d %d %d", d, z0ff5272(), est);
}

static void zbb9f5f5(const char* p, int n)
{
	if (!zedb57ac || !z4febd14() || n < 3) return;
	const int d = p[1] - 'a', q = (unsigned char)p[2];
	if (d < 0 || d > 3 || q < 1 || q > 250) return;
	if (zd0bb897 && (signed char)(q - zd0bb897) <= 0) return;
	zd0bb897 = q;
	if (d != z2db5ead()) z2549f00(d);
	bRunAhead = z9ec58c2(d);
	z503808b("u< %d %d", d, z0ff5272());
}

static void z7020e2b(void)
{
	if (!zedb57ac || !z4febd14() || !zf915871) return;
	const unsigned long long now = GetTickCount64();
	if (now < z77073bd) return;
	z77073bd = now + 1000;
	if (z446847e) {
		const char m[3] = { 0x02, (char)('a' + z446847e - 1), (char)z73efbc8 };
		if (z77d22b7(m, 3)) z446847e = 0;
	}
	const long el = (long)((now - zf915871) / 1000);
	if (z7af9695) {
		long t = 0;
		int ms = 0;
		if (sscanf(z7af9695, "%ld:%d", &t, &ms) == 2) {
			if (el >= t) {
				z046ac0f = ms;
				const char* nx = strchr(z7af9695, ';');
				z7af9695 = nx ? nx + 1 : NULL;
			}
		} else z7af9695 = NULL;
	}
	int v = -1;
	zc3f6a60 st;
	if (z70d4a8a(&st) == z30e34bc && st.valid) v = st.zd534a2f;
	if (v < 0) return;
	if (z7b9eeb8 >= 0 && el >= 3 && el < 10) { z6ce0d9e += v; zb3ae073++; }
	if (el >= 10 && !z12b318f) {
		z12b318f = 1;
		if (z7b9eeb8 >= 0 && zb3ae073 > 0) {
			int off = z6ce0d9e / zb3ae073 - z7b9eeb8;
			if (off < 0) off = 0;
			if (off > 40) off = 40;
			z326ef20 = off;
		}
		z503808b("u1 %d %d", z7b9eeb8, z326ef20);
	}
	z145c818[z54881ff] = v;
	z54881ff = (z54881ff + 1) % 5;
	if (z8c41b24 < 5) z8c41b24++;
	if (el < 10 || zd82262d != 1) return;
	int sum = 0;
	for (int i = 0; i < z8c41b24; i++) sum += z145c818[i];
	int est = sum / z8c41b24 - z326ef20;
	if (z046ac0f >= 0) est = z046ac0f;
	const int d = z2db5ead();
	if (d < 3 && est > zc919bb8[d]) z95e03d4++; else z95e03d4 = 0;
	if (d > 0 && est <= zc919bb8[d - 1] - 10) zf037b31++; else zf037b31 = 0;
	if (now - ze1a56d8 < 10000) return;
	if (z95e03d4 >= 3)       { zccb4ec0(d + 1, est); z95e03d4 = zf037b31 = 0; ze1a56d8 = now; }
	else if (zf037b31 >= 10) { zccb4ec0(d - 1, est); z95e03d4 = zf037b31 = 0; ze1a56d8 = now; }
}

void zfd67ef1(int w)
{
	if (!(z3a135ef & 1) || !z4febd14()) return;
	int d = z2db5ead();
	int k = z0ff5272();
	if (w == 0) {
		d = d ? 0 : 1;
		if (z2549f00(d) < 0) return;
		z0d8fbe5(d, 2000);
	} else {
		k = (k + 1) % 3;
		bRunAhead = k;
		z0d8fbe5(k, 2000);
	}
	if (++z6303ef7[w] > 250) z6303ef7[w] = 1;
	const char m[4] = { 0x01, (char)('a' + w), (char)('a' + (w == 0 ? d : k)), (char)z6303ef7[w] };
	z77d22b7(m, 4);
	z503808b("q %d %d", d, k);
}

int z7675262(const char* p, int n)
{
	if (p && n >= 1 && p[0] == 0x02) { zbb9f5f5(p, n); return 1; }
	if (!p || n < 1 || p[0] != 0x01) return 0;
	if (!(z3a135ef & 2) || !z4febd14() || n < 4) return 1;
	const int w = p[1] - 'a', v = p[2] - 'a', q = (unsigned char)p[3];
	if (w < 0 || w > 1 || v < 0 || v > (w == 0 ? 1 : 2) || q < 1 || q > 250) return 1;
	if (ze3ce0f1[w] && (signed char)(q - ze3ce0f1[w]) <= 0) return 1;
	ze3ce0f1[w] = q;
	if (w == 0) { if (v != z2db5ead()) z2549f00(v); }
	else bRunAhead = v;
	z503808b("q< %d %d", z2db5ead(), z0ff5272());
	return 1;
}

static char        z11d30a9[256] = "";
static const char* zb3afc55     = NULL;
static int         z9da76f5   = 0;

static void zb6d9c60(void)
{
	if (!z9da76f5) {
		z9da76f5 = 1;
		if (GetEnvironmentVariableA("RBF_TESTE_Q", z11d30a9, sizeof(z11d30a9)) && z11d30a9[0]) {
			zb3afc55 = z11d30a9;
			z503808b("teste: RBF_TESTE_Q=%s", z11d30a9);
		}
	}
	if (!zb3afc55 || !*zb3afc55) return;
	long long f = 0;
	int w = 0;
	if (sscanf(zb3afc55, "%lld:%d", &f, &w) != 2) { zb3afc55 = NULL; return; }
	if (z91b3ccd() < f) return;
	zfd67ef1(w);
	const char* n = strchr(zb3afc55, ';');
	zb3afc55 = n ? n + 1 : NULL;
}

int z054c841(const z54dec38* cfg)
{
	if (zcef8663) z912bf82();
	z9ecf998 = 1;
	z6303ef7[0] = z6303ef7[1] = ze3ce0f1[0] = ze3ce0f1[1] = 0;
	z82cde07(cfg);
	{
		char v[24] = "";
		z065abf3 = (GetEnvironmentVariableA("RBF_TESTE_FIM", v, sizeof(v)) && z9568fa1(cfg)) ? _atoi64(v) : 0;
		if (z065abf3 > 0) z503808b("teste: RBF_TESTE_FIM=%lld", z065abf3);
	}
	{
		char v[16] = "";
		z3bfe586 = GetEnvironmentVariableA("RBF_TESTE_RESET", v, sizeof(v)) ? atoi(v) : 0;
	}
	if (z909038f(cfg) != 0) {
		z503808b("FbnHostStart: startCommon failed (bad cfg / driver not ready / no digital inputs)");
		return -1;
	}

	z607dc74 = zec3f3b5();
	z2cf405c = 0;
	z0bdc044[0] = 0;
	if (GetEnvironmentVariableA("RBF_TESTE_LUA", z0bdc044, sizeof(z0bdc044)) && z0bdc044[0]) {
		if (z9568fa1(cfg)) {
			z2cf405c = 1;
			z9fff242(0);
			z503808b("teste: RBF_TESTE_LUA=%s (o roteiro joga pelo controle local)", z0bdc044);
		} else {
			z503808b("teste: RBF_TESTE_LUA ignorado - so numa sessao com todos os enderecos em 127.x");
			z0bdc044[0] = 0;
		}
	}
	if (z607dc74 > 0) z503808b("teste: RBF_TESTE_CONFERE=%d (o checksum do estado no log)", z607dc74);

	if (z5aa079a) {
		z0687dd4();
		zcef8663 = 1;
		z503808b("---- sessao sozinho: game=%s  controls=P%d  relay %s:%d  partida %s ----",
		       z94aaa38.z91da805, z05f35c5, z94aaa38.z5737392[0] ? z94aaa38.z5737392 : "-",
		       z94aaa38.zc225a0e, z94aaa38.zb306bef[0] ? z94aaa38.zb306bef : "-");
		z503808b("input: reset (F3) %s; %d botao(oes) de sistema e %d de outros jogadores desligado(s)",
		       zc3f80f4 >= 0 ? "volta ao estado de abertura" : "nao existe neste jogo", zec17866, zb22a1f2);
		if (z3bfe586 > 0) z503808b("teste: RBF_TESTE_RESET=%d (o corredor aperta reset nesse frame)", z3bfe586);
		bprintf(PRINT_IMPORTANT, _T("[fbneo_host] sessao sozinho: %d input byte(s)/player.\n"), z378679b);
		return 0;
	}

	z0687dd4();
	z4238888 bc;
	z76013f9(&bc);

	z503808b("---- starting session: game=%s  side=P%d/%d  controls=P%d  bind :%d  peer %s:%d  delay %d ----",
	       bc.z91da805, bc.ze08b6ae, bc.z0d29982, z05f35c5,
	       bc.zb3dbf67, bc.z37f80f0, bc.zbf4b4be, bc.z3b2036b);
	z503808b("input: reset (F3) %s; %d botao(oes) de sistema e %d de outros jogadores desligado(s) na partida",
	       zc3f80f4 < 0 ? "nao existe neste jogo" : zc1569cf ? "so com os dois lados" : "so do anfitriao (lado 1), nas duas maquinas", zec17866, zb22a1f2);
	if (z3bfe586 > 0) z503808b("teste: RBF_TESTE_RESET=%d (%s aperta o F3 nesse frame)", z3bfe586, zc1569cf ? "este lado" : "o anfitriao");

	if (z29b6e63(bc.zb3dbf67, z6cc50c1) != z5d01f23)
		z503808b("upnp: no mapping - relying on hole punching alone");

	if (z94aaa38.z6666865[0] && z94aaa38.z0b679d2 && z94aaa38.zb306bef[0]) {
		zf74d5eb plan;
		z73b2207(zf868484);
		int rc = z416a606(z94aaa38.z6666865, z94aaa38.z0b679d2, z94aaa38.zaece3c9,
		                             z94aaa38.zb306bef, bc.ze08b6ae, bc.zb3dbf67,
		                             &plan, z8b8720c, z6cc50c1);
		z73b2207(NULL);
		if (rc == z95c4b69 && plan.ze3a737a) {
			z503808b("nat punch: o adversario nao conectou em %d s - partida encerrada", z8b8720c / 1000);
			if (!zef1a6d1()) zbd03061(hScrnWnd, L"");
			zba9d2b1(z6f2968c(L"O advers\x00E1rio n\x00E3o conectou"));
			zea13d64(1.0);
			for (int i = 0; i < 30; i++) {
				MSG msg;
				PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
				Sleep(100);
			}
			z0b86b9d();
			za07fb62();
			_exit(0);
		}
		z0b86b9d();
		if (rc == z10fc2a7) {
			const int d0 = bc.z3b2036b;
			if (plan.mode == zaf33281 || plan.mode == z1b155da) z7b9eeb8 = plan.z5c6ee43;
			if (plan.mode == zaf33281 && plan.zf3e1f1a >= 0) {
				z94aaa38.z3b2036b = plan.zf3e1f1a;
				bc.z3b2036b    = plan.zf3e1f1a;
			}
			static const char* const zc8e4451[] = { "direct", "lan", "relay", "none" };
			char z0[16] = "-", z1[16] = "-";
			if (plan.z5c6ee43 >= 0) _snprintf(z0, sizeof(z0) - 1, "%d ms", plan.z5c6ee43);
			if (plan.mode != z7199250) _snprintf(z1, sizeof(z1) - 1, "%d", bc.z3b2036b);
			z503808b("nat punch: rtt %s (%d) - %s - delay %s (lobby %d)",
			       z0, plan.zae7efac, zc8e4451[plan.mode & 3], z1, d0);
			if (plan.mode == z7199250) {
				za07fb62();
				zd0938e8(z6f2968c(L"N\x00E3o foi poss\x00EDvel conectar direto com o seu advers\x00E1rio."),
				                  z6f2968c(L"A conex\x00E3o pelo servidor deixaria a partida lenta demais para voc\x00EAs dois."),
				                  8);
				z503808b("nat punch: fim (%d)", 7);
				_exit(7);
			}
		}
		if (rc != z10fc2a7) {
			z503808b("nat punch failed (%d) - falling back to %s:%d (LAN / port-forward)",
			       rc, bc.z37f80f0, bc.zbf4b4be);
		} else if (plan.mode == z1b155da) {
			z503808b("nat punch: same NAT - keeping the LAN peer %s:%d",
			       bc.z37f80f0, bc.zbf4b4be);
		} else if (plan.mode == za95c540 && z94aaa38.zaece3c9) {
			char z5737392[64];
			if (z82369e2(z94aaa38.z6666865, z5737392, sizeof(z5737392)) != z10fc2a7) {
				z503808b("relay: could not resolve %s - cannot relay this match", z94aaa38.z6666865);
			} else {
				z503808b("relaying the match through %s:%d = %s (no direct path)",
				       z94aaa38.z6666865, z94aaa38.zaece3c9, z5737392);
				strncpy(bc.z37f80f0, z5737392, sizeof(bc.z37f80f0) - 1);
				bc.z37f80f0[sizeof(bc.z37f80f0) - 1] = 0;
				bc.zbf4b4be = z94aaa38.zaece3c9;
			}
		} else {
			z503808b("nat punch OK: peer %s:%d (lobby had said %s:%d)",
			       plan.z10e203a, plan.zea7b53c, bc.z37f80f0, bc.zbf4b4be);
			strncpy(bc.z37f80f0, plan.z10e203a, sizeof(bc.z37f80f0) - 1);
			bc.z37f80f0[sizeof(bc.z37f80f0) - 1] = 0;
			bc.zbf4b4be = plan.zea7b53c;
		}
	}

	z8441e84(z6cc50c1);

	int r = ze0e3d01(&bc, &z6db85c1);
	if (r < 0) {
		z503808b("GgpoBridgeStart failed: %d (ggpo err %d)", r, z3186ab5());
		bprintf(PRINT_ERROR, _T("[fbneo_host] GgpoBridgeStart failed: %d\n"), r);
		return r;
	}

	zcef8663 = 1;
	z503808b("session up.");
	z2ce5e2d(bc.z3b2036b);
	z2e5f4c9(z94aaa38.z115149b && !z5aa079a && z3332e9c == 2, zd82262d == 1 ? z94aaa38.z7dd7ec2 : z94aaa38.zd843ce2, z94aaa38.z744cae6,
	          !z5aa079a && z3332e9c == 2);
	if (z94aaa38.z115149b) z503808b("chat: ligado (T escreve, F1 mostra/esconde)");
	zfe8da2e = zfb10f62();
	zf299840(zfe8da2e);
	if (zfe8da2e) z503808b("speedrun: na dupla o cronometro roda em todo quadro simulado e volta com o rollback; "
	                   "o log da corrida e o resultado saem do quadro confirmado");
	bprintf(PRINT_IMPORTANT,
	        _T("[fbneo_host] session up: %d players, local P%d, %d input byte(s)/player, delay %d, ")
	        _T("bind :%d, peer %S:%d.\n"),
	        z3332e9c, zd82262d, z378679b, bc.z3b2036b,
	        bc.zb3dbf67, bc.z37f80f0, bc.zbf4b4be);
	return 0;
}

static int z578148d(const char* szName)
{
	struct BurnInputInfo bii;
	int bit = 0;
	for (UINT32 i = 0; ; i++) {
		memset(&bii, 0, sizeof(bii));
		if (BurnDrvGetInputInfo(&bii, i)) break;
		if (z57065f7(bii.szName) != 1 || bii.nType != BIT_DIGITAL) continue;
		if (_stricmp(bii.szName, szName) == 0) return bit;
		bit++;
	}
	return -1;
}

static void zbd573ab(unsigned char* mask, const char* const* names, int n)
{
	memset(mask, 0, z102275f);
	for (int i = 0; i < n; i++) {
		const int bit = z578148d(names[i]);
		if (bit < 0 || bit / 8 >= z378679b) {
			memset(mask, 0, z102275f);
			return;
		}
		mask[bit / 8] |= (unsigned char)(1 << (bit % 8));
	}
}

int z595c26c(const z54dec38* cfg, int zb9843b9)
{
	if (zcef8663) z912bf82();
	z9ecf998 = 0;
	z3bfe586 = 0;
	z2cf405c     = 0;
	z607dc74     = 0;
	if (z909038f(cfg) != 0) return -1;
	z3faa1df(!z59b4fb8() && !z94aaa38.zed95901, z6cc50c1);

	za391f79 = 0;
	z94aaa38.z5737392[0] = 0;
	z94aaa38.zb306bef[0] = 0;

	z0687dd4();
	z4238888 bc;
	z76013f9(&bc);

	if (z40e51f6) {
		static const char* const z2295614[]         = { "P1 Button A", "P1 Button B", "P1 Button C" };
		static const char* const ze5d263d[] = { "P1 Start", "P1 Select" };
		zbd573ab(bc.za42b774[0], z2295614, 3);
		zbd573ab(bc.za42b774[1], ze5d263d, 2);
		z503808b("synctest: A+B+C e Start+Select nunca juntos (o menu da UniBIOS)");
	}

	z503808b("---- synctest: game=%s  check distance %d  %d frames ----",
	       bc.z91da805, zb9843b9, z7a81a0d);
	z8441e84(z6cc50c1);

	int r = zc0f640d(&bc, &z6db85c1, zb9843b9);
	if (r < 0) {
		z503808b("synctest %s: could not start (%d)", bc.z91da805, r);
		bprintf(PRINT_ERROR, _T("[fbneo_host] GgpoBridgeStartSyncTest failed: %d\n"), r);
		return r;
	}

	z90c98a2     = 1;
	z71317ab = zb9843b9;
	zcef8663 = 1;
	zfe8da2e = zfb10f62();
	zf299840(zfe8da2e);
	if (zfe8da2e) z503808b("synctest: o cronometro do speedrun roda em todo quadro e e conferido nos refeitos");
	bprintf(PRINT_IMPORTANT, _T("[fbneo_host] synctest up (check distance %d).\n"), zb9843b9);
	return 0;
}

static int z61f4c2a = 0;

int ze0d3fd3(const z9f83179* cfg)
{
	if (zcef8663) z912bf82();
	if (!cfg) return -1;
	const int z5d6903e = cfg->zac0db38[0] != '\0';
	if (!z5d6903e && (!cfg->z5737392[0] || !cfg->zc225a0e || !cfg->zb306bef[0])) return -1;
	if (!bDrvOkay) return -1;

	zac814b9 info;
	memset(&info, 0, sizeof(info));

	char ze1671eb[8] = "";
	const int z8fe4467 = !GetEnvironmentVariableA("RBF_SEM_PONTO", ze1671eb, sizeof(ze1671eb)) || ze1671eb[0] != '1';
	int rc = z5d6903e ? z3283911(cfg->zac0db38, z6cc50c1)
	       : z8fe4467   ? zc4093bf(cfg->z5737392, cfg->zc225a0e, cfg->zb306bef, 20000, cfg->z4fe4ccc, z6cc50c1)
	               : z0244df4(cfg->z5737392, cfg->zc225a0e, cfg->zb306bef, 20000, cfg->z4fe4ccc, z6cc50c1);
	if (rc != zd3e2c04) {
		z503808b("watch: could not join match %s (%d)", cfg->zb306bef, rc);
		bprintf(PRINT_ERROR, _T("[fbneo_host] watch: relay refused (%d).\n"), rc);
		return -1;
	}
	z7e412e1(&info);
	z256ef6d(info.z733a844);
	z256ef6d(info.zd5e0939);
	z98879fd  = z5d6903e && cfg->za501967;
	zd3b974b = z5d6903e && !z98879fd && cfg->zc85e9f4;
	z61f4c2a = z5d6903e && !z98879fd && cfg->za54c3d7 > 0;
	if (z61f4c2a) z67bbfd7(cfg->za54c3d7, nBurnFPS / 100.0);
	if (z98879fd) {
		strncpy(z579f7ed, cfg->zac0db38, sizeof(z579f7ed) - 1);
		z579f7ed[sizeof(z579f7ed) - 1] = '\0';
		z9e1660b   = z64b5a35();
		z6db68d6 = cfg->za54c3d7 > 0 ? cfg->za54c3d7 : 0;
		if (cfg->z83f94b8 == 5 || cfg->z83f94b8 == 10 || cfg->z83f94b8 == 15) z4698017 = cfg->z83f94b8;
		z7619697(zc67fd07());
		z2466609(info.z733a844, info.zd5e0939, info.szGame);
		z503808b("reprodutor: %s, %lld quadros, comeca no %lld, tomar o controle %d s",
		       z579f7ed, z9e1660b, z6db68d6, z4698017);
	}
	if (zd3b974b) {
		zda80ec2 = "rbf-indice.log";
		strncpy(z579f7ed, cfg->zac0db38, sizeof(z579f7ed) - 1);
		z579f7ed[sizeof(z579f7ed) - 1] = '\0';
		z9e1660b = z64b5a35();
		SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS);
	}

	z3332e9c    = info.z0d29982;
	zd82262d = 1;
	z05f35c5 = 1;
	z2a26285   = 0;
	z44d7871 = 0;
	z2fc19ce   = 0;
	zec0344b = 0;
	z9ecf998 = 1;
	z3bfe586 = 0;
	z2cf405c     = 0;
	z5aa079a        = 0;
	z607dc74     = zec3f3b5();
	z13cb333    = (cfg->z57d740a == 1 || cfg->z57d740a == 2) ? cfg->z57d740a : 0;

	if (z441d635() != 0) {
		z3dfe63b();
		return -1;
	}
	zf95ede8(0);
	z3faa1df(0, NULL);
	if (z378679b != info.z9df8312) {
		z503808b("watch: input size mismatch (stream %d, local %d) - refusing",
		       info.z9df8312, z378679b);
		bprintf(PRINT_ERROR, _T("[fbneo_host] watch: input size mismatch - update your build.\n"));
		z3dfe63b();
		return -1;
	}

	z7618f45  = 1;
	zcef8663 = 1;
	zd5a1a8b = z5d6903e && !z61f4c2a && !z98879fd;
	z1fa1f56  = cfg->ze6bfbdc;
	z6b3941a  = 0;
	z770c6b6(1);
	if (zd5a1a8b) {
		strncpy(z47ca634, cfg->zb306bef, sizeof(z47ca634) - 1);
		z47ca634[sizeof(z47ca634) - 1] = '\0';
		zdd50e3c = cfg->ze6bfbdc;
	}
	if (z13cb333) z8fa5b4b();
	else            z5c0c8b7(info.z733a844, info.zd5e0939, 0);
	if (!z13cb333 && !zd5a1a8b && !zd3b974b) {
		const char* quark = cfg->zb306bef;
		if (cfg->zac0db38[0]) {
			const char* a = strrchr(cfg->zac0db38, '\\');
			const char* b = strrchr(cfg->zac0db38, '/');
			if (b > a) a = b;
			quark = a ? a + 1 : cfg->zac0db38;
		}
		z5c3a407(BurnDrvGetTextA(DRV_NAME), quark, info.z733a844, info.zd5e0939, cfg->ze6bfbdc, 1);
	}
	if (z13cb333) z503808b("watch: corrida de speedrun (%d jogador(es)) - sem a barra dos nomes e sem o placar de luta", z13cb333);
	if (z607dc74 > 0) z503808b("teste: RBF_TESTE_CONFERE=%d (o checksum do estado no log)", z607dc74);
	z503808b("watching %s: %s vs %s (%s)", cfg->zb306bef, info.z733a844, info.zd5e0939, info.szGame);
	bprintf(PRINT_IMPORTANT, _T("[fbneo_host] watching %S vs %S.\n"), info.z733a844, info.zd5e0939);
	return 0;
}

int zb8f7e81(void) { return z7618f45; }
int zfe0050b(void) { return zd5a1a8b; }

#define z846b8f5     8
#define z9a586c9      1024
#define ze18898f     (z9a586c9 / z846b8f5)
#define zf095264     4
#define zd51dab8  30
#define z5036d73      59
#define z3f509bc    0.45f

static int            z41ac9d8    = 0;
static int            zef50c99     = 0;
static long long      z5fb3f79     = 0;
static long long      zdcc3701   = 0;
static long long      zb560b72     = 0;
static long long      za0f167b   = 0;
static unsigned char* z25d09e0       = NULL;
static int            z4b783fa    = 0;
static unsigned char* z997196f    = NULL;
static long long      z80fcf73[ze18898f];
static int            z9961b0e = 0;
static INT16*         zb3c682c    = NULL;
static int            za771460   = 0;
static INT16*         z53d1d60  = NULL;
static unsigned char* z61eebdf = NULL;
static int            zaef13a0      = 1;
static int            zaf7ee59  = 0;
static int            z6176c2f  = 1;
static int            z9f35782      = 1;
static int            z7284b97    = zf095264;
static int            z7f9a69a      = 0;
static unsigned char* zdd813bc = NULL;

static void z234d77e(void)
{
	free(z25d09e0);      z25d09e0 = NULL;
	free(z997196f);   z997196f = NULL;
	free(zb3c682c);   zb3c682c = NULL;
	free(z53d1d60); z53d1d60 = NULL;
	free(z61eebdf); z61eebdf = NULL;
	free(zdd813bc); zdd813bc = NULL;
	z41ac9d8 = 0;
	z5fb3f79 = za0f167b = 0;
}

static int z28d729b(void)
{
	if (z25d09e0) return 1;
	z4b783fa    = z3332e9c * z378679b;
	z9961b0e = z9e7e754();
	za771460   = nBurnSoundLen > 0 ? nBurnSoundLen : 800;
	z25d09e0       = (unsigned char*)calloc(z9a586c9, (size_t)z4b783fa);
	z997196f    = (unsigned char*)malloc((size_t)ze18898f * z9961b0e);
	zb3c682c    = (INT16*)calloc((size_t)z9a586c9 * za771460 * 2, sizeof(INT16));
	z53d1d60  = (INT16*)calloc((size_t)za771460 * 2 + 64, sizeof(INT16));
	for (int i = 0; i < ze18898f; i++) z80fcf73[i] = -1;
	if (!z25d09e0 || !z997196f || !zb3c682c || !z53d1d60) {
		z503808b("tomar o controle: sem memoria para guardar os ultimos quadros");
		z234d77e();
		return 0;
	}
	z503808b("tomar o controle: guardando %d quadros (%d estados de %d bytes)", z9a586c9, ze18898f, z9961b0e);
	return 1;
}

static void z75bc673(long long k)
{
	if (!z997196f || (k % z846b8f5) != 0) return;
	void* p = NULL;
	int n = 0;
	if (z2726b67((int)k, &p, &n, NULL) != z20fb1dd || !p || n != z9961b0e) return;
	int slot = (int)((k / z846b8f5) % ze18898f);
	memcpy(z997196f + (size_t)slot * z9961b0e, p, (size_t)n);
	z80fcf73[slot] = k;
}

static void zb41a8d0(long long k, int bDraw)
{
	zbdcd1c5(z25d09e0 + (size_t)(k % z9a586c9) * z4b783fa, z3332e9c);
	UINT8* z73e8341  = pBurnDraw;
	INT16* z4071d6c = pBurnSoundOut;
	if (bDraw) {
		pBurnSoundOut = z53d1d60;
		if (VidFrame()) { pBurnDraw = NULL; BurnDrvFrame(); }
	} else {
		pBurnDraw          = NULL;
		pBurnSoundOut      = NULL;
		bBurnRunAheadFrame = 1;
		BurnDrvFrame();
		bBurnRunAheadFrame = 0;
	}
	pBurnDraw     = z73e8341;
	pBurnSoundOut = z4071d6c;
}

static int zf8fddbe(long long d)
{
	if (d < 1) return 0;
	long long c = ((d - 1) / z846b8f5) * z846b8f5;
	int slot = (int)((c / z846b8f5) % ze18898f);
	if (z80fcf73[slot] != c) return 0;
	if (z4dac140((int)c, z997196f + (size_t)slot * z9961b0e, z9961b0e) != z20fb1dd) return 0;
	for (long long k = c; k < d - 1; k++) zb41a8d0(k, 0);
	zb41a8d0(d - 1, 1);
	zec0344b = d;
	return 1;
}

static void z6d2b96e(long long d, INT16* out, int zbb48f19)
{
	if (!out) return;
	const int len = nBurnSoundLen > 0 ? nBurnSoundLen : za771460;
	const int span = zbb48f19 * za771460;
	for (int i = 0; i < len; i++) {
		int j = span - 1 - (int)((long long)i * span / len);
		long long f = d + j / za771460;
		int s = j % za771460;
		const INT16* src = zb3c682c + ((size_t)(f % z9a586c9) * za771460 + s) * 2;
		out[i * 2]     = (INT16)(src[0] * z3f509bc);
		out[i * 2 + 1] = (INT16)(src[1] * z3f509bc);
	}
}

static void zb9d6a1a(void)
{
	if (pBurnSoundOut && nBurnSoundLen > 0) memset(pBurnSoundOut, 0, (size_t)nBurnSoundLen * 4);
}

static int zeb8974a(void)
{
	if (!z61eebdf) z61eebdf = (unsigned char*)malloc((size_t)z9961b0e);
	if (!zdd813bc) zdd813bc = (unsigned char*)malloc((size_t)zf480b00() + 1);
	void* z2916b6f = NULL;
	int zec5e271 = 0;
	if (!z61eebdf || !zdd813bc ||
	    z2726b67((int)zec0344b, &z2916b6f, &zec5e271, NULL) != z20fb1dd || !z2916b6f || zec5e271 != z9961b0e) {
		z503808b("tomar o controle: nao deu para guardar o quadro %lld", zec0344b);
		return 0;
	}
	memcpy(z61eebdf, z2916b6f, (size_t)zec5e271);
	z5eacaeb(zdd813bc);
	return 1;
}

static int zd5e815a(int bDraw);

static void z83fee2c(int z10a61bc)
{
	if (!z7618f45 || zd5a1a8b || !z2fc19ce || z41ac9d8 != 0 || !z25d09e0) return;
	if (z59b4fb8()) return;
	if (!z98879fd) z10a61bc = 1;
	const long long len = (long long)(z4698017 * nBurnFPS / 100);
	z7f9a69a = !z10a61bc;
	if (z10a61bc) {
		long long oldest = zec0344b - (z9a586c9 - z846b8f5 * 2);
		zb560b72   = zec0344b;
		zdcc3701 = zec0344b - len;
		if (zdcc3701 < oldest) zdcc3701 = oldest;
		if (zdcc3701 < 1) zdcc3701 = 1;
		if (z48296e9 > 0 && zdcc3701 < z48296e9 + z846b8f5) zdcc3701 = z48296e9 + z846b8f5;
		if (zdcc3701 >= zb560b72) return;
		if (z98879fd && !z698ca60(zdcc3701)) {
			z503808b("tomar o controle: o quadro %lld nao estava guardado", zdcc3701);
			return;
		}
		if (!zeb8974a()) return;
		z7284b97 = (int)((zb560b72 - zdcc3701) / 90);
		if (z7284b97 < zf095264) z7284b97 = zf095264;
		z503808b("tomar o controle: no quadro %lld, volta para o %lld", zb560b72, zdcc3701);
	} else {
		zdcc3701 = zec0344b;
		zb560b72   = zec0344b + len;
		if (zb560b72 > z9e1660b) zb560b72 = z9e1660b;
		if (zb560b72 - zdcc3701 < 30 || zdcc3701 < 1) return;
		const int nScore = zf480b00();
		unsigned char* z65bc10e = (unsigned char*)malloc((size_t)nScore + 1);
		if (!z65bc10e) return;
		z5eacaeb(z65bc10e);
		while (zec0344b < zb560b72)
			if (zd5e815a(0) <= 0) break;
		zb560b72 = zec0344b;
		const int ok = zeb8974a() && zf8fddbe(zdcc3701);
		zdf72ff8(z65bc10e);
		free(z65bc10e);
		if (!ok) {
			z503808b("tomar o controle: nao deu para preparar o trecho %lld a %lld", zdcc3701, zb560b72);
			if (z61eebdf) z4dac140((int)zb560b72, z61eebdf, z9961b0e);
			if (zdd813bc) zdf72ff8(zdd813bc);
			zec0344b = zb560b72;
			return;
		}
		z503808b("tomar o controle: a partir do quadro %lld, ate o %lld", zdcc3701, zb560b72);
	}
	z41ac9d8 = 1;
	zef50c99  = 0;
}

void zffad440(void) { z83fee2c(1); }

int zf8b4e34(float* pPos)
{
	if (!z7618f45 || z41ac9d8 < 1 || z41ac9d8 > 3) return 0;
	float p = 1.0f;
	if (z41ac9d8 == 1) {
		float t = (float)zef50c99 / zd51dab8;
		if (t > 1.0f) t = 1.0f;
		p = 1.0f - (1.0f - t) * (1.0f - t) * (1.0f - t);
	} else if (z41ac9d8 == 3) {
		float t = (float)(zb560b72 - zec0344b) / (float)(zb560b72 - zdcc3701);
		if (t < 0.0f) t = 0.0f;
		if (t > 1.0f) t = 1.0f;
		p = 1.0f - t * t;
	}
	if (pPos) *pPos = p;
	return 1;
}

int zbac67f0(void) { return z7618f45 && !zd5a1a8b && !z98879fd && z2fc19ce && z41ac9d8 == 0 && !zd4afa4e && !z59b4fb8(); }

static int zfc5aff1(void)
{
	if (z41ac9d8 < 1 || z41ac9d8 > 3) return 0;
	za0f167b++;
	zef50c99++;
	if (z41ac9d8 == 1 || z41ac9d8 == 2) {
		zb9d6a1a();
		z5ee653e();
		if (z41ac9d8 == 1 && zef50c99 >= zd51dab8) { z41ac9d8 = 2; zef50c99 = 0; }
		else if (z41ac9d8 == 2 && zef50c99 >= z5036d73) {
			zef50c99 = 0;
			if (z7f9a69a) {
				z41ac9d8   = 5;
				zaef13a0     = 1;
				zaf7ee59 = 0;
				z6176c2f = 1;
			} else {
				z41ac9d8 = 3;
			}
		}
		return 1;
	}
	long long d = zec0344b - z7284b97;
	if (d < zdcc3701) d = zdcc3701;
	INT16* live = pBurnSoundOut;
	if (!zf8fddbe(d)) {
		z503808b("tomar o controle: o quadro %lld nao estava guardado - para aqui", d);
		z41ac9d8 = 4;
		return 1;
	}
	z6d2b96e(d, live, z7284b97);
	if (d <= zdcc3701) {
		z41ac9d8   = 5;
		zef50c99    = 0;
		zaef13a0     = 1;
		zaf7ee59 = 0;
		z6176c2f = 1;
		z503808b("tomar o controle: voltou ao quadro %lld; escolha P1 ou P2", d);
	}
	return 1;
}

static int zddc3607(void) { return z41ac9d8 >= 5; }
static int z2862e37(void) { return zddc3607() || z98879fd || (z7618f45 && !zd5a1a8b && !z59b4fb8()); }

static const unsigned char* zf681ee3(void)
{
	const int p = (z05f35c5 >= 1 && z05f35c5 <= z3332e9c) ? z05f35c5 - 1 : 0;
	return zf1988cc[p];
}

static int z27d554e(const unsigned char* bits, int b) { return (bits[b >> 3] >> (b & 7)) & 1; }

static const char* zd8e7890(const char* name)
{
	return (name && name[0] == 'P' && name[1] >= '1' && name[1] <= '9' && name[2] == ' ') ? name + 3 : name;
}

static void zb45a47b(int* pDir, int* z2237756)
{
	const int p = (z05f35c5 >= 1 && z05f35c5 <= z3332e9c) ? z05f35c5 - 1 : 0;
	const unsigned char* bits = zf681ee3();
	int dir = 0, z212301c = 0;
	for (int b = 0; b < z3c35b6f[p].nBits; b++) {
		if (!z27d554e(bits, b) || (p == 0 && b == zc3f80f4)) continue;
		const char* n = zd8e7890(zde7202f[p][b]);
		if (!n) continue;
		if (!strcmp(n, "Left")) dir = -1;
		else if (!strcmp(n, "Right")) dir = 1;
		else if (strcmp(n, "Up") && strcmp(n, "Down")) z212301c = 1;
	}
	*pDir = dir;
	*z2237756 = z212301c;
}

static void z380d95d(long long k, unsigned char* out)
{
	const int n = z3332e9c * z378679b;
	memcpy(out, z25d09e0 + (size_t)(k % z9a586c9) * z4b783fa, (size_t)n);
	const int q = zaef13a0 - 1;
	const int p = (z05f35c5 >= 1 && z05f35c5 <= z3332e9c) ? z05f35c5 - 1 : 0;
	if (q < 0 || q >= z3332e9c) return;
	unsigned char* dst = out + (size_t)q * z378679b;
	memset(dst, 0, (size_t)z378679b);
	const unsigned char* bits = zf681ee3();
	for (int b = 0; b < z3c35b6f[p].nBits; b++) {
		if (!z27d554e(bits, b) || (p == 0 && b == zc3f80f4)) continue;
		const char* n = zd8e7890(zde7202f[p][b]);
		if (!n) continue;
		for (int c = 0; c < z3c35b6f[q].nBits; c++) {
			const char* m = zd8e7890(zde7202f[q][c]);
			if (m && !strcmp(m, n)) { dst[c >> 3] |= (unsigned char)(1 << (c & 7)); break; }
		}
	}
}

int z27c9ebc(int* pSel, int* z41ac960)
{
	if (!z7618f45 || z41ac9d8 != 5) return 0;
	if (pSel) *pSel = zaef13a0;
	if (z41ac960) *z41ac960 = ((zef50c99 / 15) & 1) == 0;
	return 1;
}

int zd31e64a(int* zbdad72a, int* z41ac960)
{
	if (!z7618f45 || z41ac9d8 != 9) return 0;
	if (zbdad72a) *zbdad72a = z9f35782;
	if (z41ac960) *z41ac960 = ((zef50c99 / 15) & 1) == 0;
	return 1;
}

int z2aa01f8(int* pNum, float* pAlpha)
{
	if (!z7618f45 || z41ac9d8 != 6) return 0;
	const int per = (int)(nBurnFPS / 100);
	const int i = zef50c99 / per;
	if (pNum) *pNum = 3 - i;
	if (pAlpha) *pAlpha = 1.0f - (float)(zef50c99 % per) / (float)per;
	return 1;
}

static int z8542e7a(void)
{
	if (z41ac9d8 < 5) return 0;
	za0f167b++;
	zef50c99++;
	if (z41ac9d8 == 5) {
		int dir, z212301c;
		zb45a47b(&dir, &z212301c);
		if (dir && dir != zaf7ee59) zaef13a0 = (dir < 0) ? 1 : 2;
		zaf7ee59 = dir;
		if (z212301c && !z6176c2f) {
			z41ac9d8 = 6;
			zef50c99  = 0;
			z503808b("tomar o controle: escolheu o P%d", zaef13a0);
		}
		z6176c2f = z212301c;
		zb9d6a1a();
		z5ee653e();
		return 1;
	}
	if (z41ac9d8 == 6) {
		zb9d6a1a();
		z5ee653e();
		if (zef50c99 >= 3 * (int)(nBurnFPS / 100)) {
			z41ac9d8 = 7;
			zef50c99  = 0;
			z503808b("tomar o controle: o P%d esta com quem assiste do quadro %lld ao %lld", zaef13a0, zec0344b, zb560b72);
		}
		return 1;
	}
	if (z41ac9d8 == 8) {
		zb9d6a1a();
		z5ee653e();
		if (zef50c99 >= 2 * (int)(nBurnFPS / 100)) {
			z41ac9d8   = 9;
			zef50c99    = 0;
			z9f35782     = 1;
			zaf7ee59 = 0;
			z6176c2f = 1;
		}
		return 1;
	}
	if (z41ac9d8 == 9) {
		int dir, z212301c;
		zb45a47b(&dir, &z212301c);
		if (dir && dir != zaf7ee59) z9f35782 = (dir < 0) ? 0 : 1;
		zaf7ee59 = dir;
		int go = (z212301c && !z6176c2f);
		z6176c2f = z212301c;
		if (!go && zef50c99 >= 5 * (int)(nBurnFPS / 100)) { go = 1; z9f35782 = 1; }
		zb9d6a1a();
		if (go && z9f35782 == 0) {
			if (zf8fddbe(zdcc3701)) {
				z41ac9d8 = 6;
				zef50c99  = 0;
				z503808b("tomar o controle: tentar de novo, do quadro %lld", zdcc3701);
				return 1;
			}
			z503808b("tomar o controle: o quadro %lld ja nao estava guardado - segue o replay", zdcc3701);
		}
		if (go) {
			if (z61eebdf && z4dac140((int)zb560b72, z61eebdf, z9961b0e) == z20fb1dd)
				z503808b("tomar o controle: o replay segue do quadro %lld", zb560b72);
			else
				z503808b("tomar o controle: nao deu para voltar ao quadro %lld", zb560b72);
			if (zdd813bc) zdf72ff8(zdd813bc);
			zec0344b = zb560b72;
			z41ac9d8 = 0;
		}
		z5ee653e();
		return 1;
	}
	const long long k = zec0344b;
	if (k >= zb560b72) {
		z41ac9d8 = 8;
		zef50c99  = 0;
		z503808b("tomar o controle: chegou ao quadro %lld", zb560b72);
		zb9d6a1a();
		z5ee653e();
		return 1;
	}
	unsigned char in[z897cf46 * z102275f];
	z380d95d(k, in);
	zbdcd1c5(in, z3332e9c);
	if (VidFrame()) { pBurnDraw = NULL; BurnDrvFrame(); }
	zec0344b = k + 1;
	return 1;
}

#define zf07bfab 64
static int        z7cb094e = -1;
static long long  zeaccd8c[zf07bfab], ze30433f[zf07bfab];
static int        zcdb5447[zf07bfab];
static unsigned char* z25f8919 = NULL;

static int z2f8feb0(long long f)
{
	if (z7cb094e < 0) {
		z7cb094e = 0;
		char v[2048] = "";
		if (GetEnvironmentVariableA("RBF_ANALISE_IMG", v, sizeof(v))) {
			char* p = v;
			while (*p && z7cb094e < zf07bfab) {
				char* end = NULL;
				const long long a = strtoll(p, &end, 10);
				if (end == p) break;
				long long b = a;
				int every = 1;
				if (*end == '-') b = strtoll(end + 1, &end, 10);
				if (*end == ':') every = (int)strtol(end + 1, &end, 10);
				zeaccd8c[z7cb094e]  = a;
				ze30433f[z7cb094e]    = b;
				zcdb5447[z7cb094e] = every > 0 ? every : 1;
				z7cb094e++;
				while (*end == ',' || *end == ' ') end++;
				p = end;
			}
			z503808b("analise: RBF_ANALISE_IMG - imagens de %d trecho(s) em rbf-rep-<quadro>.bmp", z7cb094e);
		}
	}
	for (int i = 0; i < z7cb094e; i++)
		if (f >= zeaccd8c[i] && f <= ze30433f[i] && ((f - zeaccd8c[i]) % zcdb5447[i]) == 0) return 1;
	return 0;
}

static UINT8* zc2c2eb9(int* pW, int* z932636c)
{
	const int nMax = 1024 * 1024;
	if (nBurnBpp < 2 || nBurnBpp > 4) return NULL;
	BurnDrvGetVisibleSize(pW, z932636c);
	if (*pW <= 0 || *z932636c <= 0 || *pW * *z932636c > nMax) return NULL;
	if (!z25f8919) z25f8919 = (unsigned char*)malloc((size_t)nMax * 4);
	if (!z25f8919) return NULL;
	nBurnPitch = *pW * nBurnBpp;
	return z25f8919;
}

static int zd5e815a(int bDraw)
{
	unsigned char in[z897cf46 * z102275f];
	const int n = z3332e9c * z378679b;
	const long long k = zec0344b;

	int z59453f5 = (z41ac9d8 == 4 && k < z5fb3f79 && z25d09e0);
	if (z59453f5) {
		memcpy(in, z25d09e0 + (size_t)(k % z9a586c9) * z4b783fa, (size_t)n);
	} else {
		if (z41ac9d8 == 4) { z41ac9d8 = 0; z503808b("tomar o controle: alcancou o quadro %lld", k); }
		if (z98879fd) {
			if (k >= z9e1660b || !z18d4eb5((int)k, in, n)) return -1;
		} else {
			int r = z2e8f5ef(in, n);
			if (r == 0) return 0;
			if (r < 0)  return -1;
		}
	}
	if (!zd5a1a8b && !z59b4fb8() && z28d729b()) {
		z75bc673(k);
		memcpy(z25d09e0 + (size_t)(k % z9a586c9) * z4b783fa, in, (size_t)n);
	}

	zbdcd1c5(in, z3332e9c);
	z4878edb(z27451c3(in, z3332e9c), 1);

	INT16* z4071d6c = pBurnSoundOut;
	if (bDraw) {
		if (!pBurnSoundOut) pBurnSoundOut = zb749718();
		if (VidFrame()) { pBurnDraw = NULL; BurnDrvFrame(); }
	} else {
		UINT8* z73e8341  = pBurnDraw;
		pBurnDraw          = NULL;
		const INT32 zb5157bc = nBurnPitch;
		int z98506c7 = 0, z49c34cd = 0;
		if (zd5a1a8b && z2f8feb0(k + 1)) pBurnDraw = zc2c2eb9(&z98506c7, &z49c34cd);
		pBurnSoundOut      = zb749718();
		BurnDrvFrame();
		if (pBurnDraw) zd5697a3("rbf-rep", (unsigned)(k + 1), pBurnDraw, z98506c7, z49c34cd, nBurnBpp, nBurnPitch);
		pBurnDraw          = z73e8341;
		nBurnPitch         = zb5157bc;
	}
	pBurnSoundOut = z4071d6c;
	if (zb3c682c) {
		INT16* dst = zb3c682c + (size_t)(k % z9a586c9) * za771460 * 2;
		if (bDraw && pBurnSoundOut && nBurnSoundLen > 0 && nBurnSoundLen <= za771460)
			memcpy(dst, pBurnSoundOut, (size_t)nBurnSoundLen * 4);
		else
			memset(dst, 0, (size_t)za771460 * 4);
	}
	zec0344b++;
	if (zec0344b > z5fb3f79) z5fb3f79 = zec0344b;
	if (z59453f5) za0f167b++;
	if (z607dc74 > 0 && z2fc19ce && (zec0344b % z607dc74) == 0) {
		unsigned int chk = 0;
		if (z2726b67((int)zec0344b, NULL, NULL, &chk) == z20fb1dd) ze109f17(zec0344b, chk);
	}
	if (zd5a1a8b) z1907e87();
	zb4c9a9b();
	if (z98879fd) z4ec6bf1(zec0344b);
	if (zd3b974b) z2861a61(zec0344b);
	return 1;
}

extern "C" {
int compress2(unsigned char* dest, unsigned long* destLen, const unsigned char* source, unsigned long sourceLen, int level);
int uncompress(unsigned char* dest, unsigned long* destLen, const unsigned char* source, unsigned long sourceLen);
unsigned long compressBound(unsigned long sourceLen);
}

#define z6914081      120
#define z525eb11      600
#define z9d275e7   6
#define z573496e    12
#define z7cccf04    90
#define z2f253e7     128
#define z9b7c424    1024

typedef struct { long long f; unsigned char* z; int n; } z594c5e0;
typedef struct { long long start, end; int winner, p1g, p2g, p1r, p2r; } z9317e56;
typedef struct { int game, num; long long start, end; int winner; } z82b59e7;

static z594c5e0*          z89cd391 = NULL;
static int            zbd36354 = 0, z77a9fc0 = 0;
static unsigned char* z43a8c8b = NULL;
static int            ze7edbaa = 0, zc6e41a6 = 0;
static int            z0728ad4 = 0;
static long long      z5573750 = 0;

static z9317e56         z38dd576[z2f253e7];
static int            z687a7e0 = 0;
static z82b59e7        ze51d6db[z9b7c424];
static int            z0bcd0d3 = 0;
static int            z70b9d5d = 0;
static long long      z38f8986 = 0;
static FILETIME       z6b72516;
static DWORD          z923901d = 0;
static long long      zd9f7cf1[z9b7c424];
static int            zf7d61d0 = 0;

static int            z34de379 = 0, z4331087 = 1, z2e12e17 = 0, zb8d1dcd = 0;
static long long      z2811a1f = -1;
static int            zff60ae5 = 0, z8e1a4e8 = 0;
static long long      zaf75865 = -1, z8c93801 = 0, z11e9799 = 0;
static DWORD          z676a119 = 0;
static unsigned long long z34e370c = 0;
static int            zfc24aa9 = 0;
static int            za13a072 = 0, z422e12d = 0, ze581b66 = 0;
static unsigned long long zd02cb2c = 0;
static int            z5f2aeea = 0, z36e8985 = 0, z6b37769 = 0;
static int            z71ecdc0 = 0;
static wchar_t        z264edf7[40] = L"P1", zde3b108[40] = L"P2";
static char           ze50dab7[32] = "";
static PROCESS_INFORMATION z6a93215;

#define z39a7471   0
#define zb188efd  1
#define z6e50563 2
#define zf1af1dc     3
static int            zb055ed5 = 0, z381a6bc = z39a7471, z5ed723f = 0, z68ee208 = 0;
static wchar_t        z57caa55[160] = L"";
static int            zde7c67c = 0;

#define z15c884f 5
static int            z6874830 = 0;
static int            z4f15ce2 = 0, zdbea536 = -1;

static z9317e56         z81ad1fa;
static int            z74765e5 = 0, zdb0f2eb = 0;
static z82b59e7        zf6ed190[64];
static int            z04a6853 = 0;
static FILE*          z27dc8fc = NULL;
static long long      z6f8b26c = 0;

static void z2466609(const char* z733a844, const char* zd5e0939, const char* szGame)
{
	MultiByteToWideChar(CP_UTF8, 0, z733a844 && z733a844[0] ? z733a844 : "P1", -1, z264edf7, 40);
	MultiByteToWideChar(CP_UTF8, 0, zd5e0939 && zd5e0939[0] ? zd5e0939 : "P2", -1, zde3b108, 40);
	strncpy(ze50dab7, szGame ? szGame : "", sizeof(ze50dab7) - 1);
}

static void z0f2b239(char* out, size_t n, const char* file)
{
	snprintf(out, n, "%s\\%s", z579f7ed, file);
}

static int z8651582(void)
{
	if (z43a8c8b) return 1;
	ze7edbaa   = z9e7e754();
	z0728ad4    = z59b4fb8() ? z3a3ab4d() : 0;
	zc6e41a6 = zf480b00() + z0728ad4;
	if (ze7edbaa <= 0) return 0;
	z43a8c8b = (unsigned char*)malloc((size_t)ze7edbaa + zc6e41a6);
	return z43a8c8b != NULL;
}

static int z42ef8f3(long long f)
{
	int lo = 0, hi = zbd36354 - 1, r = -1;
	while (lo <= hi) {
		const int m = (lo + hi) / 2;
		if (z89cd391[m].f <= f) { r = m; lo = m + 1; } else hi = m - 1;
	}
	return r;
}

static void z5b7e5c6(long long f, unsigned char* z, int n)
{
	const int i = z42ef8f3(f);
	if (i >= 0 && z89cd391[i].f == f) { free(z); return; }
	if (zbd36354 == z77a9fc0) {
		const int nc = z77a9fc0 ? z77a9fc0 * 2 : 256;
		z594c5e0* p = (z594c5e0*)realloc(z89cd391, (size_t)nc * sizeof(z594c5e0));
		if (!p) { free(z); return; }
		z89cd391 = p;
		z77a9fc0 = nc;
	}
	memmove(&z89cd391[i + 2], &z89cd391[i + 1], (size_t)(zbd36354 - (i + 1)) * sizeof(z594c5e0));
	z89cd391[i + 1].f = f;
	z89cd391[i + 1].z = z;
	z89cd391[i + 1].n = n;
	zbd36354++;
}

static unsigned char* z5b11a1d(int* pn)
{
	if (!z8651582()) return NULL;
	void* p = NULL;
	int n = 0;
	if (z2726b67((int)zec0344b, &p, &n, NULL) != z20fb1dd || !p || n != ze7edbaa) return NULL;
	memcpy(z43a8c8b, p, (size_t)n);
	z5eacaeb(z43a8c8b + n);
	if (z0728ad4) z66855d5(z43a8c8b + n + (zc6e41a6 - z0728ad4));
	unsigned long zd2627c2 = compressBound((unsigned long)(n + zc6e41a6));
	unsigned char* z = (unsigned char*)malloc(zd2627c2);
	if (!z) return NULL;
	if (compress2(z, &zd2627c2, z43a8c8b, (unsigned long)(n + zc6e41a6), 1) != 0) { free(z); return NULL; }
	unsigned char* zz = (unsigned char*)realloc(z, zd2627c2);
	*pn = (int)zd2627c2;
	return zz ? zz : z;
}

static int ze48572a(const z594c5e0* c)
{
	if (!z8651582()) return 0;
	unsigned long len = (unsigned long)(ze7edbaa + zc6e41a6);
	if (uncompress(z43a8c8b, &len, c->z, (unsigned long)c->n) != 0 || len != (unsigned long)(ze7edbaa + zc6e41a6))
		return 0;
	if (z4dac140((int)c->f, z43a8c8b, ze7edbaa) != z20fb1dd) return 0;
	zdf72ff8(z43a8c8b + ze7edbaa);
	if (z0728ad4) z856caea(z43a8c8b + ze7edbaa + (zc6e41a6 - z0728ad4));
	zec0344b = c->f;
	return 1;
}

static void z4ec6bf1(long long f)
{
	if ((f % z6914081) != 0) return;
	const int i = z42ef8f3(f);
	if (i >= 0 && z89cd391[i].f == f) return;
	int n = 0;
	unsigned char* z = z5b11a1d(&n);
	if (z) z5b7e5c6(f, z, n);
}

static void z1bdb305(void)
{
	char path[300];
	z0f2b239(path, sizeof(path), "estados.bin");
	FILE* f = fopen(path, "rb");
	if (!f) return;
	if (_fseeki64(f, z5573750, SEEK_SET) == 0) {
		for (;;) {
			unsigned char h[16];
			if (fread(h, 1, 16, f) != 16 || memcmp(h, "RBCP", 4) != 0) break;
			long long fr;
			int n;
			memcpy(&fr, h + 4, 8);
			memcpy(&n, h + 12, 4);
			if (n <= 0 || n > (64 << 20)) break;
			unsigned char* z = (unsigned char*)malloc((size_t)n);
			if (!z) break;
			if ((int)fread(z, 1, (size_t)n, f) != n) { free(z); break; }
			z5b7e5c6(fr, z, n);
			z5573750 += 16 + n;
		}
	}
	fclose(f);
}

#define z1176457 3
static int z46fb7af(void) { return z59b4fb8() ? 100000 + z3a3ab4d() : z1176457; }

static void z620129c(int bForce)
{
	if (!bForce && GetTickCount() - z923901d < 1500) return;
	z923901d = GetTickCount();
	char path[300];
	z0f2b239(path, sizeof(path), "indice.txt");
	WIN32_FILE_ATTRIBUTE_DATA a;
	if (!GetFileAttributesExA(path, GetFileExInfoStandard, &a)) return;
	if (!bForce && CompareFileTime(&a.ftLastWriteTime, &z6b72516) == 0) return;
	FILE* f = fopen(path, "rb");
	if (!f) return;
	z6b72516 = a.ftLastWriteTime;
	z687a7e0 = z0bcd0d3 = 0;
	z70b9d5d = 0;
	int ver = 0;
	char line[256];
	while (fgets(line, sizeof(line), f)) {
		z9317e56 g;
		z82b59e7 r;
		int n = 0, gn = 0, rn = 0;
		long long v = 0;
		memset(&g, 0, sizeof(g));
		memset(&r, 0, sizeof(r));
		if (sscanf(line, "partida=%d inicio=%lld fim=%lld vencedor=%d placar=%d-%d rounds=%d-%d",
		           &n, &g.start, &g.end, &g.winner, &g.p1g, &g.p2g, &g.p1r, &g.p2r) == 8) {
			if (z687a7e0 < z2f253e7) z38dd576[z687a7e0++] = g;
		} else if (sscanf(line, "round=%d.%d inicio=%lld fim=%lld vencedor=%d",
		                  &gn, &rn, &r.start, &r.end, &r.winner) == 5) {
			r.game = gn;
			r.num  = rn;
			if (z0bcd0d3 < z9b7c424) ze51d6db[z0bcd0d3++] = r;
		} else if (sscanf(line, "analisado=%lld", &v) == 1) {
			z38f8986 = v;
		} else if (!strncmp(line, "completo=1", 10)) {
			z70b9d5d = 1;
		} else {
			sscanf(line, "v=%d", &ver);
		}
	}
	fclose(f);
	if (ver != z46fb7af()) z70b9d5d = 0;
}

static void z74bb311(long long f)
{
	for (int i = 0; i < zf7d61d0; i++) if (zd9f7cf1[i] == f) return;
	if (zf7d61d0 < z9b7c424) zd9f7cf1[zf7d61d0++] = f;
}

static long long zf5c2f23(long long f)
{
	long long best = -1;
	for (int i = 0; i < z0bcd0d3; i++)
		if (ze51d6db[i].start > f && (best < 0 || ze51d6db[i].start < best)) best = ze51d6db[i].start;
	for (int i = 0; i < zf7d61d0; i++)
		if (zd9f7cf1[i] > f && (best < 0 || zd9f7cf1[i] < best)) best = zd9f7cf1[i];
	return best;
}

static long long z5a0aed7(long long f)
{
	long long best = -1;
	for (int i = 0; i < z0bcd0d3; i++)
		if (ze51d6db[i].start <= f && ze51d6db[i].start > best) best = ze51d6db[i].start;
	for (int i = 0; i < zf7d61d0; i++)
		if (zd9f7cf1[i] <= f && zd9f7cf1[i] > best) best = zd9f7cf1[i];
	return best;
}

static int zeba0a0a(long long f)
{
	int g = 0;
	for (int i = 0; i < z687a7e0; i++) if (z38dd576[i].start <= f) g = i + 1;
	return g;
}

static void zc85cc9e(void)
{
	if (!z27dc8fc) return;
	int n = 0;
	unsigned char* z = z5b11a1d(&n);
	if (!z) return;
	unsigned char h[16];
	memcpy(h, "RBCP", 4);
	long long f = zec0344b;
	memcpy(h + 4, &f, 8);
	memcpy(h + 12, &n, 4);
	fwrite(h, 1, 16, z27dc8fc);
	fwrite(z, 1, (size_t)n, z27dc8fc);
	fflush(z27dc8fc);
	free(z);
}

static void z1965cc9(int bDone)
{
	char path[300], tmp[310];
	z0f2b239(path, sizeof(path), "indice.txt");
	snprintf(tmp, sizeof(tmp), "%s.tmp", path);
	FILE* f = fopen(tmp, "wb");
	if (!f) return;
	fprintf(f, "v=%d\nquadros=%lld\nanalisado=%lld\n", z46fb7af(), z9e1660b, zec0344b);
	for (int i = 0; i < z687a7e0; i++) {
		const z9317e56* g = &z38dd576[i];
		fprintf(f, "partida=%d inicio=%lld fim=%lld vencedor=%d placar=%d-%d rounds=%d-%d\n",
		        i + 1, g->start, g->end, g->winner, g->p1g, g->p2g, g->p1r, g->p2r);
	}
	for (int i = 0; i < z0bcd0d3; i++) {
		const z82b59e7* r = &ze51d6db[i];
		fprintf(f, "round=%d.%d inicio=%lld fim=%lld vencedor=%d\n", r->game, r->num, r->start, r->end, r->winner);
	}
	if (z74765e5) {
		int p1g = 0, p2g = 0, p1r = 0, p2r = 0;
		zd82e415(&p1g, &p2g);
		for (int i = 0; i < z04a6853; i++) {
			if (zf6ed190[i].winner == 1 && zf6ed190[i].end) p1r++;
			else if (zf6ed190[i].winner == 2 && zf6ed190[i].end) p2r++;
		}
		fprintf(f, "partida=%d inicio=%lld fim=-1 vencedor=-1 placar=%d-%d rounds=%d-%d\n",
		        z687a7e0 + 1, z81ad1fa.start, p1g, p2g, p1r, p2r);
		for (int i = 0; i < z04a6853; i++)
			fprintf(f, "round=%d.%d inicio=%lld fim=%lld vencedor=%d\n", z687a7e0 + 1, zf6ed190[i].num,
			        zf6ed190[i].start, zf6ed190[i].end ? zf6ed190[i].end : -1LL, zf6ed190[i].end ? zf6ed190[i].winner : -1);
	}
	if (bDone) fprintf(f, "completo=1\n");
	fclose(f);
	MoveFileExA(tmp, path, MOVEFILE_REPLACE_EXISTING);
	z6f8b26c = zec0344b;
}

static void z4c2dd21(int nType, long long f, int zdb52f45)
{
	if (nType == z69eb802) {
		z74765e5 = 1;
		memset(&z81ad1fa, 0, sizeof(z81ad1fa));
		z81ad1fa.start = f;
		z04a6853 = 0;
		zdb0f2eb = 0;
	}
	if (!z74765e5) return;
	if (nType == z69eb802 || nType == z5ea2195) {
		if (z04a6853 < 64) {
			z82b59e7* r = &zf6ed190[z04a6853++];
			memset(r, 0, sizeof(*r));
			r->num   = ++zdb0f2eb;
			r->start = f;
		}
		zc85cc9e();
	} else if (nType == zafb1e7e) {
		if (z04a6853 > 0 && !zf6ed190[z04a6853 - 1].end) {
			zf6ed190[z04a6853 - 1].end    = f;
			zf6ed190[z04a6853 - 1].winner = zdb52f45;
		}
	} else if (nType == z93d035c) {
		z81ad1fa.end    = f;
		z81ad1fa.winner = zdb52f45;
		zd82e415(&z81ad1fa.p1g, &z81ad1fa.p2g);
		for (int i = 0; i < z04a6853; i++) {
			if (zf6ed190[i].winner == 1) z81ad1fa.p1r++;
			else if (zf6ed190[i].winner == 2) z81ad1fa.p2r++;
		}
		if (z687a7e0 < z2f253e7) {
			z38dd576[z687a7e0++] = z81ad1fa;
			for (int i = 0; i < z04a6853 && z0bcd0d3 < z9b7c424; i++) {
				zf6ed190[i].game = z687a7e0;
				if (!zf6ed190[i].end) zf6ed190[i].end = f;
				ze51d6db[z0bcd0d3++] = zf6ed190[i];
			}
		}
		z74765e5 = 0;
		z04a6853 = 0;
		z1965cc9(0);
	}
}

static void z6ddd091(int nType, long long f, int zdb52f45)
{
	if (zd3b974b) z4c2dd21(nType, f, zdb52f45);
	if (z98879fd && (nType == z69eb802 || nType == z5ea2195)) {
		z74bb311(f);
		if (zff60ae5 && f > z8c93801 && zaf75865 < 0) zaf75865 = f;
	}
}

static void z2861a61(long long f)
{
	if ((f % z525eb11) == 0) zc85cc9e();
	if (f - z6f8b26c >= 3600) z1965cc9(0);
}

static void z41779d8(void)
{
	char path[300];
	z0f2b239(path, sizeof(path), "estados.bin");
	z27dc8fc = fopen(path, "wb");
	z687a7e0 = z0bcd0d3 = 0;
	z74765e5 = 0;
	z6f8b26c = 0;
	zc85cc9e();
	z1965cc9(0);
	z503808b("analisador: %s, %lld quadros", z579f7ed, z9e1660b);
}

static void zdd22904(void)
{
	if (!zd3b974b) return;
	z1965cc9(1);
	if (z27dc8fc) { fclose(z27dc8fc); z27dc8fc = NULL; }
	z503808b("analisador: pronto - %d partidas, %d rounds", z687a7e0, z0bcd0d3);
}

static void z5a1a13d(void)
{
	z620129c(1);
	if (z70b9d5d) { z1bdb305(); return; }
	char path[300];
	z0f2b239(path, sizeof(path), "indice.txt");  DeleteFileA(path);
	z0f2b239(path, sizeof(path), "estados.bin"); DeleteFileA(path);
	z5573750 = 0;
	char exe[MAX_PATH] = { 0 };
	if (!GetModuleFileNameA(NULL, exe, MAX_PATH - 1)) return;
	char cmd[1024];
	char run[48] = "";
	if (z13cb333) snprintf(run, sizeof(run), ",corrida=%d", z13cb333);
	snprintf(cmd, sizeof(cmd), "\"%s\" %s -w -rbffundo -rbfreplay dir=%s,indice=1%s -noscan%s", exe, ze50dab7, z579f7ed,
	         run, z59b4fb8() ? " -rbfspeedrun" : "");
	STARTUPINFOA si;
	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	memset(&z6a93215, 0, sizeof(z6a93215));
	if (CreateProcessA(NULL, cmd, NULL, NULL, FALSE, BELOW_NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &z6a93215))
		z503808b("reprodutor: analisador aberto (pid %lu)", (unsigned long)z6a93215.dwProcessId);
	else
		z503808b("reprodutor: nao deu para abrir o analisador (%lu)", GetLastError());
}

static void z1c4e295(long long t)
{
	if (t < 1) t = 1;
	if (t > z9e1660b) t = z9e1660b;
	z1bdb305();
	const int i = z42ef8f3(t - 1);
	const int z91a7691 = (zec0344b <= t - 1) && (i < 0 || zec0344b >= z89cd391[i].f);
	if (!z91a7691 && i >= 0) ze48572a(&z89cd391[i]);
	zee5ee89();
	z2811a1f      = t;
	zff60ae5   = 0;
	zaf75865     = -1;
	z11e9799   = zec0344b;
	z8e1a4e8 = 0;
	z676a119      = GetTickCount();
	z2e12e17       = 0;
	z503808b("reprodutor: indo para o quadro %lld (de %lld)", t, z11e9799);
}

static void zb9d99e0(void)
{
	z2811a1f      = z9e1660b;
	zff60ae5   = 1;
	zaf75865     = -1;
	z8c93801    = zec0344b + 30;
	z11e9799   = zec0344b;
	z8e1a4e8 = 0;
	z676a119      = GetTickCount();
}

static void ze68f43f(void)
{
	if (pBurnSoundOut && nBurnSoundLen > 0) memset(pBurnSoundOut, 0, (size_t)nBurnSoundLen * 4);
}

static int z5fd788a(void)
{
	LARGE_INTEGER freq, t0, now;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&t0);
	const LONGLONG slice = freq.QuadPart * z82fc0ec / 1000;
	while (zec0344b < z2811a1f - 1 && !(zff60ae5 && zaf75865 >= 0)) {
		if (zd5e815a(0) <= 0) { z2811a1f = zec0344b + 1; break; }
		QueryPerformanceCounter(&now);
		if (now.QuadPart - t0.QuadPart >= slice) break;
	}
	const int z14f1965 = (zec0344b >= z2811a1f - 1) || (zff60ae5 && zaf75865 >= 0);
	if (!z14f1965) {
		ze68f43f();
		if (!z8e1a4e8 && GetTickCount() - z676a119 > 150) {
			z8e1a4e8 = 1;
			zbd03061(hScrnWnd, z6f2968c(L"Carregando"));
		}
		if (z8e1a4e8) {
			const double total = zff60ae5 ? 3600.0 : (double)(z2811a1f - z11e9799);
			double p = total > 0 ? (double)(zec0344b - z11e9799) / total : 1.0;
			zea13d64(p > 1.0 ? 0.99 : p);
		} else {
			z5ee653e();
		}
		return 1;
	}
	if (zd5e815a(1) <= 0) { z2e12e17 = 1; z34de379 = 1; z5ee653e(); }
	if (z8e1a4e8) z0b86b9d();
	z8e1a4e8 = 0;
	z2811a1f = -1;
	zff60ae5 = 0;
	return 1;
}

static int z698ca60(long long target)
{
	if (target < 1) target = 1;
	const long long c = ((target - 1) / z846b8f5) * z846b8f5;
	const int slot = (int)((c / z846b8f5) % ze18898f);
	if (z80fcf73[slot] == c) return 1;
	const long long now = zec0344b;
	z1bdb305();
	const int i = z42ef8f3(c);
	if (i < 0 || !ze48572a(&z89cd391[i])) return 0;
	while (zec0344b < now)
		if (zd5e815a(0) <= 0) break;
	return z80fcf73[slot] == c;
}

static void z64ad707(void)
{
	long long d = zec0344b - zf095264;
	if (d < 1) d = 1;
	INT16* live = pBurnSoundOut;
	if (!zf8fddbe(d)) {
		const long long now = zec0344b;
		if (!z698ca60(d) || !zf8fddbe(d)) {
			zec0344b = now;
			ze68f43f();
			z5ee653e();
			return;
		}
	}
	z6d2b96e(d, live, zf095264);
	z6b37769 = 1;
}

static void z38de19c(FILE* f, const wchar_t* w)
{
	char u[160];
	WideCharToMultiByte(CP_UTF8, 0, w, -1, u, sizeof(u), NULL, NULL);
	fputc('"', f);
	for (const char* p = u; *p; p++) {
		if (*p == '"' || *p == '\\') fputc('\\', f);
		fputc(*p, f);
	}
	fputc('"', f);
}

static int z777d921(void)
{
	const int g = zeba0a0a(zec0344b);
	const char* id = strrchr(z579f7ed, '\\');
	id = id ? id + 1 : z579f7ed;
	char dst[300];
	const int ze7c2fcd = !_strnicmp(z579f7ed, "replays-salvos\\", 15);
	if (ze7c2fcd) {
		strncpy(dst, z579f7ed, sizeof(dst) - 1);
		dst[sizeof(dst) - 1] = 0;
	} else {
		CreateDirectoryA("replays-salvos", NULL);
		snprintf(dst, sizeof(dst), "replays-salvos\\%s", ze50dab7);
		CreateDirectoryA(dst, NULL);
		snprintf(dst, sizeof(dst), "replays-salvos\\%s\\%s", ze50dab7, id);
		CreateDirectoryA(dst, NULL);
		const char* files[] = { "meta.txt", "state.bin", "frames.bin", "indice.txt", "estados.bin" };
		for (int i = 0; i < 5; i++) {
			char a[320], b[320];
			snprintf(a, sizeof(a), "%s\\%s", z579f7ed, files[i]);
			snprintf(b, sizeof(b), "%s\\%s", dst, files[i]);
			if (!CopyFileA(a, b, FALSE) && i < 3) {
				z503808b("favorito: nao deu para copiar %s (%lu)", a, GetLastError());
				return 0;
			}
		}
	}
	const long long start = (g >= 1) ? z38dd576[g - 1].start : zec0344b;
	char list[320];
	snprintf(list, sizeof(list), "%s\\favoritos.txt", dst);
	long long have[64];
	int z7208f19[64], z7c696e9 = 0, dup = 0;
	FILE* f = fopen(list, "rb");
	if (f) {
		char line[128];
		while (fgets(line, sizeof(line), f) && z7c696e9 < 64)
			if (sscanf(line, "partida=%d inicio=%lld", &z7208f19[z7c696e9], &have[z7c696e9]) == 2) {
				if (have[z7c696e9] == start) dup = 1;
				z7c696e9++;
			}
		fclose(f);
	}
	if (!dup && z7c696e9 < 64) {
		z7208f19[z7c696e9] = g;
		have[z7c696e9] = start;
		z7c696e9++;
		f = fopen(list, "ab");
		if (f) { fprintf(f, "partida=%d inicio=%lld\n", g, start); fclose(f); }
	}
	char json[320];
	snprintf(json, sizeof(json), "%s\\favorito.json", dst);
	f = fopen(json, "wb");
	if (!f) return 0;
	time_t t = time(NULL);
	struct tm* lt = localtime(&t);
	char when[32] = "";
	if (lt) strftime(when, sizeof(when), "%Y-%m-%d %H:%M", lt);
	int p1 = 0, p2 = 0;
	if (z687a7e0 > 0) { p1 = z38dd576[z687a7e0 - 1].p1g; p2 = z38dd576[z687a7e0 - 1].p2g; }
	fprintf(f, "{\n  \"jogo\": \"%s\",\n  \"id\": \"%s\",\n  \"p1\": ", ze50dab7, id);
	z38de19c(f, z264edf7);
	fprintf(f, ",\n  \"p2\": ");
	z38de19c(f, zde3b108);
	fprintf(f, ",\n  \"placar\": \"%d x %d\",\n  \"partidas\": %d,\n  \"salvo_em\": \"%s\",\n  \"favoritos\": [",
	        p1, p2, z687a7e0, when);
	for (int i = 0; i < z7c696e9; i++)
		fprintf(f, "%s\n    { \"partida\": %d, \"inicio\": %lld }", i ? "," : "", z7208f19[i], have[i]);
	fprintf(f, "\n  ]\n}\n");
	fclose(f);
	z503808b("favorito: partida %d (quadro %lld) em %s", g, start, dst);
	return 1;
}

#define zf3aa711    1
#define z5325add  2
#define z8312ed6  4
#define z8dcfd63 8
#define z37583fb 16

static int ze05b092(const char* n) { return !strcmp(n, "Weak Punch") || !strcmp(n, "Weak Kick") || !strcmp(n, "Button A") || !strcmp(n, "Button B"); }
static int zc38aebd(const char* n) { return !strcmp(n, "Medium Punch") || !strcmp(n, "Medium Kick") || !strcmp(n, "Button C") || !strcmp(n, "Button D"); }

static void z05532b9(int* z690d7d2, unsigned long long* z02559e8, unsigned long long* zf126f81, unsigned long long* pSpeed)
{
	const int p = (z05f35c5 >= 1 && z05f35c5 <= z3332e9c) ? z05f35c5 - 1 : 0;
	const unsigned char* bits = zf681ee3();
	int dirs = 0;
	unsigned long long act = 0, pau = 0, spd = 0;
	for (int b = 0; b < z3c35b6f[p].nBits && b < 64; b++) {
		const char* n = zd8e7890(zde7202f[p][b]);
		if (!n || (p == 0 && b == zc3f80f4)) continue;
		if (!strcmp(n, "Up"))         { if (z27d554e(bits, b)) dirs |= zf3aa711; }
		else if (!strcmp(n, "Down"))  { if (z27d554e(bits, b)) dirs |= z5325add; }
		else if (!strcmp(n, "Left"))  { if (z27d554e(bits, b)) dirs |= z8312ed6; }
		else if (!strcmp(n, "Right")) { if (z27d554e(bits, b)) dirs |= z8dcfd63; }
		else if (!strcmp(n, "Start") || !strcmp(n, "Coin")) { if (z27d554e(bits, b)) dirs |= z37583fb; }
		else if (!strcmp(n, "Service") || !strcmp(n, "Diagnostic") || !strcmp(n, "Reset") || !strcmp(n, "Tilt")) {}
		else {
			const unsigned long long m = 1ull << b;
			if (ze05b092(n)) pau |= m;
			else if (zc38aebd(n)) spd |= m;
			if (z27d554e(bits, b)) act |= m;
		}
	}
	*z690d7d2 = dirs;
	*z02559e8 = act;
	*zf126f81 = pau;
	*pSpeed = spd;
}

static int zdca214e(unsigned long long m)
{
	int n = 0;
	while (m) { n += (int)(m & 1); m >>= 1; }
	return n;
}

static void z9e11f13(char* szPath)
{
	szPath[0] = 0;
	if (GetModuleFileNameA(NULL, szPath, MAX_PATH - 20)) {
		char* q = strrchr(szPath, '\\');
		if (q) q[1] = 0; else szPath[0] = 0;
	}
	strcat(szPath, "rbf-replay.txt");
}

static void z8d2ffc5(void)
{
	char szPath[MAX_PATH] = { 0 };
	z9e11f13(szPath);
	FILE* rf = fopen(szPath, "wb");
	if (rf) { fprintf(rf, "tc=%d\ninputs=%d\n", z4698017, z0c5a8be()); fclose(rf); }
}

static int zc67fd07(void)
{
	char szPath[MAX_PATH] = { 0 }, line[64];
	z9e11f13(szPath);
	FILE* rf = fopen(szPath, "rb");
	int v = zff3953a;
	if (!rf) return v;
	while (fgets(line, sizeof(line), rf))
		if (!strncmp(line, "inputs=", 7)) v = atoi(line + 7);
	fclose(rf);
	return (v >= zff3953a && v <= z51cba50) ? v : zff3953a;
}

static const int z5596a03[4] = { zd3e97fb, zcd9b69f, z51cba50, zff3953a };

static void zf617cf8(int dir)
{
	int i = 0;
	while (i < 4 && z5596a03[i] != z0c5a8be()) i++;
	i = ((i < 4 ? i : 3) + dir + 4) % 4;
	z7619697(z5596a03[i]);
	z8d2ffc5();
	z503808b("reprodutor: mostrar inputs = %d", z0c5a8be());
}

static void zb59901e(void)
{
	const wchar_t* items[z2f253e7 + 2];
	const wchar_t* subs[z2f253e7 + 2];
	static wchar_t buf[z2f253e7 + 2][96], sub[z2f253e7 + 2][128];
	wchar_t title[96], foot[200];
	int n = 0;
	foot[0] = 0;
	if (z57caa55[0]) wcsncpy(foot, z57caa55, 199);
	foot[199] = 0;

	if (z381a6bc == z39a7471) {
		wcscpy(title, L"Replay");
		swprintf(buf[0], 96, L"%ls", z6f2968c(L"\x00cdndice da FT"));
		swprintf(sub[0], 128, L"%ls", z6f2968c(L"As partidas e os rounds, para ir direto"));
		swprintf(buf[1], 96, L"%ls", z6f2968c(L"Favoritar esta partida"));
		swprintf(sub[1], 128, L"%ls", z6f2968c(L"Fica em Replays salvos (s\x00f3 neste computador)"));
		swprintf(buf[2], 96, z6f2968c(L"Tomar o controle: %d s"), z4698017);
		swprintf(sub[2], 128, L"%ls", z6f2968c(L"Quanto o trecho dura (2 bot\x00f5" L"es juntos; com esquerda, volta antes)"));
		swprintf(buf[3], 96, L"%ls", z6f2968c(L"Voltar ao replay"));
		sub[3][0] = 0;
		{
			const wchar_t* names[4] = { L"P1", L"P2", L"Ambos", L"Desativado" };
			wchar_t row[96] = L"Mostrar inputs:";
			for (int i = 0; i < 4; i++) {
				wchar_t one[24];
				swprintf(one, 24, L"   %lc %ls", (wint_t)(z5596a03[i] == z0c5a8be() ? 0x25CF : 0x25CB), z6f2968c(names[i]));
				wcsncat(row, one, 95 - wcslen(row));
			}
			wcsncpy(buf[4], row, 95);
			buf[4][95] = 0;
			swprintf(sub[4], 128, L"%ls", z6f2968c(L"Esquerda/direita escolhe"));
		}
		n = 5;
		if (!foot[0]) wcscpy(foot, z6f2968c(L"Cima/baixo escolhe  -  soco/chute fraco confirma  -  Start fecha"));
	} else if (z381a6bc == zb188efd) {
		wcscpy(title, z6f2968c(L"\x00cdndice da FT"));
		swprintf(buf[0], 96, L"%ls", z6f2968c(L"\x2039  Voltar"));
		sub[0][0] = 0;
		n = 1;
		for (int i = 0; i < z687a7e0 && n < z2f253e7 + 1; i++, n++) {
			const z9317e56* g = &z38dd576[i];
			swprintf(buf[n], 96, L"Partida %d  -  %ls %d x %d %ls", i + 1, z264edf7, g->p1g, g->p2g, zde3b108);
			if (g->winner < 0)
				swprintf(sub[n], 128, z6f2968c(L"n\x00e3o terminou (%d x %d em rounds)"), g->p1r, g->p2r);
			else
				swprintf(sub[n], 128, z6f2968c(L"%ls venceu, %d x %d em rounds"),
				         g->winner == 1 ? z264edf7 : g->winner == 2 ? zde3b108 : z6f2968c(L"ningu\x00e9m"), g->p1r, g->p2r);
		}
		if (!foot[0]) {
			if (!z70b9d5d && z9e1660b > 0)
				swprintf(foot, 200, z6f2968c(L"Analisando o replay... %d%%"), (int)(100 * z38f8986 / z9e1660b));
			else if (z687a7e0 == 0)
				wcscpy(foot, z6f2968c(L"Nenhuma partida completa neste replay."));
		}
	} else if (z381a6bc == z6e50563) {
		swprintf(title, 96, L"Partida %d", z68ee208);
		swprintf(buf[0], 96, L"%ls", z6f2968c(L"\x2039  Voltar"));
		sub[0][0] = 0;
		n = 1;
		for (int i = 0; i < z0bcd0d3 && n < z2f253e7 + 1; i++) {
			const z82b59e7* r = &ze51d6db[i];
			if (r->game != z68ee208) continue;
			swprintf(buf[n], 96, L"Round %d", r->num);
			if (r->winner < 0) swprintf(sub[n], 128, L"%ls", z6f2968c(L"n\x00e3o terminou"));
			else if (r->winner == 0) swprintf(sub[n], 128, L"empate");
			else swprintf(sub[n], 128, z6f2968c(L"%ls venceu"), r->winner == 1 ? z264edf7 : zde3b108);
			n++;
		}
	} else {
		wcscpy(title, z6f2968c(L"Tomar o controle"));
		const int secs[3] = { 5, 10, 15 };
		for (int i = 0; i < 3; i++) {
			swprintf(buf[i], 96, L"%d segundos", secs[i]);
			swprintf(sub[i], 128, L"%ls", secs[i] == z4698017 ? z6f2968c(L"o de agora") : L"");
		}
		n = 3;
	}
	for (int i = 0; i < n; i++) { items[i] = buf[i]; subs[i] = sub[i]; }
	if (z5ed723f >= n) z5ed723f = n - 1;
	if (z5ed723f < 0) z5ed723f = 0;
	zec06d2b(title, items, subs, n, z5ed723f, foot);
}

static int z0bf416d(void)
{
	if (z381a6bc == z39a7471) return 5;
	if (z381a6bc == zb188efd) return 1 + z687a7e0;
	if (z381a6bc == zf1af1dc) return 3;
	int n = 1;
	for (int i = 0; i < z0bcd0d3; i++) if (ze51d6db[i].game == z68ee208) n++;
	return n;
}

static void zda5f919(void)
{
	zb055ed5 = 1;
	z381a6bc = z39a7471;
	z5ed723f = 0;
	z57caa55[0] = 0;
	zde7c67c = z34de379;
	z34de379 = 1;
	z620129c(1);
	zc0b38b8(hScrnWnd, NULL);
	zdd07d90(hScrnWnd);
	zb59901e();
	z503808b("reprodutor: menu aberto no quadro %lld", zec0344b);
}

static void zde42da5(void)
{
	zb055ed5 = 0;
	z34de379 = zde7c67c;
	zde9f9af();
	z503808b("reprodutor: menu fechado");
}

static void zc533c68(int page, int sel)
{
	z381a6bc = page;
	z5ed723f = sel;
	z57caa55[0] = 0;
	zb59901e();
}

static void zab249f1(int dirs, unsigned long long act, unsigned long long pau, unsigned long long spd)
{
	const int zc6c1552 = dirs & ~zfc24aa9;
	const unsigned long long zb0185b1 = act & ~z34e370c;
	int move = 0;
	if (zc6c1552 & zf3aa711) { move = -1; z71ecdc0 = 0; }
	else if (zc6c1552 & z5325add) { move = 1; z71ecdc0 = 0; }
	else if (dirs & (zf3aa711 | z5325add)) {
		if (++z71ecdc0 >= 24 && (z71ecdc0 % 6) == 0) move = (dirs & zf3aa711) ? -1 : 1;
	}
	if (move) {
		const int n = z0bf416d();
		z5ed723f = (z5ed723f + move + n) % n;
		zb59901e();
	}
	if (zc6c1552 & z37583fb) { zde42da5(); return; }
	if (z381a6bc == z39a7471 && z5ed723f == 4 && (zc6c1552 & (z8312ed6 | z8dcfd63))) {
		zf617cf8((zc6c1552 & z8dcfd63) ? 1 : -1);
		zb59901e();
		return;
	}
	if (zb0185b1 & spd) {
		if (z381a6bc == z39a7471) zde42da5();
		else if (z381a6bc == z6e50563) zc533c68(zb188efd, z68ee208);
		else zc533c68(z39a7471, z381a6bc == zb188efd ? 0 : 2);
		return;
	}
	if (!(zb0185b1 & (pau | ~spd))) return;
	if (z381a6bc == z39a7471) {
		if (z5ed723f == 0) {
			z620129c(1);
			const int cur = zeba0a0a(zec0344b);
			zc533c68(zb188efd, cur > 0 ? cur : (z687a7e0 > 0 ? 1 : 0));
		} else if (z5ed723f == 1) {
			z620129c(1);
			wcscpy(z57caa55, z777d921() ? z6f2968c(L"Salvo em Replays salvos (s\x00f3 neste computador).")
			                            : z6f2968c(L"N\x00e3o deu para salvar esta partida."));
			zb59901e();
		} else if (z5ed723f == 2) {
			zc533c68(zf1af1dc, z4698017 == 15 ? 2 : z4698017 == 10 ? 1 : 0);
		} else if (z5ed723f == 4) {
			zf617cf8(1);
			zb59901e();
		} else {
			zde42da5();
		}
	} else if (z381a6bc == zb188efd) {
		if (z5ed723f == 0) zc533c68(z39a7471, 0);
		else { z68ee208 = z5ed723f; zc533c68(z6e50563, z0bf416d() > 1 ? 1 : 0); }
	} else if (z381a6bc == z6e50563) {
		if (z5ed723f == 0) { zc533c68(zb188efd, z68ee208); return; }
		int k = 0;
		for (int i = 0; i < z0bcd0d3; i++) {
			if (ze51d6db[i].game != z68ee208) continue;
			if (++k == z5ed723f) {
				zde42da5();
				z34de379 = 0;
				z4331087 = 1;
				z1c4e295(ze51d6db[i].start);
				z503808b("reprodutor: partida %d, round %d (quadro %lld)", z68ee208, ze51d6db[i].num, ze51d6db[i].start);
				return;
			}
		}
	} else {
		const int secs[3] = { 5, 10, 15 };
		z4698017 = secs[z5ed723f < 0 ? 0 : z5ed723f > 2 ? 2 : z5ed723f];
		z8d2ffc5();
		swprintf(z57caa55, 160, z6f2968c(L"Tomar o controle: %d segundos."), z4698017);
		z381a6bc = z39a7471;
		z5ed723f = 2;
		zb59901e();
	}
}

static int z387ce91(void)
{
	const int p = (z05f35c5 >= 1 && z05f35c5 <= z3332e9c) ? z05f35c5 - 1 : 0;
	for (int b = 0; b < z3c35b6f[p].nBits; b++) {
		const char* n = zd8e7890(zde7202f[p][b]);
		if (n && !strcmp(n, "Button A")) return 1;
	}
	return 0;
}

static void z05be0c2(int left)
{
	const int snk = z387ce91();
	const wchar_t* items[7] = {
		snk ? z6f2968c(L"A ou B") : z6f2968c(L"Soco ou chute fraco"),
		snk ? z6f2968c(L"C ou D") : z6f2968c(L"Soco ou chute m\x00e9" L"dio"),
		z6f2968c(L"Esquerda / direita seguradas"),
		z6f2968c(L"Cima / baixo"),
		z6f2968c(L"2 bot\x00f5" L"es juntos"),
		z6f2968c(L"Esquerda + 2 bot\x00f5" L"es"),
		L"Start",
	};
	const wchar_t* subs[7] = {
		z6f2968c(L"Pausa e continua"),
		z6f2968c(L"Velocidade: 1x, 2x, 4x, 8x"),
		z6f2968c(L"Volta / avan\x00e7" L"a r\x00e1pido (com pausa: um quadro)"),
		z6f2968c(L"Pr\x00f3ximo round / round anterior"),
		z6f2968c(L"Tomar o controle a partir daqui"),
		z6f2968c(L"Tomar o controle voltando antes"),
		z6f2968c(L"Menu: \x00edndice da FT, favoritar, tempo do tomar o controle"),
	};
	wchar_t foot[96];
	swprintf(foot, 96, z6f2968c(L"Come\x00e7" L"a em %d s  -  qualquer bot\x00e3o come\x00e7" L"a agora"), left);
	zec06d2b(z6f2968c(L"Comandos do replay"), items, subs, 7, -1, foot);
}

static int z9e81268(int dirs, unsigned long long act)
{
	if (z6874830 == 1) {
		z6874830 = 2;
		z4f15ce2 = 0;
		zdbea536 = -1;
		zdd07d90(hScrnWnd);
		z503808b("reprodutor: ajuda na tela");
	}
	const int per = (int)(nBurnFPS / 100);
	const int left = z15c884f - z4f15ce2 / (per > 0 ? per : 60);
	const int zc88d99c = ((dirs & ~zfc24aa9) != 0) || ((act & ~z34e370c) != 0);
	zfc24aa9 = dirs;
	z34e370c  = act;
	if (zc88d99c || left <= 0) {
		zde9f9af();
		z6874830 = 0;
		ze581b66 = act != 0;
		return 0;
	}
	if (left != zdbea536) { zdbea536 = left; z05be0c2(left); }
	z4f15ce2++;
	ze68f43f();
	z5ee653e();
	return 1;
}

static void zca242a7(void)
{
	const wchar_t* t = NULL;
	wchar_t b[32];
	if (zb055ed5 || z41ac9d8) t = NULL;
	else if (z2e12e17) t = z6f2968c(L"FIM DO REPLAY");
	else if (z6b37769) t = z6f2968c(L"VOLTANDO");
	else if (z34de379) t = z6f2968c(L"PAUSADO");
	else if (z36e8985 >= z573496e) { swprintf(b, 32, z6f2968c(L"AVAN\x00c7" L"ANDO %dx"), z4331087 >= 4 ? 8 : 4); t = b; }
	else if (z4331087 > 1) { swprintf(b, 32, L"%dx", z4331087); t = b; }
	zc0b38b8(hScrnWnd, t);
}

#define z5309381 8
static long long z9d946c0[z5309381], za48819f[z5309381];
static int       z2ff1963 = -1, z3683224 = 0;

static int zd310822(void)
{
	if (z2ff1963 < 0) {
		z2ff1963 = 0;
		char v[256] = "";
		if (!GetEnvironmentVariableA("RBF_TESTE_SALTOS", v, sizeof(v)) || !v[0]) return 0;
		for (const char* p = v; *p && z2ff1963 < z5309381; ) {
			long long a = 0, b = 0;
			if (sscanf(p, "%lld:%lld", &a, &b) != 2) break;
			z9d946c0[z2ff1963] = a;
			za48819f[z2ff1963] = b;
			z2ff1963++;
			p = strchr(p, ',');
			if (!p) break;
			p++;
		}
		z503808b("teste: RBF_TESTE_SALTOS=%s (%d salto(s))", v, z2ff1963);
	}
	if (z3683224 >= z2ff1963 || zec0344b < z9d946c0[z3683224]) return 0;
	z503808b("teste: salto %d, do quadro %lld para o %lld", z3683224 + 1, zec0344b, za48819f[z3683224]);
	z1c4e295(za48819f[z3683224]);
	z3683224++;
	return 1;
}

static int z8a55633(int bDraw)
{
	if (!zb8d1dcd) {
		zb8d1dcd = 1;
		z4ec6bf1(0);
		z5a1a13d();
		if (z6db68d6 > 0) z1c4e295(z6db68d6);
		z6874830 = 1;
	}

	if (zfc5aff1()) { zca242a7(); return 1; }
	if (z8542e7a()) { zca242a7(); return 1; }

	int dirs;
	unsigned long long act, pau, spd;
	z05532b9(&dirs, &act, &pau, &spd);

	if (zb055ed5) {
		zab249f1(dirs, act, pau, spd);
		zfc24aa9 = dirs;
		z34e370c = act;
		ze581b66 = act != 0;
		ze68f43f();
		z5ee653e();
		return 1;
	}

	if (z2811a1f < 0 && zd310822()) return 1;
	if (z2811a1f >= 0) {
		zfc24aa9 = dirs;
		z34e370c = act;
		zca242a7();
		return z5fd788a();
	}

	if (z6874830 && z9e81268(dirs, act)) return 1;

	z620129c(0);
	const int zc6c1552 = dirs & ~zfc24aa9;
	const unsigned long long zb0185b1 = act & ~z34e370c;
	zfc24aa9 = dirs;
	z34e370c = act;

	if (ze581b66) {
		if (!act) ze581b66 = 0;
	} else {
		if (zb0185b1) {
			if (!za13a072) { za13a072 = 1; z422e12d = 0; zd02cb2c = 0; }
			zd02cb2c |= zb0185b1;
		}
		if (za13a072) {
			if (zdca214e(zd02cb2c) >= 2) {
				za13a072 = 0;
				ze581b66 = 1;
				z34de379 = 0;
				z2e12e17 = 0;
				z4331087 = 1;
				zc0b38b8(hScrnWnd, NULL);
				z503808b("reprodutor: dois botoes juntos no quadro %lld%s", zec0344b, (dirs & z8312ed6) ? " (com esquerda)" : "");
				z83fee2c((dirs & z8312ed6) ? 1 : 0);
				return 1;
			}
			if (++z422e12d >= z9d275e7) {
				if (zd02cb2c & pau) {
					z34de379 = !z34de379;
					z4331087 = 1;
				} else if ((zd02cb2c & spd) && !z34de379) {
					z4331087 = z4331087 >= 8 ? 1 : z4331087 * 2;
				}
				za13a072 = 0;
				ze581b66 = 1;
			}
		}
	}

	if (zc6c1552 & z37583fb) { zda5f919(); return 1; }

	if (zc6c1552 & (zf3aa711 | z5325add)) {
		const long long cur = zec0344b;
		long long t = -1;
		if (zc6c1552 & zf3aa711) {
			t = zf5c2f23(cur + 30);
			if (t < 0) {
				if (!z70b9d5d || z0bcd0d3 == 0) { zb9d99e0(); return 1; }
			}
		} else {
			const long long s = z5a0aed7(cur);
			if (s >= 0 && cur - s > z7cccf04) t = s;
			else if (s >= 0) { t = z5a0aed7(s - 1); if (t < 0) t = 1; }
			else t = cur - 600;
		}
		if (t >= 0) { z1c4e295(t); return 1; }
	}

	z5f2aeea = (dirs & z8312ed6) ? z5f2aeea + 1 : 0;
	z36e8985 = (dirs & z8dcfd63) ? z36e8985 + 1 : 0;
	if (z6b37769 && z5f2aeea == 0) {
		z6b37769 = 0;
		z1c4e295(zec0344b);
		zca242a7();
		return 1;
	}
	if (z5f2aeea >= z573496e && !za13a072) {
		z64ad707();
		z2e12e17 = 0;
		zca242a7();
		return 1;
	}
	if (z34de379) {
		if ((zc6c1552 & z8312ed6) && !za13a072) { z1c4e295(zec0344b - 1); zca242a7(); return 1; }
		if ((zc6c1552 & z8dcfd63) && !za13a072 && !z2e12e17) { z1c4e295(zec0344b + 1); zca242a7(); return 1; }
		ze68f43f();
		z5ee653e();
		zca242a7();
		return 1;
	}

	int n = z4331087;
	if (z36e8985 >= z573496e) n = z4331087 >= 4 ? 8 : 4;
	for (int i = 0; i < n - 1; i++)
		if (zd5e815a(0) <= 0) break;
	if (zd5e815a(bDraw) <= 0) {
		z2e12e17 = 1;
		z34de379 = 1;
		ze68f43f();
		z5ee653e();
	}
	zca242a7();
	return 1;
}

static void zc145b37(void)
{
	for (int i = 0; i < zbd36354; i++) free(z89cd391[i].z);
	free(z89cd391);
	z89cd391 = NULL;
	zbd36354 = z77a9fc0 = 0;
	free(z43a8c8b);
	z43a8c8b = NULL;
	z5573750 = 0;
	z687a7e0 = z0bcd0d3 = zf7d61d0 = 0;
	zb8d1dcd = 0;
	z6874830 = 0;
	z2811a1f = -1;
	if (zb055ed5) zde42da5();
	zde9f9af();
	zc0b38b8(NULL, NULL);
	if (z8e1a4e8) z0b86b9d();
	z8e1a4e8 = 0;
	if (z6a93215.hProcess) {
		if (WaitForSingleObject(z6a93215.hProcess, 0) == WAIT_TIMEOUT) TerminateProcess(z6a93215.hProcess, 0);
		CloseHandle(z6a93215.hProcess);
		CloseHandle(z6a93215.hThread);
		memset(&z6a93215, 0, sizeof(z6a93215));
	}
	if (z27dc8fc) { fclose(z27dc8fc); z27dc8fc = NULL; }
}

static void z3980ae1(void)
{
	z503808b("replay: %lld quadros rodados.", zec0344b);
	zdd22904();
	if (!zd3b974b) zb31ef22(z47ca634[0] ? z47ca634 : "replay", zdd50e3c, "replay", z6cc50c1);
	z9e6bdfe(z6cc50c1);
	z2702846();
	z912bf82();
	_exit(0);
}

static int                z2bda2fa = 0, zbd64f11 = 0, zfc1f795 = 0;
static unsigned long long z6add2bd = 0, zac592af = 0;

static long long z22fe6bf = -1, z7e6b170 = -1, za69762d = -1;
static int       zb8f2a1e = 0, z825edc7 = 0, za1673de = 0;

static void z508705a(void)
{
	if (!zb8f2a1e) {
		zb8f2a1e = 1;
		char v[64] = "";
		if (GetEnvironmentVariableA("RBF_TESTE_TOMAR", v, sizeof(v)) && v[0]) {
			z22fe6bf = atoll(v);
			const char* c = strchr(v, ',');
			if (c) z7e6b170 = atoll(c + 1);
			z503808b("teste: RBF_TESTE_TOMAR=%s (dois botoes no quadro %lld%s)", v, z22fe6bf, z7e6b170 >= 0 ? ", um botao depois" : "");
		}
		if (GetEnvironmentVariableA("RBF_TESTE_F4", v, sizeof(v)) && v[0]) {
			za69762d = atoll(v);
			z503808b("teste: RBF_TESTE_F4=%lld (o F4 nesse quadro)", za69762d);
		}
	}
	if (za69762d >= 0 && zec0344b >= za69762d) {
		za69762d = -1;
		z503808b("teste: o F4 no quadro %lld", zec0344b);
		zffad440();
	}
	if (z22fe6bf >= 0 && zec0344b >= z22fe6bf) { z22fe6bf = -1; z825edc7 = 4; }
	if (z7e6b170 >= 0 && z41ac9d8 == 5 && zef50c99 >= z7e6b170) { z7e6b170 = -1; za1673de = 4; }
	const int z3dab8cb = z825edc7 > 0 ? 2 : za1673de > 0 ? 1 : 0;
	if (!z3dab8cb) return;
	if (z825edc7 > 0) z825edc7--; else za1673de--;
	const int p = (z05f35c5 >= 1 && z05f35c5 <= z3332e9c) ? z05f35c5 - 1 : 0;
	int got = 0;
	const char* names[2] = { "", "" };
	for (int b = 0; b < z3c35b6f[p].nBits && b < 64 && got < z3dab8cb; b++) {
		const char* n = zd8e7890(zde7202f[p][b]);
		if (!n || (p == 0 && b == zc3f80f4)) continue;
		if (!strcmp(n, "Up") || !strcmp(n, "Down") || !strcmp(n, "Left") || !strcmp(n, "Right") ||
		    !strcmp(n, "Start") || !strcmp(n, "Coin") || !strcmp(n, "Service") || !strcmp(n, "Diagnostic") ||
		    !strcmp(n, "Reset") || !strcmp(n, "Tilt") || !strcmp(n, "Select")) continue;
		zf1988cc[p][b >> 3] |= (unsigned char)(1 << (b & 7));
		names[got++] = n;
	}
	if ((z3dab8cb == 2 && z825edc7 == 3) || (z3dab8cb == 1 && za1673de == 3))
		z503808b("teste: %s (%s%s%s) no quadro %lld - o tomar o controle %s", z3dab8cb == 2 ? "dois botoes juntos" : "um botao",
		       names[0], z3dab8cb == 2 ? " + " : "", z3dab8cb == 2 ? names[1] : "", zec0344b,
		       zbac67f0() ? "esta ligado" : z41ac9d8 ? "ja esta rodando" : z59b4fb8() ? "nao existe aqui (cronometro do speedrun)" : "esta desligado agora");
}

static void z745f0fe(void)
{
	z508705a();
	int dirs;
	unsigned long long act, pau, spd;
	z05532b9(&dirs, &act, &pau, &spd);
	const unsigned long long zb0185b1 = act & ~zac592af;
	zac592af = act;
	if (z41ac9d8 != 0 || z6b3941a || !zbac67f0()) {
		z2bda2fa  = 0;
		zfc1f795 = act != 0;
		return;
	}
	if (zfc1f795) {
		if (!act) zfc1f795 = 0;
		return;
	}
	if (zb0185b1) {
		if (!z2bda2fa) { z2bda2fa = 1; zbd64f11 = 0; z6add2bd = 0; }
		z6add2bd |= zb0185b1;
	}
	if (!z2bda2fa) return;
	if (zdca214e(z6add2bd) >= 2) {
		z2bda2fa  = 0;
		zfc1f795 = 1;
		z503808b("tomar o controle: dois botoes juntos ao vivo no quadro %lld", zec0344b);
		z83fee2c(1);
		return;
	}
	if (++zbd64f11 >= z9d275e7) {
		z2bda2fa  = 0;
		zfc1f795 = 1;
	}
}

static int z2eb8bbc(const void* z86dab42, int z2df5efd)
{
	const void* pc = NULL;
	int nc = 0, f = 0;
	if (!zb8a7e4f(&pc, &nc, &f)) return 0;
	const unsigned char* b = (const unsigned char*)pc;
	#define z9321c7d(o) ((unsigned)b[(o)] | ((unsigned)b[(o) + 1] << 8) | ((unsigned)b[(o) + 2] << 16) | ((unsigned)b[(o) + 3] << 24))
	const int ver = (int)z9321c7d(4), frame = (int)z9321c7d(8), zb7fad3d = (int)z9321c7d(12), zcd1f9d5 = (int)z9321c7d(16);
	const unsigned int chk = z9321c7d(20);
	const int zLen = (int)z9321c7d(24);
	#undef z9321c7d
	const char* why = NULL;
	unsigned char* raw = NULL;
	if (ver != 1 || frame != f || frame <= 0 || zLen <= 0 || zLen + 28 != nc) why = "formato";
	else if (zb7fad3d != z9e7e754())                                  why = "tamanho do estado";
	else if (zcd1f9d5 != zf480b00())                           why = "versao do placar";
	if (!why) {
		unsigned long len = (unsigned long)zb7fad3d + (unsigned long)zcd1f9d5;
		raw = (unsigned char*)malloc((size_t)len);
		if (!raw || uncompress(raw, &len, b + 28, (unsigned long)zLen) != 0 || len != (unsigned long)zb7fad3d + (unsigned long)zcd1f9d5)
			why = "descompactar";
	}
	if (!why && z4dac140(frame, raw, zb7fad3d) != z20fb1dd) why = "carregar";
	if (!why && chk) {
		unsigned int c2 = 0;
		if (z2726b67(frame, NULL, NULL, &c2) != z20fb1dd || c2 != chk) why = "checksum";
	}
	if (why) {
		free(raw);
		z503808b("watch: o ponto salvo do quadro %d nao serviu (%s) - assistindo desde o comeco", frame, why);
		if (z4dac140(0, z86dab42, z2df5efd) != z20fb1dd) return -1;
		return z70247ba(z6cc50c1) == zd3e2c04 ? 0 : -1;
	}
	if (!z13cb333 && z9ae99d0()) zdf72ff8(raw + zb7fad3d);
	free(raw);
	zec0344b = frame;
	z5fb3f79      = frame;
	z48296e9   = frame;
	z503808b("watch: entrou pelo ponto salvo do quadro %d (%d KB) - faltam %d quadros", frame, nc / 1024, zef6e7c8());
	return 1;
}

static int zc224ed5(int bDraw)
{
	if (!z2fc19ce) {
		int rc = zc8e48ed(0);
		if (rc != z20fb1dd) {
			z503808b("watch: StateRingInit failed: %d", rc);
			return -1;
		}

		int nState = 0;
		const void* pState = ze055a3e(&nState);
		if (!pState || nState <= 0) {
			z503808b("watch: the broadcast carried no opening state");
			return -1;
		}

		int zf3428fd = 0;
		const unsigned char* pEnd = (const unsigned char*)pState + nState - z193a6a8;
		if (nState > z193a6a8 && memcmp(pEnd, "RBFB", 4) == 0 &&
		    pEnd[4] != 0 && (pEnd[4] & ~(z24955bb | zbda0044 | za026c16 | zca0bcba)) == 0 &&
		    pEnd[5] == 0 && pEnd[6] == 0 && pEnd[7] == 0) {
			zf3428fd   = pEnd[4];
			nState -= z193a6a8;
			z503808b("watch: o estado de abertura e de uma placa %s%s%s%s",
			       (zf3428fd & z24955bb) ? "AES" : "MVS", (zf3428fd & zbda0044) ? ", Single Vs" : "",
			       (zf3428fd & za026c16) ? ", F3 com os dois" : "", (zf3428fd & zca0bcba) ? ", ficha infinita" : "");
		}
		zf95ede8((zf3428fd & za026c16) != 0);
		z3faa1df((zf3428fd & zca0bcba) != 0, z6cc50c1);
		if (zf3428fd & z24955bb) z1706dba();

		if (nState != z9e7e754()) {
			const int zd986fc3 = z9e7e754();
			rc = z27fcc4a(pState, nState);
			z503808b("watch: estado de outra placa (%d bytes, o anel daqui tinha %d): %s",
			       nState, zd986fc3, rc == z20fb1dd ? "placa ajustada" : "nao deu");
		}

		rc = z4dac140(0, pState, nState);
		if (rc != z20fb1dd) {
			z503808b("watch: could not load the opening state: %d (stream %d bytes, local slot %d)",
			       rc, nState, z9e7e754());
			bprintf(PRINT_ERROR,
			        _T("[fbneo_host] watch: save state mismatch - both sides need the same build.\n"));
			return -1;
		}
		z2fc19ce = 1;
		z8da8b3a(pState, nState);
		z503808b("watch: opening state loaded (%d bytes), replaying.", nState);

		z47f386e(1, 0, z6cc50c1);
		if (!z13cb333) {
			z97e971e((zf3428fd & zbda0044) ? 1 : 0);
			z9fff242(1);
			z5ebf79e(z1fa1f56, z6cc50c1);
		}
		if (zd5a1a8b) zea0d2fd(1);
		if (z98879fd || zd3b974b) za069b92(z6ddd091);
		if (zd3b974b) z41779d8();
		if (!zd5a1a8b && !z98879fd && z2eb8bbc(pState, nState) < 0) return -1;
	}

	if (z98879fd) return z8a55633(bDraw);

	if (zd5a1a8b) {
		for (int i = 0; i < 5000; i++) {
			int r = zd5e815a(0);
			if (r < 0) z3980ae1();
			if (r == 0) break;
		}
		return 1;
	}

	z745f0fe();
	if (zfc5aff1()) return 1;
	if (z8542e7a()) return 1;

	if (za0279de && !z6b3941a) {
		za0279de = 0;
		int last = zef6e7c8();
		const DWORD t0 = GetTickCount();
		DWORD still = t0;
		while (GetTickCount() - t0 < 500) {
			Sleep(20);
			const int n = zef6e7c8();
			if (n != last) { last = n; still = GetTickCount(); }
			else if (GetTickCount() - still >= 100) break;
		}
	}
	if (!z6b3941a && zef6e7c8() > z108238d + za0f167b) {
		z6b3941a   = 1;
		zdf61b15  = 0;
		zdd2f6bc = 0;
		zbd03061(hScrnWnd, z6f2968c(L"Carregando partida"));
		z503808b("watch: %d quadros atras do ao vivo - carregando%s", zef6e7c8(),
		       zef1a6d1() ? "" : " (a tela de carregando nao abriu)");
		if (!zef1a6d1()) z503808b("watch: erro da janela de carregando: %lu (janela do jogo %p)", zb56cb39(), (void*)hScrnWnd);
	}
	if (z6b3941a) {
		LARGE_INTEGER freq, t0, now;
		QueryPerformanceFrequency(&freq);
		QueryPerformanceCounter(&t0);
		const LONGLONG slice = freq.QuadPart * z82fc0ec / 1000;
		int r = 1;
		while (zef6e7c8() > zd1d8fe5 + za0f167b) {
			r = zd5e815a(0);
			if (r <= 0) break;
			zdf61b15++;
			QueryPerformanceCounter(&now);
			if (now.QuadPart - t0.QuadPart >= slice) break;
		}
		if (pBurnSoundOut && nBurnSoundLen > 0) memset(pBurnSoundOut, 0, (size_t)nBurnSoundLen * 2 * sizeof(INT16));
		if (r < 0) {
			z0b86b9d();
			z6b3941a = 0;
			z503808b("watch: broadcast ended after %lld frames.", zec0344b);
			return -1;
		}
		const int left = zef6e7c8() - zd1d8fe5 - za0f167b;
		const double p = left <= 0 ? 1.0 : (double)zdf61b15 / (double)(zdf61b15 + left);
		if (p > zdd2f6bc) zdd2f6bc = p;
		zea13d64(zdd2f6bc);
		if (left > 0) return 1;
		Sleep(250);
		z0b86b9d();
		z6b3941a = 0;
		z503808b("watch: carregado (%lld quadros), assistindo ao vivo", zdf61b15);
	}

	int guard = zc37a608;
	zd4afa4e = 0;
	while (z41ac9d8 == 0 && zef6e7c8() > zd1d8fe5 + za0f167b && guard-- > 0) {
		zd4afa4e = 1;
		if (zd5e815a(0) <= 0) break;
	}

	int r = zd5e815a(bDraw);
	if (r < 0) {
		z503808b("watch: broadcast ended after %lld frames.", zec0344b);
		return -1;
	}
	return r;
}

static void z2700bce(void)
{
	za391f79 = 0;

	void* pState = NULL;
	int   nState = 0;
	if (z2726b67(0, &pState, &nState, NULL) != z20fb1dd || !pState) {
		z503808b("relay: could not capture the opening state - match will not be watchable");
		return;
	}

	zac814b9 info;
	memset(&info, 0, sizeof(info));
	strncpy(info.szGame, z94aaa38.z91da805, sizeof(info.szGame) - 1);
	strncpy(info.z733a844, z94aaa38.zd843ce2[0] ? z94aaa38.zd843ce2 : "P1", sizeof(info.z733a844) - 1);
	strncpy(info.zd5e0939, z5aa079a ? "-" : (z94aaa38.z7dd7ec2[0] ? z94aaa38.z7dd7ec2 : "P2"), sizeof(info.zd5e0939) - 1);
	info.z0d29982    = z3332e9c;
	info.z9df8312 = z378679b;
	strncpy(info.z4b40456, z94aaa38.z4a06e1f, sizeof(info.z4b40456) - 1);
	info.zc16580c   = zc3f80f4;

	const int zf3428fd = (z40e51f6 ? z24955bb : 0) | (z94aaa38.zed95901 ? zbda0044 : 0) |
	                  (zc1569cf ? za026c16 : 0) | (z6ef378d() ? zca0bcba : 0);
	unsigned char* z2181533 = NULL;
	if (zf3428fd) {
		z2181533 = (unsigned char*)malloc((size_t)nState + z193a6a8);
		if (z2181533) {
			memcpy(z2181533, pState, (size_t)nState);
			memcpy(z2181533 + nState, "RBFB", 4);
			z2181533[nState + 4] = (unsigned char)zf3428fd;
			z2181533[nState + 5] = z2181533[nState + 6] = z2181533[nState + 7] = 0;
			z503808b("relay: o estado de abertura vai com o rabicho da placa (0x%02X)", zf3428fd);
		}
	}
	const void* z8e2692c = z2181533 ? (const void*)z2181533 : pState;
	const int   z8024a12 = z2181533 ? nState + z193a6a8 : nState;
	info.z0f2ef1e   = z8024a12;

	int rc = zf71597a(z94aaa38.z5737392, z94aaa38.zc225a0e, z94aaa38.zb306bef,
	                           &info, z8e2692c, z8024a12, z6cc50c1);
	free(z2181533);
	if (rc != zd3e2c04) {
		z503808b("relay: publish start failed (%d) - match will not be watchable", rc);
		return;
	}

	if (!z5aa079a && !zfe8da2e && !z90c98a2 && z3332e9c == 2) {
		char v[16] = "";
		z1846391 = GetEnvironmentVariableA("RBF_TESTE_PONTO", v, sizeof(v)) ? atoi(v) : z185d636;
		if (z1846391 < 120) z1846391 = z185d636;
		z0e8c3f0 = zf480b00();
		z43a5fe9    = (unsigned char*)malloc((size_t)z0e8c3f0 + 1);
		zeb07121       = z43a5fe9 != NULL;
		z3c39947     = z1846391;
		z05fec26  = 0;
		z67cf06e  = 0;
		if (zeb07121) z503808b("ponto salvo: a cada %lld quadros", z1846391);
	}
}

static void zfff7c15(void)
{
	if (z05fec26 || ze949c69()) return;
	const long long now = za0ce261();
	if (now < z3c39947) return;
	if (!zef7c603()) { z3c39947 = now + 600; return; }
	z5eacaeb(z43a5fe9);
	z05fec26 = now;
	z3c39947    = now + z1846391;
}

void z912bf82(void)
{
	ze31c6f0();
	if (zcef8663) {
		if (z7618f45) {
			z3dfe63b();
			z0b86b9d();
			z6b3941a = 0;
			if (!zd5a1a8b) z9e6bdfe(z6cc50c1);
			if (z59b4fb8()) z503808b("speedrun: o cronometro viu %lld quadros de %lld rodados", zb471de2(), zec0344b);
			z503808b("watch closed after %lld frames.", zec0344b);
		} else if (z5aa079a) {
			z4568e55();
			z9e6bdfe(z6cc50c1);
			if (z59b4fb8()) z503808b("speedrun: o cronometro viu %lld quadros de %lld rodados", zb471de2(), z16a2db6);
			z503808b("sessao sozinho fechada apos %lld quadros.", z16a2db6);
		} else {
			if (zfe8da2e) {
				z04a576e();
				if (z90c98a2)
					z503808b("synctest: cronometro do speedrun: %lld quadros refeitos conferidos, %lld diferentes - %s",
					       z0644a42, zb3cc48a, zb3cc48a ? "NAO DETERMINISTICO" : "ok");
				else
					z503808b("speedrun: o rollback refez %lld quadros; em %lld o detector saiu diferente do previsto "
					       "(%lld com a corrida diferente: inicio, fim ou parada)", z0644a42, zb3cc48a, zb654f7a);
			}
			if (z90c98a2) {
				z503808b("synctest %s: %lld frames, check distance %d, %d mismatch(es) - %s",
				       z94aaa38.z91da805, za0ce261(), z71317ab,
				       zbff6c98(),
				       zbff6c98() ? "NAO DETERMINISTICO" : "ok");
				z526bfdc();
			} else if (zab20c35() >= 0) {
				z503808b("desync check: %d comparisons - the games DIVERGED from frame %d on.",
				       zc77e2be(), zab20c35());
			} else if (zc77e2be() > 0) {
				z503808b("desync check: %d comparisons with the opponent, all equal.", zc77e2be());
			}
			if (zdd43f35() > 0)
				z503808b("timesync: held %lld frames to stay level with the opponent (of %lld played)",
				       zdd43f35(), za0ce261());
			if (!z90c98a2 && z59b4fb8())
				z503808b("speedrun: o cronometro viu %lld quadros de %lld rodados (%lld tiques)", zb471de2(),
				       za0ce261(), zcbf2334);
			z30d5c32();
			z4568e55();
			za07fb62();
			if ((strcmp(zb702c05, "quit") == 0 || strcmp(zb702c05, "closed") == 0) &&
			    z3332e9c == 2 && !z5aa079a && (zd82262d == 1 || zd82262d == 2)) {
				const int z07cb90d = zd82262d == 1 ? 2 : 1;
				if (z26c0539(z07cb90d)) {
					z1cd003b = 1 << (z07cb90d - 1);
					zb702c05  = "ausente";
					z3440738(z1cd003b);
					z503808b("inatividade: saiu com o aviso de inatividade do P%d na tela.", z07cb90d);
				}
			}
			zb31ef22(z94aaa38.zb306bef, z94aaa38.ze6bfbdc, zb702c05, z6cc50c1);
			z9e6bdfe(z6cc50c1);
			z503808b("session closed.");
		}
		zcef8663 = 0;
		bprintf(PRINT_IMPORTANT, _T("[fbneo_host] session closed.\n"));
	}
	z8fa5b4b();
	z770c6b6(0);
	za069b92(NULL);
	if (zfe8da2e) zf299840(0);
	zfe8da2e = 0;
	zf75197b();
	zc145b37();
	z98879fd  = 0;
	zd3b974b = 0;
	z234d77e();
	z7618f45        = 0;
	zd5a1a8b     = 0;
	za391f79 = 0;
	zeb07121         = 0;
	z05fec26    = 0;
	free(z43a5fe9);
	z43a5fe9      = NULL;
	z48296e9    = 0;
	za0279de = 1;
	z2fc19ce    = 0;
	z5aa079a         = 0;
	zcc7a50c     = 0;
	z13cb333     = 0;
	z2cf405c      = 0;
	z8da8b3a(NULL, 0);
}

int zb71d2fc(void) { return zcef8663; }

int ze9b2e0c(void) { return strcmp(zb702c05, "limit") == 0; }

#define zdedc5dd 2000
static unsigned long long zb65b6cf = 0;
static int                z897beb2   = -1;
static int                z8a9a977  = 0;
static int                z1bebf91      = 0;

void z8a35ea7(int z4067262) { z1bebf91 = z4067262 ? 1 : 0; }
int  zf796ce3(void) { return z1bebf91; }

int z1b0f145(void)
{
	return zcef8663 && !z7618f45 && !zd5a1a8b && !z90c98a2 && !z5aa079a && !z69d3fdc &&
	       !ze949c69();
}

static int z685eaf8(void)
{
	const int p = (z05f35c5 >= 1 && z05f35c5 <= z3332e9c) ? z05f35c5 - 1 : 0;
	unsigned char bits[z102275f];
	z7c7574b(p, bits, z102275f);
	int start = 0, coin = 0;
	for (int b = 0; b < z3c35b6f[p].nBits && b < z2d35323 && (b >> 3) < z102275f; b++) {
		const char* n = zd8e7890(zde7202f[p][b]);
		if (!n || !z27d554e(bits, b)) continue;
		if (!strcmp(n, "Start")) start = 1;
		else if (!strcmp(n, "Coin")) coin = 1;
	}
	return start && coin;
}

#define z11bb702 400
static int                ze388bcb      = 0;
static unsigned long long z5c58e7f    = 0;
static int                zd4e2ee7   = 0;
static int                za1e75b4 = 0;
static unsigned int       zcd075d1    = 0;

static int zd3391ee(void)
{
	return zcef8663 && !z7618f45 && !zd5a1a8b && !z90c98a2 && !z5aa079a && z3332e9c == 2 && z60692a4();
}

static int z1339444(void)
{
	return z2cf405c ? 0 : (z05f35c5 >= 1 && z05f35c5 <= z3332e9c) ? z05f35c5 - 1 : 0;
}

static int z50e9899(const char* n)
{
	return !strcmp(n, "Select") || !strncmp(n, "Service", 7) || !strncmp(n, "Diag", 4) ||
	       !strcmp(n, "Reset") || !strncmp(n, "Test", 4) || !strcmp(n, "Tilt");
}

static unsigned int z6174763(int* pStart, int* zd490586, int* pAny)
{
	const int p = z1339444();
	unsigned char bits[z102275f];
	z7c7574b(p, bits, z102275f);
	unsigned int e = 0;
	int start = 0, coin = 0, any = 0, fire = 0;
	for (int b = 0; b < z3c35b6f[p].nBits && b < z2d35323 && (b >> 3) < z102275f; b++) {
		if (p == 0 && b == zc3f80f4) continue;
		const char* n = zd8e7890(zde7202f[p][b]);
		if (!n) continue;
		const int on = z27d554e(bits, b);
		any |= on;
		if      (!strcmp(n, "Start")) start |= on;
		else if (!strcmp(n, "Coin"))  coin |= on;
		else if (!strcmp(n, "Up"))    { if (on) e |= z1eb4eb1; }
		else if (!strcmp(n, "Down"))  { if (on) e |= zbd4b563; }
		else if (!strcmp(n, "Left"))  { if (on) e |= z6a08570; }
		else if (!strcmp(n, "Right")) { if (on) e |= z9e5f2af; }
		else if (!z50e9899(n) && fire < 2) {
			if (on) e |= fire == 0 ? z32e3848 : z884daca;
			fire++;
		}
	}
	*pStart = start;
	*zd490586  = coin;
	*pAny   = any;
	return e;
}

static void z8ca5ff3(void)
{
	if (!zd3391ee()) {
		ze388bcb = zd4e2ee7 = za1e75b4 = 0;
		zcd075d1 = 0;
		return;
	}
	int start = 0, coin = 0, any = 0;
	unsigned int cur = z6174763(&start, &coin, &any);
	static int zd5c0735 = -1;
	if (zd5c0735 < 0) zd5c0735 = GetEnvironmentVariableA("RBF_TESTE_PROXIMA_PAD", NULL, 0) ? 1 : 0;
	if (z2cf405c && !zd5c0735) { cur = 0; start = coin = 0; }
	const unsigned long long now = GetTickCount64();
	if (start && coin) {
		if (!ze388bcb) { ze388bcb = 1; z5c58e7f = now; }
		else if (ze388bcb == 1 && now - z5c58e7f > z11bb702) {
			ze388bcb = 2;
			z503808b("chat: Start+Select segurado - e o segurar para sair (quadro %lld)", z91b3ccd());
		}
	} else if (ze388bcb) {
		if (ze388bcb == 1 && now - z5c58e7f <= z11bb702 && z2ae00bb())
			z503808b("chat: Start+Select no controle (quadro %lld)", z91b3ccd());
		ze388bcb = (start || coin) ? 2 : 0;
	}
	if (!ze388bcb) {
		const unsigned int edges = cur & ~zcd075d1;
		if (edges) z52fc988(edges);
	}
	zcd075d1 = cur;
	const int zcd1214d = z6f4ded3();
	if (za1e75b4 && !zcd1214d) zd4e2ee7 = 1;
	if (zd4e2ee7 && !any) zd4e2ee7 = 0;
	za1e75b4 = zcd1214d;
}

static void ze1b2699(unsigned char* out, int p, int z9df8312)
{
	if (!ze388bcb && !zd4e2ee7 && !z6f4ded3()) return;
	if (!zd3391ee()) return;
	if (zd4e2ee7 || z6f4ded3()) {
		memset(out, 0, (size_t)z9df8312);
		return;
	}
	for (int b = 0; b < z3c35b6f[p].nBits && b < z2d35323 && (b >> 3) < z9df8312; b++) {
		const char* n = zd8e7890(zde7202f[p][b]);
		if (n && (!strcmp(n, "Start") || !strcmp(n, "Coin"))) out[b >> 3] &= (unsigned char)~(1 << (b & 7));
	}
}

int z17f8ebd(int bHasFocus)
{
	z8ca5ff3();
	if (z8a9a977) return 0;
	const int esc = bHasFocus && !z806b0c0() && (GetAsyncKeyState(VK_ESCAPE) & 0x8000);
	const int held = z1bebf91 || (z1b0f145() && (esc || z685eaf8()));
	if (!held) {
		zb65b6cf = 0;
		z897beb2   = -1;
		return 0;
	}
	const unsigned long long now = GetTickCount64();
	if (!zb65b6cf) zb65b6cf = now;
	const unsigned long long el = now - zb65b6cf;
	z897beb2 = el >= zdedc5dd ? 100 : (int)(el * 100 / zdedc5dd);
	if (!z1bebf91 && !esc && ze388bcb == 1 && el <= z11bb702) z897beb2 = -1;
	if (el < zdedc5dd) return 0;
	z8a9a977 = 1;
	if (z1b0f145() && strcmp(zb702c05, "closed") == 0) zb702c05 = "quit";
	z503808b(z1bebf91 ? "o jogador daqui segurou o X da janela: o emulador fecha (quit)."
	                  : "o jogador daqui segurou para sair: a partida acaba aqui (quit).");
	return 1;
}

int zedf061d(void) { return z897beb2; }

int zc8b4cc6(void) { return zfeed50d != 0; }

int ze12829b(void)
{
	if (!zfeed50d) return 0;
	const unsigned long long el = GetTickCount64() - zfeed50d;
	return el >= z7cc0a06 ? 0 : (int)((z7cc0a06 - el + 999) / 1000);
}

int zf5650f6(int* pSide, int* z4b39f95)
{
	if (pSide) *pSide = 0;
	if (z4b39f95) *z4b39f95 = -1;
	if (!zcef8663 || z7618f45 || !zc1569cf) return 0;
	const ULONGLONG now = GetTickCount64();
	if (now < z94d9e98) return 0;
	const int mine = z165ce15 != 0 && now < z165ce15;
	if (mine == zb1c7a2d) return 0;
	const int other = zd82262d == 1 ? 2 : 1;
	if (mine) {
		if (z2edaa31) return 0;
		if (pSide) *pSide = zd82262d;
		if (z4b39f95) *z4b39f95 = (int)((z165ce15 - now) * 100 / z40e133b);
		return 1;
	}
	if (pSide) *pSide = other;
	return 2;
}

void ze69a66a(void)
{
	if (zfe62411()) { z503808b("fim (teste de fundo): sem o aviso na tela"); return; }
	wchar_t p1[40], p2[40], head[64], score[160];
	z22ec6fe(z94aaa38.zd843ce2, L"P1", p1, 40);
	z22ec6fe(z94aaa38.z7dd7ec2, L"P2", p2, 40);

	_snwprintf(head, 64, z6f2968c(L"FT%d encerrada"), z94aaa38.ze6bfbdc);
	_snwprintf(score, 160, L"%ls   %d x %d   %ls", p1, z6a2ac86, z6a3da4b, p2);
	head[63] = score[159] = 0;

	z639f64f(head, score, 3);
}

void zb97f210(long long nRan)
{
	if (zfe62411()) { z503808b("fim (teste de fundo): sem o aviso na tela"); return; }
	if (strcmp(zb702c05, "ausente") == 0) {
		const int z1b39dee = (z1cd003b & (1 << (zd82262d - 1))) != 0;
		z639f64f(z1b39dee ? z6f2968c(L"Partida encerrada por inatividade") : z6f2968c(L"Adversário inativo"),
		              z1b39dee ? z6f2968c(L"você ficou sem jogar") : z6f2968c(L"partida encerrada"), 3);
		return;
	}
	z639f64f(nRan == 0 ? z6f2968c(L"Sem conexão com o adversário") : z6f2968c(L"Partida encerrada"), NULL, 3);
}

void z7c206bb(void)
{
	if (zfe62411()) { z503808b("fim (teste de fundo): sem o aviso na tela"); return; }
	if (z61f4c2a) { z639f64f(z6f2968c(L"Fim da demonstra\u00e7\u00e3o"), NULL, 3); return; }
	z639f64f(z6f2968c(L"Transmissão encerrada"), NULL, 3);
}

static int ze11fccc(void)
{
	unsigned char in[z897cf46 * z102275f];
	unsigned char local[z102275f];
	memset(in, 0, sizeof(in));

	zf125d1e(local, z378679b, NULL);
	const int side = (!z2cf405c && z05f35c5 == 2 && z3332e9c >= 2) ? 2 : 1;
	memcpy(in + (size_t)(side - 1) * z378679b, local, (size_t)z378679b);
	if (side == 2 && zc3f80f4 >= 0) {
		unsigned char p1[z102275f];
		z7c7574b(0, p1, z378679b);
		const unsigned char m = (unsigned char)(1 << (zc3f80f4 & 7));
		if (p1[zc3f80f4 >> 3] & m) in[zc3f80f4 >> 3] |= m;
	}

	z7c85c59(in, z3332e9c, 0,  0, NULL);
	z3ebe12a((int)z16a2db6, in, z3332e9c * z378679b, NULL);
	z16a2db6++;

	if (z607dc74 > 0 && zcc7a50c && (z16a2db6 % z607dc74) == 0) {
		unsigned int chk = 0;
		if (z2726b67((int)z16a2db6, NULL, NULL, &chk) == z20fb1dd) ze109f17(z16a2db6, chk);
	}
	return 1;
}

int z607366a(int bDraw)
{
	if (!zcef8663) return -1;
	zc300cc9 = bDraw;

	if (z7618f45) return zc224ed5(bDraw);

	if (!z2fc19ce) {
		int rc = zc8e48ed(z94aaa38.z7f20ee6);
		if (rc != z20fb1dd) {
			z503808b("StateRingInit failed: %d", rc);
			bprintf(PRINT_ERROR, _T("[fbneo_host] StateRingInit failed: %d\n"), rc);
			if (!z5aa079a) return -1;
			z503808b("sozinho: sem o anel de estados - a corrida segue sem transmissao, e o F3 religa a placa");
			z2fc19ce    = 1;
			za391f79 = 0;
		} else {
			z2fc19ce = 1;
			zcc7a50c  = z5aa079a;
			z503808b("state ring: %d slots x %d bytes (%d areas, %d excluded from checksum).",
			       zecd0b95(), z9e7e754(), z9976ce3(), zdb4ae3e());

			{
				void* z86dab42 = NULL;
				int   z2df5efd = 0;
				if (z2726b67(0, &z86dab42, &z2df5efd, NULL) == z20fb1dd && z86dab42)
					z8da8b3a(z86dab42, z2df5efd);
				if (!zbcde670) z503808b("F3: sem o estado de abertura - o F3 do anfitriao religa a placa");
			}
			if (za391f79) z2700bce();
		}
	}

	if (zeb07121) zfff7c15();
	zcbf2334++;
	if (z5aa079a) return ze11fccc();

	zb6d9c60();
	z7020e2b();
	LARGE_INTEGER za449319, kq1;
	QueryPerformanceCounter(&za449319);
	int r = zeba63f1();
	QueryPerformanceCounter(&kq1);
	if (r == z30e34bc && za66db34 && zae94e00.QuadPart) z94eceb6((double)(kq1.QuadPart - za449319.QuadPart) * 1000.0 / (double)zae94e00.QuadPart);
	zfc2ac67();

	if (z90c98a2) {
		if (r < 0) {
			z503808b("synctest %s: tick failed (bridge %d, ggpo err %d)", z94aaa38.z91da805, r, z3186ab5());
			z912bf82();
			_exit(3);
		}
		if (z7a81a0d > 0 && za0ce261() >= z7a81a0d) {
			const int bad = zbff6c98() + (zb3cc48a > 0 ? 1 : 0);
			z912bf82();
			_exit(bad ? 2 : 0);
		}
		return r == z30e34bc ? 1 : 0;
	}

	if (ze949c69() || (z065abf3 > 0 && z91b3ccd() >= z065abf3)) {
		if (!zfeed50d) {
			zb702c05 = "limit";
			zfeed50d = GetTickCount64();
			zd82e415(&z6a2ac86, &z6a3da4b);
			z503808b("first to %d reached (%d x %d) - closing in %d s.",
			       z94aaa38.ze6bfbdc, z6a2ac86, z6a3da4b, z7cc0a06 / 1000);
			zcf8c9b2(z7cc0a06 / 1000);
		}
		if (r != z30e34bc && r != zdebf434) {
			z503808b("ggpo tick failed during the closing seconds (bridge %d) - ending now.", r);
			return -1;
		}
		if (r == z30e34bc) z59322ff++;
		if (GetTickCount64() - zfeed50d >= z7cc0a06 || z69d3fdc) {
			z503808b("ending the session.");
			return -1;
		}
		return r == zdebf434 ? 0 : 1;
	}

	if (zf7cdc79() && !z2cf405c) {
		z1cd003b = zf7cdc79();
		zb702c05  = "ausente";
		zd82e415(&z6a2ac86, &z6a3da4b);
		z503808b("inatividade: %s - fim da contagem, a partida acaba aqui (%d x %d).",
		       z1cd003b == 3 ? "os dois" : z1cd003b == 1 ? "o P1" : "o P2", z6a2ac86, z6a3da4b);
		return -1;
	}

	if (z69d3fdc) return -1;

	if (r == z30e34bc)      return 1;
	if (r == zdebf434) return 0;

	z503808b("ggpo tick FATAL (bridge %d, ggpo err %d) - dropping to offline.", r, z3186ab5());
	bprintf(PRINT_ERROR,
	        _T("[fbneo_host] ggpo tick fatal (bridge %d, ggpo err %d) - dropping to offline.\n"),
	        r, z3186ab5());
	return -1;
}

void      za68575c(int nFrames) { z7a81a0d = nFrames > 0 ? nFrames : 0; }
int       zf780d4d(void) { return z378679b; }
long long zca74129(void)          { return z5aa079a ? z16a2db6 : za0ce261(); }
int       z9fa6516(void)           { return z3186ab5(); }
