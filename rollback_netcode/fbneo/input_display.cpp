#include "input_display.h"

#include <stdio.h>
#include <string.h>

extern "C" unsigned int z3859adb(int nPlayer);
extern "C" const char*  z536c3a8(int nPlayer, int nBit);

#define z7e3c39f 32
#define z81eecbc 18

typedef struct { unsigned int nState; int nFrames; } z6f7d419;

typedef struct {
	int          zee1e43f;
	unsigned int zd3cae42, zf164fd3, zdc304fe, zad258ad, z156adbb;
	char         za8f3d1d[z7e3c39f][4];
	z6f7d419        aRow[z81eecbc];
	int          z7631798, nCount;
} zcdee996;

static int    z9126c4e = zff3953a;
static int    zc2a5719 = 0;
static zcdee996 zcd602dd[2];

static int has(const char* s, const char* w) { return s && strstr(s, w) != NULL; }

static void label(const char* n, char* out)
{
	const char* s = strchr(n, ' ');
	s = s ? s + 1 : n;
	if (!strncmp(s, "Button ", 7) && s[7]) { out[0] = s[7]; out[1] = 0; return; }
	if (has(s, "Weak Punch") || has(s, "Jab"))                      { strcpy(out, "LP"); return; }
	if (has(s, "Medium Punch"))                                     { strcpy(out, "MP"); return; }
	if (has(s, "Strong Punch") || has(s, "Heavy Punch") || has(s, "Fierce")) { strcpy(out, "HP"); return; }
	if (has(s, "Weak Kick") || has(s, "Short"))                     { strcpy(out, "LK"); return; }
	if (has(s, "Medium Kick") || has(s, "Forward"))                 { strcpy(out, "MK"); return; }
	if (has(s, "Strong Kick") || has(s, "Heavy Kick") || has(s, "Roundhouse")) { strcpy(out, "HK"); return; }
	if (!strncmp(s, "Fire ", 5) && s[5]) { out[0] = s[5]; out[1] = 0; return; }
	out[0] = s[0] ? s[0] : '?';
	out[1] = (s[0] && s[1] && s[1] != ' ') ? s[1] : 0;
	out[2] = 0;
	if (out[1] >= 'a' && out[1] <= 'z') out[1] -= 32;
}

static void masks(int p, zcdee996* s)
{
	s->zee1e43f = 1;
	for (int b = 0; b < z7e3c39f; b++) {
		const char* n = z536c3a8(p, b);
		if (!n) continue;
		if (has(n, "Coin") || has(n, "Start") || has(n, "Service") || has(n, "Reset") ||
		    has(n, "Diag") || has(n, "Test") || has(n, "Select") || has(n, "Dip") || has(n, "Tilt")) continue;
		const unsigned int m = 1u << b;
		if (has(n, " Up"))    { s->zd3cae42    |= m; continue; }
		if (has(n, " Down"))  { s->zf164fd3  |= m; continue; }
		if (has(n, " Left"))  { s->zdc304fe  |= m; continue; }
		if (has(n, " Right")) { s->zad258ad |= m; continue; }
		s->z156adbb |= m;
		label(n, s->za8f3d1d[b]);
	}
}

void z7619697(int nMode)
{
	z9126c4e = (nMode >= zff3953a && nMode <= z51cba50) ? nMode : zff3953a;
	zc2a5719 = 1;
}

int z0c5a8be(void) { return z9126c4e; }

void zee5ee89(void)
{
	for (int p = 0; p < 2; p++) { zcd602dd[p].z7631798 = 0; zcd602dd[p].nCount = 0; }
}

void z0b2e6fa(void)
{
	if (!zc2a5719) return;
	for (int p = 0; p < 2; p++) {
		zcdee996* s = &zcd602dd[p];
		if (!s->zee1e43f) masks(p, s);
		const unsigned int st = z3859adb(p) & (s->zd3cae42 | s->zf164fd3 | s->zdc304fe | s->zad258ad | s->z156adbb);
		z6f7d419* r = &s->aRow[s->z7631798];
		if (s->nCount > 0 && r->nState == st) {
			if (r->nFrames < 999) r->nFrames++;
			continue;
		}
		s->z7631798 = (s->z7631798 + 1) % z81eecbc;
		s->aRow[s->z7631798].nState  = st;
		s->aRow[s->z7631798].nFrames = 1;
		if (s->nCount < z81eecbc) s->nCount++;
	}
}

int za5c0699(int nSide, z72561c1* out, int nMax)
{
	if (z9126c4e == zff3953a || nSide < 1 || nSide > 2 || !out) return 0;
	if (!(z9126c4e & nSide)) return 0;
	const zcdee996* s = &zcd602dd[nSide - 1];
	int n = 0;
	for (int i = 0; i < s->nCount && n < nMax; i++) {
		const z6f7d419* r = &s->aRow[(s->z7631798 - i + z81eecbc) % z81eecbc];
		const int u = (r->nState & s->zd3cae42) != 0, d = (r->nState & s->zf164fd3) != 0;
		const int l = (r->nState & s->zdc304fe) != 0, rt = (r->nState & s->zad258ad) != 0;
		const int v = (u && !d) ? 1 : (d && !u) ? -1 : 0;
		const int h = (rt && !l) ? 1 : (l && !rt) ? -1 : 0;
		wchar_t c = 0x2022;
		if (v == 1)  c = h == 1 ? 0x2197 : h == -1 ? 0x2196 : 0x2191;
		if (v == -1) c = h == 1 ? 0x2198 : h == -1 ? 0x2199 : 0x2193;
		if (v == 0 && h != 0) c = h == 1 ? 0x2192 : 0x2190;
		z72561c1* o = &out[n++];
		o->z64f3ab9[0] = c;
		o->z64f3ab9[1] = 0;
		o->z34fc6fc[0] = 0;
		for (int b = 0; b < z7e3c39f; b++) {
			if (!(r->nState & s->z156adbb & (1u << b))) continue;
			if (o->z34fc6fc[0] && strlen(o->z34fc6fc) + 1 < sizeof(o->z34fc6fc)) strcat(o->z34fc6fc, " ");
			if (strlen(o->z34fc6fc) + strlen(s->za8f3d1d[b]) < sizeof(o->z34fc6fc)) strcat(o->z34fc6fc, s->za8f3d1d[b]);
		}
		o->nFrames = r->nFrames;
	}
	return n;
}
