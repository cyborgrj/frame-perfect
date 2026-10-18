#include "overlay.h"
#include "match_score.h"
#include "anticheat.h"
#include "input_display.h"
#include "lang.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern "C" void z5d90bae(const char* s);
extern "C" int  zedf061d(void);
extern "C" int  zc8b4cc6(void);
extern "C" int  ze12829b(void);
extern "C" int  z1b0f145(void);
extern "C" int  z15e2010(void);
extern "C" int  zf796ce3(void);
extern "C" int  zf5650f6(int* pSide, int* z4b39f95);

#define z66711b9    0x8BE9FDu
#define z5f72e17   0xBD93F9u
#define z98784dd   0xF8F8F2u
#define z0aadfc3   0x6272A4u
#define z937b93b     0x282A36u
#define z8f95492  0xFFB86Cu
#define z01db8b8     0xFF5555u
#define z940b7f3     0xF5C542u
#define z170e95d   216

#define zad641fd 40
#define z9f11055 9
#define z15094f0 12
#define z0b35ab8   9

#define z7cd97ef "Arial"

#define zed58912 640
#define z3fdea15 32

static int  z910e658   = 0;
static int  z9126c4e = 1;
static char z93f4c3c[zad641fd + 1];
static char z55eba79[zad641fd + 1];
static int  za5fd107 = 0;

static HDC      z1e0ebd8      = NULL;
static HBITMAP  z54259d6     = NULL;
static HBITMAP  z4623512  = NULL;
static unsigned char* z791d936 = NULL;
static HFONT    z0e7a0ce[3] = { NULL, NULL, NULL };
static int      z0cc97b8 = 0;
static int      zc104563  = 0;
static int      z58890ec   = 0;
static int      z8183630 = 0;
static int      z16fe25d  = 0;
static int      z72f5dac = 0;

static const char* z4b550f3[] = {
	"fonte_placar.otf",
	"fonte_placar.ttf",
	"support\\fonte_placar.otf",
	"support\\fonte_placar.ttf",
};

static const char* zeacf74a[] = {
	"fonte_metricas.ttf",
	"fonte_metricas.otf",
	"support\\fonte_metricas.ttf",
	"support\\fonte_metricas.otf",
};

static char za65b087[64] = "Arial";
static char z38b7d1b[64] = "Arial";
static int  zfbdeda7 = 0;

static unsigned be16(const unsigned char* p) { return (unsigned)((p[0] << 8) | p[1]); }
static unsigned zb3f22c3(const unsigned char* p)
{
	return ((unsigned)p[0] << 24) | ((unsigned)p[1] << 16) | ((unsigned)p[2] << 8) | p[3];
}

static int z640e0a3(const char* szPath, char* out, size_t cap)
{
	FILE* f = fopen(szPath, "rb");
	if (!f) return 0;

	fseek(f, 0, SEEK_END);
	long len = ftell(f);
	fseek(f, 0, SEEK_SET);
	if (len < 12 || len > 8 * 1024 * 1024) { fclose(f); return 0; }

	unsigned char* buf = (unsigned char*)malloc((size_t)len);
	if (!buf) { fclose(f); return 0; }
	if (fread(buf, 1, (size_t)len, f) != (size_t)len) { free(buf); fclose(f); return 0; }
	fclose(f);

	int ok = 0;
	const unsigned z9c5f68e = be16(buf + 4);
	unsigned zc499003 = 0, z6eac82a = 0;

	for (unsigned i = 0; i < z9c5f68e; i++) {
		const unsigned rec = 12 + i * 16;
		if (rec + 16 > (unsigned)len) break;
		if (memcmp(buf + rec, "name", 4) == 0) {
			zc499003 = zb3f22c3(buf + rec + 8);
			z6eac82a = zb3f22c3(buf + rec + 12);
			break;
		}
	}

	if (zc499003 && z6eac82a && zc499003 + 6 <= (unsigned)len) {
		const unsigned count   = be16(buf + zc499003 + 2);
		const unsigned zd660e8e = zc499003 + be16(buf + zc499003 + 4);
		int best = -1;

		for (unsigned i = 0; i < count; i++) {
			const unsigned r = zc499003 + 6 + i * 12;
			if (r + 12 > (unsigned)len) break;
			if (be16(buf + r + 6) != 1) continue;

			const unsigned z2bf385b = be16(buf + r);
			const unsigned sl   = be16(buf + r + 8);
			const unsigned so   = zd660e8e + be16(buf + r + 10);
			if (so + sl > (unsigned)len || sl == 0) continue;

			size_t o = 0;
			if (z2bf385b == 3) {
				for (unsigned k = 1; k < sl && o + 1 < cap; k += 2) out[o++] = (char)buf[so + k];
			} else {
				for (unsigned k = 0; k < sl && o + 1 < cap; k++)    out[o++] = (char)buf[so + k];
			}
			out[o] = '\0';

			if (o) { ok = 1; if (z2bf385b == 3) { best = 1; break; } }
		}
		(void)best;
	}

	free(buf);
	return ok;
}

static int z1326218(const char* const* files, int n, char* za6060a4, size_t cap,
                        const char* z56152ed)
{
	char z24c0c98[64] = "";
	for (int i = 0; i < n; i++) {
		if (!z640e0a3(files[i], z24c0c98, sizeof(z24c0c98))) continue;
		if (AddFontResourceExA(files[i], FR_PRIVATE, NULL) <= 0) continue;
		strncpy(za6060a4, z24c0c98, cap - 1);
		za6060a4[cap - 1] = '\0';
		char zde61a7d[128];
		snprintf(zde61a7d, sizeof(zde61a7d), "overlay: usando a fonte de %s", z56152ed);
		z5d90bae(zde61a7d);
		return 1;
	}
	return 0;
}

static HFONT zbd602ca(int px, const char* z8294d4e)
{
	return CreateFontA(-px, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
	                   DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
	                   ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, z8294d4e);
}

static HFONT zcdefa47(int px)
{
	return CreateFontA(-px, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
	                   DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
	                   NONANTIALIASED_QUALITY, DEFAULT_PITCH | FF_SWISS, z7cd97ef);
}

static int z90dba01(HFONT f, const char* z245c249)
{
	if (!f || !z1e0ebd8) return 0;
	HFONT old = (HFONT)SelectObject(z1e0ebd8, f);
	char z169647e[64] = "";
	GetTextFaceA(z1e0ebd8, sizeof(z169647e), z169647e);
	SelectObject(z1e0ebd8, old);
	return _stricmp(z169647e, z245c249) == 0;
}

static void z8b719e7(void)
{
	if (z0cc97b8) return;
	z0cc97b8 = 1;

	zc104563 = z1326218(z4b550f3, (int)(sizeof(z4b550f3) / sizeof(z4b550f3[0])),
	                          za65b087, sizeof(za65b087), "fonte_placar");
	zfbdeda7 = z1326218(zeacf74a, (int)(sizeof(zeacf74a) / sizeof(zeacf74a[0])),
	                                z38b7d1b, sizeof(z38b7d1b), "fonte_metricas");

	HDC screen = GetDC(NULL);
	z1e0ebd8 = CreateCompatibleDC(screen);
	ReleaseDC(NULL, screen);
	if (!z1e0ebd8) return;

	BITMAPINFO bi;
	memset(&bi, 0, sizeof(bi));
	bi.bmiHeader.biSize        = sizeof(bi.bmiHeader);
	bi.bmiHeader.biWidth       = zed58912;
	bi.bmiHeader.biHeight      = -z3fdea15;
	bi.bmiHeader.biPlanes      = 1;
	bi.bmiHeader.biBitCount    = 32;
	bi.bmiHeader.biCompression = BI_RGB;

	z54259d6 = CreateDIBSection(z1e0ebd8, &bi, DIB_RGB_COLORS, (void**)&z791d936, NULL, 0);
	if (!z54259d6) { DeleteDC(z1e0ebd8); z1e0ebd8 = NULL; return; }
	z4623512 = (HBITMAP)SelectObject(z1e0ebd8, z54259d6);

	if (zc104563) {
		HFONT probe = zbd602ca(z9f11055, za65b087);
		if (!z90dba01(probe, za65b087)) {
			z5d90bae("overlay: o Windows nao aceitou a fonte de fonte_placar - usando Arial");
			zc104563 = 0;
			strcpy(za65b087, "Arial");
		}
		if (probe) DeleteObject(probe);
	}

	if (zfbdeda7) {
		HFONT probe = zbd602ca(z0b35ab8, z38b7d1b);
		if (!z90dba01(probe, z38b7d1b)) {
			z5d90bae("overlay: o Windows nao aceitou a fonte de fonte_metricas - usando Arial");
			zfbdeda7 = 0;
			strcpy(z38b7d1b, "Arial");
		}
		if (probe) DeleteObject(probe);
	}

	z0e7a0ce[0] = zcdefa47(z9f11055);
	z0e7a0ce[1] = zcdefa47(z15094f0);
	z0e7a0ce[2] = zcdefa47(z0b35ab8);

	SetBkMode(z1e0ebd8, OPAQUE);
	SetBkColor(z1e0ebd8, RGB(0, 0, 0));
	SetTextColor(z1e0ebd8, RGB(255, 255, 255));
}

static void z883ffb9(const unsigned char* px, int bpp, int* r, int* g, int* b)
{
	if (bpp == 2) {
		const unsigned short v = *(const unsigned short*)px;
		*r = ((v >> 11) & 31) << 3;
		*g = ((v >>  5) & 63) << 2;
		*b = ( v        & 31) << 3;
	} else {
		*b = px[0]; *g = px[1]; *r = px[2];
	}
}

static void z24de44d(unsigned char* px, int bpp, int r, int g, int b)
{
	if (bpp == 2) {
		*(unsigned short*)px =
			(unsigned short)(((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3));
	} else {
		px[0] = (unsigned char)b; px[1] = (unsigned char)g; px[2] = (unsigned char)r;
	}
}

static void z5640c41(unsigned char* img, int w, int h, int bpp, int pitch,
                    int x, int y, unsigned int rgb, int alpha)
{
	if (x < 0 || y < 0 || x >= w || y >= h || alpha <= 0) return;
	unsigned char* px = img + (size_t)y * pitch + (size_t)x * bpp;

	const int sr = (rgb >> 16) & 0xFF, sg = (rgb >> 8) & 0xFF, sb = rgb & 0xFF;
	if (alpha >= 255) { z24de44d(px, bpp, sr, sg, sb); return; }

	int dr, dg, db;
	z883ffb9(px, bpp, &dr, &dg, &db);
	z24de44d(px, bpp,
	       dr + ((sr - dr) * alpha) / 255,
	       dg + ((sg - dg) * alpha) / 255,
	       db + ((sb - db) * alpha) / 255);
}

static void z936df24(unsigned char* img, int w, int h, int bpp, int pitch,
                  int y0, int y1)
{
	for (int y = y0; y < y1; y++)
		for (int x = 0; x < w; x++)
			z5640c41(img, w, h, bpp, pitch, x, y, z937b93b, z170e95d);
}

static int z50ccdbc(const char* s, int idx, int* z932636c)
{
	if (!z1e0ebd8 || !z0e7a0ce[idx] || !s || !*s) { if (z932636c) *z932636c = 0; return 0; }
	HFONT old = (HFONT)SelectObject(z1e0ebd8, z0e7a0ce[idx]);
	SIZE sz; sz.cx = sz.cy = 0;
	GetTextExtentPoint32A(z1e0ebd8, s, (int)strlen(s), &sz);
	SelectObject(z1e0ebd8, old);
	if (z932636c) *z932636c = sz.cy;
	return sz.cx;
}

static int zd1ca55b(unsigned char* img, int w, int h, int bpp, int pitch,
                  int x, int y, const char* s, unsigned int rgb, int idx)
{
	if (!z1e0ebd8 || !z791d936 || !z0e7a0ce[idx] || !s || !*s) return 0;

	int th = 0;
	const int tw = z50ccdbc(s, idx, &th);
	if (tw <= 0 || tw > zed58912 || th > z3fdea15) return tw;

	RECT rc; rc.left = 0; rc.top = 0; rc.right = tw; rc.bottom = th;
	HFONT old = (HFONT)SelectObject(z1e0ebd8, z0e7a0ce[idx]);
	ExtTextOutA(z1e0ebd8, 0, 0, ETO_OPAQUE, &rc, s, (int)strlen(s), NULL);
	SelectObject(z1e0ebd8, old);
	GdiFlush();

	for (int ty = 0; ty < th; ty++) {
		const unsigned char* row = z791d936 + (size_t)ty * zed58912 * 4;
		for (int tx = 0; tx < tw; tx++) {
			const int a = row[tx * 4 + 1];
			if (a) z5640c41(img, w, h, bpp, pitch, x + tx, y + ty, rgb, a);
		}
	}
	return tw;
}

static void z71abf75(char* dst, size_t cap, const char* src)
{
	size_t o = 0;
	for (size_t i = 0; src && src[i] && o + 1 < cap; i++) {
		if (src[i] == '%' && src[i + 1] && src[i + 2]) {
			int hi = src[i + 1], lo = src[i + 2];
			hi = (hi >= '0' && hi <= '9') ? hi - '0' : ((hi | 32) >= 'a' && (hi | 32) <= 'f') ? (hi | 32) - 'a' + 10 : -1;
			lo = (lo >= '0' && lo <= '9') ? lo - '0' : ((lo | 32) >= 'a' && (lo | 32) <= 'f') ? (lo | 32) - 'a' + 10 : -1;
			if (hi >= 0 && lo >= 0) { dst[o++] = (char)((hi << 4) | lo); i += 2; continue; }
		}
		dst[o++] = src[i];
	}
	dst[o] = '\0';
}

void z5c0c8b7(const char* z733a844, const char* zd5e0939, int ze6bfbdc)
{
	memset(z93f4c3c, 0, sizeof(z93f4c3c));
	memset(z55eba79, 0, sizeof(z55eba79));
	z71abf75(z93f4c3c, sizeof(z93f4c3c), z733a844);
	z71abf75(z55eba79, sizeof(z55eba79), zd5e0939);
	za5fd107 = (ze6bfbdc > 0 && ze6bfbdc < 100) ? ze6bfbdc : 0;
	z910e658 = (z93f4c3c[0] || z55eba79[0]) ? 1 : 0;
}

void z8fa5b4b(void) { z910e658 = 0; z93f4c3c[0] = z55eba79[0] = '\0'; za5fd107 = 0; }
int  z4b7d267(void) { return z910e658; }
int  zfe2b00f(void) { return z9126c4e; }

void z1c35054(void)
{
	z9126c4e = (z9126c4e + 1) % 3;
}

int z8e16a23(z1fa3283* out)
{
	if (!out) return 0;
	if (!z910e658 || z9126c4e == 0) return 0;

	memset(out, 0, sizeof(*out));
	out->nMode     = z9126c4e;
	out->z03b5823     = z66711b9;
	out->z20dcc8d     = z5f72e17;
	out->ze871279  = z98784dd;
	out->z73d250a  = z0aadfc3;
	out->z02d18e2    = z937b93b;
	out->ze326e54 = z170e95d;

	if (z9ae99d0()) {
		int zc40dd08 = 0, zb1468ac = 0;
		zd82e415(&zc40dd08, &zb1468ac);
		snprintf(out->zfad56d7, sizeof(out->zfad56d7), "%d", zc40dd08);
		snprintf(out->z561be8d, sizeof(out->z561be8d), "%d", zb1468ac);
	} else {
		snprintf(out->zfad56d7, sizeof(out->zfad56d7), "-");
		snprintf(out->z561be8d, sizeof(out->z561be8d), "-");
	}
	if (ze2aa305()) {
		snprintf(out->z5ea9399, sizeof(out->z5ea9399), "PAUSA");
		out->z73d250a = z98784dd;
	} else if (za5fd107) snprintf(out->z5ea9399, sizeof(out->z5ea9399), "FT%d", za5fd107);

	strncpy(out->z733a844, z93f4c3c[0] ? z93f4c3c : "P1", sizeof(out->z733a844) - 1);
	strncpy(out->zd5e0939, z55eba79[0] ? z55eba79 : "P2", sizeof(out->zd5e0939) - 1);
	return 1;
}

int z7fb98df();

static DWORD z818de3a = 0;
static int   zbeeb4de   = 0;
void z6987e0c(int z5cddbe5)
{
	zbeeb4de = z5cddbe5 < 0 ? 0 : z5cddbe5 > 100 ? 100 : z5cddbe5;
	z818de3a = GetTickCount() + 1500;
	if (!z818de3a) z818de3a = 1;
}

int z70213e0(z2c2b1a7* out)
{
	if (!out) return 0;
	const int za9e2613 = (z910e658 || zf796ce3()) ? zedf061d() : -1;
	const int pad = za9e2613 >= 0 ? 0 : z7fb98df();
	if (pad) {
		memset(out, 0, sizeof(*out));
		out->z8b6ca0e = -1;
		if (pad == 1) {
			snprintf(out->z0f6d336, sizeof(out->z0f6d336), "%s", z34ba319("CONTROLE DESCONECTADO"));
			snprintf(out->z6af1ebd, sizeof(out->z6af1ebd), "%s", z34ba319("RECONECTE PARA CONTINUAR"));
			out->zd4c6117 = z8f95492;
		} else {
			snprintf(out->z0f6d336, sizeof(out->z0f6d336), "%s", z34ba319("CONTROLE RECONECTADO"));
			out->zd4c6117 = z8f95492;
		}
		out->z0456974    = z937b93b;
		out->z70f9d25 = z170e95d;
		return 1;
	}
	if (za9e2613 < 0 && z818de3a && (int)(z818de3a - GetTickCount()) > 0) {
		memset(out, 0, sizeof(*out));
		snprintf(out->z0f6d336, sizeof(out->z0f6d336), "%s", z34ba319("VOLUME"));
		snprintf(out->z6af1ebd, sizeof(out->z6af1ebd), "%d%%", zbeeb4de);
		out->zd4c6117   = z8f95492;
		out->z0456974    = z937b93b;
		out->z70f9d25 = z170e95d;
		out->z430cab8    = 1;
		out->z8b6ca0e   = zbeeb4de;
		return 1;
	}
	if (!z910e658) return 0;
	if (za9e2613 >= 0) {
		memset(out, 0, sizeof(*out));
		if (z1b0f145() && zc8b4cc6()) {
			snprintf(out->z0f6d336, sizeof(out->z0f6d336), "%s", z34ba319("ENCERRANDO"));
			snprintf(out->z6af1ebd, sizeof(out->z6af1ebd), "%s", z34ba319("A FT JA TERMINOU"));
		} else if (z1b0f145()) {
			snprintf(out->z0f6d336, sizeof(out->z0f6d336), "%s", z34ba319("ENCERRANDO A PARTIDA"));
			snprintf(out->z6af1ebd, sizeof(out->z6af1ebd), "%s", z15e2010() ? z34ba319("SOLTE PARA CONTINUAR") : z34ba319("HAVERA PERDA DE PONTOS..."));
		} else {
			snprintf(out->z0f6d336, sizeof(out->z0f6d336), "%s", z34ba319("FECHANDO O JOGO"));
			snprintf(out->z6af1ebd, sizeof(out->z6af1ebd), "%s", z34ba319("SOLTE PARA CONTINUAR"));
		}
		out->zd4c6117   = z8f95492;
		out->z0456974    = z937b93b;
		out->z70f9d25 = z170e95d;
		out->z8b6ca0e   = za9e2613 > 100 ? 100 : za9e2613;
		return 1;
	}
	const int z1f9fd65 = ze12829b();
	if (z1f9fd65 > 0) {
		memset(out, 0, sizeof(*out));
		snprintf(out->z0f6d336, sizeof(out->z0f6d336), "%s - %s %d S", z34ba319("FT ENCERRADA"), z34ba319("FECHANDO EM"), z1f9fd65);
		snprintf(out->z6af1ebd, sizeof(out->z6af1ebd), "%s", z15e2010() ? z34ba319("ESC PARA SAIR AGORA") : z34ba319("ESC SAI SEM PERDER PONTOS"));
		out->zd4c6117   = z8f95492;
		out->z0456974    = z937b93b;
		out->z70f9d25 = z170e95d;
		out->z430cab8    = 1;
		out->z8b6ca0e   = -1;
		return 1;
	}
	int side = 0, stage = 0, left = 0;
	if (!z969ea27(&side, &stage, &left)) {
		int pct = -1;
		const int ask = zf5650f6(&side, &pct);
		if (ask) {
			memset(out, 0, sizeof(*out));
			snprintf(out->z0f6d336, sizeof(out->z0f6d336), "%s", z34ba319("RECOMECAR A PARTIDA?"));
			snprintf(out->z6af1ebd, sizeof(out->z6af1ebd), "%s", ask == 1 ? z34ba319("AGUARDANDO O ADVERSARIO") : z34ba319("APERTE F3 PARA ACEITAR"));
			out->zd4c6117   = z8f95492;
			out->z0456974    = z937b93b;
			out->z70f9d25 = z170e95d;
			out->nSide     = side;
			out->z430cab8    = 1;
			out->z8b6ca0e   = ask == 1 ? pct : -1;
			return 1;
		}
		int kind = 0;
		if (!z8486cb4(&side, &kind)) return 0;
		memset(out, 0, sizeof(*out));
		out->z8b6ca0e = -1;
		snprintf(out->z0f6d336, sizeof(out->z0f6d336), "%s", kind == z0f4913a ? z34ba319("POSSIVEL MACRO") : z34ba319("POSSIVEL TURBO"));
		snprintf(out->z6af1ebd, sizeof(out->z6af1ebd), "%s", side == 1 ? (z93f4c3c[0] ? z93f4c3c : "P1") : (z55eba79[0] ? z55eba79 : "P2"));
		out->zd4c6117   = z8f95492;
		out->z0456974    = z937b93b;
		out->z70f9d25 = z170e95d;
		out->nSide     = side;
		out->z430cab8    = 1;
		return 1;
	}

	memset(out, 0, sizeof(*out));
	out->z8b6ca0e = -1;
	snprintf(out->z0f6d336, sizeof(out->z0f6d336), z34ba319("INATIVIDADE DO %s"),
	         side == 3 ? z34ba319("P1 E DO P2") : side == 1 ? "P1" : "P2");
	if (stage == 1) {
		snprintf(out->z6af1ebd, sizeof(out->z6af1ebd), "%s", z34ba319("A PARTIDA SERA ENCERRADA"));
		out->zd4c6117 = z8f95492;
	} else {
		snprintf(out->z6af1ebd, sizeof(out->z6af1ebd), z34ba319("ENCERRANDO EM %d"), (left + 59) / 60);
		out->zd4c6117 = z01db8b8;
	}
	out->z0456974    = z937b93b;
	out->z70f9d25 = z170e95d;
	return 1;
}

const char* z5c1451e(void)
{
	z8b719e7();
	return za65b087;
}

void z73bbb8d(int z056997a)
{
	const int now = z056997a ? 1 : 0;
	if (now != z58890ec) {
		z58890ec = now;
		z5d90bae(now
			? "overlay: o blitter desenha na resolucao da janela (nitido)"
			: "overlay: o blitter devolveu o desenho da barra");
	}
}

int  z9534659(void) { return z58890ec; }

void z81ef24e(void) { z8b719e7(); }

static int z0095087(int n) { return (n < 0) ? 0 : (n > 2) ? 2 : n; }

int zf92b87e(const char* s, int nSize, int* pHeight)
{
	z8b719e7();
	return z50ccdbc(s, z0095087(nSize), pHeight);
}

int z11f9d81(unsigned char* img, int w, int h, int bpp, int pitch,
                   int x, int y, const char* s, unsigned int rgb, int nSize)
{
	return zd1ca55b(img, w, h, bpp, pitch, x, y, s, rgb, z0095087(nSize));
}

const char* zdff8e52(void)
{
	z8b719e7();
	return z38b7d1b;
}

void z5bbbc0c(unsigned char* img, int w, int h, int bpp, int pitch,
                     int x0, int y0, int x1, int y1, unsigned int rgb, int alpha)
{
	if (x0 < 0) x0 = 0;
	if (y0 < 0) y0 = 0;
	if (x1 > w) x1 = w;
	if (y1 > h) y1 = h;
	for (int y = y0; y < y1; y++)
		for (int x = x0; x < x1; x++)
			z5640c41(img, w, h, bpp, pitch, x, y, rgb, alpha);
}

void zcf14204(int* pRight, int* pBottom)
{
	if (pRight)  *pRight  = z16fe25d;
	if (pBottom) *pBottom = z72f5dac;
}

static void z444375c(unsigned char* img, int w, int h, int bpp, int pitch);

static void z1f618b1(unsigned char* img, int w, int h, int bpp, int pitch)
{
	z2c2b1a7 B;
	if (!z70213e0(&B)) return;
	z8b719e7();
	if (!z1e0ebd8) return;

	const int idx = B.z430cab8 ? 0 : 1;
	int h1 = 0, h2 = 0;
	const int w1 = z50ccdbc(B.z0f6d336, idx, &h1);
	const int w2 = z50ccdbc(B.z6af1ebd, idx, &h2);
	if (h1 <= 0) return;

	const int pad = B.z430cab8 ? 3 : 4;
	const int z404a75f = B.z8b6ca0e >= 0 ? (h1 / 2 > 4 ? h1 / 2 : 4) : 0;
	const int bw  = (w1 > w2 ? w1 : w2) + pad * 4;
	const int bh  = h1 + h2 + pad * 3 + (z404a75f ? z404a75f + pad : 0);
	const int cx  = B.nSide == 1 ? w / 4 : B.nSide == 2 ? (w * 3) / 4 : w / 2;
	const int x0  = cx - bw / 2, y0 = B.nSide ? (h * 16) / 100 : (h - bh) / 2;
	z5bbbc0c(img, w, h, bpp, pitch, x0, y0, x0 + bw, y0 + bh, B.z0456974, B.z70f9d25);
	zd1ca55b(img, w, h, bpp, pitch, cx - w1 / 2, y0 + pad, B.z0f6d336, B.z430cab8 ? B.zd4c6117 : z98784dd, idx);
	zd1ca55b(img, w, h, bpp, pitch, cx - w2 / 2, y0 + pad * 2 + h1, B.z6af1ebd, B.z430cab8 ? z98784dd : B.zd4c6117, idx);
	if (z404a75f) {
		const int z5986516 = x0 + pad * 2, z89cf666 = x0 + bw - pad * 2;
		const int z4f2e51e = y0 + pad * 3 + h1 + h2, zbf5b071 = z4f2e51e + z404a75f;
		z5bbbc0c(img, w, h, bpp, pitch, z5986516, z4f2e51e, z89cf666, zbf5b071, z98784dd, 60);
		z5bbbc0c(img, w, h, bpp, pitch, z5986516, z4f2e51e, z5986516 + (z89cf666 - z5986516) * B.z8b6ca0e / 100, zbf5b071, B.zd4c6117, 255);
	}
}

static const char* const zd72a479[7] = {
	"...#...", "..###..", ".#.#.#.", "...#...", "...#...", "...#...", "...#..." };
static const char* const z4391774[7] = {
	"..#####", ".....##", "....#.#", "...#..#", "..#...#", ".#.....", "#......" };

static void z54a3ec2(unsigned char* img, int w, int h, int bpp, int pitch,
                    int x0, int y0, wchar_t c, unsigned int rgb)
{
	if (c == 0x2022) {
		for (int y = 2; y < 5; y++)
			for (int x = 2; x < 5; x++) z5640c41(img, w, h, bpp, pitch, x0 + x, y0 + y, rgb, 255);
		return;
	}
	int rot = 0;
	const char* const* pic = zd72a479;
	switch (c) {
		case 0x2191: rot = 0; break;
		case 0x2192: rot = 1; break;
		case 0x2193: rot = 2; break;
		case 0x2190: rot = 3; break;
		case 0x2197: rot = 0; pic = z4391774; break;
		case 0x2198: rot = 1; pic = z4391774; break;
		case 0x2199: rot = 2; pic = z4391774; break;
		case 0x2196: rot = 3; pic = z4391774; break;
		default: return;
	}
	for (int y = 0; y < 7; y++)
		for (int x = 0; x < 7; x++) {
			if (pic[y][x] != '#') continue;
			int px = x, py = y;
			for (int r = 0; r < rot; r++) { const int t = px; px = 6 - py; py = t; }
			z5640c41(img, w, h, bpp, pitch, x0 + px, y0 + py, rgb, 255);
		}
}

static unsigned int zf13e146(const char* s)
{
	if (!strcmp(s, "A")) return 0xFF5555;
	if (!strcmp(s, "B")) return 0xF1FA8C;
	if (!strcmp(s, "C")) return 0x50FA7B;
	if (!strcmp(s, "D")) return 0x6CB4FF;
	if (!strcmp(s, "LP") || !strcmp(s, "LK")) return 0x6CB4FF;
	if (!strcmp(s, "MP") || !strcmp(s, "MK")) return 0xF1FA8C;
	if (!strcmp(s, "HP") || !strcmp(s, "HK")) return 0xFF5555;
	return z98784dd;
}

static unsigned int z3d4cfa1(unsigned int rgb, int pct)
{
	return ((((rgb >> 16) & 0xFF) * pct / 100) << 16) |
	       ((((rgb >> 8) & 0xFF) * pct / 100) << 8) |
	       ((rgb & 0xFF) * pct / 100);
}

static void zce4915c(unsigned char* img, int w, int h, int bpp, int pitch)
{
	if (z0c5a8be() == zff3953a) return;
	z8b719e7();
	if (!z1e0ebd8) return;

	int th = 0;
	const int zcdda22c = z50ccdbc("999", 0, &th);
	if (th <= 0) return;
	const int z7a61baa = th > 8 ? th : 8;
	const int top  = (h * 25) / 100;
	int nMax = ((h * 80) / 100 - top) / z7a61baa;
	if (nMax > 18) nMax = 18;
	if (nMax < 1) return;

	static int za3ff378[2] = { 0, 0 };
	const int z1d5dcc6 = z50ccdbc("A B", 0, NULL) - z50ccdbc("AB", 0, NULL);
	for (int side = 1; side <= 2; side++) {
		z72561c1 rows[18];
		const int n = za5c0699(side, rows, nMax);
		if (n <= 0) continue;
		if (!za3ff378[side - 1]) za3ff378[side - 1] = z50ccdbc("A B C D", 0, NULL);
		for (int i = 0; i < n; i++) {
			const int wb = z50ccdbc(rows[i].z34fc6fc, 0, NULL);
			if (wb > za3ff378[side - 1]) za3ff378[side - 1] = wb;
		}
		const int ze4e04b0 = 2 + zcdda22c + 2 + 7 + 3 + za3ff378[side - 1] + 2;
		const int x0 = side == 1 ? 2 : w - 2 - ze4e04b0;
		z5bbbc0c(img, w, h, bpp, pitch, x0, top - 1, x0 + ze4e04b0, top + n * z7a61baa + 1, z937b93b, 150);

		for (int i = 0; i < n; i++) {
			const int pct = 100 - (i * 45) / (nMax > 1 ? nMax - 1 : 1);
			const int y = top + i * z7a61baa;
			char sz[16];
			snprintf(sz, sizeof(sz), "%d", rows[i].nFrames);
			const int wc = z50ccdbc(sz, 0, NULL);
			zd1ca55b(img, w, h, bpp, pitch, x0 + 2 + zcdda22c - wc, y, sz, z3d4cfa1(0xA6ACCD, pct), 0);
			z54a3ec2(img, w, h, bpp, pitch, x0 + 2 + zcdda22c + 2, y + (z7a61baa - 7) / 2, rows[i].z64f3ab9[0],
			        z3d4cfa1(rows[i].z64f3ab9[0] == 0x2022 ? z0aadfc3 : z98784dd, pct));
			int x = x0 + 2 + zcdda22c + 2 + 7 + 3;
			char zd24b553[32];
			strncpy(zd24b553, rows[i].z34fc6fc, sizeof(zd24b553) - 1); zd24b553[sizeof(zd24b553) - 1] = 0;
			for (char* tok = strtok(zd24b553, " "); tok; tok = strtok(NULL, " "))
				x += zd1ca55b(img, w, h, bpp, pitch, x, y, tok, z3d4cfa1(zf13e146(tok), pct), 0) + z1d5dcc6;
		}
	}
}

static void z610fb2b(char* out, int cap, const wchar_t* s)
{
	out[0] = 0;
	if (!s) return;
	int n = WideCharToMultiByte(CP_ACP, 0, s, -1, out, cap, "?", NULL);
	if (n <= 0) out[0] = 0;
	out[cap - 1] = 0;
}

static void z43680e3(char* s, int idx, int maxW)
{
	int len = (int)strlen(s);
	while (len > 0 && z50ccdbc(s, idx, NULL) > maxW) {
		s[--len] = 0;
		if (len > 3) { s[len - 1] = s[len - 2] = s[len - 3] = '.'; }
	}
}

struct zc8658a8 { unsigned char* img; int w, h, bpp, pitch; };
static void zfba7f65(void* ctx, int x0, int y0, int x1, int y1, unsigned int rgb, int alpha)
{
	const zc8658a8* c = (const zc8658a8*)ctx;
	z5bbbc0c(c->img, c->w, c->h, c->bpp, c->pitch, x0, y0, x1, y1, rgb, alpha);
}

static int z212e6db(void* ctx, const wchar_t* s, int x, int y, unsigned int rgb, int  )
{
	const zc8658a8* c = (const zc8658a8*)ctx;
	char a[256];
	z610fb2b(a, sizeof(a), s);
	return zd1ca55b(c->img, c->w, c->h, c->bpp, c->pitch, x, y, a, rgb, 0);
}

static int zc03da91(void*  , const wchar_t* s, int  , int* z932636c)
{
	char a[256];
	z610fb2b(a, sizeof(a), s);
	return z50ccdbc(a, 0, z932636c);
}

static int z7827c08(unsigned char* img, int w, int h, int bpp, int pitch, const char* s,
                      int x, int y, int maxW, int lh, unsigned int rgb, int draw, int z24f3184)
{
	char buf[512], cur[512];
	strncpy(buf, s, sizeof(buf) - 1); buf[sizeof(buf) - 1] = 0;
	int n = 0;
	cur[0] = 0;
	for (char* tok = strtok(buf, " "); tok; tok = strtok(NULL, " ")) {
		char z798ea1d[512];
		snprintf(z798ea1d, sizeof(z798ea1d), cur[0] ? "%s %s" : "%s%s", cur, tok);
		if (cur[0] && z50ccdbc(z798ea1d, 0, NULL) > maxW) {
			if (n == z24f3184 - 1) { z43680e3(z798ea1d, 0, maxW); strcpy(cur, z798ea1d); break; }
			if (draw) zd1ca55b(img, w, h, bpp, pitch, x, y + n * lh, cur, rgb, 0);
			n++;
			strncpy(cur, tok, sizeof(cur) - 1); cur[sizeof(cur) - 1] = 0;
		} else {
			strcpy(cur, z798ea1d);
		}
	}
	if (cur[0]) {
		z43680e3(cur, 0, maxW);
		if (draw) zd1ca55b(img, w, h, bpp, pitch, x, y + n * lh, cur, rgb, 0);
		n++;
	}
	return n;
}

static void zfa8d765(unsigned char* img, int w, int h, int bpp, int pitch)
{
	z1ea8be6 V;
	if (!z8ad8d82(&V)) return;
	z8b719e7();
	if (!z1e0ebd8) return;
	int lh = 0;
	z50ccdbc("Ag", 0, &lh);
	if (lh <= 0) return;
	zc8658a8 ctx = { img, w, h, bpp, pitch };
	const int m = h / 112 > 2 ? h / 112 : 2;
	const int ih = lh + 4, iw = ih * 2 + 2;
	const int ix = (w - iw) / 2, iy = h - m - ih;

	const int pw = w * 80 / 100, pad = 3, tw = pw - pad * 2;
	char name[64], line[512], zd87126d[512];
	z610fb2b(name, sizeof(name), V.zd8f5856);
	z5509597 P;
	P.ctx = &ctx; P.rect = zfba7f65; P.text = z212e6db; P.measure = zc03da91;
	P.unit = lh; P.small = lh; P.ansi = 1;
	const int z19fd162 = V.z7a57a1d ? z07cecca(&P, 0, 0, tw, 0) : 0;
	const int nShow = V.z24baf09 ? 0 : V.z19ef79a;
	int rows = (V.z24baf09 ? 0 : 1) + (V.ze6affeb ? 1 : 0);
	int z9e62f73[z0452cf9];
	for (int i = 0; i < nShow; i++) {
		if (V.lines[i].sys) { z610fb2b(zd87126d, sizeof(zd87126d), V.lines[i].text); z9e62f73[i] = z7827c08(img, w, h, bpp, pitch, zd87126d, 0, 0, tw, lh, 0, 0, 3); }
		else z9e62f73[i] = 1;
	}
	int first = 0, body = 0;
	const int zc25b6c8 = z19fd162 ? 2 : 5;
	for (int i = nShow - 1; i >= 0; i--) {
		if (body + z9e62f73[i] > zc25b6c8) { first = i + 1; break; }
		body += z9e62f73[i];
	}
	if (nShow == 0 && !V.z24baf09 && !z19fd162) body = 1;
	rows += body;
	const int zb992224 = z19fd162 ? z19fd162 + (rows ? pad : 0) : 0;
	const int ph = pad * 2 + rows * lh + zb992224 + (V.zcf8a8e5 ? pad + 2 * lh : 0);
	const int z7108971 = (w - pw) / 2, py0 = h - m - ph;

	if (V.fOpen <= 0.001f) {
		if (V.z5ad0488) { zb1eff89(&P, w / 2, iy + ih, ih, V.z1868977); return; }
		int bx = 0, by = 0, br = 0;
		zb52af7e(zfba7f65, &ctx, ix, iy, iw, ih, 110, V.z92c652b, V.z822f0cf, &bx, &by, &br);
		if (br > 0) {
			char num[8];
			snprintf(num, sizeof(num), "%d", V.z822f0cf > 9 ? 9 : V.z822f0cf);
			int th = 0;
			const int z46f6df3 = z50ccdbc(num, 0, &th);
			zd1ca55b(img, w, h, bpp, pitch, bx - z46f6df3 / 2, by - th / 2, num, 0x0B0D14, 0);
		}
		return;
	}
	if (V.fOpen < 0.999f) {
		const float t = V.fOpen;
		const int x = (int)(ix + (z7108971 - ix) * t), y = (int)(iy + (py0 - iy) * t);
		const int ww = (int)(iw + (pw - iw) * t), hh = (int)(ih + (ph - ih) * t);
		const int r = (int)(ih / 2 + (3 - ih / 2) * t);
		z8eca64b(zfba7f65, &ctx, x, y, ww, hh, r, 0x1B1F27, (int)(110 + (230 - 110) * t));
		return;
	}

	z8eca64b(zfba7f65, &ctx, z7108971, py0, pw, ph, 3, 0x1B1F27, 230);
	int y = py0 + pad;
	if (!V.z24baf09) {
		snprintf(line, sizeof(line), "CHAT - %s", name);
		z43680e3(line, 0, tw);
		zd1ca55b(img, w, h, bpp, pitch, z7108971 + pad, y, line, 0x8A93A6, 0);
		y += lh;
	}
	if (V.ze6affeb) {
		char z1d5151e[128];
		z610fb2b(z1d5151e, sizeof(z1d5151e), V.ze6affeb);
		z43680e3(z1d5151e, 0, tw);
		zd1ca55b(img, w, h, bpp, pitch, z7108971 + pad, y, z1d5151e, z8f95492, 0);
		y += lh;
	}
	if (nShow == 0 && !V.z24baf09 && !z19fd162) { zd1ca55b(img, w, h, bpp, pitch, z7108971 + pad, y, z34ba319("Nenhuma mensagem ainda."), 0x8A93A6, 0); y += lh; }
	for (int i = first; i < nShow; i++) {
		char t[512];
		z610fb2b(t, sizeof(t), V.lines[i].text);
		if (V.lines[i].sys) {
			y += lh * z7827c08(img, w, h, bpp, pitch, t, z7108971 + pad, y, tw, lh, 0xF1FA8C, 1, 3);
			continue;
		}
		snprintf(line, sizeof(line), "%s: %s%s", V.lines[i].mine ? z34ba319("Voc\xEA") : name, t, V.lines[i].mine && V.lines[i].read ? z34ba319(" (lido)") : "");
		z43680e3(line, 0, tw);
		zd1ca55b(img, w, h, bpp, pitch, z7108971 + pad, y, line, V.lines[i].mine ? 0x7FE3F0 : z98784dd, 0);
		y += lh;
	}
	if (z19fd162) {
		if (rows) y += pad;
		z07cecca(&P, z7108971 + pad, y, tw, 1);
		y += z19fd162;
	}
	if (V.zcf8a8e5) {
		y += pad;
		char t[512];
		z610fb2b(t, sizeof(t), V.z1e5d86a);
		snprintf(line, sizeof(line), "> %s%s", t, (!V.zcc658c2 && ((GetTickCount() / 500) & 1)) ? "|" : "");
		char* p = line;
		while (*p && z50ccdbc(p, 0, NULL) > tw) p++;
		zd1ca55b(img, w, h, bpp, pitch, z7108971 + pad, y, p, z98784dd, 0);
		y += lh;
		char z1d5151e[80];
		snprintf(z1d5151e, sizeof(z1d5151e), z34ba319("Enter envia - Esc fecha   %d/%d"), V.nInput, V.nMax);
		zd1ca55b(img, w, h, bpp, pitch, z7108971 + pad, y, z1d5151e, 0x8A93A6, 0);
	}
}

void zf9658e7(unsigned char* img, int w, int h, int bpp, int pitch)
{
	z444375c(img, w, h, bpp, pitch);
	if (z58890ec || !img || w <= 0 || h <= 0 || pitch <= 0) return;
	if (bpp != 2 && bpp != 3 && bpp != 4) return;
	z1f618b1(img, w, h, bpp, pitch);
	zce4915c(img, w, h, bpp, pitch);
	zfa8d765(img, w, h, bpp, pitch);
}

static void z444375c(unsigned char* img, int w, int h, int bpp, int pitch)
{
	z16fe25d = z72f5dac = 0;
	if (z58890ec) return;
	if (!img || w <= 0 || h <= 0 || pitch <= 0) return;
	if (bpp != 2 && bpp != 3 && bpp != 4) return;

	z1fa3283 L;
	if (!z8e16a23(&L)) return;

	z8b719e7();
	if (!z1e0ebd8) return;

	if (!z8183630) {
		z8183630 = 1;
		z5d90bae("overlay: desenhando DENTRO da imagem do jogo "
		               "(este blitter nao desenha a barra) - vai ficar borrado. "
		               "Use o blitter DirectX 9 no menu Video.");
	}

	const int idx = (L.nMode == 2) ? 1 : 0;

	auto zbbb2811 = [&](int x, const char* s, unsigned int rgb) -> int {
		int n = 0;
		if (s && s[0] == '[')
			for (int i = 1; s[i] && i < 12; i++)
				if (s[i] == ']') { n = s[i + 1] ? i + 1 : 0; break; }
		if (!n) return zd1ca55b(img, w, h, bpp, pitch, x, 1, s, rgb, idx);
		char tag[16];
		memcpy(tag, s, n);
		tag[n] = 0;
		const int wt = zd1ca55b(img, w, h, bpp, pitch, x, 1, tag, z940b7f3, idx);
		return wt + zd1ca55b(img, w, h, bpp, pitch, x + wt, 1, s + n, rgb, idx);
	};

	int th = 0;
	const int w1  = z50ccdbc(L.z733a844, idx, &th);
	const int ws1 = z50ccdbc(L.zfad56d7, idx, NULL);
	const int z8d214a5 = z50ccdbc(L.z5ea9399, idx, NULL);
	const int z6437052 = z50ccdbc(L.z561be8d, idx, NULL);
	if (th <= 0) return;

	const int gap  = (idx ? z15094f0 : z9f11055) / 2 + 2;
	const int z91e120f = gap * 2;

	const int z404a75f = th + 2;
	z936df24(img, w, h, bpp, pitch, 0, z404a75f);

	const int mid   = w / 2;
	const int half  = z8d214a5 ? (z8d214a5 / 2 + z91e120f) : z91e120f / 2;
	const int y     = 1;

	int x = mid - half - ws1;
	zd1ca55b(img, w, h, bpp, pitch, x, y, L.zfad56d7, L.ze871279, idx);
	zbbb2811(x - gap - w1, L.z733a844, L.z03b5823);

	if (z8d214a5) zd1ca55b(img, w, h, bpp, pitch, mid - z8d214a5 / 2, y, L.z5ea9399, L.z73d250a, idx);

	x = mid + half;
	zd1ca55b(img, w, h, bpp, pitch, x, y, L.z561be8d, L.ze871279, idx);
	const int wp2 = zbbb2811(x + z6437052 + gap, L.zd5e0939, L.z20dcc8d);

	z16fe25d  = x + z6437052 + gap + wp2;
	z72f5dac = z404a75f;
}
