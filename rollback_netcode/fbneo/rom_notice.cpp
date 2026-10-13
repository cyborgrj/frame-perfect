#include "rom_notice.h"
#include "img_hash.h"
#include "fbneo_host.h"
#include "lang.h"

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

static int zcd2491d = 0;
static int z1aac7b8   = 0;

void zd9b8167(int bOn) { zcd2491d = bOn ? 1 : 0; }
int  zd9791ed(void)   { return zcd2491d; }
int  zc4b05e3(void)     { return z1aac7b8; }

static void ze5ef9d5(const char* szFull, const char* zb49f688, char* out, int n)
{
	out[0] = 0;
	const char* s = (szFull && szFull[0]) ? szFull : (zb49f688 ? zb49f688 : "");
	strncpy(out, s, n - 1);
	out[n - 1] = 0;
	static const char* cuts[] = { " / ", " (", " [" };
	for (unsigned i = 0; i < sizeof(cuts) / sizeof(cuts[0]); i++) {
		char* c = strstr(out, cuts[i]);
		if (c && c != out) *c = 0;
	}
	char* d = strstr(out, " - ");
	if (d && d - out >= 8) *d = 0;
	if (!out[0] && zb49f688) { strncpy(out, zb49f688, n - 1); out[n - 1] = 0; }
}

static void cat(wchar_t* dst, int n, const wchar_t* s)
{
	const int have = (int)wcslen(dst);
	if (have < n - 1) wcsncat(dst, s, (size_t)(n - 1 - have));
}

static void z3b029f1(wchar_t* dst, int n, const char* s)
{
	wchar_t w[512];
	if (!s) return;
	if (MultiByteToWideChar(CP_ACP, 0, s, -1, w, 512) <= 0) return;
	w[511] = 0;
	cat(dst, n, w);
}

static void z255b231(const wchar_t* szText)
{
	char a[1024];
	if (WideCharToMultiByte(CP_ACP, 0, szText, -1, a, sizeof(a), NULL, NULL) <= 0) a[0] = 0;
	a[sizeof(a) - 1] = 0;
	for (char* c = a; *c; c++) if (*c == '\n' || *c == '\r') *c = ' ';
	char line[1100];
	snprintf(line, sizeof(line), "rom: caixa \"%s\"", a);
	z5d90bae(line);
}

void z90fdabc(int z2642b88, const char* szFullName, const char* szShortName,
                   const char* za6a9f59, const wchar_t* zc6728fe)
{
	char title[160];
	ze5ef9d5(szFullName, szShortName, title, sizeof(title));

	char own[64], list[300];
	snprintf(own, sizeof(own), "%s.zip", szShortName ? szShortName : "");
	snprintf(list, sizeof(list), ", %s,", za6a9f59 ? za6a9f59 : "");
	char z7a63d3b[80];
	snprintf(z7a63d3b, sizeof(z7a63d3b), " %s,", own);
	const int bOther = za6a9f59 && za6a9f59[0] && strstr(list, z7a63d3b) == NULL;
	const int bMany  = bOther && strchr(za6a9f59, ',') != NULL;

	wchar_t text[1024];
	text[0] = 0;
	const int n = (int)(sizeof(text) / sizeof(text[0]));
	if (z2642b88 == z2d06792) {
		cat(text, n, z6f2968c(L"N\x00e3o encontramos a ROM de "));
		z3b029f1(text, n, title);
		if (bOther) {
			cat(text, n, bMany ? z6f2968c(L" (faltam os arquivos ") : z6f2968c(L" (falta o arquivo "));
			z3b029f1(text, n, za6a9f59);
			cat(text, n, L")");
		}
		cat(text, n, z6f2968c(L".\n\nBaixe a ROM pelo Frame Perfect e tente de novo."));
	} else if (z2642b88 == z3724591) {
		cat(text, n, z6f2968c(L"A ROM de "));
		z3b029f1(text, n, title);
		cat(text, n, z6f2968c(L" est\x00e1 incompleta ou \x00e9 de outra vers\x00e3o.\n\n"));
		if (zc6728fe && zc6728fe[0]) {
			cat(text, n, z6f2968c(L"Apague este arquivo e baixe a ROM de novo pelo Frame Perfect:\n"));
			cat(text, n, zc6728fe);
		} else {
			cat(text, n, z6f2968c(L"Apague o arquivo dela e baixe a ROM de novo pelo Frame Perfect."));
		}
	} else if (z2642b88 == z4f1931a) {
		cat(text, n, z6f2968c(L"Esta vers\x00e3o do emulador n\x00e3o tem o jogo "));
		z3b029f1(text, n, title);
		cat(text, n, z6f2968c(L".\n\nAtualize o Frame Perfect e tente de novo."));
	} else {
		cat(text, n, z6f2968c(L"N\x00e3o foi poss\x00edvel abrir "));
		z3b029f1(text, n, title);
		cat(text, n, z6f2968c(L".\n\nO motivo est\x00e1 em rbf-netplay.log, ao lado do emulador."));
	}

	{
		char line[600], path[400];
		path[0] = 0;
		if (zc6728fe && WideCharToMultiByte(CP_ACP, 0, zc6728fe, -1, path, sizeof(path), NULL, NULL) <= 0) path[0] = 0;
		path[sizeof(path) - 1] = 0;
		const int z81f2c42 = za6a9f59 && za6a9f59[0];
		snprintf(line, sizeof(line), "rom: %s nao abriu - %s%s%s%s",
		         szShortName ? szShortName : "?",
		         z2642b88 == z2d06792 ? "faltando" : z2642b88 == z3724591 ? "incompleta ou de outra versao" :
		         z2642b88 == z4f1931a ? "este emulador nao tem o jogo" : "o jogo nao iniciou",
		         z81f2c42 ? " (sem " : "", z81f2c42 ? za6a9f59 : "", z81f2c42 ? ")" : "");
		z5d90bae(line);
		if (path[0]) {
			snprintf(line, sizeof(line), "rom: arquivo %s", path);
			z5d90bae(line);
		}
	}
	z255b231(text);
	z1aac7b8 = 1;

	const char* szCmd = GetCommandLineA();
	if (zfe62411() || (szCmd && strstr(szCmd, "-rbffundo"))) {
		z5d90bae("rom: -rbffundo - sem a caixa, so o log");
		return;
	}
	MessageBoxW(NULL, text, L"Frame Perfect", MB_OK | MB_ICONWARNING | MB_SETFOREGROUND | MB_TOPMOST);
}
