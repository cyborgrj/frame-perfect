#include "burnint.h"
#include "match_score.h"
#include "speedrun.h"
#include "anticheat.h"
#include "input_display.h"
#include "ram_probe.h"
#include "stream_files.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

extern "C" void z5d90bae(const char* s);
extern "C" unsigned int z3859adb(int nPlayer);

#define z70b1c52 12

typedef struct {
	const char*  szGame;
	unsigned int z2eb75b6, zd819386;
	unsigned int z8d3029b, z393d5c5;
	int          z37051f2;
	int          z03bf6c7;
	int          z5be7c34;
	int          z8b1bda1;
	int          z8b4af0c;
	unsigned int z5ba4a3e, z73d0ac3;
	unsigned int zd930992, zfd72437;
	int          za9ac628;
	unsigned int nTimer;
	unsigned int zfd93067, zaf6a098;
	int z5c29d25;
	unsigned int zfa752f9;
	int          z8e62725;
	int zed018c9;
	unsigned int zadec966, z1f3fb3d;
	int z4e3ee48;
	unsigned int z66158d8, z3afb86c;
	int zdad73f6;
	int z50efb64;
	unsigned int z8acd703, zfea5464;
	int          zd1e415f;
	unsigned int za884a39, zaac9881;
} z67dd7e2;

static const z67dd7e2 z7090d52[] = {
	{ "sf2ce", 0xFF83E8, 0xFF86E8, 0xFF864F, 0xFF894F, 1, 0x0090, 0, 0, 2, 0, 0, 0, 0, 0, 0xFF8ABE,
	           0, 0, 0, 0, 0, 0, 0xFF864E, 0xFF894E },
	{ "ssf2t", 0xFF8478, 0xFF8878, 0xFF87DF, 0xFF8BDF, 1, 0x0090, 0, 0, 2, 0, 0, 0, 0, 0, 0xFF8DCE,
	           0, 0, 0, 0, 0, 0, 0xFF87DE, 0xFF8BDE, 0, 0, 0, 0, 0, 0xFF8804, 0xFF8C04, 0, 0, 0 },
	{ "ssf2xj", 0xFF8478, 0xFF8878, 0xFF87DF, 0xFF8BDF, 1, 0x0090, 0, 0, 2, 0, 0, 0, 0, 0, 0xFF8DCE,
	           0, 0, 0, 0, 0, 0, 0xFF87DE, 0xFF8BDE, 0, 0, 0, 0, 0, 0xFF8804, 0xFF8C04, 0, 0, 0 },
	{ "sfa2",  0xFF8450, 0xFF8850, 0xFF8482, 0xFF8882, 1, 0x0090, 0, 0, 2, 0, 0, 0, 0, 0, 0xFF2863,
	           0, 0, 0, 0, 0, 0, 0xFF84A6, 0xFF88A6 },
	{ "sfa2u", 0xFF8450, 0xFF8850, 0xFF8482, 0xFF8882, 1, 0x0090, 0, 0, 2, 0, 0, 0, 0, 0, 0xFF2861,
	           0, 0, 0, 0, 0, 0, 0xFF84A6, 0xFF88A6 },
	{ "sfa3",  0xFF8450, 0xFF8850, 0xFF8502, 0xFF8902, 1, 0x0090, 0, 0, 2, 0, 0, 0, 0, 0, 0xFF22D9,
	           0, 0, 0, 0, 0, 0, 0xFF8526, 0xFF8926 },
	{ "xmvsf", 0xFF4211, 0xFF4611, 0xFF4053, 0xFF4453, 1, 0x0120, 0, 0, 1, 0, 0, 0, 0, 1, 0xFF5008,
	           0xFF4A11, 0xFF4E11, 0, 0, 0, 0 },
	{ "mshvsf", 0xFF3A51, 0xFF3E51, 0xFF3853, 0xFF3C53, 1, 0x0120, 0, 0, 1, 0, 0, 0, 0, 1, 0xFF4808,
	           0xFF4251, 0xFF4651, 0, 0, 0, 0 },
	{ "mvsc", 0xFF3271, 0xFF3671, 0xFF3053, 0xFF3453, 1, 0x0120, 0, 0, 1, 0, 0, 0, 0, 1, 0xFF4008,
	           0xFF3A71, 0xFF3E71, 0, 0, 0, 0 },
	{ "msh",  0xFF4191, 0xFF4591, 0xFF4051, 0xFF4451, 1, 0x0090, 0, 0, 2, 0, 0, 0, 0, 1, 0xFF4808 },
	{ "sfiii3", 0x02068D0A, 0x020691A2, 0x02011387, 0x02011388, 1, 0x00A0, 0, 0, 2, 0, 0, 0, 0, 0, 0x02011377,
	            0, 0, 0, 0, 0, 0, 0x02011383, 0x02011385 },
	{ "umk3", 0x01060A60, 0x01061610, 0x01060A30, 0x010615E0, 1, 0x00A6, 0, 0, 2, 0, 0, 0, 0, 0, 0,
	          0, 0, 1, 0, 0, 0, 0x01060A90, 0x01061640 },
	{ "vsav",  0xFF8450, 0xFF8850, 0xFF841D, 0xFF881D, 1, 0x0120, 1, 1 },
	{ "vsavj", 0xFF8450, 0xFF8850, 0xFF841D, 0xFF881D, 1, 0x0120, 1, 1 },
	{ "kof98", 0x108238, 0x108438, 0x10A84E, 0x10A85F, 3, 0x0067, 0, 0, 3,
	           0x108171, 0x108371, 0x10A84C, 0x10A85D, 0, 0x10A83A, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	           0x10A858, 0x10A869, 1, 1, 0x10A85A, 0x10A86B, 1, 0x1082D6, 0x1084D6 },
	{ "kof2002", 0x108238, 0x108438, 0x10A7EE, 0x10A814, 3, 0x0066, 0, 0, 3,
	           0x108171, 0x108371, 0, 0, 0, 0x10A7D2, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	           0x10A7F3, 0x10A819, 1, 1 },
	{ "kof97", 0x108238, 0x108438, 0x10A84B, 0x10A85C, 3, 0x0067, 0, 0, 3,
	           0x108171, 0x108371, 0, 0, 0, 0x10A83A, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	           0x10A855, 0x10A866, 1 },
	{ "rbff1", 0x100489, 0x100589, 0x100411, 0x100511, 1, 0x00C0, 0, 0, 2,
	           0, 0, 0, 0, 1, 0x106AA8, 0, 0, 0, 0, 0, 0, 0x106A76, 0x106A77, 0, 0, 0, 1 },
	{ "rbff2", 0x10048B, 0x10058B, 0x100411, 0x100511, 1, 0x00C0, 0, 0, 2,
	           0, 0, 0, 0, 1, 0x107C28 },
	{ "rbffspec", 0x10048B, 0x10058B, 0x100411, 0x100511, 1, 0x00C0, 0, 0, 2,
	           0, 0, 0, 0, 1, 0x106AB4 },
	{ "fatfursp", 0x10049A, 0x10059A, 0x10045D, 0x10055D, 1, 0x0060, 0, 0, 2,
	           0, 0, 0, 0, 1, 0x10092A },
	{ "garou", 0x10048E, 0x10058E, 0x100411, 0x100511, 1, 0x0078, 0, 0, 2,
	           0, 0, 0, 0, 1, 0x107490, 0, 0, 0, 0, 0, 0, 0x107474, 0x107475, 0, 0, 0, 1 },
	{ "samsho3", 0x108573, 0x108673, 0x108470, 0x108471, 1, 0x0080, 0, 0, 2,
	           0, 0, 0, 0, 1, 0x10849E },
	{ "samsho", 0x100A0C, 0x100A10, 0x1000FE, 0x1000FF, 1, 0x0080, 0, 0, 2,
	           0, 0, 0, 0, 1, 0x100A08, 0, 0, 0, 0x100000, 0xA5, 0, 0, 0, 1 },
	{ "samsho2", 0x100A48, 0x100A4C, 0x100D0B, 0x100D1B, 1, 0x0080, 0, 0, 2,
	           0, 0, 0, 0, 1, 0x100AC6, 0, 0, 0, 0x100000, 0xBB, 0, 0, 0, 1 },
	{ "samsho4", 0x108443, 0x108643, 0x108328, 0x108329, 1, 0x00FC, 0, 0, 2,
	           0, 0, 0, 0, 1, 0x108368 },
	{ "breakrev", 0x10734E, 0x1078AE, 0x10716F, 0x1076CF, 1, 0x1E00, 0, 0, 2,
	           0, 0, 0, 0, 0, 0x107C27 },
	{ "karnovr", 0x1037B7, 0x1039B7, 0x103671, 0x103871, 1, 0x00FF, 0, 0, 2,
	           0, 0, 0, 0, 1, 0x103A70 },
	{ "doubledr", 0x100450, 0x100550, 0x100445, 0x100545, 1, 0x6800, 0, 0, 2,
	           0, 0, 0, 0, 0, 0x102242, 0, 0, 0, 0, 0, 1, 0x102294, 0x102295 },
	{ "lastbld2", 0x10E346, 0x10E34A, 0x10E355, 0x10E357, 1, 0x0100, 0, 0, 2,
	           0, 0, 0, 0, 0, 0x10E595, 0, 0, 0, 0x100000, 0x17E, 0 },
};

static const z67dd7e2* z3c35b6f = NULL;
static int      zbde1601 = 0;
static z67dd7e2 z5ee47ee;
static int      zf71007b = 0;
static za78fa53  z9165167;
static int  z23e6cef = 0, zd19389f = 0;
static int  zf48ab02 = -1, z5d651a9 = -1;
static int  z30cdea0 = -1, z1037ebf = -1, z49f03e5 = 0;
static int  z93ce11c = 0, z9e9683e = 0;
static int  ze42b016  = 0x7FFFFFFF;
static int  zd38b888  = 0x7FFFFFFF;
static long long ze9bc394 = 0;
static long long z6c98266 = 0;
static int  z2000072 = 0;
static int  zd49138a = 0;
static int  z6329982   = 0;
static char z20725d5[48] = "";
static int  z36b7768 = -1, zb200482 = -1;
static void (*g_log)(const char*) = NULL;
static int  z323ac1f = 0;
static int  z6a90649 = 0, z671f77c = 0;
static int  za4fdb46 = 0, zb271d66 = 0, z6cbdaa2 = 0;
static int  z748e839 = -1, z5042295 = -1;
static int  z3d8a142 = 0, z24af3f3 = 0;
static int  z8dcdb7e = 0;

static int  z3a4dc4d[2][z5cee153][256];

#define zca2ff70 8
static int  z41f7547[2][zca2ff70];
static int  za9212d1[2];
static int  zd0532d6[2][3];
static z21b0d90 ze4f6240[z77b92c2];
static int  zd58046c;

static z309d1ab z960fc96 = NULL;
static int  z9095135 = 0;
typedef struct zb409662 {
	unsigned int z3b7e8a8;
	int z9904cd2, z165cb5c, z0222e74, zbefba42, nCount, zabda886;
} zb409662;
static int     zd7cb6f8 = 0;
static zb409662 z05863a7[2];
static int     zea0bcb5 = 0, zcc83e81 = 0;
static long long z860b20e[2] = { 0, 0 };
static int       z6e9fbde = 0;
static int     z5cd9a92 = 0;
#define z9c67193 120
static int     z085f58c = -1, z0e05425 = 0, zf71c1cb = 0;
static int     zcbd65b0 = 0;
static void z49cfe13(void);

typedef struct {
	const char*  szGame;
	char         cType;
	unsigned int zc40dd08, zb1468ac;
	int          z0eee1db;
} za60c9c9;

static const za60c9c9 zbb8301a[] = {
	{ "kof98",    'G', 0x10DAF9, 0x10DAFB, 1 },
	{ "kof97",    'G', 0x10DAF5, 0x10DAF7, 1 },
	{ "kof2002",  'G', 0x10DAB7, 0x10DAB9, 1 },
	{ "garou",    'G', 0x107483, 0x107485 },
	{ "garou",    'R', 0x107477, 0x107476 },
	{ "samsho2",  'G', 0x100D0C, 0x100D1C, 1 },
	{ "samsho4",  'R', 0x108462, 0x108662 },
	{ "lastbld2", 'G', 0x10E362, 0x10E363 },
	{ "lastbld2", 'R', 0x1006E8, 0x1006E9 },
	{ "rbff2",    'G', 0x107C1B, 0x107C1D },
	{ "rbff2",    'R', 0x107C0F, 0x107C0E },
	{ "rbffspec", 'R', 0x106A83, 0x106A82 },
	{ "breakrev", 'R', 0x10717B, 0x1076DB },
	{ "sfa2",     'G', 0xFF84B0, 0xFF88B0, 1 },
	{ "sfa3",     'G', 0xFF8530, 0xFF8930, 1 },
	{ "sfa3",     'G', 0xFF8563, 0xFF8963 },
	{ "vsav",     'G', 0xFF879F, 0xFF8B9F },
	{ "vsavj",    'G', 0xFF879F, 0xFF8B9F },
	{ "mvsc",     'G', 0xFF32A4, 0xFF36A4 },
	{ "xmvsf",    'G', 0xFF4244, 0xFF4644 },
	{ "mshvsf",   'G', 0xFF3A84, 0xFF3E84 },
	{ "msh",      'R', 0xFF4802, 0xFF4803 },
	{ "sfiii3",   'G', 0x02016CD5, 0x02016CD7, 1 },
	{ "umk3",     'G', 0x010622F0, 0x01062300 },
};

#define z4c3d052 4
typedef struct {
	int zda5b744[2];
	int zd01acad[2];
	int z83cba3a[2];
} z19df62d;
static const za60c9c9* z5cf2ba5[z4c3d052];
static int         z45febb2 = 0;
static int         zaf5bb81   = 0;
static z19df62d z4104fea[z4c3d052];
static int         z495ea96 = 0;
#define zfcd69f2 600
#define z58b8ba7  1500
static int       zee708d8 = 0;
static long long zaac41d8 = 0;
static int       zbbba39d = 0;
static long long z54eb609 = 0;
static int       z4c1b295 = 0, zff473ed = 1;
static int       z311e729 = 0;

static void zdc5347d(const char* szGame)
{
	z45febb2 = 0;
	for (unsigned i = 0; i < sizeof(zbb8301a) / sizeof(zbb8301a[0]) && z45febb2 < z4c3d052; i++)
		if (strcmp(szGame, zbb8301a[i].szGame) == 0) z5cf2ba5[z45febb2++] = &zbb8301a[i];
	for (int i = 0; i < z4c3d052; i++)
		for (int s = 0; s < 2; s++) { z4104fea[i].zda5b744[s] = -1; z4104fea[i].zd01acad[s] = -1; z4104fea[i].z83cba3a[s] = 0; }
	zaf5bb81 = z45febb2 && z5cf2ba5[0]->zc40dd08 >= 0x100000 && z5cf2ba5[0]->zc40dd08 < 0x110000;
	z495ea96 = 0;
	zee708d8 = 0;
	zaac41d8 = 0;
	zbbba39d = 0;
	z54eb609 = 0;
	z4c1b295 = 0;
	zff473ed = 1;
	z311e729 = 0;
}

void za069b92(z309d1ab pfn) { z960fc96 = pfn; }

static void z399002f(int nType, int zdb52f45)
{
	if (z960fc96) z960fc96(nType, ze9bc394, zdb52f45);
}

#define z38efc9c(X) \
	X(z9165167) X(z23e6cef) X(zd19389f) X(z93ce11c) X(z9e9683e) X(ze42b016) X(zd38b888) \
	X(ze9bc394) X(z6c98266) X(z2000072) X(zd49138a) X(z6a90649) X(z671f77c) \
	X(za4fdb46) X(zb271d66) X(z6cbdaa2) X(z748e839) X(z5042295) \
	X(z3d8a142) X(z24af3f3) X(z8dcdb7e) \
	X(z3a4dc4d) X(z41f7547) X(za9212d1) X(zd0532d6) X(ze4f6240) X(zd58046c) X(z9095135) \
	X(z05863a7) X(zea0bcb5) X(zcc83e81) X(z085f58c) X(z0e05425) X(zf71c1cb) X(zcbd65b0) \
	X(z860b20e) X(z6e9fbde) \
	X(zf48ab02) X(z5d651a9) X(z30cdea0) X(z1037ebf) X(z49f03e5) 	X(z4104fea) X(z495ea96) X(zee708d8) X(zaac41d8) X(zbbba39d) \
	X(z54eb609) X(z4c1b295) X(zff473ed) X(z311e729)

int zf480b00(void)
{
	int n = 0;
#define ze45b1ea(v) n += (int)sizeof(v);
	z38efc9c(ze45b1ea)
#undef ze45b1ea
	return n;
}

void z5eacaeb(void* pOut)
{
	unsigned char* p = (unsigned char*)pOut;
#define z3ff12f5(v) memcpy(p, &v, sizeof(v)); p += sizeof(v);
	z38efc9c(z3ff12f5)
#undef z3ff12f5
}

void zdf72ff8(const void* pIn)
{
	const unsigned char* p = (const unsigned char*)pIn;
#define zd4309d5(v) memcpy(&v, p, sizeof(v)); p += sizeof(v);
	z38efc9c(zd4309d5)
#undef zd4309d5
	z49cfe13();
}

static void z0716b34(void)
{
	memset(z3a4dc4d, 0, sizeof(z3a4dc4d));
	memset(z41f7547, 0, sizeof(z41f7547));
	memset(za9212d1, 0, sizeof(za9212d1));
	memset(zd0532d6, 0, sizeof(zd0532d6));
	memset(ze4f6240, 0, sizeof(ze4f6240));
	zd58046c = 0;
}

static void z13074ae(int nSide, const int* za55e476, int* pOut)
{
	const int n = z3c35b6f->z37051f2;
	int k = 0;
	for (int i = 0; i < za9212d1[nSide] && k < n; i++) {
		int z18744d7 = 0, dup = 0;
		for (int j = 0; j < n; j++) if (za55e476[j] == z41f7547[nSide][i]) z18744d7 = 1;
		for (int j = 0; j < k; j++) if (pOut[j] == z41f7547[nSide][i]) dup = 1;
		if (z18744d7 && !dup) pOut[k++] = z41f7547[nSide][i];
	}
	for (int i = 0; i < n && k < n; i++) {
		int seen = 0;
		for (int j = 0; j < k; j++) if (pOut[j] == za55e476[i]) seen = 1;
		if (!seen) pOut[k++] = za55e476[i];
	}
}

static int zaee4595(int nSide)
{
	const int* c = zd0532d6[nSide];
	if (c[z4d63468] > c[z4ca563e]) return z4d63468;
	return c[z4ca563e] > 0 ? z4ca563e : z2f4b4cc;
}

static void z23525cb(int* ze8821d1, int* z0f82ab8)
{
	*ze8821d1 = z9e9683e ? 2 : ((zd38b888 <= z3c35b6f->z03bf6c7 / 2) ? 1 : 0);
	*z0f82ab8 = z93ce11c ? 2 : ((ze42b016 <= z3c35b6f->z03bf6c7 / 2) ? 1 : 0);
}

static size_t z961cbec(const char* zb306bef, char* szOut, size_t nOut)
{
	size_t n = 0;
	for (size_t i = 0; zb306bef && zb306bef[i] && n + 1 < nOut; i++) {
		const char c = zb306bef[i];
		if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') ||
		    (c >= 'A' && c <= 'Z') || c == '-') szOut[n++] = c;
	}
	szOut[n] = '\0';
	return n;
}

static void zb533d50(char* szOut, size_t nOut)
{
	snprintf(szOut, nOut, "rbf-live-%s.txt", z20725d5);
}

static void z3534113(void)
{
	if (!z20725d5[0]) return;
	if (z9165167.z45e0144 == z36b7768 && z9165167.z78ca99a == zb200482) return;
	z36b7768 = z9165167.z45e0144;
	zb200482 = z9165167.z78ca99a;

	char szPath[128];
	zb533d50(szPath, sizeof(szPath));
	FILE* f = fopen(szPath, "wb");
	if (!f) return;
	fprintf(f, "p1games=%d\np2games=%d\n", z36b7768, zb200482);
	fclose(f);
}

static void z49cfe13(void)
{
	z36b7768 = zb200482 = -1;
	z3534113();
}

void z5126edb(const char* zb306bef)
{
	z36b7768 = zb200482 = -1;
	if (!zb306bef || !zb306bef[0]) { z20725d5[0] = 0; return; }
	z961cbec(zb306bef, z20725d5, sizeof(z20725d5));
}

static void zaf4dd4f(void (*pfn)(const char*), const char* fmt, ...);

static void zefadd78(void)
{
	z9165167.z726041c = z9165167.zae1a069 = 0;
	ze42b016 = zd38b888 = 0x7FFFFFFF;
	z93ce11c = z9e9683e = 0;
	z23e6cef = zd19389f = 0;
	z6c98266 = 0;
	memset(z9165167.zc4ac0ca, 0, sizeof(z9165167.zc4ac0ca));
	memset(z9165167.zd1372c9, 0, sizeof(z9165167.zd1372c9));
	z0716b34();
	z9165167.zdafe8ef = z9165167.zdebfa6d = 0;
	z9165167.z2ed2c96 = 0;
	za4fdb46 = zb271d66 = z6cbdaa2 = 0;
	z748e839 = z5042295 = -1;
	z3d8a142 = z24af3f3 = z8dcdb7e = 0;
	z9095135 = 0;
	zcbd65b0 = 0;
}

void z65fd730(void)
{
	z48f734b();
	if (!z3c35b6f) return;
	if (z323ac1f && (z9165167.z726041c || z9165167.zae1a069))
		zaf4dd4f(g_log, "score: f%lld partida interrompida pelo F3: rounds %d x %d nao contam",
		          ze9bc394, z9165167.z726041c, z9165167.zae1a069);
	zefadd78();
}

static void z0695713(void)
{
	int p1 = z9165167.z726041c, p2 = z9165167.zae1a069;
	if (z3c35b6f && z3c35b6f->z5be7c34) z23525cb(&p1, &p2);
	if (z323ac1f)
		zaf4dd4f(g_log, "score: f%lld fim de partida: rounds %d x %d%s", ze9bc394, p1, p2,
		          (p1 == 0 && p2 == 0) ? " (0 x 0: nao conta)" : "");

	if (p1 == 0 && p2 == 0) {
		zefadd78();
		return;
	}

	const int zdb52f45 = (p1 > p2) ? 1 : (p2 > p1) ? 2 : 0;
	z54eb609  = ze9bc394;
	z4c1b295   = zdb52f45;
	zff473ed = 0;
	z311e729         = 0;
	if (zee708d8) {
		if (zee708d8 != zdb52f45)
			zaf4dd4f(g_log, "score: f%lld testemunha: o jogo deu a partida ao P%d e o leitor ao P%d (fica a do leitor)",
			          ze9bc394, zee708d8, zdb52f45);
		zee708d8 = 0;
		zff473ed = 1;
	}
	if (zdb52f45 == 1)      z9165167.z45e0144++;
	else if (zdb52f45 == 2) z9165167.z78ca99a++;
	z9095135 = 0;
	z399002f(z93d035c, zdb52f45);

	if (z9165167.z535cef6 < z87df4c7) {
		z6ac2385* row = &z9165167.z5400403[z9165167.z535cef6++];
		memset(row, 0, sizeof(*row));
		memcpy(row->zc4ac0ca, z9165167.zc4ac0ca, sizeof(row->zc4ac0ca));
		memcpy(row->zd1372c9, z9165167.zd1372c9, sizeof(row->zd1372c9));
		if (z3c35b6f && z3c35b6f->z5ba4a3e && z9165167.zdafe8ef) z13074ae(0, z9165167.zc4ac0ca, row->zc4ac0ca);
		if (z3c35b6f && z3c35b6f->z73d0ac3 && z9165167.zdebfa6d) z13074ae(1, z9165167.zd1372c9, row->zd1372c9);
		row->zfb4df8e = zaee4595(0);
		row->zb21a368 = zaee4595(1);
		row->zd29aa1b  = zd58046c;
		memcpy(row->za9df5d5, ze4f6240, sizeof(row->za9df5d5));
		row->zdafe8ef = z9165167.zdafe8ef;
		row->zdebfa6d = z9165167.zdebfa6d;
		row->z726041c   = p1;
		row->zae1a069   = p2;
		row->zdb52f45     = zdb52f45;
		row->nFrames     = z6c98266 ? (int)(ze9bc394 - z6c98266) : 0;
	} else {
		z9165167.z219e1e5 = 1;
	}

	z9165167.z1903fc4++;
	if (z6329982 > 0 && (z9165167.z45e0144 >= z6329982 || z9165167.z78ca99a >= z6329982))
		z9165167.z1f29c92 = 1;

	z9165167.z726041c = 0;
	z9165167.zae1a069 = 0;
	ze42b016 = zd38b888 = 0x7FFFFFFF;
	z93ce11c = z9e9683e = 0;
	z23e6cef = zd19389f = 0;
	z6c98266 = 0;
	memset(z9165167.zc4ac0ca, 0, sizeof(z9165167.zc4ac0ca));
	memset(z9165167.zd1372c9, 0, sizeof(z9165167.zd1372c9));
	z0716b34();
	z9165167.zdafe8ef = z9165167.zdebfa6d = 0;
	z9165167.z2ed2c96 = 0;
	za4fdb46 = zb271d66 = z6cbdaa2 = 0;
	z748e839 = z5042295 = -1;
	z3d8a142 = z24af3f3 = z8dcdb7e = 0;
	zcbd65b0 = 0;

	z3534113();
}

static void zaf4dd4f(void (*pfn)(const char*), const char* fmt, ...)
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

static int zdff543e(unsigned int nAddr);

static unsigned int zdd10220(unsigned int nAddr)
{
	if (!z3c35b6f->zfa752f9 || !nAddr) return nAddr;
	const unsigned int lo = (zaefff6f(nAddr) << 8) | zaefff6f(nAddr + 1);
	return (z3c35b6f->zfa752f9 | lo) + z3c35b6f->z8e62725;
}

static int zaba0fbe(int side)
{
	const int a = zdff543e(zdd10220(side ? z3c35b6f->zd819386 : z3c35b6f->z2eb75b6));
	const unsigned int a2 = side ? z3c35b6f->zaf6a098 : z3c35b6f->zfd93067;
	if (!a2) return a;
	const int b = zdff543e(a2);
	if (a <= 0 && b <= 0) return (a < 0 || b < 0) ? -1 : 0;
	return (a > 0 ? a : 0) + (b > 0 ? b : 0);
}

static int zdff543e(unsigned int nAddr)
{
	if (z3c35b6f && z3c35b6f->zed018c9) {
		const int v = (zaefff6f(nAddr) << 8) | zaefff6f(nAddr + 1);
		return z3c35b6f->z03bf6c7 - (v > z3c35b6f->z03bf6c7 ? z3c35b6f->z03bf6c7 : v);
	}
	if (z3c35b6f && z3c35b6f->z5c29d25)
		return zaefff6f(nAddr) | (zaefff6f(nAddr + 8) << 8);
	if (z3c35b6f && z3c35b6f->za9ac628) {
		unsigned char b = zaefff6f(nAddr);
		if (b == 0xFF && z3c35b6f->z03bf6c7 != 0xFF) return -1;
		if (z3c35b6f->z03bf6c7 < 0x80 && b >= 0x80) return (int)(signed char)b;
		return (int)b;
	}
	int hi = zaefff6f(nAddr);
	int lo = zaefff6f(nAddr + 1);
	return (short)((hi << 8) | lo);
}

static int z4e3d88a(const int* h, int z543f62e)
{
	int best = 0, zffb92d8 = -1, z5dd902a = -1;
	for (int c = 0; c < 256; c++) {
		const int own   = h[c];
		const int score = own + ((z543f62e && c < 255) ? h[c + 1] : 0);
		if (score > zffb92d8 || (score == zffb92d8 && own > z5dd902a)) {
			best = c; zffb92d8 = score; z5dd902a = own;
		}
	}
	return best;
}

static int zd2c8081(unsigned int nAddr, int i, int bMask)
{
	if (!nAddr) return 0;
	const int v = zaefff6f(nAddr);
	return (bMask ? (v >> i) & 1 : v != 0) ? z3f24ff9 : 0;
}

static void z82b6e54(int nSide, unsigned int nAddr, int* pOut, int nCount, int* zbec1e4f, int z543f62e, unsigned int zad4c35d)
{
	if (!nAddr) return;
	for (int i = 0; i < nCount && i < z5cee153; i++) {
		int* h = z3a4dc4d[nSide][i];
		h[(zaefff6f(nAddr + i) | zd2c8081(zad4c35d, i, z3c35b6f->zd1e415f)) & 0xFF]++;
		pOut[i] = z4e3d88a(h, z543f62e);
	}
	*zbec1e4f = 1;
}

static void z5b74a62(int nSide, unsigned int nAddr, unsigned int z6166ecd, const int* za55e476, int zab46fd3)
{
	if (!nAddr || !zab46fd3) return;
	const int v = zaefff6f(nAddr) | zd2c8081(z6166ecd, 0, 0);
	const int n = z3c35b6f->z37051f2;
	int z18744d7 = 0;
	for (int i = 0; i < n; i++) if (za55e476[i] == v) z18744d7 = 1;
	if (!z18744d7) return;
	for (int i = 0; i < za9212d1[nSide]; i++) if (z41f7547[nSide][i] == v) return;
	if (za9212d1[nSide] < zca2ff70)
		z41f7547[nSide][za9212d1[nSide]++] = v;
}

static void z4f1c6d2(int nSide, unsigned int nAddr)
{
	if (!nAddr) return;
	const int v = zaefff6f(nAddr) + 1;
	if (v == z4ca563e || v == z4d63468) zd0532d6[nSide][v]++;
}

static void z2b71de9(char* szOut, size_t nOut, const int* pChars, int nCount, int zab46fd3)
{
	if (!zab46fd3 || nCount <= 0) { snprintf(szOut, nOut, "?"); return; }
	int n = snprintf(szOut, nOut, "%d", pChars[0]);
	for (int i = 1; i < nCount && n > 0 && n < (int)nOut; i++)
		n += snprintf(szOut + n, nOut - n, "/%d", pChars[i]);
}

void zea0d2fd(int bOn) { z323ac1f = bOn ? 1 : 0; }

void z97e971e(int bOn) { zbde1601 = bOn ? 1 : 0; }

int z5ebf79e(int ze6bfbdc, void (*z930c6f2)(const char*))
{
	z3c35b6f = NULL;
	g_log = z930c6f2;
	z6a90649 = z671f77c = 0;
	za4fdb46 = zb271d66 = z6cbdaa2 = 0;
	z748e839 = z5042295 = -1;
	z3d8a142 = z24af3f3 = z8dcdb7e = 0;
	z6329982 = (ze6bfbdc > 0 && ze6bfbdc < 100) ? ze6bfbdc : 0;
	memset(&z9165167, 0, sizeof(z9165167));
	z0716b34();
	z23e6cef = zd19389f = 0;
	zf48ab02 = z5d651a9 = -1;
	z30cdea0 = z1037ebf = -1;
	z49f03e5 = 0;
	z93ce11c = z9e9683e = 0;
	ze42b016 = zd38b888 = 0x7FFFFFFF;
	ze9bc394 = 0;
	z6c98266 = 0;
	z9095135 = 0;
	memset(z05863a7, 0, sizeof(z05863a7));
	zea0bcb5 = zcc83e81 = 0;
	z860b20e[0] = z860b20e[1] = 0;
	z6e9fbde = 0;
	z085f58c = -1;
	z0e05425 = 0;
	zf71c1cb = 0;
	zcbd65b0 = 0;
	z5cd9a92 = 0;
	zdc5347d("");

	if (zfc8fae7(z930c6f2) != z7e634bf)
		return z58c624b;

	const char* zfa0e88b = BurnDrvGetTextA(DRV_NAME);
	if (!zfa0e88b) zfa0e88b = "";
	const char* szGame = zfa0e88b;
	if (!strcmp(szGame, "umk3jug")) szGame = "umk3";
	if (!strcmp(szGame, "sfa2u"))   szGame = "sfa2";
	strncpy(z9165167.szGame, szGame, sizeof(z9165167.szGame) - 1);

	for (unsigned i = 0; i < sizeof(z7090d52) / sizeof(z7090d52[0]); i++) {
		if (strcmp(zfa0e88b, z7090d52[i].szGame) == 0) { z3c35b6f = &z7090d52[i]; break; }
	}
	for (unsigned i = 0; !z3c35b6f && i < sizeof(z7090d52) / sizeof(z7090d52[0]); i++) {
		if (strcmp(szGame, z7090d52[i].szGame) == 0) { z3c35b6f = &z7090d52[i]; break; }
	}

	if (!z3c35b6f) {
		zaf4dd4f(z930c6f2, "score: %s is not a game I know how to read - "
		                  "this match will not be scored", szGame);
		return z1d7d469;
	}

	if (zbde1601 && z3c35b6f->z37051f2 > 1) {
		z5ee47ee = *z3c35b6f;
		z5ee47ee.z37051f2 = 1;
		z5ee47ee.z8b4af0c  = 2;
		z5ee47ee.z50efb64 = 0;
		z3c35b6f = &z5ee47ee;
		zaf4dd4f(z930c6f2, "score: Single Vs - um lutador por lado, a partida acaba em 2 rounds");
	}

	z9165167.z37051f2 = z3c35b6f->z37051f2;
	zdc5347d(szGame);
	if (z45febb2) zaf4dd4f(z930c6f2, "score: segunda testemunha: %d contador(es) do jogo", z45febb2);
	if (z6329982 > 0)
		zaf4dd4f(z930c6f2, "score: reading %s, limite FT%d", szGame, z6329982);
	else
		zaf4dd4f(z930c6f2, "score: reading %s, sem limite (livre) - "
		                  "se voce pediu FT, o servidor mandou 0: ele esta desatualizado?",
		          szGame);
	return za4ce288;
}

void z770c6b6(int bOn) { zf71007b = bOn ? 1 : 0; }

void z9fff242(int bOn) { zd7cb6f8 = bOn ? 1 : 0; }
int  ze2aa305(void) { return z3c35b6f && zea0bcb5; }
int  zb139002(void) { return z3c35b6f ? z6e9fbde : 0; }

void z3440738(int nSide) { z5cd9a92 = nSide; }

int z719f6d2(int nSide)
{
	if (!z3c35b6f || !zd7cb6f8 || nSide < 1 || nSide > 2) return 0;
	return z05863a7[nSide - 1].nCount;
}

int z26c0539(int nSide)
{
	if (!z3c35b6f || !zd7cb6f8 || nSide < 1 || nSide > 2) return 0;
	return (z05863a7[nSide - 1].z0222e74 || z05863a7[nSide - 1].nCount) ? 1 : 0;
}

int zf7cdc79(void)
{
	if (!z3c35b6f || !zd7cb6f8) return 0;
	return (z05863a7[0].zabda886 ? 1 : 0) | (z05863a7[1].zabda886 ? 2 : 0);
}

int z969ea27(int* pSide, int* z5a4156e, int* z335f6f7)
{
	if (!z3c35b6f || !zd7cb6f8) return 0;
	int side = 0, stage = 0, left = 0;
	for (int s = 0; s < 2; s++) {
		const zb409662* a = &z05863a7[s];
		if (!a->z0222e74 && !a->nCount && !a->zabda886) continue;
		const int st = (a->nCount || a->zabda886) ? 2 : 1;
		if (st > stage) { stage = st; side = 0; left = 0; }
		if (st == stage) {
			side |= 1 << s;
			if (st == 2 && (left == 0 || a->nCount < left)) left = a->nCount;
		}
	}
	if (!stage) return 0;
	if (pSide)       *pSide = side;
	if (z5a4156e)      *z5a4156e = stage;
	if (z335f6f7) *z335f6f7 = left;
	return 1;
}

static void ze46111d(void);

static void z355304c(void)
{
	static int z69e6433 = 0;
	if (z69e6433) return;
	z69e6433 = 1;
	char v[16] = "";
	if (!GetEnvironmentVariableA("RBF_TESTE_PLACAR", v, sizeof(v))) return;
	if (z3c35b6f) return;
	zea0d2fd(1);
	z5ebf79e(atoi(v), z5d90bae);
}

void z21b472f(void)
{
	z355304c();
	if (!zf71007b) { ze46111d(); z4aa24cc(); }
}

void zb4c9a9b(void) { ze46111d(); z4aa24cc(); }

#define z0936da4 120
static int zacd238a(void)
{
	return zaefff6f(0x10FDB6) == 1 && zaefff6f(0x10FDB7) == 1;
}

static int z96e973d(int l1, int l2)
{
	const int bOpen = l1 == z3c35b6f->z03bf6c7 && l2 == z3c35b6f->z03bf6c7;
	if (z3c35b6f->z66158d8) return bOpen && zaefff6f(z3c35b6f->z66158d8) == 0 && zaefff6f(z3c35b6f->z3afb86c) == 0;
	if (z3c35b6f->zadec966) return bOpen && zaefff6f(z3c35b6f->zadec966) == 0 && zaefff6f(z3c35b6f->z1f3fb3d) == 0;
	return l1 == 0 && l2 == 0;
}

static void ze7c83ba(int s, int z0623ddc)
{
	zb409662* a = &z05863a7[s];
	const unsigned int bits = z3859adb(s);
	const int z0ba0002 = (bits & ~a->z3b7e8a8) != 0;
	a->z3b7e8a8 = bits;
	if (a->zabda886) {
		if (z0ba0002 && ++a->zbefba42 >= z6fc34b9) {
			memset(a, 0, sizeof(*a));
			a->z3b7e8a8 = bits;
			zaf4dd4f(g_log, "score: f%lld o P%d voltou depois do fim da contagem (replay antigo)", ze9bc394, s + 1);
		}
		return;
	}

	a->z9904cd2  = bits   ? 0 : (a->z9904cd2  < 0x7FFFFFFF && z0623ddc ? a->z9904cd2  + 1 : a->z9904cd2);
	a->z165cb5c = z0ba0002 ? 0 : (a->z165cb5c < 0x7FFFFFFF && z0623ddc ? a->z165cb5c + 1 : a->z165cb5c);

	if (a->nCount) {
		if (--a->nCount == 0) {
			a->zabda886 = 1;
			a->zbefba42 = 0;
			zaf4dd4f(g_log, "score: f%lld inatividade do P%d - fim da contagem, a partida acaba", ze9bc394, s + 1);
		}
		return;
	}
	if (!a->z0222e74) {
		if (a->z9904cd2 >= z5149dd1) {
			a->z0222e74  = 1;
			a->zbefba42  = 0;
			a->z165cb5c = 0;
			zaf4dd4f(g_log, "score: f%lld inatividade: o P%d nao aperta nada ha 1 min - aviso na tela",
			          ze9bc394, s + 1);
			if (z860b20e[s] && ze9bc394 - z860b20e[s] < zb2570fd) {
				if (!(z6e9fbde & (1 << s)))
					zaf4dd4f(g_log, "score: f%lld inatividade repetida do P%d (o aviso anterior foi no quadro %lld): "
					          "a partida em andamento nao conta e a sessao nao vale pontos", ze9bc394, s + 1, z860b20e[s]);
				else
					zaf4dd4f(g_log, "score: f%lld inatividade repetida do P%d de novo: a partida em andamento nao conta",
					          ze9bc394, s + 1);
				z6e9fbde |= 1 << s;
				zefadd78();
				z311e729 = 1;
			}
			z860b20e[s] = ze9bc394;
		}
		return;
	}
	if (z0ba0002 && ++a->zbefba42 >= z6fc34b9) {
		a->z0222e74 = 0;
		zaf4dd4f(g_log, "score: f%lld o P%d voltou (%d toques) - o aviso sai", ze9bc394, s + 1, a->zbefba42);
		return;
	}
	if (a->z165cb5c >= z5149dd1) {
		a->nCount = z1d4bc00;
		zaf4dd4f(g_log, "score: f%lld inatividade do P%d de novo, depois do aviso - contagem de 5 s",
		          ze9bc394, s + 1);
	}
}

static int zb1bab8e(int l1, int l2)
{
	int z0623ddc = 1;
	if (z3c35b6f->nTimer) {
		const int t = zaefff6f(z3c35b6f->nTimer);
		if (t != z085f58c && z085f58c >= 0 && l1 > 0 && l2 > 0) zf71c1cb = 1;
		z0e05425 = (t != z085f58c) ? 0 : (z0e05425 < 1000000 ? z0e05425 + 1 : z0e05425);
		z085f58c = t;
		z0623ddc = (z0e05425 <= z9c67193 && l1 > 0 && l2 > 0) || (!zf71c1cb && z9165167.zd61d2b1);
	}
	ze7c83ba(0, z0623ddc);
	ze7c83ba(1, z0623ddc);
	const int z423f8b3 = z05863a7[0].zabda886 || z05863a7[1].zabda886;

	if (z423f8b3 && !zea0bcb5) {
		zea0bcb5 = 1;
		zcc83e81   = 0;
		if (z9165167.z726041c || z9165167.zae1a069)
			zaf4dd4f(g_log, "score: f%lld inatividade ate o fim: a partida em andamento (rounds %d x %d) nao conta",
			          ze9bc394, z9165167.z726041c, z9165167.zae1a069);
		zefadd78();
		z311e729 = 1;
	} else if (zea0bcb5 && !z423f8b3) {
		zea0bcb5 = 0;
		zcc83e81   = 1;
		zaf4dd4f(g_log, "score: f%lld sem aviso de inatividade - o placar volta na proxima partida", ze9bc394);
	}
	if (zcc83e81 && z96e973d(l1, l2)) {
		zcc83e81 = 0;
		zefadd78();
		zaf4dd4f(g_log, "score: f%lld partida nova - o placar volta a contar", ze9bc394);
	}
	return zea0bcb5 || zcc83e81;
}

static void zc2b6372(int nSide, long long f);

static void zc3bdec0(void)
{
	for (int i = 0; i < z45febb2; i++) {
		z19df62d* w = &z4104fea[i];
		for (int s = 0; s < 2; s++) {
			const int v = zaefff6f(s ? z5cf2ba5[i]->zb1468ac : z5cf2ba5[i]->zc40dd08);
			if (w->zda5b744[s] < 0) { w->zda5b744[s] = v; w->zd01acad[s] = v; w->z83cba3a[s] = 0; continue; }
			if (v == w->zda5b744[s]) { w->zd01acad[s] = v; w->z83cba3a[s] = 0; continue; }
			if (v != w->zd01acad[s]) { w->zd01acad[s] = v; w->z83cba3a[s] = 1; continue; }
			if (++w->z83cba3a[s] < z70b1c52) continue;
			const long long f = ze9bc394 + z495ea96 - (z70b1c52 - 1);
			if (zaf5bb81)
				zaf4dd4f(g_log, "score: f%lld testemunha %c%d P%d %d -> %d (BIOS %d%d, inatividade %d, rounds %d x %d, luta %d)",
				          f, z5cf2ba5[i]->cType, i, s + 1, w->zda5b744[s], v,
				          zaefff6f(0x10FDB6), zaefff6f(0x10FDB7), zea0bcb5 || zcc83e81,
				          z9165167.z726041c, z9165167.zae1a069, z9165167.z2ed2c96);
			else
				zaf4dd4f(g_log, "score: f%lld testemunha %c%d P%d %d -> %d (inatividade %d, rounds %d x %d, luta %d)",
				          f, z5cf2ba5[i]->cType, i, s + 1, w->zda5b744[s], v, zea0bcb5 || zcc83e81,
				          z9165167.z726041c, z9165167.zae1a069, z9165167.z2ed2c96);
			const int zac3f25d = v > w->zda5b744[s];
			w->zda5b744[s] = v;
			w->z83cba3a[s] = 0;
			if (zac3f25d && z5cf2ba5[i]->z0eee1db) zc2b6372(s + 1, f);
		}
	}
}

static void zc2b6372(int nSide, long long f)
{
	if (zbde1601 || z9165167.z1f29c92) return;
	if (!zff473ed && f - z54eb609 <= z58b8ba7) {
		zff473ed = 1;
		if (nSide != z4c1b295)
			zaf4dd4f(g_log, "score: f%lld testemunha: o jogo deu a partida ao P%d e o leitor ao P%d (fica a do leitor)",
			          f, nSide, z4c1b295);
		return;
	}
	if (z311e729) {
		z311e729 = 0;
		zaf4dd4f(g_log, "score: f%lld testemunha: o jogo deu ao P%d a partida anulada por inatividade - nao conta",
		          f, nSide);
		return;
	}
	zee708d8 = nSide;
	zaac41d8   = f;
	zbbba39d = !z3c35b6f->zdad73f6 || zacd238a();
}

static void zff2c22e(void)
{
	if (!zee708d8 || ze9bc394 - zaac41d8 < zfcd69f2) return;
	const int nSide = zee708d8;
	zee708d8 = 0;
	if (!zbbba39d || zea0bcb5 || !z9165167.zd61d2b1) {
		zaf4dd4f(g_log, "score: f%lld testemunha: o jogo deu a partida ao P%d, mas fora de uma luta entre os dois - nao conta",
		          ze9bc394, nSide);
		return;
	}
	const int z4048f0e = z3c35b6f->z8b4af0c > 0 ? z3c35b6f->z8b4af0c : 2;
	if (nSide == 1 && z9165167.z726041c < z4048f0e) z9165167.z726041c = z4048f0e;
	if (nSide == 2 && z9165167.zae1a069 < z4048f0e) z9165167.zae1a069 = z4048f0e;
	zaf4dd4f(g_log, "score: f%lld testemunha: o leitor nao fechou a partida que o jogo deu ao P%d - fecha agora (%d x %d)",
	          ze9bc394, nSide, z9165167.z726041c, z9165167.zae1a069);
	z0695713();
	zff473ed = 1;
}

static void ze46111d(void)
{
	z818176f();
	z22b4a5e();
	z0b2e6fa();

	if (!z3c35b6f) return;

	if (z9165167.z1f29c92) { z495ea96++; zc3bdec0(); return; }

	ze9bc394++;
	zc3bdec0();
	zff2c22e();
	if (z9165167.z1f29c92) return;

	const int l1 = zaba0fbe(0);
	const int l2 = zaba0fbe(1);

	if (zd7cb6f8 && zb1bab8e(l1, l2)) {
		z6a90649 = l1;
		z671f77c = l2;
		return;
	}

	if (z323ac1f && ((l1 < 0) != (z6a90649 < 0) || (l2 < 0) != (z671f77c < 0))) {
		zaf4dd4f(g_log, "score: f%lld vida P1 %d -> %d, P2 %d -> %d  (na tela %d x %d, iniciada=%d, rounds %d x %d)",
		          ze9bc394, z6a90649, l1, z671f77c, l2,
		          z3c35b6f->z5ba4a3e ? zaefff6f(z3c35b6f->z5ba4a3e) : -1,
		          z3c35b6f->z73d0ac3 ? zaefff6f(z3c35b6f->z73d0ac3) : -1,
		          z9165167.z2ed2c96, z9165167.z726041c, z9165167.zae1a069);
	}
	z6a90649 = l1;
	z671f77c = l2;

	if (!z9165167.z2ed2c96) {
		if (l1 != z3c35b6f->z03bf6c7 || l2 != z3c35b6f->z03bf6c7) return;
		if (z3c35b6f->zdad73f6 && !zacd238a()) return;
		if (z3c35b6f->z66158d8 && (zaefff6f(z3c35b6f->z66158d8) || zaefff6f(z3c35b6f->z3afb86c))) return;
		z9165167.z2ed2c96     = 1;
		z9165167.zd61d2b1 = 1;
		if (!z6c98266)     z6c98266     = ze9bc394;
		if (!z9165167.nStartFrame) z9165167.nStartFrame = ze9bc394;
		if (z323ac1f) zaf4dd4f(g_log, "score: f%lld luta comecou (vidas cheias)", ze9bc394);
		z9095135 = 0;
		z399002f(z69eb802, 0);
	}

	if (z9095135 && l1 > 0 && l2 > 0) {
		z9095135 = 0;
		z399002f(z5ea2195, 0);
	}

	if (z3c35b6f->nTimer) {
		const int t = zaefff6f(z3c35b6f->nTimer);
		if (!z3d8a142 && t != 0 && z24af3f3 != 0 && t < z24af3f3 && l1 > 0 && l2 > 0) {
			z3d8a142 = 1;
			memset(z3a4dc4d, 0, sizeof(z3a4dc4d));
			memset(z41f7547, 0, sizeof(z41f7547));
			memset(za9212d1, 0, sizeof(za9212d1));
			memset(zd0532d6, 0, sizeof(zd0532d6));
			if (z323ac1f) zaf4dd4f(g_log, "score: f%lld o relogio andou: daqui em diante e a luta", ze9bc394);
		}
		z8dcdb7e = (t != z24af3f3) ? 0 : (z8dcdb7e < 1000000 ? z8dcdb7e + 1 : z8dcdb7e);
		z24af3f3 = t;
	}

	const int z477b3f2 = !z3c35b6f->zdad73f6 || zacd238a();
	if (l1 > 0 && l2 > 0 && z477b3f2) {
		z82b6e54(0, z3c35b6f->z8d3029b, z9165167.zc4ac0ca, z3c35b6f->z37051f2, &z9165167.zdafe8ef, z3c35b6f->z8b1bda1, z3c35b6f->z8acd703);
		z82b6e54(1, z3c35b6f->z393d5c5, z9165167.zd1372c9, z3c35b6f->z37051f2, &z9165167.zdebfa6d, z3c35b6f->z8b1bda1, z3c35b6f->zfea5464);
		z5b74a62(0, z3c35b6f->z5ba4a3e, z3c35b6f->za884a39, z9165167.zc4ac0ca, z9165167.zdafe8ef);
		z5b74a62(1, z3c35b6f->z73d0ac3, z3c35b6f->zaac9881, z9165167.zd1372c9, z9165167.zdebfa6d);
		z4f1c6d2(0, z3c35b6f->zd930992);
		z4f1c6d2(1, z3c35b6f->zfd72437);
	}

	if (l1 > 0 && l1 < ze42b016) ze42b016 = l1;
	if (l2 > 0 && l2 < zd38b888) zd38b888 = l2;

	int zbc89174 = 0, zc28386e = 0;

	const int z458170e = z3c35b6f && (z3c35b6f->za9ac628 || z3c35b6f->z5c29d25 || z3c35b6f->zed018c9);
	int z1db9d9e = (l1 < 0) || (z458170e && l1 == 0 && l2 > 0);
	int z58b74c5 = (l2 < 0) || (z458170e && l2 == 0 && l1 > 0);
	if (z3c35b6f->z4e3ee48 && (zb271d66 || !zacd238a() || zaefff6f(z3c35b6f->nTimer) == 0x99 ||
	                       z8dcdb7e > z0936da4))
		z1db9d9e = z58b74c5 = 0;
	if (!z477b3f2) z1db9d9e = z58b74c5 = 0;

	int z176d922 = 0;
	if (z3c35b6f->zadec966) {
		const int w1 = zaefff6f(z3c35b6f->zadec966);
		const int w2 = zaefff6f(z3c35b6f->z1f3fb3d);
		if (zf48ab02 < 0) {
			zf48ab02 = w1; z5d651a9 = w2; z49f03e5 = 0;
		} else if (w1 == zf48ab02 && w2 == z5d651a9) {
			z49f03e5 = 0;
		} else {
			if (w1 == z30cdea0 && w2 == z1037ebf) z49f03e5++;
			else { z30cdea0 = w1; z1037ebf = w2; z49f03e5 = 1; }
			if (z49f03e5 >= z70b1c52) {
				if (z9165167.z2ed2c96 && z477b3f2) {
					if (w1 == zf48ab02 + 1) { z9165167.z726041c++; z9165167.nEndFrame = ze9bc394; zc28386e = 1; }
					if (w2 == z5d651a9 + 1) { z9165167.zae1a069++; z9165167.nEndFrame = ze9bc394; zbc89174 = 1; }
					if (w1 == 0 && w2 == 0 && (z9165167.z726041c || z9165167.zae1a069)) z176d922 = 1;
				} else if (z323ac1f && (w1 > zf48ab02 || w2 > z5d651a9)) {
					zaf4dd4f(g_log, "score: f%lld contador de rounds do jogo %d x %d -> %d x %d fora de uma luta (nao conta)",
					          ze9bc394, zf48ab02, z5d651a9, w1, w2);
				}
				zf48ab02 = w1; z5d651a9 = w2; z49f03e5 = 0;
			}
		}
		z1db9d9e = z58b74c5 = 0;
	}

	if (z1db9d9e) {
		if (z23e6cef < z70b1c52) z23e6cef++;
		if (z23e6cef >= z70b1c52 && !z93ce11c) {
			z93ce11c = 1;
			z9165167.zae1a069++;
			z9165167.nEndFrame = ze9bc394;
			zbc89174 = 1;
		}
	} else {
		z23e6cef = 0;
		if (l1 > 0) z93ce11c = 0;
	}

	if (z58b74c5) {
		if (zd19389f < z70b1c52) zd19389f++;
		if (zd19389f >= z70b1c52 && !z9e9683e) {
			z9e9683e = 1;
			z9165167.z726041c++;
			z9165167.nEndFrame = ze9bc394;
			zc28386e = 1;
		}
	} else {
		zd19389f = 0;
		if (l2 > 0) z9e9683e = 0;
	}

	if (z323ac1f && (zbc89174 || zc28386e))
		zaf4dd4f(g_log, "score: f%lld NOCAUTE %s  (vida %d / %d, na tela %d x %d) -> rounds %d x %d",
		          ze9bc394, (zbc89174 && zc28386e) ? "duplo" : zc28386e ? "do P1 (P2 caiu)" : "do P2 (P1 caiu)", l1, l2,
		          z3c35b6f->z5ba4a3e ? zaefff6f(z3c35b6f->z5ba4a3e) : -1,
		          z3c35b6f->z73d0ac3 ? zaefff6f(z3c35b6f->z73d0ac3) : -1,
		          z9165167.z726041c, z9165167.zae1a069);
	if (zbc89174 || zc28386e) za4fdb46 = 0;
	if (zbc89174 || zc28386e) {
		z9095135 = 1;
		z399002f(zafb1e7e, (zbc89174 && zc28386e) ? 0 : zc28386e ? 1 : 2);
	}

	if (z3c35b6f->nTimer && z9165167.z2ed2c96 && !z3c35b6f->zadec966) {
		const int t = zaefff6f(z3c35b6f->nTimer);
		if (t != 0) {
			if (!(zbc89174 || zc28386e)) za4fdb46 = 1;
			zb271d66 = 0;
			z6cbdaa2 = 0;
			z748e839 = z5042295 = -1;
		} else if (za4fdb46 && !zb271d66 && l1 > 0 && l2 > 0 && z477b3f2 && (!z3c35b6f->z4e3ee48 || zacd238a())) {
			int zee7eda1 = 0, z61c5299 = 0, z0eee1db = 0;
			if (z3c35b6f->z66158d8) {
				const int d1 = zaefff6f(z3c35b6f->z66158d8);
				const int d2 = zaefff6f(z3c35b6f->z3afb86c);
				if (z748e839 < 0) { z748e839 = d1; z5042295 = d2; }
				zee7eda1 = d1 > z748e839;
				z61c5299 = d2 > z5042295;
				if (!(zee7eda1 || z61c5299))                   z6cbdaa2 = 0;
				else if (++z6cbdaa2 >= z70b1c52) z0eee1db = 1;
			} else if (++z6cbdaa2 >= z70b1c52) {
				const int ze299900 = (l1 == l2) && !z3c35b6f->z4e3ee48;
				zee7eda1 = (l1 < l2 || ze299900);
				z61c5299 = (l2 < l1 || ze299900);
				z0eee1db = 1;
			}
			if (z0eee1db) {
				zb271d66  = 1;
				za4fdb46 = 0;
				if (zee7eda1) { z9165167.zae1a069++; zbc89174 = 1; }
				if (z61c5299) { z9165167.z726041c++; zc28386e = 1; }
				z9165167.nEndFrame = ze9bc394;
				z9095135 = 1;
				z399002f(zafb1e7e, (zbc89174 == zc28386e) ? 0 : zc28386e ? 1 : 2);
				if (z323ac1f)
					zaf4dd4f(g_log, "score: f%lld TEMPO ESGOTADO (vida %d / %d, na tela %d x %d%s): %s -> rounds %d x %d",
					          ze9bc394, l1, l2,
					          z3c35b6f->z5ba4a3e ? zaefff6f(z3c35b6f->z5ba4a3e) : -1,
					          z3c35b6f->z73d0ac3 ? zaefff6f(z3c35b6f->z73d0ac3) : -1,
					          z3c35b6f->z66158d8 ? ", pelo veredito do jogo" : "",
					          (zbc89174 && zc28386e) ? "empate, os dois caem" : zc28386e ? "o P1 leva" : zbc89174 ? "o P2 leva" : "empate, ninguem leva",
					          z9165167.z726041c, z9165167.zae1a069);
			}
		} else {
			z6cbdaa2 = 0;
		}
	}

	if ((zbc89174 || zc28386e) && z3c35b6f->z5ba4a3e && z3c35b6f->z73d0ac3 && zd58046c < z77b92c2) {
		z21b0d90* b = &ze4f6240[zd58046c++];
		b->zc4ac0ca = zaefff6f(z3c35b6f->z5ba4a3e) | zd2c8081(z3c35b6f->za884a39, 0, 0);
		b->zd1372c9 = zaefff6f(z3c35b6f->z73d0ac3) | zd2c8081(z3c35b6f->zaac9881, 0, 0);
		b->zdb52f45 = (zbc89174 && zc28386e) ? 0 : zc28386e ? 1 : 2;
	}

	if (z3c35b6f->z8b4af0c > 0 &&
	    (z9165167.z726041c >= z3c35b6f->z8b4af0c || z9165167.zae1a069 >= z3c35b6f->z8b4af0c) &&
	    !(z23e6cef > 0 && !z93ce11c) && !(zd19389f > 0 && !z9e9683e)) {
		const int z738d2b6 = z3c35b6f->z50efb64 && z9165167.z726041c == z9165167.zae1a069;
		if (z738d2b6 && !zcbd65b0) {
			zcbd65b0 = 1;
			if (z323ac1f)
				zaf4dd4f(g_log, "score: f%lld empate em %d x %d nos ultimos lutadores: o round FINAL decide a partida",
				          ze9bc394, z9165167.z726041c, z9165167.zae1a069);
		} else if (!z738d2b6 || zbc89174 || zc28386e) {
			z0695713();
			return;
		}
	}
	if (z176d922) {
		if (z323ac1f) zaf4dd4f(g_log, "score: f%lld os contadores do jogo voltaram a 0: a partida acaba em %d x %d",
		                         ze9bc394, z9165167.z726041c, z9165167.zae1a069);
		z0695713();
		return;
	}

	if (l1 == 0 && l2 == 0 && !z3c35b6f->z4e3ee48 && !z3c35b6f->z66158d8 && !z3c35b6f->zadec966) {
		if (z2000072 < z70b1c52) z2000072++;
		if (z2000072 >= z70b1c52 && !zd49138a) {
			zd49138a = 1;
			z0695713();
		}
	} else {
		z2000072 = 0;
		zd49138a = 0;
	}
}

int z5e8792b(za78fa53* out)
{
	if (!out) return 0;
	*out = z9165167;

	if (z3c35b6f && z3c35b6f->z5be7c34) {
		z23525cb(&out->z726041c, &out->zae1a069);
	}

	return z9165167.zd61d2b1 ? 1 : 0;
}

int ze949c69(void) { return z9165167.z1f29c92; }

void zd82e415(int* ze8821d1, int* z0f82ab8)
{
	if (ze8821d1) *ze8821d1 = z9165167.z45e0144;
	if (z0f82ab8) *z0f82ab8 = z9165167.z78ca99a;
}

void z9e6bdfe(void (*z930c6f2)(const char*))
{
	za41c2ab();
	if (!z3c35b6f) return;

	za78fa53 d;
	if (z5e8792b(&d)) {
		zaf4dd4f(z930c6f2, "score: %s  %d x %d  (%d partidas, %s)",
		          d.szGame, d.z45e0144, d.z78ca99a, d.z1903fc4,
		          d.z45e0144 > d.z78ca99a ? "P1 venceu"
		          : d.z78ca99a > d.z45e0144 ? "P2 venceu" : "empate");

		for (int i = 0; i < d.z535cef6; i++) {
			const z6ac2385* g = &d.z5400403[i];
			char z733a844[64], zd5e0939[64];
			z2b71de9(z733a844, sizeof(z733a844), g->zc4ac0ca, d.z37051f2, g->zdafe8ef);
			z2b71de9(zd5e0939, sizeof(zd5e0939), g->zd1372c9, d.z37051f2, g->zdebfa6d);
			zaf4dd4f(z930c6f2, "score:   %d. P1[%s] %d x %d P2[%s]  %s",
			          i + 1, z733a844, g->z726041c, g->zae1a069, zd5e0939,
			          g->zdb52f45 == 1 ? "P1 venceu" :
			          g->zdb52f45 == 2 ? "P2 venceu" : "empate");
		}
		if (d.z219e1e5)
			zaf4dd4f(z930c6f2, "score:   (sessao longa: so as primeiras %d partidas "
			                  "foram detalhadas)", z87df4c7);
		if (d.z726041c || d.zae1a069)
			zaf4dd4f(z930c6f2, "score:   partida interrompida no meio: %d x %d rounds",
			          d.z726041c, d.zae1a069);
	} else {
		zaf4dd4f(z930c6f2, "score: no fight started, nothing to report");
	}

	z3c35b6f = NULL;

	if (z20725d5[0]) {
		char szPath[128];
		zb533d50(szPath, sizeof(szPath));
		remove(szPath);
		z20725d5[0] = 0;
	}
}

int z9ae99d0(void) { return z3c35b6f != NULL; }

int z4f974fb(int* ze8821d1, int* z0f82ab8)
{
	int a = -1, b = -1;
	if (z3c35b6f) {
		const int n = (z9165167.z37051f2 > 0 && z9165167.z37051f2 <= z5cee153) ? z9165167.z37051f2 : 1;
		if (z9165167.zdafe8ef) a = z9165167.zc4ac0ca[z9165167.zae1a069 < n ? z9165167.zae1a069 : n - 1];
		if (z9165167.zdebfa6d) b = z9165167.zd1372c9[z9165167.z726041c < n ? z9165167.z726041c : n - 1];
	}
	if (ze8821d1) *ze8821d1 = a;
	if (z0f82ab8) *z0f82ab8 = b;
	return a >= 0 || b >= 0;
}

static void ze49c886(FILE* f, const char* szKey, const int* pChars, int nCount, int zab46fd3)
{
	if (!zab46fd3) return;
	fprintf(f, "%s=", szKey);
	for (int i = 0; i < nCount; i++) fprintf(f, i ? ",%d" : "%d", pChars[i]);
	fputc('\n', f);
}

static void zb8d3ccb(FILE* f, int nIndex, const z6ac2385* g, int z37051f2)
{
	fprintf(f, "partida%d=", nIndex);
	if (g->zdafe8ef) {
		fprintf(f, "p1chars=");
		for (int i = 0; i < z37051f2; i++) fprintf(f, i ? ",%d" : "%d", g->zc4ac0ca[i]);
		fputc(' ', f);
	}
	if (g->zdebfa6d) {
		fprintf(f, "p2chars=");
		for (int i = 0; i < z37051f2; i++) fprintf(f, i ? ",%d" : "%d", g->zd1372c9[i]);
		fputc(' ', f);
	}
	if (g->zd29aa1b > 0) {
		fprintf(f, "lutas=");
		for (int i = 0; i < g->zd29aa1b && i < z77b92c2; i++)
			fprintf(f, i ? ",%d-%d-%d" : "%d-%d-%d",
			        g->za9df5d5[i].zc4ac0ca, g->za9df5d5[i].zd1372c9, g->za9df5d5[i].zdb52f45);
		fputc(' ', f);
	}
	if (g->zfb4df8e) fprintf(f, "p1modo=%s ", g->zfb4df8e == z4d63468 ? "extra" : "advanced");
	if (g->zb21a368) fprintf(f, "p2modo=%s ", g->zb21a368 == z4d63468 ? "extra" : "advanced");
	fprintf(f, "rounds=%d-%d vencedor=%s frames=%d\n",
	        g->z726041c, g->zae1a069,
	        g->zdb52f45 == 1 ? "p1" : g->zdb52f45 == 2 ? "p2" : "empate",
	        g->nFrames);
}

int zb31ef22(const char* zb306bef, int ze6bfbdc, const char* z303e5ed,
                          void (*z930c6f2)(const char*))
{
	za78fa53 d;
	if (!z5e8792b(&d)) {
		zaf4dd4f(z930c6f2, "result: nenhuma luta comecou - nada a registrar");
		return -1;
	}
	if (!zb306bef || !zb306bef[0]) {
		zaf4dd4f(z930c6f2, "result: partida sem id - nada a registrar");
		return -1;
	}

	char zaa1e328[48];
	if (!z961cbec(zb306bef, zaa1e328, sizeof(zaa1e328))) return -1;

	char szPath[128];
	snprintf(szPath, sizeof(szPath), "rbf-result-%s.txt", zaa1e328);

	FILE* f = fopen(szPath, "wb");
	if (!f) {
		zaf4dd4f(z930c6f2, "result: nao consegui escrever %s", szPath);
		return -1;
	}
	if (z45febb2) zaf4dd4f(z930c6f2, "score: f%lld testemunha: fim da sessao", ze9bc394 + z495ea96);

	fprintf(f, "match=%s\n",  zaa1e328);
	fprintf(f, "game=%s\n",   d.szGame);
	fprintf(f, "p1games=%d\n", d.z45e0144);
	fprintf(f, "p2games=%d\n", d.z78ca99a);
	fprintf(f, "games=%d\n",   d.z1903fc4);
	fprintf(f, "firstto=%d\n", ze6bfbdc);
	fprintf(f, "reason=%s\n",  z303e5ed ? z303e5ed : "closed");
	{
		const int z003e6c6 = (z5cd9a92 ? z5cd9a92 : zf7cdc79()) | z6e9fbde;
		if (z003e6c6) fprintf(f, "ausente=%d\n", z003e6c6);
	}
	for (int s = 1; s <= 2; s++) {
		const int t = z709ab7c(s, zc4d6e9b), m = z709ab7c(s, z0f4913a);
		if (t) fprintf(f, "p%dturbo=%d\n", s, t);
		if (m) fprintf(f, "p%dmacro=%d\n", s, m);
	}
	fprintf(f, "chars=%d\n",   d.z37051f2);
	ze49c886(f, "p1chars", d.zc4ac0ca, d.z37051f2, d.zdafe8ef);
	ze49c886(f, "p2chars", d.zd1372c9, d.z37051f2, d.zdebfa6d);

	for (int i = 0; i < d.z535cef6; i++)
		zb8d3ccb(f, i + 1, &d.z5400403[i], d.z37051f2);
	if (d.z219e1e5)
		fprintf(f, "truncado=1\n");
	if (d.z726041c || d.zae1a069)
		fprintf(f, "parcial=rounds=%d-%d\n", d.z726041c, d.zae1a069);
	fclose(f);

	zaf4dd4f(z930c6f2, "result: %s escrito (%d x %d, %d partidas detalhadas, %s)",
	          szPath, d.z45e0144, d.z78ca99a, d.z535cef6,
	          z303e5ed ? z303e5ed : "closed");
	return 0;
}
