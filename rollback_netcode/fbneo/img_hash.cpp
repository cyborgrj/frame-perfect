#include "burnint.h"
#include "img_hash.h"
#include "fbneo_host.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

static FILE* z066dade   = NULL;
static int   zdc94368   = 0;
static int   z06ed080  = 0;
static int   zd07604b   = 0;
static int   z4ffc7e6 = 0;
static unsigned zac6f163 = 0, z9de8ef3 = 0xFFFFFFFFu;

static void z03973aa(const unsigned char* zf154067, int w, int h, int bpp, int pitch, const char* szPrefix = "rbf-img",
                    unsigned nFrame = (unsigned)-1, int zb4ad809 = 0)
{
	char path[64];
	snprintf(path, sizeof(path), zb4ad809 ? "%s-%u.bmp" : "%s-%06u.bmp", szPrefix, nFrame == (unsigned)-1 ? (unsigned)nCurrentFrame : nFrame);
	FILE* f = fopen(path, "wb");
	if (!f) return;
	const int row = (w * 3 + 3) & ~3;
	const unsigned size = 54 + (unsigned)row * h;
	unsigned char hd[54];
	memset(hd, 0, sizeof(hd));
	hd[0] = 'B'; hd[1] = 'M';
	memcpy(hd + 2, &size, 4);
	hd[10] = 54; hd[14] = 40;
	memcpy(hd + 18, &w, 4); memcpy(hd + 22, &h, 4);
	hd[26] = 1; hd[28] = 24;
	fwrite(hd, 1, 54, f);
	unsigned char* line = (unsigned char*)calloc(row, 1);
	for (int y = h - 1; y >= 0; y--) {
		const unsigned char* src = zf154067 + (size_t)y * pitch;
		for (int x = 0; x < w; x++) {
			unsigned char r, g, b;
			if (bpp == 2) {
				const unsigned v = src[x * 2] | (src[x * 2 + 1] << 8);
				r = (unsigned char)(((v >> 11) & 31) << 3);
				g = (unsigned char)(((v >> 5) & 63) << 2);
				b = (unsigned char)((v & 31) << 3);
			} else {
				b = src[x * bpp]; g = src[x * bpp + 1]; r = src[x * bpp + 2];
			}
			line[x * 3] = b; line[x * 3 + 1] = g; line[x * 3 + 2] = r;
		}
		fwrite(line, 1, row, f);
	}
	free(line);
	fclose(f);
}

void zed87ca0(const char* zdc47492, int nFrames)
{
	char path[128];
	snprintf(path, sizeof(path), "rbf-imghash-%s.txt", (zdc47492 && zdc47492[0]) ? zdc47492 : "x");
	z066dade = fopen(path, "w");
	if (!z066dade) return;
	setvbuf(z066dade, NULL, _IOFBF, 1 << 20);
	zdc94368  = nFrames > 0 ? nFrames : 3000;
	z06ed080 = 0;
	zd07604b  = 0;
	char v[16] = "";
	z4ffc7e6 = GetEnvironmentVariableA("RBF_IMGDUMP", v, sizeof(v)) ? atoi(v) : 0;
	if (GetEnvironmentVariableA("RBF_IMGDUMP_DE", v, sizeof(v)))  zac6f163 = (unsigned)strtoul(v, NULL, 10);
	if (GetEnvironmentVariableA("RBF_IMGDUMP_ATE", v, sizeof(v))) z9de8ef3 = (unsigned)strtoul(v, NULL, 10);
}

void za50e763(const unsigned char* zf154067, int w, int h, int bpp, int pitch)
{
	static int z459319d = -1;
	static unsigned z98cd887 = 0, z5dafe4b = 0xFFFFFFFFu;
	if (z459319d < 0) {
		char v[16] = "";
		z459319d = GetEnvironmentVariableA("RBF_IMGDUMP_TELA", v, sizeof(v)) ? atoi(v) : 0;
		if (GetEnvironmentVariableA("RBF_IMGDUMP_DE", v, sizeof(v)))  z98cd887 = (unsigned)strtoul(v, NULL, 10);
		if (GetEnvironmentVariableA("RBF_IMGDUMP_ATE", v, sizeof(v))) z5dafe4b = (unsigned)strtoul(v, NULL, 10);
	}
	if (z459319d <= 0 || !zf154067 || w <= 0 || h <= 0 || (bpp != 2 && bpp != 3 && bpp != 4)) return;
	if ((nCurrentFrame % z459319d) != 0 || nCurrentFrame < z98cd887 || nCurrentFrame > z5dafe4b) return;
	z03973aa(zf154067, w, h, bpp, pitch, "rbf-tela");
}

void zd5697a3(const char* szPrefix, unsigned nFrame, const unsigned char* zf154067, int w, int h, int bpp, int pitch)
{
	if (!zf154067 || w <= 0 || h <= 0 || (bpp != 2 && bpp != 3 && bpp != 4)) return;
	z03973aa(zf154067, w, h, bpp, pitch, szPrefix, nFrame);
}

static int z5d53e64 = 0;
void z40cd21d(int bOn) { z5d53e64 = bOn ? 1 : 0; }
int  zfe62411(void)       { return z5d53e64; }

#define zd80184b 32
static int zcd50d63 = -1;
static int z13cc027[zd80184b];

int ze609dec(void)
{
	if (zcd50d63 < 0) {
		zcd50d63 = 0;
		char v[256] = "";
		if (z5d53e64 && GetEnvironmentVariableA("RBF_TESTE_PRINT", v, sizeof(v)) && v[0]) {
			for (char* t = strtok(v, ",; "); t && zcd50d63 < zd80184b; t = strtok(NULL, ",; ")) {
				const int f = atoi(t);
				if (f > 0) z13cc027[zcd50d63++] = f;
			}
		}
	}
	if (!zcd50d63 || !z5d53e64) return -1;
	const long long now = zb71d2fc() ? zca74129() : (long long)nCurrentFrame;
	for (int i = 0; i < zcd50d63; i++) {
		if (z13cc027[i] <= 0 || now < z13cc027[i]) continue;
		const int f = z13cc027[i];
		z13cc027[i] = 0;
		return f;
	}
	return -1;
}

void z18c8ef8(int nFrame, const unsigned char* zf154067, int w, int h, int bpp, int pitch)
{
	if (!zf154067 || w <= 0 || h <= 0 || (bpp != 2 && bpp != 3 && bpp != 4)) return;
	z03973aa(zf154067, w, h, bpp, pitch, "rbf-print", (unsigned)nFrame, 1);
	char line[96];
	snprintf(line, sizeof(line), "teste: RBF_TESTE_PRINT - rbf-print-%d.bmp (%dx%d)", nFrame, w, h);
	z5d90bae(line);
}

void z363f3e1(const unsigned char* zf154067, int w, int h, int bpp, int pitch)
{
	const int f = ze609dec();
	if (f > 0) z18c8ef8(f, zf154067, w, h, bpp, pitch);
}

int z21a6605(void) { return z066dade != NULL && !zd07604b; }

int z3a34c01(const unsigned char* zf154067, int nWidth, int nHeight, int nBpp, int nPitch)
{
	if (!z21a6605() || !zf154067 || nWidth <= 0 || nHeight <= 0) return 0;

	unsigned long long h = 1469598103934665603ULL;
	const int nRow = nWidth * nBpp;
	for (int y = 0; y < nHeight; y++) {
		const unsigned char* p = zf154067 + (size_t)y * nPitch;
		for (int x = 0; x < nRow; x++) {
			h ^= p[x];
			h *= 1099511628211ULL;
		}
	}

	fprintf(z066dade, "%u %016llx\n", (unsigned)nCurrentFrame, h);
	if (z4ffc7e6 > 0 && (nCurrentFrame % z4ffc7e6) == 0 && nCurrentFrame >= zac6f163 && nCurrentFrame <= z9de8ef3)
		z03973aa(zf154067, nWidth, nHeight, nBpp, nPitch);

	if (++z06ed080 >= zdc94368) {
		fclose(z066dade);
		z066dade = NULL;
		zd07604b = 1;
		return 1;
	}
	return 0;
}
