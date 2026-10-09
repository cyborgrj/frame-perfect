#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <process.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "relay.h"

#define z883d5c8        "RBF1"
#define zcb668c7     (256 * 1024)
#define zb865e8b   8000
#define z0cdf08a      512
#define z3da9a23      (4 + 64)
#define z964e4ff     500

#define zd59814f     4096
#define zc2c6d55      (32 * 1024)
#define z926b46c       (16 * 1024 * 1024)
#define z62bc0f9       28

extern "C" {
int compress2(unsigned char* dest, unsigned long* destLen, const unsigned char* source, unsigned long sourceLen, int level);
unsigned long compressBound(unsigned long sourceLen);
}

static void z2db8ee2(unsigned char* p, unsigned int v)
{
	p[0] = (unsigned char)v; p[1] = (unsigned char)(v >> 8); p[2] = (unsigned char)(v >> 16); p[3] = (unsigned char)(v >> 24);
}

static unsigned int get32(const unsigned char* p)
{
	return (unsigned)p[0] | ((unsigned)p[1] << 8) | ((unsigned)p[2] << 16) | ((unsigned)p[3] << 24);
}

static void z1d4d1c9(void (*pfn)(const char*), const char* fmt, ...)
{
	if (!pfn) return;
	char buf[400];
	va_list ap;
	va_start(ap, fmt);
	_vsnprintf(buf, sizeof(buf) - 1, fmt, ap);
	va_end(ap);
	buf[sizeof(buf) - 1] = 0;
	pfn(buf);
}

static void sanitize(char* dst, int cap, const char* src)
{
	int i = 0;
	if (cap <= 0) return;
	for (; src && src[i] && i < cap - 1; i++)
		dst[i] = (src[i] <= ' ' || src[i] == 0x7f) ? '_' : src[i];
	if (i == 0 && cap > 1) dst[i++] = '?';
	dst[i] = 0;
}

static SOCKET z332a46a(const char* zf785ae5, unsigned short nPort, int* pErr, void (*z930c6f2)(const char*))
{
	char z00fcef4[16];
	sprintf(z00fcef4, "%u", (unsigned)nPort);

	struct addrinfo hints, *res = NULL;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family   = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	if (getaddrinfo(zf785ae5, z00fcef4, &hints, &res) != 0 || !res) {
		z1d4d1c9(z930c6f2, "relay: cannot resolve %s:%u (winsock %d)", zf785ae5, (unsigned)nPort, WSAGetLastError());
		*pErr = zeeb14a8;
		return INVALID_SOCKET;
	}

	SOCKET s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (s == INVALID_SOCKET) {
		freeaddrinfo(res);
		z1d4d1c9(z930c6f2, "relay: socket() failed (winsock %d)", WSAGetLastError());
		*pErr = z63d00a0;
		return INVALID_SOCKET;
	}

	int rc = connect(s, res->ai_addr, (int)res->ai_addrlen);
	freeaddrinfo(res);
	if (rc != 0) {
		closesocket(s);
		z1d4d1c9(z930c6f2, "relay: connect to %s:%u failed (winsock %d)", zf785ae5, (unsigned)nPort, WSAGetLastError());
		*pErr = zc62b008;
		return INVALID_SOCKET;
	}

	DWORD to = zb865e8b;
	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (const char*)&to, sizeof(to));
	setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, (const char*)&to, sizeof(to));
	int one = 1;
	setsockopt(s, IPPROTO_TCP, TCP_NODELAY, (const char*)&one, sizeof(one));

	*pErr = zd3e2c04;
	return s;
}

static int zc7cef08(SOCKET s, const void* p, int n)
{
	const char* b = (const char*)p;
	while (n > 0) {
		int k = send(s, b, n, 0);
		if (k <= 0) return z81c8baf;
		b += k;
		n -= k;
	}
	return zd3e2c04;
}

static int z8a1932a(SOCKET s, void* p, int n)
{
	char* b = (char*)p;
	while (n > 0) {
		int k = recv(s, b, n, 0);
		if (k <= 0) return z81c8baf;
		b += k;
		n -= k;
	}
	return zd3e2c04;
}

static int z0b5ac61(SOCKET s, char* out, int cap)
{
	int i = 0;
	while (i < cap - 1) {
		char c;
		int k = recv(s, &c, 1, 0);
		if (k <= 0) return z81c8baf;
		if (c == '\n') break;
		if (c != '\r') out[i++] = c;
	}
	out[i] = 0;
	return zd3e2c04;
}

static int zdb4a99e(void (*z930c6f2)(const char*))
{
	WSADATA z2957bc2;
	int rc = WSAStartup(MAKEWORD(2, 2), &z2957bc2);
	if (rc != 0) z1d4d1c9(z930c6f2, "relay: WSAStartup failed (%d)", rc);
	return rc == 0;
}

static struct {
	int              up;
	int              zfd4ce47;
	volatile LONG    stop;
	volatile LONG    broken;
	HANDLE           thread;
	CRITICAL_SECTION cs;
	int              z1e66b7c;

	char             zf785ae5[128];
	unsigned short   nPort;
	char             zb306bef[40];
	zac814b9  info;
	unsigned char*   pState;
	int              z0f2ef1e;
	void           (*z930c6f2)(const char*);

	unsigned char*   zaba658c;
	int              z7631798;
	int              zf3428fd;
	int              z313288a;
	volatile SOCKET   sock;
	char             zf275051[40];
} P;

static struct {
	HANDLE           thread;
	HANDLE           ev;
	volatile LONG    quit;
	volatile LONG    busy;
	volatile LONG    off;
	volatile SOCKET  sock;
	unsigned char*   raw;
	int              z77d09d2;
	int              zb7fad3d, zcd1f9d5, frame;
	unsigned int     chk;
	int              sent;
	char             zf785ae5[128];
	unsigned short   nPort;
	char             zb306bef[40];
	char             szKey[40];
	void           (*z930c6f2)(const char*);
} C;

static int z4be6b46(const void* p, int n)
{
	if (P.z313288a + n > zcb668c7) return 0;
	const unsigned char* b = (const unsigned char*)p;
	for (int i = 0; i < n; i++) {
		P.zaba658c[P.z7631798] = b[i];
		P.z7631798 = (P.z7631798 + 1) % zcb668c7;
	}
	P.z313288a += n;
	return 1;
}

static int z451d029(SOCKET s)
{
	fd_set rd;
	FD_ZERO(&rd);
	FD_SET(s, &rd);
	timeval tv = { 0, 0 };
	if (select(0, &rd, NULL, NULL, &tv) != 1) return 0;
	char line[128];
	const int n = recv(s, line, sizeof(line) - 1, 0);
	if (n <= 0) return 0;
	line[n] = 0;
	if (char* nl = strpbrk(line, "\r\n")) *nl = 0;
	if (strncmp(line, z883d5c8 " ERR", strlen(z883d5c8 " ERR")) != 0) return 0;
	z1d4d1c9(P.z930c6f2, "relay: o relay recusou a transmissao (%s) - o jogo segue sem espectadores", line);
	return 1;
}

static unsigned __stdcall ze1ca2ef(void*)
{
	int err = zd3e2c04;
	SOCKET s = z332a46a(P.zf785ae5, P.nPort, &err, P.z930c6f2);
	if (s == INVALID_SOCKET) {
		z1d4d1c9(P.z930c6f2, "relay: publish connect failed (%d) - match will not be watchable", err);
		InterlockedExchange(&P.broken, 1);
		return 0;
	}
	P.sock = s;

	char z733a844[zfb7dd07], zd5e0939[zfb7dd07], szGame[64];
	sanitize(z733a844, sizeof(z733a844), P.info.z733a844);
	sanitize(zd5e0939, sizeof(zd5e0939), P.info.zd5e0939);
	sanitize(szGame, sizeof(szGame), P.info.szGame);

	char extra[160] = "";
	if (P.info.z4b40456[0]) {
		char tok[40];
		sanitize(tok, sizeof(tok), P.info.z4b40456);
		_snprintf(extra, sizeof(extra) - 1, " tok=%s", tok);
	}
	if (P.info.zc16580c >= 0) {
		const size_t k = strlen(extra);
		_snprintf(extra + k, sizeof(extra) - 1 - k, " rst=%d", P.info.zc16580c);
	}
	if (P.zf275051[0]) {
		const size_t k = strlen(extra);
		_snprintf(extra + k, sizeof(extra) - 1 - k, " cpk=%s", P.zf275051);
	}
	extra[sizeof(extra) - 1] = 0;

	char hdr[z0cdf08a];
	int n = _snprintf(hdr, sizeof(hdr) - 1, "%s PUB %s %s %d %d %d %s %s%s\n",
	                  z883d5c8, P.zb306bef, szGame,
	                  P.info.z0d29982, P.info.z9df8312, P.z0f2ef1e, z733a844, zd5e0939, extra);
	if (n < 0) n = (int)strlen(hdr);

	if (zc7cef08(s, hdr, n) != zd3e2c04 ||
	    (P.z0f2ef1e > 0 && zc7cef08(s, P.pState, P.z0f2ef1e) != zd3e2c04)) {
		if (!z451d029(s)) z1d4d1c9(P.z930c6f2, "relay: publish header/state failed - match will not be watchable");
		P.sock = INVALID_SOCKET;
		closesocket(s);
		InterlockedExchange(&P.broken, 1);
		return 0;
	}
	if (z451d029(s)) {
		P.sock = INVALID_SOCKET;
		closesocket(s);
		InterlockedExchange(&P.broken, 1);
		return 0;
	}

	z1d4d1c9(P.z930c6f2, "relay: publishing match %s (%d byte state)", P.zb306bef, P.z0f2ef1e);
	P.up = 1;

	unsigned char chunk[4096];
	int   z3cd299d = 0;
	DWORD tStop = 0;
	for (;;) {
		const int z41c89a0 = P.stop != 0;
		if (z41c89a0 && !tStop) tStop = GetTickCount() | 1;
		int take = 0;
		EnterCriticalSection(&P.cs);
		while (take < (int)sizeof(chunk) && P.z313288a > 0) {
			chunk[take++] = P.zaba658c[P.zf3428fd];
			P.zf3428fd = (P.zf3428fd + 1) % zcb668c7;
			P.z313288a--;
		}
		const int left = P.z313288a;
		LeaveCriticalSection(&P.cs);

		if (take == 0) {
			if (z41c89a0) break;
			if (z451d029(s)) { z3cd299d = 1; break; }
			Sleep(4);
			continue;
		}
		if (zc7cef08(s, chunk, take) != zd3e2c04) {
			if (!z451d029(s)) z1d4d1c9(P.z930c6f2, "relay: publish stream dropped");
			z3cd299d = 1;
			break;
		}
		if (z41c89a0 && left > 0 && GetTickCount() - tStop > z964e4ff) {
			z1d4d1c9(P.z930c6f2, "relay: fim da transmissao com %d bytes ainda no anel (sem tempo para mandar)", left);
			break;
		}
	}

	if (!z3cd299d && P.stop) {
		shutdown(s, SD_SEND);
		DWORD to = z964e4ff;
		setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (const char*)&to, sizeof(to));
		char sink[64];
		while (recv(s, sink, sizeof(sink), 0) > 0) {}
	}

	P.up = 0;
	P.sock = INVALID_SOCKET;
	closesocket(s);
	InterlockedExchange(&P.broken, 1);
	return 0;
}

static void z2ed32ae(char* out, int cap);

int zf71597a(const char* zf785ae5, unsigned short nPort,
                      const char* zb306bef, const zac814b9* info,
                      const void* pState, int z0f2ef1e,
                      void (*z930c6f2)(const char*))
{
	if (!zf785ae5 || !*zf785ae5 || !nPort || !zb306bef || !*zb306bef || !info) return z1cd7dae;
	if (info->z0d29982 < 1 || info->z9df8312 < 1)                         return z1cd7dae;
	if (z0f2ef1e < 0 || (z0f2ef1e > 0 && !pState))                         return z1cd7dae;
	if (info->z0d29982 * info->z9df8312 > z3da9a23 - 4)              return z1cd7dae;

	z4568e55();
	memset(&P, 0, sizeof(P));

	if (!zdb4a99e(z930c6f2)) return z63d00a0;
	P.zfd4ce47 = 1;

	P.zaba658c  = (unsigned char*)malloc(zcb668c7);
	P.pState = z0f2ef1e > 0 ? (unsigned char*)malloc(z0f2ef1e) : NULL;
	if (!P.zaba658c || (z0f2ef1e > 0 && !P.pState)) {
		free(P.zaba658c);
		free(P.pState);
		WSACleanup();
		memset(&P, 0, sizeof(P));
		return zf2605d7;
	}
	if (z0f2ef1e > 0) memcpy(P.pState, pState, z0f2ef1e);
	P.z0f2ef1e = z0f2ef1e;

	strncpy(P.zf785ae5, zf785ae5, sizeof(P.zf785ae5) - 1);
	strncpy(P.zb306bef, zb306bef, sizeof(P.zb306bef) - 1);
	P.nPort  = nPort;
	P.info   = *info;
	P.z930c6f2 = z930c6f2;

	InitializeCriticalSection(&P.cs);
	P.z1e66b7c = 1;
	z2ed32ae(P.zf275051, sizeof(P.zf275051));

	P.thread = (HANDLE)_beginthreadex(NULL, 0, ze1ca2ef, NULL, 0, NULL);
	if (!P.thread) {
		DeleteCriticalSection(&P.cs);
		free(P.zaba658c);
		free(P.pState);
		WSACleanup();
		memset(&P, 0, sizeof(P));
		return z63d00a0;
	}
	return zd3e2c04;
}

void z59d3a98(int nFrame, const void* pInputs, int nBytes)
{
	if (!P.z1e66b7c || P.broken || nBytes <= 0 || nBytes > z3da9a23 - 4) return;

	unsigned char rec[z3da9a23];
	rec[0] = (unsigned char)(nFrame & 0xff);
	rec[1] = (unsigned char)((nFrame >> 8) & 0xff);
	rec[2] = (unsigned char)((nFrame >> 16) & 0xff);
	rec[3] = (unsigned char)((nFrame >> 24) & 0xff);
	memcpy(rec + 4, pInputs, nBytes);

	EnterCriticalSection(&P.cs);
	int ok = z4be6b46(rec, 4 + nBytes);
	LeaveCriticalSection(&P.cs);

	if (!ok) {
		InterlockedExchange(&P.broken, 1);
		z1d4d1c9(P.z930c6f2, "relay: publish buffer full - stopping the broadcast");
	}
}

int zcd7ed90(void) { return P.up && !P.broken; }

static void z2ed32ae(char* out, int cap)
{
	unsigned char b[16];
	memset(b, 0, sizeof(b));
	typedef BOOLEAN (WINAPI* z689d8b6)(PVOID, ULONG);
	HMODULE adv = LoadLibraryA("advapi32.dll");
	z689d8b6 gen = adv ? (z689d8b6)GetProcAddress(adv, "SystemFunction036") : NULL;
	if (!gen || !gen(b, sizeof(b))) {
		LARGE_INTEGER t;
		QueryPerformanceCounter(&t);
		for (int i = 0; i < 16; i++) b[i] = (unsigned char)((t.QuadPart >> ((i % 8) * 8)) ^ (GetCurrentProcessId() * 131 + i * 17));
	}
	static const char hex[] = "0123456789abcdef";
	int k = 0;
	for (int i = 0; i < 16 && k + 2 < cap; i++) { out[k++] = hex[b[i] >> 4]; out[k++] = hex[b[i] & 15]; }
	out[k] = 0;
}

static unsigned __stdcall z78905f9(void*)
{
	for (;;) {
		if (WaitForSingleObject(C.ev, INFINITE) != WAIT_OBJECT_0 || C.quit) break;
		if (!C.busy) continue;

		const int ze584d20 = C.zb7fad3d + C.zcd1f9d5;
		unsigned long zd2627c2 = compressBound((unsigned long)ze584d20);
		unsigned char* blob = (unsigned char*)malloc((size_t)z62bc0f9 + zd2627c2);
		if (blob && compress2(blob + z62bc0f9, &zd2627c2, C.raw, (unsigned long)ze584d20, 1) == 0 &&
		    z62bc0f9 + (int)zd2627c2 <= z926b46c) {
			memcpy(blob, "RBCK", 4);
			z2db8ee2(blob + 4, 1);
			z2db8ee2(blob + 8, (unsigned)C.frame);
			z2db8ee2(blob + 12, (unsigned)C.zb7fad3d);
			z2db8ee2(blob + 16, (unsigned)C.zcd1f9d5);
			z2db8ee2(blob + 20, C.chk);
			z2db8ee2(blob + 24, (unsigned)zd2627c2);
			const int total = z62bc0f9 + (int)zd2627c2;

			int err = zd3e2c04;
			SOCKET s = C.quit ? INVALID_SOCKET : z332a46a(C.zf785ae5, C.nPort, &err, NULL);
			if (s != INVALID_SOCKET) {
				C.sock = s;
				char line[z0cdf08a];
				int n = _snprintf(line, sizeof(line) - 1, "%s CPT %s %s %d %d\n", z883d5c8, C.zb306bef, C.szKey, C.frame, total);
				line[sizeof(line) - 1] = 0;
				if (n < 0) n = (int)strlen(line);
				int sent = zc7cef08(s, line, n) == zd3e2c04;
				const DWORD t0 = GetTickCount();
				for (int off = 0; sent && off < total && !C.quit; ) {
					const int k = total - off < zd59814f ? total - off : zd59814f;
					if (zc7cef08(s, blob + off, k) != zd3e2c04) { sent = 0; break; }
					off += k;
					const DWORD due = (DWORD)((long long)off * 1000 / zc2c6d55);
					const DWORD el = GetTickCount() - t0;
					if (due > el) Sleep(due - el);
				}
				if (sent && !C.quit) {
					DWORD to = 10000;
					setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (const char*)&to, sizeof(to));
					char resp[128] = "";
					if (z0b5ac61(s, resp, sizeof(resp)) == zd3e2c04 && strncmp(resp, z883d5c8 " OK", 7) == 0) {
						if (C.sent++ < 3)
							z1d4d1c9(C.z930c6f2, "relay: ponto salvo no quadro %d (%d KB)", C.frame, total / 1024);
					} else if (resp[0]) {
						z1d4d1c9(C.z930c6f2, "relay: o relay recusou o ponto salvo do quadro %d (%s)", C.frame, resp);
					} else if (!C.sent) {
						InterlockedExchange(&C.off, 1);
						z1d4d1c9(C.z930c6f2, "relay: o relay nao recebe ponto salvo (versao antiga) - so a transmissao de sempre");
					}
				}
				C.sock = INVALID_SOCKET;
				closesocket(s);
			}
		}
		free(blob);
		InterlockedExchange(&C.busy, 0);
	}
	return 0;
}

int zef7c603(void)
{
	return P.z1e66b7c && P.up && !P.broken && !P.stop && !C.off && !C.busy && P.zf275051[0];
}

int z99502a6(int nFrame, const void* z7106887, int z15b1a6d, const void* z9ae6e3a, int nScore, unsigned int z4570d0f)
{
	if (!zef7c603() || !z7106887 || z15b1a6d <= 0 || nScore < 0 || (nScore > 0 && !z9ae6e3a)) return 0;
	if (C.z77d09d2 < z15b1a6d + nScore) {
		if (C.raw) return 0;
		C.raw = (unsigned char*)malloc((size_t)z15b1a6d + nScore);
		if (!C.raw) return 0;
		C.z77d09d2 = z15b1a6d + nScore;
	}
	if (!C.thread) {
		strncpy(C.zf785ae5, P.zf785ae5, sizeof(C.zf785ae5) - 1);
		C.nPort = P.nPort;
		strncpy(C.zb306bef, P.zb306bef, sizeof(C.zb306bef) - 1);
		strncpy(C.szKey, P.zf275051, sizeof(C.szKey) - 1);
		C.z930c6f2 = P.z930c6f2;
		C.sock   = INVALID_SOCKET;
		C.ev  = CreateEventA(NULL, FALSE, FALSE, NULL);
		C.thread = C.ev ? (HANDLE)_beginthreadex(NULL, 0, z78905f9, NULL, 0, NULL) : NULL;
		if (!C.thread) { InterlockedExchange(&C.off, 1); return 0; }
		SetThreadPriority(C.thread, THREAD_PRIORITY_BELOW_NORMAL);
	}
	memcpy(C.raw, z7106887, (size_t)z15b1a6d);
	if (nScore > 0) memcpy(C.raw + z15b1a6d, z9ae6e3a, (size_t)nScore);
	C.zb7fad3d   = z15b1a6d;
	C.zcd1f9d5 = nScore;
	C.frame    = nFrame;
	C.chk      = z4570d0f;
	InterlockedExchange(&C.busy, 1);
	SetEvent(C.ev);
	return 1;
}

void z4568e55(void)
{
	if (!P.z1e66b7c) return;
	InterlockedExchange(&P.stop, 1);
	if (C.thread) {
		InterlockedExchange(&C.quit, 1);
		if (C.ev) SetEvent(C.ev);
		const SOCKET cs = C.sock;
		if (cs != INVALID_SOCKET && cs != 0) shutdown(cs, SD_BOTH);
		if (WaitForSingleObject(C.thread, 300) != WAIT_TIMEOUT) {
			CloseHandle(C.thread);
			if (C.ev) CloseHandle(C.ev);
			free(C.raw);
			memset(&C, 0, sizeof(C));
		}
	}
	if (P.thread) {
		if (WaitForSingleObject(P.thread, 3000) == WAIT_TIMEOUT) {
			const SOCKET s = P.sock;
			if (s != INVALID_SOCKET && s != 0) shutdown(s, SD_BOTH);
			if (WaitForSingleObject(P.thread, 3000) == WAIT_TIMEOUT) {
				return;
			}
		}
		CloseHandle(P.thread);
	}
	DeleteCriticalSection(&P.cs);
	free(P.zaba658c);
	free(P.pState);
	if (P.zfd4ce47) WSACleanup();
	memset(&P, 0, sizeof(P));
}

static struct {
	int              zfd4ce47;
	volatile LONG    stop;
	volatile LONG    ended;
	HANDLE           thread;
	CRITICAL_SECTION cs;
	int              z1e66b7c;

	SOCKET           sock;
	zac814b9  info;
	unsigned char*   pState;

	unsigned char*   pFrames;
	int              z91939f6;
	int              zb6a75c6;
	int              z176af93;
	int              nRead;

	int              zdd25234;
	int              zbe5e403;
	int              z74a693d;
	double           z8b34806;
	LONGLONG         z51e1324, zd0f7364;

	int              zca0b11f;
	unsigned char*   z9dbc625;
	int              z258b8c6;

	void           (*z930c6f2)(const char*);
} S;

static void zf62d1c0(void)
{
	if (!S.zdd25234) return;
	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	const double secs = (double)(now.QuadPart - S.z51e1324) / (double)S.zd0f7364;
	LONGLONG n = S.zbe5e403 + (LONGLONG)(secs * S.z8b34806);
	if (n >= S.z74a693d) { n = S.z74a693d; S.ended = 1; S.zdd25234 = 0; }
	S.z176af93 = (int)n;
}

int z67bbfd7(int nStart, double fps)
{
	if (!S.z1e66b7c || S.sock != INVALID_SOCKET || fps <= 0.0) return z1cd7dae;
	if (nStart < 0) nStart = 0;
	if (nStart > S.zb6a75c6) nStart = S.zb6a75c6;
	LARGE_INTEGER f, t;
	QueryPerformanceFrequency(&f);
	QueryPerformanceCounter(&t);
	S.z74a693d = S.zb6a75c6;
	S.zbe5e403 = nStart;
	S.z8b34806   = fps;
	S.zd0f7364  = f.QuadPart;
	S.z51e1324    = t.QuadPart;
	S.z176af93     = nStart;
	S.ended     = 0;
	S.zdd25234    = 1;
	return zd3e2c04;
}

static int z313ccaf(int nFrames)
{
	if (nFrames <= S.zb6a75c6) return 1;
	int cap = S.zb6a75c6 ? S.zb6a75c6 : 4096;
	while (cap < nFrames) cap *= 2;
	unsigned char* p = (unsigned char*)realloc(S.pFrames, (size_t)cap * S.z91939f6);
	if (!p) return 0;
	S.pFrames = p;
	S.zb6a75c6 = cap;
	return 1;
}

static unsigned __stdcall z4322d81(void*)
{
	unsigned char rec[z3da9a23];
	const int zd1b0d7d = 4 + S.z91939f6;

	while (!S.stop) {
		if (z8a1932a(S.sock, rec, zd1b0d7d) != zd3e2c04) break;

		int frame = (int)((unsigned)rec[0] | ((unsigned)rec[1] << 8) |
		                  ((unsigned)rec[2] << 16) | ((unsigned)rec[3] << 24));

		EnterCriticalSection(&S.cs);
		int expected = S.zca0b11f + S.z176af93;
		int ok = (frame == expected) && z313ccaf(S.z176af93 + 1);
		if (ok) {
			memcpy(S.pFrames + (size_t)S.z176af93 * S.z91939f6, rec + 4, S.z91939f6);
			S.z176af93++;
		}
		LeaveCriticalSection(&S.cs);

		if (!ok) {
			z1d4d1c9(S.z930c6f2, "relay: watch stream broke at frame %d (expected %d)", frame, expected);
			break;
		}
	}

	InterlockedExchange(&S.ended, 1);
	return 0;
}

static int z3efb156(const char* zf785ae5, unsigned short nPort,
                      const char* zb306bef, int zf13fff5, const char* szKey,
                      void (*z930c6f2)(const char*), int z8fe4467);

int z0244df4(const char* zf785ae5, unsigned short nPort,
                    const char* zb306bef, int zf13fff5, const char* szKey,
                    void (*z930c6f2)(const char*))
{
	return z3efb156(zf785ae5, nPort, zb306bef, zf13fff5, szKey, z930c6f2, 0);
}

static char           zb9029fb[128], z905ec26[64], z17cfaab[40];
static unsigned short za8df9d0;
static int            z4338b31;

int zc4093bf(const char* zf785ae5, unsigned short nPort,
                      const char* zb306bef, int zf13fff5, const char* szKey,
                      void (*z930c6f2)(const char*))
{
	if (!zf785ae5 || !zb306bef) return z1cd7dae;
	strncpy(zb9029fb, zf785ae5, sizeof(zb9029fb) - 1);
	strncpy(z905ec26, zb306bef, sizeof(z905ec26) - 1);
	z17cfaab[0] = 0;
	if (szKey) strncpy(z17cfaab, szKey, sizeof(z17cfaab) - 1);
	za8df9d0 = nPort;
	z4338b31 = zf13fff5;
	int rc = z3efb156(zf785ae5, nPort, zb306bef, zf13fff5, szKey, z930c6f2, 1);
	if (rc == z81c8baf) {
		z1d4d1c9(z930c6f2, "relay: o relay nao conhece o ponto salvo - assistindo desde o comeco");
		rc = z3efb156(zf785ae5, nPort, zb306bef, zf13fff5, szKey, z930c6f2, 0);
	}
	return rc;
}

int z70247ba(void (*z930c6f2)(const char*))
{
	if (!zb9029fb[0] || !z905ec26[0]) return z1cd7dae;
	return z3efb156(zb9029fb, za8df9d0, z905ec26, z4338b31, z17cfaab[0] ? z17cfaab : NULL, z930c6f2, 0);
}

int zb8a7e4f(const void** ppData, int* pnLen, int* z9d2014b)
{
	if (!S.z1e66b7c || !S.z9dbc625 || S.z258b8c6 <= 0) return 0;
	if (ppData)  *ppData  = S.z9dbc625;
	if (pnLen)   *pnLen   = S.z258b8c6;
	if (z9d2014b) *z9d2014b = S.zca0b11f;
	return 1;
}

static int z3efb156(const char* zf785ae5, unsigned short nPort,
                      const char* zb306bef, int zf13fff5, const char* szKey,
                      void (*z930c6f2)(const char*), int z8fe4467)
{
	if (!zf785ae5 || !*zf785ae5 || !nPort || !zb306bef || !*zb306bef) return z1cd7dae;

	z3dfe63b();
	memset(&S, 0, sizeof(S));
	S.sock = INVALID_SOCKET;
	S.z930c6f2 = z930c6f2;

	if (!zdb4a99e(z930c6f2)) return z63d00a0;
	S.zfd4ce47 = 1;

	int err = zd3e2c04;
	SOCKET s = z332a46a(zf785ae5, nPort, &err, z930c6f2);
	if (s == INVALID_SOCKET) { WSACleanup(); S.zfd4ce47 = 0; return err; }

	if (zf13fff5 > 0) {
		DWORD to = (DWORD)zf13fff5;
		setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (const char*)&to, sizeof(to));
	}

	char req[160];
	char key[40] = "";
	if (szKey && szKey[0]) sanitize(key, sizeof(key), szKey);
	int n = z8fe4467    ? _snprintf(req, sizeof(req) - 1, "%s SUBC %s %s\n", z883d5c8, zb306bef, key[0] ? key : "-")
	      : key[0] ? _snprintf(req, sizeof(req) - 1, "%s SUB %s %s\n", z883d5c8, zb306bef, key)
	               : _snprintf(req, sizeof(req) - 1, "%s SUB %s\n", z883d5c8, zb306bef);
	req[sizeof(req) - 1] = 0;
	if (n < 0) n = (int)strlen(req);
	if (zc7cef08(s, req, n) != zd3e2c04) { closesocket(s); WSACleanup(); S.zfd4ce47 = 0; return z81c8baf; }

	char line[z0cdf08a];
	if (z0b5ac61(s, line, sizeof(line)) != zd3e2c04) { closesocket(s); WSACleanup(); S.zfd4ce47 = 0; return z81c8baf; }

	char magic[8] = "", verb[8] = "", game[64] = "";
	char p1[zfb7dd07] = "", p2[zfb7dd07] = "";
	int players = 0, z0634c93 = 0, z4b04765 = 0, zf2bc5c4 = 0, z7158c9f = 0;
	int got = sscanf(line, "%7s %7s %63s %d %d %d %31s %31s %d %d",
	                 magic, verb, game, &players, &z0634c93, &z4b04765, p1, p2, &zf2bc5c4, &z7158c9f);
	const int z629806d = strcmp(verb, "HDRC") == 0;
	if (z629806d && (got < 10 || zf2bc5c4 <= 0 || z7158c9f <= z62bc0f9 || z7158c9f > z926b46c)) got = 0;

	if (got < 8 || strcmp(magic, z883d5c8) != 0 || (strcmp(verb, "HDR") != 0 && !z629806d) ||
	    players < 1 || players > 4 || z0634c93 < 1 || z0634c93 > 8 ||
	    players * z0634c93 > z3da9a23 - 4 ||
	    z4b04765 < 0 || z4b04765 > 64 * 1024 * 1024) {
		z1d4d1c9(z930c6f2, "relay: watch refused: %s", line);
		closesocket(s);
		WSACleanup(); S.zfd4ce47 = 0;
		return z3f960f0;
	}

	strncpy(S.info.szGame, game, sizeof(S.info.szGame) - 1);
	strncpy(S.info.z733a844, p1, sizeof(S.info.z733a844) - 1);
	strncpy(S.info.zd5e0939, p2, sizeof(S.info.zd5e0939) - 1);
	S.info.z0d29982    = players;
	S.info.z9df8312 = z0634c93;
	S.info.z0f2ef1e   = z4b04765;
	S.z91939f6        = players * z0634c93;

	if (z4b04765 > 0) {
		S.pState = (unsigned char*)malloc(z4b04765);
		if (!S.pState) { closesocket(s); WSACleanup(); S.zfd4ce47 = 0; return zf2605d7; }
		if (z8a1932a(s, S.pState, z4b04765) != zd3e2c04) {
			free(S.pState);
			S.pState = NULL;
			closesocket(s);
			WSACleanup(); S.zfd4ce47 = 0;
			return z81c8baf;
		}
	}
	if (z629806d) {
		S.z9dbc625 = (unsigned char*)malloc(z7158c9f);
		if (!S.z9dbc625 || z8a1932a(s, S.z9dbc625, z7158c9f) != zd3e2c04 || memcmp(S.z9dbc625, "RBCK", 4) != 0 ||
		    (int)get32(S.z9dbc625 + 8) != zf2bc5c4) {
			free(S.z9dbc625); S.z9dbc625 = NULL;
			free(S.pState); S.pState = NULL;
			closesocket(s);
			WSACleanup(); S.zfd4ce47 = 0;
			z1d4d1c9(z930c6f2, "relay: o ponto salvo de %s veio quebrado", zb306bef);
			return z81c8baf;
		}
		S.z258b8c6   = z7158c9f;
		S.zca0b11f = zf2bc5c4;
	}

	DWORD to = zb865e8b;
	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (const char*)&to, sizeof(to));

	S.sock = s;
	InitializeCriticalSection(&S.cs);
	S.z1e66b7c = 1;

	S.thread = (HANDLE)_beginthreadex(NULL, 0, z4322d81, NULL, 0, NULL);
	if (!S.thread) {
		DeleteCriticalSection(&S.cs);
		S.z1e66b7c = 0;
		closesocket(s);
		free(S.pState);
		free(S.z9dbc625);
		WSACleanup();
		memset(&S, 0, sizeof(S));
		return z63d00a0;
	}

	z1d4d1c9(z930c6f2, "relay: watching %s (%s, %s vs %s, %d byte state%s)",
	      zb306bef, game, p1, p2, z4b04765, S.z9dbc625 ? ", com ponto salvo" : "");
	if (S.z9dbc625) z1d4d1c9(z930c6f2, "relay: ponto salvo no quadro %d (%d KB)", S.zca0b11f, S.z258b8c6 / 1024);
	return zd3e2c04;
}

static unsigned char* zc2a957c(const char* szPath, int* pnLen)
{
	*pnLen = 0;
	FILE* f = fopen(szPath, "rb");
	if (!f) return NULL;
	fseek(f, 0, SEEK_END);
	long n = ftell(f);
	fseek(f, 0, SEEK_SET);
	if (n < 0 || n > 64L * 1024 * 1024) { fclose(f); return NULL; }
	unsigned char* p = (unsigned char*)malloc(n > 0 ? (size_t)n : 1);
	if (p && n > 0 && fread(p, 1, (size_t)n, f) != (size_t)n) { free(p); p = NULL; }
	fclose(f);
	if (p) *pnLen = (int)n;
	return p;
}

int z3283911(const char* szDir, void (*z930c6f2)(const char*))
{
	if (!szDir || !*szDir) return z1cd7dae;

	z3dfe63b();
	memset(&S, 0, sizeof(S));
	S.sock = INVALID_SOCKET;
	S.z930c6f2 = z930c6f2;

	char path[512];
	int n = 0;

	_snprintf(path, sizeof(path) - 1, "%s\\meta.txt", szDir);
	path[sizeof(path) - 1] = '\0';
	unsigned char* meta = zc2a957c(path, &n);
	if (!meta) { z1d4d1c9(z930c6f2, "replay: sem %s", path); return z1cd7dae; }
	char line[256];
	int m = n < (int)sizeof(line) - 1 ? n : (int)sizeof(line) - 1;
	memcpy(line, meta, m);
	line[m] = '\0';
	free(meta);
	char game[64] = "", p1[zfb7dd07] = "", p2[zfb7dd07] = "";
	int players = 0, z0634c93 = 0;
	int got = sscanf(line, "%63s %d %d %31s %31s", game, &players, &z0634c93, p1, p2);
	if (got < 5 || players < 1 || players > 4 || z0634c93 < 1 || z0634c93 > 8 ||
	    players * z0634c93 > z3da9a23 - 4) {
		z1d4d1c9(z930c6f2, "replay: meta.txt invalido: %s", line);
		return z3f960f0;
	}

	_snprintf(path, sizeof(path) - 1, "%s\\state.bin", szDir);
	int nState = 0;
	unsigned char* state = zc2a957c(path, &nState);
	if (!state) { z1d4d1c9(z930c6f2, "replay: sem %s", path); return z1cd7dae; }

	_snprintf(path, sizeof(path) - 1, "%s\\frames.bin", szDir);
	int z05ee856 = 0;
	unsigned char* z7709d74 = zc2a957c(path, &z05ee856);
	if (!z7709d74) { free(state); z1d4d1c9(z930c6f2, "replay: sem %s", path); return z1cd7dae; }

	const int z91939f6 = players * z0634c93;
	const int zd1b0d7d = 4 + z91939f6;
	const int total = zd1b0d7d > 0 ? z05ee856 / zd1b0d7d : 0;
	unsigned char* frames = (unsigned char*)malloc((size_t)(total > 0 ? total : 1) * z91939f6);
	if (!frames) { free(state); free(z7709d74); return zf2605d7; }
	int count = 0;
	for (int i = 0; i < total; i++) {
		const unsigned char* r = z7709d74 + (size_t)i * zd1b0d7d;
		int frame = (int)((unsigned)r[0] | ((unsigned)r[1] << 8) | ((unsigned)r[2] << 16) | ((unsigned)r[3] << 24));
		if (frame != count) {
			z1d4d1c9(z930c6f2, "replay: frames.bin quebra no quadro %d (esperava %d) - fica ate ai", frame, count);
			break;
		}
		memcpy(frames + (size_t)count * z91939f6, r + 4, z91939f6);
		count++;
	}
	free(z7709d74);

	strncpy(S.info.szGame, game, sizeof(S.info.szGame) - 1);
	strncpy(S.info.z733a844, p1, sizeof(S.info.z733a844) - 1);
	strncpy(S.info.zd5e0939, p2, sizeof(S.info.zd5e0939) - 1);
	S.info.z0d29982    = players;
	S.info.z9df8312 = z0634c93;
	S.info.z0f2ef1e   = nState;
	S.z91939f6 = z91939f6;
	S.pState    = state;
	S.pFrames   = frames;
	S.zb6a75c6      = count;
	S.z176af93     = count;
	S.nRead     = 0;
	S.ended     = 1;
	InitializeCriticalSection(&S.cs);
	S.z1e66b7c = 1;

	z1d4d1c9(z930c6f2, "replay: %s (%s, %s vs %s, %d byte state, %d quadros)",
	      szDir, game, p1, p2, nState, count);
	return zd3e2c04;
}

int z64b5a35(void)
{
	if (!S.z1e66b7c || S.sock != INVALID_SOCKET) return 0;
	return S.zb6a75c6;
}

int z18d4eb5(int k, void* pOut, int nBytes)
{
	if (!S.z1e66b7c || S.sock != INVALID_SOCKET || !pOut || nBytes != S.z91939f6) return 0;
	if (k < 0 || k >= S.zb6a75c6) return 0;
	memcpy(pOut, S.pFrames + (size_t)k * S.z91939f6, S.z91939f6);
	return 1;
}

int z7e412e1(zac814b9* out)
{
	if (!out || !S.z1e66b7c) return z1cd7dae;
	*out = S.info;
	return zd3e2c04;
}

const void* ze055a3e(int* pnLen)
{
	if (pnLen) *pnLen = S.info.z0f2ef1e;
	return S.pState;
}

int zef6e7c8(void)
{
	if (!S.z1e66b7c) return 0;
	EnterCriticalSection(&S.cs);
	zf62d1c0();
	int n = S.z176af93 - S.nRead;
	LeaveCriticalSection(&S.cs);
	return n;
}

int zd26e5b7(void) { if (S.z1e66b7c) zf62d1c0(); return S.z1e66b7c ? (int)S.ended : 1; }

int z2e8f5ef(void* pOut, int nBytes)
{
	if (!S.z1e66b7c || !pOut || nBytes != S.z91939f6) return z1cd7dae;

	EnterCriticalSection(&S.cs);
	zf62d1c0();
	int have = S.nRead < S.z176af93;
	if (have) {
		memcpy(pOut, S.pFrames + (size_t)S.nRead * S.z91939f6, S.z91939f6);
		S.nRead++;
	}
	LeaveCriticalSection(&S.cs);

	if (have)    return 1;
	if (S.ended) return z81c8baf;
	return 0;
}

void z3dfe63b(void)
{
	if (!S.z1e66b7c) return;
	InterlockedExchange(&S.stop, 1);
	if (S.sock != INVALID_SOCKET) {
		shutdown(S.sock, SD_BOTH);
		closesocket(S.sock);
		S.sock = INVALID_SOCKET;
	}
	if (S.thread) {
		WaitForSingleObject(S.thread, 3000);
		CloseHandle(S.thread);
	}
	DeleteCriticalSection(&S.cs);
	free(S.pState);
	free(S.pFrames);
	free(S.z9dbc625);
	if (S.zfd4ce47) WSACleanup();
	memset(&S, 0, sizeof(S));
}
