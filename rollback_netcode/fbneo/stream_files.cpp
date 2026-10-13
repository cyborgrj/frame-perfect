#include "stream_files.h"
#include "match_score.h"
#include "char_names.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

static int     z910e658 = 0;
static wchar_t z909a4d0[MAX_PATH];
static char    z716dbe5[32];
static char    z93f4c3c[128], z55eba79[128];
static int     za5fd107 = 0;
static int     ze70b5bb = -1, za56ef75 = -1;
static int     z468a6da = -2, z4d782db = -2;
static int     z70c8cfd = 0;

static void put(const wchar_t* name, const char* text, size_t len)
{
	wchar_t path[MAX_PATH], tmp[MAX_PATH];
	_snwprintf(path, MAX_PATH, L"%s\\%s", z909a4d0, name);
	_snwprintf(tmp, MAX_PATH, L"%s\\%s.tmp", z909a4d0, name);
	path[MAX_PATH - 1] = tmp[MAX_PATH - 1] = 0;
	FILE* f = _wfopen(tmp, L"wb");
	if (!f) return;
	if (len) fwrite(text, 1, len, f);
	fclose(f);
	if (!MoveFileExW(tmp, path, MOVEFILE_REPLACE_EXISTING)) DeleteFileW(tmp);
}

static void z00b0657(const wchar_t* name, const char* text) { put(name, text ? text : "", text ? strlen(text) : 0); }

static void z406dedc(const wchar_t* name, int n)
{
	char b[16];
	snprintf(b, sizeof(b), "%d", n);
	z00b0657(name, b);
}

static void z6fe6fdb(const char* z4601777, char* out, int nOut)
{
	out[0] = 0;
	if (!z4601777 || !z4601777[0]) return;
	wchar_t w[128];
	if (MultiByteToWideChar(CP_ACP, 0, z4601777, -1, w, 128) <= 0) return;
	w[127] = 0;
	if (WideCharToMultiByte(CP_UTF8, 0, w, -1, out, nOut, NULL, NULL) <= 0) out[0] = 0;
	out[nOut - 1] = 0;
}

static void z9c2fd51(const wchar_t* name, char* out, int nOut)
{
	wchar_t w[64];
	out[0] = 0;
	DWORD n = GetEnvironmentVariableW(name, w, 64);
	if (!n || n >= 64) return;
	if (WideCharToMultiByte(CP_UTF8, 0, w, -1, out, nOut, NULL, NULL) <= 0) out[0] = 0;
	out[nOut - 1] = 0;
}

static void zeff54eb(const wchar_t* name, const char* z2831ef1)
{
	wchar_t src[MAX_PATH], dst[MAX_PATH];
	_snwprintf(dst, MAX_PATH, L"%s\\%s", z909a4d0, name);
	dst[MAX_PATH - 1] = 0;
	int ok = 0;
	if (z2831ef1 && strlen(z2831ef1) == 2) {
		_snwprintf(src, MAX_PATH, L"%s\\bandeiras\\%c%c.png", z909a4d0, (wchar_t)z2831ef1[0], (wchar_t)z2831ef1[1]);
		src[MAX_PATH - 1] = 0;
		ok = CopyFileW(src, dst, FALSE);
	}
	if (!ok) {
		_snwprintf(src, MAX_PATH, L"%s\\bandeiras\\vazio.png", z909a4d0);
		src[MAX_PATH - 1] = 0;
		if (!CopyFileW(src, dst, FALSE)) DeleteFileW(dst);
	}
}

static const char* canonical(const char* g)
{
	if (!strcmp(g, "vsavj"))  return "vsav";
	if (!strcmp(g, "ssf2xj")) return "ssf2t";
	if (!strcmp(g, "umk3jug")) return "umk3";
	return g;
}

static const char* z1f00c1c(int id)
{
	const char* g = canonical(z716dbe5);
	for (size_t i = 0; i < sizeof(zda8350e) / sizeof(zda8350e[0]); i++)
		if (zda8350e[i].nId == id && !strcmp(zda8350e[i].szGame, g)) return zda8350e[i].szName;
	return NULL;
}

void z5c3a407(const char* szGame, const char* zb306bef, const char* z733a844,
                      const char* zd5e0939, int ze6bfbdc, int z9feb042)
{
	z910e658 = 0;
	DWORD n = GetEnvironmentVariableW(L"RBF_STREAM_DIR", z909a4d0, MAX_PATH);
	if (!n || n >= MAX_PATH) return;
	CreateDirectoryW(z909a4d0, NULL);
	z910e658 = 1;

	strncpy(z716dbe5, szGame ? szGame : "", sizeof(z716dbe5) - 1);
	z716dbe5[sizeof(z716dbe5) - 1] = 0;
	z6fe6fdb(z733a844, z93f4c3c, sizeof(z93f4c3c));
	z6fe6fdb(zd5e0939, z55eba79, sizeof(z55eba79));
	za5fd107 = ze6bfbdc;
	ze70b5bb = za56ef75 = -1;
	z468a6da = z4d782db = -2;
	z70c8cfd = 0;

	char type[64], c1[8], c2[8];
	z9c2fd51(L"RBF_STREAM_GAMETYPE", type, sizeof(type));
	if (!type[0] && z9feb042) strcpy(type, "Spectator");
	z9c2fd51(L"RBF_STREAM_P1COUNTRY", c1, sizeof(c1));
	z9c2fd51(L"RBF_STREAM_P2COUNTRY", c2, sizeof(c2));

	char vs[16];
	if (ze6bfbdc > 0) snprintf(vs, sizeof(vs), "FT%d", ze6bfbdc);
	else strcpy(vs, "Livre");

	z00b0657(L"game.txt", canonical(z716dbe5));
	z00b0657(L"gamequark.txt", zb306bef);
	z00b0657(L"gametype.txt", type);
	z00b0657(L"vs.txt", vs);
	z00b0657(L"p1name.txt", z93f4c3c);
	z00b0657(L"p2name.txt", z55eba79);
	z00b0657(L"p1country.txt", c1);
	z00b0657(L"p2country.txt", c2);
	zeff54eb(L"p1country.png", c1);
	zeff54eb(L"p2country.png", c2);
	z00b0657(L"p1rank.txt", "");
	z00b0657(L"p2rank.txt", "");
	z00b0657(L"p1score.txt", "0");
	z00b0657(L"p2score.txt", "0");
	z00b0657(L"p1character.txt", "");
	z00b0657(L"p2character.txt", "");
	z00b0657(L"winner.txt", "");
	z00b0657(L"started.inf", "1");
}

void z4aa24cc(void)
{
	if (!z910e658) return;

	int a = 0, b = 0;
	zd82e415(&a, &b);
	if (a != ze70b5bb || b != za56ef75) {
		if (za5fd107 <= 0 && ze70b5bb >= 0 && (a > ze70b5bb || b > za56ef75))
			z00b0657(L"winner.txt", a > ze70b5bb ? z93f4c3c : z55eba79);
		if (a != ze70b5bb) z406dedc(L"p1score.txt", a);
		if (b != za56ef75) z406dedc(L"p2score.txt", b);
		ze70b5bb = a;
		za56ef75 = b;
	}
	if (za5fd107 > 0 && !z70c8cfd && ze949c69()) {
		z00b0657(L"winner.txt", a > b ? z93f4c3c : z55eba79);
		z70c8cfd = 1;
	}

	int c1 = -1, c2 = -1;
	z4f974fb(&c1, &c2);
	if (c1 >= 0 && c1 != z468a6da) {
		const char* s = z1f00c1c(c1);
		char num[16];
		if (!s) { snprintf(num, sizeof(num), "%d", c1); s = num; }
		z00b0657(L"p1character.txt", s);
		z468a6da = c1;
	}
	if (c2 >= 0 && c2 != z4d782db) {
		const char* s = z1f00c1c(c2);
		char num[16];
		if (!s) { snprintf(num, sizeof(num), "%d", c2); s = num; }
		z00b0657(L"p2character.txt", s);
		z4d782db = c2;
	}
}

void za41c2ab(void)
{
	if (!z910e658) return;
	z00b0657(L"started.inf", "0");
	z910e658 = 0;
}
