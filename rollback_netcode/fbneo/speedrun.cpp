#include "burnint.h"
#include "speedrun.h"
#include "ram_probe.h"

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

extern "C" void z5d90bae(const char* s);

typedef struct {
	const char* szGame;
	double      z83da8b4;
	const char* szCategory;
	const char* szLabel;
	void      (*z52aceb1)(void);
} z636e541;

static int       z58890ec   = 0;
static int       z9c65e87      = 0;
static int       z910e658        = 0;
static int       z4eb792f   = 1;
static int       z917b0d4    = 0;
static const z636e541* z716dbe5  = NULL;

#define z892f642         0x01060A30u
#define z9581198         0x010615E0u

#define zc2bccf2 8
#define z30b7fad  224

typedef struct {
	int       state;
	long long frame;
	long long start;
	long long end;
	int       c1, c2;
	char      fighter[48];
	unsigned long long zd57a547, z17f26b3;
	unsigned int finishes;
	unsigned int zfc44818;
	char      ev[zc2bccf2][z30b7fad];
	struct { int z284dc91, zebd06b0; unsigned int block, me, z154a515; } umk3;
	struct { int z6c14296, zb36dd9a; } ffight;
	struct { int prevMask, zb36dd9a; } captcomm;
	struct { int prev1, prev2, zb36dd9a, fills, slot; unsigned char full[80]; } mslugx;
	struct { int prev1, prev2, z07fa7ac, zc469cc1, z89eaed3; } dino;
	struct { int prev1, prev2, named, zb36dd9a; } avsp;
	struct { int prev, z350ecff; } ddragonj;
	struct { int prev1, prev2, zb36dd9a, slot; } mslug3;
	struct { int z64ed941, z3e97c45; int zb36dd9a[46]; } punisher;
} z61b46b8;

static z61b46b8 z7f80fd4(void)
{
	z61b46b8 s;
	memset(&s, 0, sizeof(s));
	s.start = s.end = -1;
	s.c1 = s.c2 = -1;
	s.umk3.z284dc91 = s.umk3.zebd06b0 = -1;
	s.umk3.me = z892f642;
	s.umk3.z154a515 = z9581198;
	s.ffight.z6c14296 = -1;
	s.dino.z89eaed3 = -1;
	s.ddragonj.prev = s.ddragonj.z350ecff = -1;
	return s;
}

static z61b46b8 zcd54cd2 = z7f80fd4();

static int          z8bb2aa4      = 0;
static unsigned int zb9626bb  = 0;
static unsigned int z2d0bf63 = 0;

static unsigned long long z5f653c0(void)
{
	unsigned int len = 0, cpu = 0;
	const unsigned char* p = zbc4b3c3(&len, &cpu);
	unsigned long long h = 1469598103934665603ULL;
	for (unsigned int i = 0; p && i < len; i++) { h ^= p[i]; h *= 1099511628211ULL; }
	return h;
}

#define z48910c9 "rbf-speedrun.txt"

static void zece5d43(const char* fmt, ...)
{
	char buf[256];
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	buf[sizeof(buf) - 1] = 0;
	z5d90bae(buf);
}

static void z1c02c63(const char* fmt, ...)
{
	char buf[z30b7fad];
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	buf[sizeof(buf) - 1] = 0;
	if (!z8bb2aa4) { z5d90bae(buf); return; }
	char* dst = zcd54cd2.ev[zcd54cd2.zfc44818 % zc2bccf2];
	memset(dst, 0, z30b7fad);
	memcpy(dst, buf, strlen(buf));
	zcd54cd2.zfc44818++;
}

static long long zef7c525(const z61b46b8* s)
{
	if (s->state == 2) return s->end - s->start;
	if (s->state == 1 && s->frame >= s->start) return s->frame - s->start;
	return 0;
}

static int z45be80c(const z61b46b8* s, char* sz, int n)
{
	if (!sz || n <= 0) return 0;
	sz[0] = 0;
	if (!z910e658 || s->state == 0) return 0;
	const long long cs = (long long)(zef7c525(s) * 100.0 / zf26f0e1());
	const long long sec = cs / 100;
	if (sec >= 60) snprintf(sz, n, "%lld:%02lld.%02lld", sec / 60, sec % 60, cs % 100);
	else           snprintf(sz, n, "%lld.%02lld", sec, cs % 100);
	return 1;
}

static void zb71fbd5(const z61b46b8* s)
{
	if (z917b0d4) return;
	FILE* f = fopen(z48910c9, "w");
	if (!f) { zece5d43("speedrun: nao deu para gravar %s", z48910c9); return; }
	char t[32];
	z45be80c(s, t, sizeof(t));
	fprintf(f, "game=%s\n", BurnDrvGetTextA(DRV_NAME));
	fprintf(f, "category=%s\n", z716dbe5 ? z716dbe5->szCategory : "any");
	fprintf(f, "frames=%lld\n", zef7c525(s));
	const long long micro = (long long)(zf26f0e1() * 1000000.0 + 0.5);
	fprintf(f, "fps=%lld.%06lld\n", micro / 1000000, micro % 1000000);
	fprintf(f, "time=%s\n", t);
	fprintf(f, "players=%d\n", z4eb792f);
	if (z4eb792f == 2) fprintf(f, "chars=%d,%d\n", s->c1, s->c2);
	else                fprintf(f, "chars=%d\n", s->c1);
	fprintf(f, "fighter=%s\n", s->fighter);
	fprintf(f, "start_hash=%016llx\n", s->zd57a547);
	fprintf(f, "end_hash=%016llx\n", s->z17f26b3);
	fclose(f);
	zece5d43("speedrun: resultado em %s", z48910c9);
}

static void zf0c2961(long long nStart, int c1, int c2, const char* zda76043)
{
	zcd54cd2.state = 1;
	zcd54cd2.start = nStart;
	zcd54cd2.end = -1;
	zcd54cd2.c1 = c1;
	zcd54cd2.c2 = c2;
	strncpy(zcd54cd2.fighter, zda76043 ? zda76043 : "", sizeof(zcd54cd2.fighter) - 1);
	zcd54cd2.fighter[sizeof(zcd54cd2.fighter) - 1] = 0;
	z1c02c63("speedrun: comecou no quadro %lld (%s)", zcd54cd2.start, zcd54cd2.fighter);
}

static void z8f2d77c(const char* z94ecf24)
{
	if (zcd54cd2.state != 1) return;
	zcd54cd2.state = 0;
	z1c02c63("speedrun: corrida abandonada (%s)", z94ecf24 ? z94ecf24 : "?");
}

static void z7c5f6ee(void)
{
	if (zcd54cd2.state != 1) return;
	zcd54cd2.state = 2;
	zcd54cd2.end = zcd54cd2.frame;
	zcd54cd2.z17f26b3 = z5f653c0();
	char t[32];
	z45be80c(&zcd54cd2, t, sizeof(t));
	z1c02c63("speedrun: terminou no quadro %lld - %s (%lld quadros, %s) end_hash %016llx", zcd54cd2.frame, t,
	        zcd54cd2.end - zcd54cd2.start, zcd54cd2.fighter, zcd54cd2.z17f26b3);
	zcd54cd2.finishes++;
	if (!z8bb2aa4) zb71fbd5(&zcd54cd2);
}

static void ze14aa7c(void)
{
	if (zcd54cd2.state == 1 && zcd54cd2.frame == zcd54cd2.start) {
		zcd54cd2.zd57a547 = z5f653c0();
		z1c02c63("speedrun: start_hash %016llx (quadro %lld)", zcd54cd2.zd57a547, zcd54cd2.frame);
	}
}

#define z5bd9f8a       0x00
#define zd56d024       0x30
#define z97547db       0x60
#define ze042475      0x01062FB0u
#define z3dec5ec     3
#define zb4256e6       25
#define z76bb4d5     4
#define z098597b      0x0106A0A0u
#define zbfbbac9     0x00
#define z28a90c1      0x60
#define z5e26ce9     0xE0
#define z5905928 0xFFA83E20u
#define z4e9b7c4 0xFFA842E0u
#define zeaa8ff4    0x01400000u

static const char* const z78c88b7[] = {
	"Kano", "Sonya", "Jax", "Nightwolf", "Sub-Zero", "Stryker", "Sindel", "Sektor",
	"Cyrax", "Kung Lao", "Kabal", "Sheeva", "Shang Tsung", "Liu Kang", "Smoke", "Kitana",
	"Jade", "Mileena", "Scorpion", "Reptile", "Ermac", "Classic Sub-Zero", "Human Smoke",
	"Noob Saibot",
};

static int z6811dd4(unsigned int a)
{
	return zaefff6f(a) | (zaefff6f(a + 8) << 8);
}

static unsigned int z755c37f(unsigned int a)
{
	return (unsigned int)z6811dd4(a) | ((unsigned int)z6811dd4(a + 16) << 16);
}

static int z0fad033(unsigned int* pBlock)
{
	unsigned int p = z755c37f(z098597b);
	for (int n = 0; n < 64 && p; n++) {
		if (p < 0x01000000u || p >= zeaa8ff4 || (p & 0xF)) return -1;
		if (z755c37f(p + z5e26ce9) == z5905928) {
			const unsigned int c = z755c37f(p + z28a90c1);
			*pBlock = p;
			return c <= 3 ? (int)c : -1;
		}
		p = z755c37f(p + zbfbbac9);
	}
	return -1;
}

static void z11f9fe8(void)
{
	unsigned int block = 0;
	const int cur = z0fad033(&block);

	if (zcd54cd2.umk3.z284dc91 >= 0 && cur < 0 && z755c37f(zcd54cd2.umk3.block + z5e26ce9) == z4e9b7c4) {
		const int tower = z6811dd4(ze042475);
		if (tower == z3dec5ec) {
			const int c1 = z6811dd4(z892f642 + z5bd9f8a), c2 = z6811dd4(z9581198 + z5bd9f8a);
			const int z0a553c8 = (int)(sizeof(z78c88b7) / sizeof(z78c88b7[0]));
			const int p2 = !(c1 >= 0 && c1 < z0a553c8) && (c2 >= 0 && c2 < z0a553c8);
			zcd54cd2.umk3.me  = p2 ? z9581198 : z892f642;
			zcd54cd2.umk3.z154a515 = p2 ? z892f642 : z9581198;
			const int c = p2 ? c2 : c1;
			char zde4da44[32];
			if (c >= 0 && c < z0a553c8) snprintf(zde4da44, sizeof(zde4da44), "%s", z78c88b7[c]);
			else snprintf(zde4da44, sizeof(zde4da44), "#%d", c);
			zcd54cd2.umk3.zebd06b0 = -1;
			zf0c2961(zcd54cd2.frame + z76bb4d5, c, -1, zde4da44);
			if (z8bb2aa4) z1c02c63("speedrun: MASTER escolhida pelo %s no quadro %lld", p2 ? "P2" : "P1", zcd54cd2.frame);
			else      z1c02c63("speedrun: MASTER escolhida pelo %s no quadro %lld (imagem %u)", p2 ? "P2" : "P1",
			                  zcd54cd2.frame, (unsigned)nCurrentFrame);
		} else {
			z8f2d77c("outra torre");
		}
	}
	zcd54cd2.umk3.z284dc91 = cur;
	zcd54cd2.umk3.block = block;

	if (zcd54cd2.state != 1 || zcd54cd2.frame < zcd54cd2.start) return;
	ze14aa7c();

	const int z154a515  = z6811dd4(zcd54cd2.umk3.z154a515 + z5bd9f8a);
	const int life = z6811dd4(zcd54cd2.umk3.z154a515 + zd56d024);
	if (z154a515 == zb4256e6 && zcd54cd2.umk3.zebd06b0 > 0 && life == 0 && z6811dd4(zcd54cd2.umk3.me + z97547db) == 1) z7c5f6ee();
	zcd54cd2.umk3.zebd06b0 = (z154a515 == zb4256e6) ? life : -1;
}

static int zd3baff0(unsigned int a) { return (zaefff6f(a) << 8) | zaefff6f(a + 1); }
static int z3990601(unsigned int a) { const int v = zd3baff0(a); return v >= 0x8000 ? v - 0x10000 : v; }

static void za550b95(int c1, int c2, const char* const* names, int z0a553c8)
{
	if (zcd54cd2.state == 1) return;
	char zde4da44[48];
	const char* n1 = (c1 >= 0 && c1 < z0a553c8) ? names[c1] : "?";
	if (z4eb792f == 2) {
		const char* n2 = (c2 >= 0 && c2 < z0a553c8) ? names[c2] : "?";
		snprintf(zde4da44, sizeof(zde4da44), "%s + %s", n1, n2);
	} else {
		snprintf(zde4da44, sizeof(zde4da44), "%s", n1);
	}
	zf0c2961(zcd54cd2.frame, c1, z4eb792f == 2 ? c2 : -1, zde4da44);
	ze14aa7c();
}

static void zcc97854(int c1, int c2, const char* const* names, int z0a553c8)
{
	zcd54cd2.c1 = c1;
	zcd54cd2.c2 = z4eb792f == 2 ? c2 : -1;
	const char* n1 = (c1 >= 0 && c1 < z0a553c8) ? names[c1] : "?";
	if (z4eb792f == 2) {
		const char* n2 = (c2 >= 0 && c2 < z0a553c8) ? names[c2] : "?";
		snprintf(zcd54cd2.fighter, sizeof(zcd54cd2.fighter), "%s + %s", n1, n2);
	} else {
		snprintf(zcd54cd2.fighter, sizeof(zcd54cd2.fighter), "%s", n1);
	}
	z1c02c63("speedrun: personagens %s", zcd54cd2.fighter);
}

static void z136ffc7(int zd4dbd73, unsigned int zfad7f3e, int* z577728c)
{
	const int hp = z3990601(zfad7f3e);
	if (zd4dbd73 && *z577728c >= 0 && hp < 0) z7c5f6ee();
	*z577728c = zd4dbd73 ? hp : 0;
}

static const char* const z44518f0[] = { "Guy", "Cody", "Haggar" };
static void zacc713b(void)
{
	const int mode = zaefff6f(0xFF1517);
	if (zcd54cd2.ffight.z6c14296 == 2 && mode == 4) za550b95(zaefff6f(0xFF857C), zaefff6f(0xFF863C), z44518f0, 3);
	zcd54cd2.ffight.z6c14296 = mode;
	if (zcd54cd2.state != 1) return;
	z136ffc7(zaefff6f(0xFF80BE) == 5 && zaefff6f(0xFF9A7B) == 5, 0xFF9A80, &zcd54cd2.ffight.zb36dd9a);
}

static const char* const z96ed72b[] = { "Mack the Knife", "Captain Commando", "Ginzu the Ninja", "Baby Head" };
static void zdd725f1(void)
{
	const int mask = zaefff6f(0xFF9278);
	if (zcd54cd2.captcomm.prevMask != 0 && mask == 0 && zaefff6f(0xFFA997) == 4)
		za550b95(zaefff6f(0xFFAA34), zaefff6f(0xFFAB34), z96ed72b, 4);
	zcd54cd2.captcomm.prevMask = mask;
	if (zcd54cd2.state != 1) return;
	z136ffc7(zaefff6f(0xFFA8CE) == 8 && zaefff6f(0xFFCAEA) == 0x11, 0xFFCB10, &zcd54cd2.captcomm.zb36dd9a);
}

#define z5d076d2    0x102C72
#define z37c7d3b   0xB0
#define z7c5cb36     (-33)
#define z71e64d9  80
static const char* const zea7d1fb[] = { "Marco", "Tarma", "Eri", "Fio" };
static int z125f091(int i) { return zd3baff0(z5d076d2 + (z7c5cb36 + i) * z37c7d3b); }
static void zfe9ab50(void)
{
	zcd54cd2.mslugx.fills = 0;
	zcd54cd2.mslugx.slot = 0;
	zcd54cd2.mslugx.zb36dd9a = 0;
	memset(zcd54cd2.mslugx.full, 0, sizeof(zcd54cd2.mslugx.full));
}
static void zf195d92(void)
{
	const int st1 = zaefff6f(0x101288), st2 = zaefff6f(0x101338);
	const int picked = (zcd54cd2.mslugx.prev1 == 3 && st1 == 2) || (zcd54cd2.mslugx.prev2 == 3 && st2 == 2);
	if (picked && st1 != 3 && st2 != 3 && (st1 != 0 || st2 != 0)) {
		if (zcd54cd2.state != 1) zfe9ab50();
		za550b95(zaefff6f(0x101286), zaefff6f(0x101336), zea7d1fb, 4);
	}
	zcd54cd2.mslugx.prev1 = st1; zcd54cd2.mslugx.prev2 = st2;
	if (zcd54cd2.state != 1) return;
	if (zaefff6f(0x10C2F5) != 5) { zfe9ab50(); return; }
	for (int i = 0; i < z71e64d9; i++) {
		if (z125f091(i) == 6500) {
			if (!zcd54cd2.mslugx.full[i]) {
				zcd54cd2.mslugx.fills++;
				zcd54cd2.mslugx.slot = i + 1;
				zcd54cd2.mslugx.zb36dd9a = 6500;
			}
			zcd54cd2.mslugx.full[i] = 1;
		} else {
			zcd54cd2.mslugx.full[i] = 0;
		}
	}
	if (!zcd54cd2.mslugx.slot) return;
	const int hp = z125f091(zcd54cd2.mslugx.slot - 1);
	if (zcd54cd2.mslugx.fills >= 2 && zcd54cd2.mslugx.zb36dd9a > 0 && hp == 0) z7c5f6ee();
	zcd54cd2.mslugx.zb36dd9a = hp;
}

static const char* const zc0a1199[] = { "Jack", "Mustapha", "Hannah", "Mess" };
static void z1b30d2a(void)
{
	const int c1 = zaefff6f(0xFFB2E6), c2 = zaefff6f(0xFFB466);
	const int ze55fb96 = zaefff6f(0xFFB3F4) != 0;
	if (zaefff6f(0xFFB274) == 0 && !ze55fb96) zcd54cd2.dino.z07fa7ac = zcd54cd2.dino.zc469cc1 = 0;
	if (zcd54cd2.dino.prev1 == 0 && c1 == 0x3C) zcd54cd2.dino.z07fa7ac = 1;
	if (zcd54cd2.dino.prev2 == 0 && c2 == 0x3C) zcd54cd2.dino.zc469cc1 = 1;
	if (((zcd54cd2.dino.prev1 == 0 && c1 == 0x3C) || (zcd54cd2.dino.prev2 == 0 && c2 == 0x3C)) && zcd54cd2.dino.z07fa7ac &&
	    (zcd54cd2.dino.zc469cc1 || !ze55fb96))
		za550b95(zaefff6f(0xFFB277), zaefff6f(0xFFB3F7), zc0a1199, 4);
	zcd54cd2.dino.prev1 = c1; zcd54cd2.dino.prev2 = c2;
	const int st = zaefff6f(0xFF84D6);
	if (zcd54cd2.state == 1 && zaefff6f(0xFF84D9) == 7 && st == 4 && zcd54cd2.dino.z89eaed3 != 4 && zcd54cd2.dino.z89eaed3 >= 0)
		z7c5f6ee();
	zcd54cd2.dino.z89eaed3 = st;
}

static const char* const z4f56473[] = { "Predator Warrior", "Schaefer", "Predator Hunter", "Linn Kurosawa" };
static void z98cace1(void)
{
	const int k1 = zaefff6f(0xFF8383), k2 = zaefff6f(0xFF8483);
	const int who = zaefff6f(0xFF81BF);
	const int conf = (zcd54cd2.avsp.prev1 == 0 && k1 == 1) || (zcd54cd2.avsp.prev2 == 0 && k2 == 1);
	if (conf && (!(who & 1) || k1 == 1) && (!(who & 2) || k2 == 1)) {
		zcd54cd2.avsp.named = 0;
		za550b95(-1, -1, z4f56473, 4);
	}
	zcd54cd2.avsp.prev1 = k1; zcd54cd2.avsp.prev2 = k2;
	if (zcd54cd2.state != 1) return;
	if (!zcd54cd2.avsp.named && zcd54cd2.frame - zcd54cd2.start > 450 && zaefff6f(0xFF8380) != 0) {
		zcd54cd2.avsp.named = 1;
		zcc97854(zaefff6f(0xFF83E4), zaefff6f(0xFF84E4), z4f56473, 4);
	}
	z136ffc7(zaefff6f(0xFF81E9) == 0x24 && zaefff6f(0xFF94C0) != 0 && zaefff6f(0xFF94C2) == 5,
	              0xFF9500, &zcd54cd2.avsp.zb36dd9a);
}

static const char* const z37ebac5[] = { "Billy", "Jimmy" };
static void zad6f005(void)
{
	const int st = zaefff6f(0x0E30);
	if (zcd54cd2.ddragonj.prev == 2 && st == 3) za550b95(0, 1, z37ebac5, 2);
	zcd54cd2.ddragonj.prev = st;
	const int area = zaefff6f(0x0E2D);
	if (zcd54cd2.state == 1 && zaefff6f(0x0036) == 3 && zcd54cd2.ddragonj.z350ecff == 0 && area != 0) z7c5f6ee();
	zcd54cd2.ddragonj.z350ecff = area;
}

#define z51cb3e7  0x1012BC
#define z43351ac     (-19)
#define z7a78cc1  80
static unsigned int zb4d7e66(int i) { return (unsigned int)(z51cb3e7 + (z43351ac + i) * 0xB0); }
static int z78a376c(unsigned int s) { return zd3baff0(s + 0x50) == 0x0009 && zd3baff0(s + 0x52) == 0xF40E; }
static void ze5cdb2e(void)
{
	const int st1 = zaefff6f(0x1002BE), st2 = zaefff6f(0x10036E);
	const int picked = (zcd54cd2.mslug3.prev1 == 1 && st1 == 2) || (zcd54cd2.mslug3.prev2 == 1 && st2 == 2);
	if (picked && st1 != 1 && st2 != 1 && (st1 != 0 || st2 != 0))
		za550b95(zaefff6f(0x1002BA), zaefff6f(0x10036A), zea7d1fb, 4);
	zcd54cd2.mslug3.prev1 = st1; zcd54cd2.mslug3.prev2 = st2;
	if (zcd54cd2.state != 1) return;
	if (zaefff6f(0x10ED7A) != 4 || zaefff6f(0x10040D) != 0x0E) {
		zcd54cd2.mslug3.slot = 0;
		zcd54cd2.mslug3.zb36dd9a = 0;
		return;
	}
	if (!zcd54cd2.mslug3.slot || !z78a376c(zb4d7e66(zcd54cd2.mslug3.slot - 1))) {
		zcd54cd2.mslug3.slot = 0;
		zcd54cd2.mslug3.zb36dd9a = 0;
		for (int i = 0; i < z7a78cc1; i++) {
			if (z78a376c(zb4d7e66(i))) {
				zcd54cd2.mslug3.slot = i + 1;
				z1c02c63("speedrun: Rootmars na vaga k=%d (0x%X) no quadro %lld", z43351ac + i, zb4d7e66(i), zcd54cd2.frame);
				break;
			}
		}
		if (!zcd54cd2.mslug3.slot) return;
	}
	const unsigned int s = zb4d7e66(zcd54cd2.mslug3.slot - 1);
	const int hp = zd3baff0(s + 0x4E);
	if (zaefff6f(s + 0x6E) == 0xFF && zcd54cd2.mslug3.zb36dd9a > 0 && hp == 0) z7c5f6ee();
	zcd54cd2.mslug3.zb36dd9a = hp;
}

static const char* const z2b12c50[] = { "Punisher", "Nick Fury" };
static void zd1c5ad7(void)
{
	int* const zb36dd9a = zcd54cd2.punisher.zb36dd9a;
	const int cnt = zaefff6f(0xFF54C5), who = zaefff6f(0xFF5B3D);
	if (z4eb792f == 2) {
		if (cnt == 1 && who == 3 && !(zcd54cd2.punisher.z64ed941 == 1 && zcd54cd2.punisher.z3e97c45 == 3)) {
			za550b95(0, 1, z2b12c50, 2);
			if (zcd54cd2.state == 1 && zcd54cd2.punisher.z64ed941 == 0 && zcd54cd2.start == zcd54cd2.frame)
				zcd54cd2.start = zcd54cd2.frame - 1;
		}
	} else if (zcd54cd2.punisher.z64ed941 == 0 && cnt == 1) {
		za550b95((who & 1) ? 0 : 1, -1, z2b12c50, 2);
		if (zcd54cd2.state == 1 && zcd54cd2.start == zcd54cd2.frame) zcd54cd2.start = zcd54cd2.frame - 1;
	}
	zcd54cd2.punisher.z64ed941 = cnt; zcd54cd2.punisher.z3e97c45 = who;
	if (zcd54cd2.state != 1) return;
	const int last = zaefff6f(0xFF5BB9) == 5;
	for (int k = 0; k < 46; k++) {
		const unsigned int o = 0xFFAE68 + k * 0xC0;
		if (zd3baff0(o + 2) != 0x1100) { zb36dd9a[k] = 0; continue; }
		const int hp = z3990601(o + 0x36);
		if (last && zb36dd9a[k] >= 0 && hp < 0) { z7c5f6ee(); return; }
		zb36dd9a[k] = hp;
	}
}

static const z636e541 z76a1a90[] = {
	{ "umk3",     54.706840, "master", "UMK3, torre MASTER",                   z11f9fe8 },
	{ "ffight",   59.63,     "any",    "Final Fight, ate o Belger",             zacc713b },
	{ "captcomm", 59.63,     "any",    "Captain Commando, ate o Scumocide",     zdd725f1 },
	{ "mslugx",   59.185606, "any",    "Metal Slug X, ate a nave-mae",          zf195d92 },
	{ "dino",     59.63,     "any",    "Cadillacs, ate o Dr. Fessenden",        z1b30d2a },
	{ "avsp",     59.63,     "any",    "Alien vs. Predator, ate a Rainha",      z98cace1 },
	{ "ddragonj", 57.444853, "any",    "Double Dragon, ate o Willy",            zad6f005 },
	{ "mslug3",   59.185606, "any",    "Metal Slug 3, ate o Rootmars",          ze5cdb2e },
	{ "punisher", 59.63,     "any",    "The Punisher, ate o Kingpin",           zd1c5ad7 },
};

static void z04ffa01(const char* z94ecf24)
{
	if (zcd54cd2.state == 0) return;
	if (zcd54cd2.state == 1) z1c02c63("speedrun: corrida desfeita (%s)", z94ecf24);
	zcd54cd2.state = 0;
	zcd54cd2.start = zcd54cd2.end = -1;
}

static int z29a52d5(void)
{
	static UINT8* zdeb4c4a = NULL;
	static int z2088f56 = 0;
	if (!z2088f56) {
		z2088f56 = 1;
		struct BurnInputInfo bii;
		for (UINT32 i = 0; BurnDrvGetInputInfo(&bii, i) == 0; i++) {
			if (bii.nType != BIT_DIGITAL || bii.pVal == NULL) continue;
			if ((bii.szName && _stricmp(bii.szName, "Reset") == 0) || (bii.szInfo && _stricmp(bii.szInfo, "reset") == 0)) {
				zdeb4c4a = bii.pVal;
				break;
			}
		}
	}
	return zdeb4c4a && *zdeb4c4a;
}

static long long z53d739a      = -1;
static UINT8*    z0f3fcce = NULL;

static void zb0624bc(void)
{
	char v[96] = "";
	if (!GetEnvironmentVariableA("RBF_TESTE_FIM", v, sizeof(v)) || !v[0]) return;
	z53d739a = atoll(v);
	const char* name = strchr(v, ',');
	if (name) {
		name++;
		struct BurnInputInfo bii;
		for (UINT32 i = 0; BurnDrvGetInputInfo(&bii, i) == 0; i++)
			if (bii.nType == BIT_DIGITAL && bii.pVal && bii.szName && _stricmp(bii.szName, name) == 0) {
				z0f3fcce = bii.pVal;
				break;
			}
		if (!z0f3fcce) { zece5d43("teste: RBF_TESTE_FIM - a entrada \"%s\" nao existe; ignorado", name); z53d739a = -1; return; }
	}
	zece5d43("teste: RBF_TESTE_FIM - a corrida termina no quadro %lld%s%s", z53d739a,
	         z0f3fcce ? " ou depois, com " : "", z0f3fcce ? name : "");
}

static void z9ef9a9b(void)
{
	if (z53d739a < 0 || zcd54cd2.state != 1 || zcd54cd2.frame < z53d739a) return;
	if (z0f3fcce ? (*z0f3fcce != 0) : (zcd54cd2.frame == z53d739a)) z7c5f6ee();
}

void z48f734b(void) { z04ffa01("F3 do anfitriao"); }

static int z225d108(const char* cmd, const char* szArg, const char* szKey, const char* szVal)
{
	if (!cmd || !szArg) return 0;
	const char* p = strstr(cmd, szArg);
	if (!p) return 0;
	p += strlen(szArg);
	while (*p == ' ') p++;
	char list[512];
	int n = 0;
	while (p[n] && p[n] != ' ' && n < (int)sizeof(list) - 1) { list[n] = p[n]; n++; }
	list[n] = 0;
	const size_t k = strlen(szKey), v = strlen(szVal);
	for (char* t = strtok(list, ","); t; t = strtok(NULL, ",")) {
		if (strncmp(t, szKey, k) == 0 && t[k] == '=' && strncmp(t + k + 1, szVal, v) == 0 && t[k + 1 + v] == 0)
			return 1;
	}
	return 0;
}

static const char* z22f8e00(const char* cmd)
{
	return strstr(cmd, "-rbfwatch ") ? "-rbfwatch " : "-rbfreplay ";
}

static void z7ea44a9(void)
{
	if (z9c65e87) return;
	z9c65e87 = 1;
	const char* cmd = GetCommandLineA();
	if (!cmd || !strstr(cmd, "-rbfspeedrun")) return;
	const char* name = BurnDrvGetTextA(DRV_NAME);
	for (unsigned i = 0; name && i < sizeof(z76a1a90) / sizeof(z76a1a90[0]); i++)
		if (strcmp(name, z76a1a90[i].szGame) == 0) z716dbe5 = &z76a1a90[i];
	if (!z716dbe5) {
		zece5d43("speedrun: %s ainda nao tem speedrun", name ? name : "?");
		return;
	}
	if (zfc8fae7(z5d90bae) != z7e634bf) {
		zece5d43("speedrun: sem acesso a RAM do jogo - desligado");
		return;
	}
	z910e658 = 1;
	z917b0d4 = (strstr(cmd, "-rbfwatch ") || strstr(cmd, "-rbfreplay ")) ? 1 : 0;
	if (z917b0d4)                    z4eb792f = z225d108(cmd, z22f8e00(cmd), "corrida", "2") ? 2 : 1;
	else if (strstr(cmd, "-rbfnet ")) z4eb792f = z225d108(cmd, "-rbfnet ", "solo", "1") ? 1 : 2;
	else                             z4eb792f = 1;
	if (GetEnvironmentVariableA("RBF_TESTE_DUPLA", NULL, 0)) z4eb792f = 2;
	if (!z917b0d4) remove(z48910c9);
	zece5d43("speedrun: %s, %.5f quadros/s, %d jogador(es)%s", z716dbe5->szLabel, zf26f0e1(), z4eb792f,
	         z917b0d4 ? " (assistindo)" : "");
	zb0624bc();
}

double zf26f0e1(void) { return z716dbe5 ? z716dbe5->z83da8b4 : (nBurnFPS > 0 ? nBurnFPS / 100.0 : 60.0); }
int    z59b4fb8(void) { z7ea44a9(); return z910e658; }
int    zb114d6e(void) { return zcd54cd2.state; }
const char* z414a8e9(void) { return zcd54cd2.fighter; }

long long zb471de2(void) { return z910e658 ? zcd54cd2.frame : 0; }

long long zf3bf8e1(void) { return zef7c525(&zcd54cd2); }

static void za63bbf5(void)
{
	zcd54cd2.frame++;
	if (z29a52d5()) {
		z04ffa01("F3");
		return;
	}
	z716dbe5->z52aceb1();
	z9ef9a9b();
}

void z818176f(void)
{
	z7ea44a9();
	if (!z910e658 || z8bb2aa4) return;
	za63bbf5();
}

void z2e9e89c(void)
{
	z7ea44a9();
	if (!z910e658) return;
	za63bbf5();
}

void zf299840(int bOn)
{
	z8bb2aa4      = bOn ? 1 : 0;
	zb9626bb  = zcd54cd2.zfc44818;
	z2d0bf63 = zcd54cd2.finishes;
}

int  z3a3ab4d(void) { return (int)sizeof(z61b46b8); }
void z66855d5(void* pOut) { if (pOut) memcpy(pOut, &zcd54cd2, sizeof(z61b46b8)); }
void z856caea(const void* pIn) { if (pIn) memcpy(&zcd54cd2, pIn, sizeof(z61b46b8)); }

static void za4a447b(const z61b46b8* s, char* sz, int n)
{
	if (s->state == 1)      snprintf(sz, n, "correndo desde o quadro %lld", s->start);
	else if (s->state == 2) snprintf(sz, n, "terminou (quadros %lld a %lld)", s->start, s->end);
	else                    snprintf(sz, n, "parado");
}

int z40c67c6(const void* pA, const void* pB, char* z56152ed, int n)
{
	const z61b46b8* a = (const z61b46b8*)pA;
	const z61b46b8* b = (const z61b46b8*)pB;
	if (z56152ed && n > 0) z56152ed[0] = 0;
	if (memcmp(a, b, sizeof(z61b46b8)) == 0) return 0;
	const int run = a->state != b->state || a->start != b->start || a->end != b->end;
	if (z56152ed && n > 0) {
		char sa[64], sb[64];
		za4a447b(a, sa, sizeof(sa));
		za4a447b(b, sb, sizeof(sb));
		if (run)                     snprintf(z56152ed, n, "previsto %s, certo %s", sa, sb);
		else if (a->frame != b->frame) snprintf(z56152ed, n, "contagem %lld x %lld (%s)", a->frame, b->frame, sb);
		else                         snprintf(z56152ed, n, "memoria do detector (%s)", sb);
	}
	return run ? 2 : 1;
}

static void zb5ca01c(const z61b46b8* s)
{
	if ((int)(s->zfc44818 - zb9626bb) <= 0) return;
	if (s->zfc44818 - zb9626bb > zc2bccf2) {
		zece5d43("speedrun: %u linha(s) do log perdida(s) (mais de %d sem confirmar)",
		         s->zfc44818 - zb9626bb - zc2bccf2, zc2bccf2);
		zb9626bb = s->zfc44818 - zc2bccf2;
	}
	for (; zb9626bb != s->zfc44818; zb9626bb++) z5d90bae(s->ev[zb9626bb % zc2bccf2]);
}

void z4508746(const void* zc472cd8)
{
	if (!z8bb2aa4 || !z910e658 || !zc472cd8) return;
	const z61b46b8* s = (const z61b46b8*)zc472cd8;
	zb5ca01c(s);
	if (s->finishes != z2d0bf63) {
		z2d0bf63 = s->finishes;
		if (s->state == 2) zb71fbd5(s);
	}
}

void z04a576e(void)
{
	if (!z8bb2aa4 || !z910e658) return;
	if (zcd54cd2.zfc44818 != zb9626bb || zcd54cd2.finishes != z2d0bf63)
		zece5d43("speedrun: a sessao acabou antes de confirmar os ultimos quadros - vale o estado final:");
	zb5ca01c(&zcd54cd2);
	if (zcd54cd2.finishes != z2d0bf63) {
		z2d0bf63 = zcd54cd2.finishes;
		if (zcd54cd2.state == 2) zb71fbd5(&zcd54cd2);
	}
}

int z8cde76f(char* sz, int n) { return z45be80c(&zcd54cd2, sz, n); }

static int CALLBACK zc1f6f11(const LOGFONTA*, const TEXTMETRICA*, DWORD, LPARAM lp)
{
	*(int*)lp = 1;
	return 0;
}

static int zdf2ea6d(const char* face)
{
	HDC dc = GetDC(NULL);
	LOGFONTA lf;
	memset(&lf, 0, sizeof(lf));
	lf.lfCharSet = DEFAULT_CHARSET;
	strncpy(lf.lfFaceName, face, LF_FACESIZE - 1);
	int found = 0;
	EnumFontFamiliesExA(dc, &lf, zc1f6f11, (LPARAM)&found, 0);
	ReleaseDC(NULL, dc);
	return found;
}

const char* z723432b(void)
{
	static const char* zce1b148 = NULL;
	if (!zce1b148) {
		if (zdf2ea6d("Tw Cen MT Condensed"))            zce1b148 = "Tw Cen MT Condensed";
		else if (zdf2ea6d("Bahnschrift SemiCondensed")) zce1b148 = "Bahnschrift SemiCondensed";
		else                                                 zce1b148 = "Arial";
		zece5d43("speedrun: fonte do cronometro: %s", zce1b148);
	}
	return zce1b148;
}

static void zb934cd6(unsigned char* img, int w, int h, int bpp, int pitch,
                      const unsigned int* mask, int mw, int mh, int ox, int oy, unsigned int rgb)
{
	const int r = (rgb >> 16) & 0xFF, g = (rgb >> 8) & 0xFF, b = rgb & 0xFF;
	for (int y = 0; y < mh; y++) {
		const int iy = oy + y;
		if (iy < 0 || iy >= h) continue;
		for (int x = 0; x < mw; x++) {
			const int ix = ox + x;
			if (ix < 0 || ix >= w) continue;
			const int a = mask[y * mw + x] & 0xFF;
			if (!a) continue;
			unsigned char* p = img + (size_t)iy * pitch + ix * bpp;
			if (bpp == 2) {
				unsigned v = p[0] | (p[1] << 8);
				int pr = ((v >> 11) & 31) << 3, pg = ((v >> 5) & 63) << 2, pb = (v & 31) << 3;
				pr += (r - pr) * a / 255; pg += (g - pg) * a / 255; pb += (b - pb) * a / 255;
				v = ((pr >> 3) << 11) | ((pg >> 2) << 5) | (pb >> 3);
				p[0] = (unsigned char)v; p[1] = (unsigned char)(v >> 8);
			} else {
				p[0] = (unsigned char)(p[0] + (b - p[0]) * a / 255);
				p[1] = (unsigned char)(p[1] + (g - p[1]) * a / 255);
				p[2] = (unsigned char)(p[2] + (r - p[2]) * a / 255);
			}
		}
	}
}

void zf7949f6(unsigned char* img, int w, int h, int bpp, int pitch)
{
	char t[32];
	if (!img || w <= 0 || h <= 0 || (bpp != 2 && bpp != 3 && bpp != 4)) return;
	if (!z8cde76f(t, sizeof(t))) return;

	static HDC z4763b9e = NULL;
	static HFONT zfd3772a = NULL;
	static int zf6a7f28 = 0;
	const int px = h / 11 < 12 ? 12 : h / 11;
	if (!z4763b9e) z4763b9e = CreateCompatibleDC(NULL);
	if (zf6a7f28 != px) {
		if (zfd3772a) DeleteObject(zfd3772a);
		zfd3772a = CreateFontA(-px, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, OUT_TT_PRECIS,
		                     CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, z723432b());
		zf6a7f28 = px;
	}
	HGDIOBJ zfb5ff58 = SelectObject(z4763b9e, zfd3772a);
	SIZE sz;
	GetTextExtentPoint32A(z4763b9e, t, (int)strlen(t), &sz);
	const int pad = 2, mw = sz.cx + pad * 2, mh = sz.cy + pad * 2;

	BITMAPINFO bi;
	memset(&bi, 0, sizeof(bi));
	bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
	bi.bmiHeader.biWidth = mw;
	bi.bmiHeader.biHeight = -mh;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 32;
	void* bits = NULL;
	HBITMAP bmp = CreateDIBSection(z4763b9e, &bi, DIB_RGB_COLORS, &bits, NULL, 0);
	if (!bmp || !bits) { SelectObject(z4763b9e, zfb5ff58); return; }
	HGDIOBJ z6794910 = SelectObject(z4763b9e, bmp);
	SetBkMode(z4763b9e, TRANSPARENT);
	SetTextColor(z4763b9e, RGB(255, 255, 255));

	const int ox = (w - mw) / 2, oy = h - mh - h / 40;
	memset(bits, 0, (size_t)mw * mh * 4);
	for (int dy = -1; dy <= 1; dy++)
		for (int dx = -1; dx <= 1; dx++)
			if (dx || dy) TextOutA(z4763b9e, pad + dx, pad + dy, t, (int)strlen(t));
	GdiFlush();
	zb934cd6(img, w, h, bpp, pitch, (const unsigned int*)bits, mw, mh, ox, oy, 0x000000);
	memset(bits, 0, (size_t)mw * mh * 4);
	TextOutA(z4763b9e, pad, pad, t, (int)strlen(t));
	GdiFlush();
	zb934cd6(img, w, h, bpp, pitch, (const unsigned int*)bits, mw, mh, ox, oy,
	          zcd54cd2.state == 2 ? 0x50FA7B : 0xFFFFFF);

	SelectObject(z4763b9e, z6794910);
	DeleteObject(bmp);
	SelectObject(z4763b9e, zfb5ff58);
}

void zd471feb(int z056997a) { z58890ec = z056997a ? 1 : 0; }
int  zf22a9a5(void) { return z58890ec; }
