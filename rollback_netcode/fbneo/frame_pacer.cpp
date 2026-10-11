#include "frame_pacer.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>

extern "C" void z5d90bae(const char* s);

#ifndef CREATE_WAITABLE_TIMER_HIGH_RESOLUTION
#define CREATE_WAITABLE_TIMER_HIGH_RESOLUTION 0x00000002
#endif

#define zdfaabae     0.005
#define z1d28d53        0.002
#define z5b87956      0.02
#define z7c79852 2
#define zca291e0  100.0
#define z8194839       30.0

static int    ze7c8dcc = 0;
static int    z9b9472d = 0;
static double zf4d0cff = 0.0;
static HANDLE s_timer = NULL;
static int    z8486064 = 0;
static double zf2959cd = 0.0;
static double zdebb5d7 = -1.0;
static long   z1d3c75b = 0;
static double z6f2f5fd = 0.0;
static double z73f7fa7 = 0.0, z1565a41 = 0.0, z713f60b = 0.0;
static long   z0045e0f = 0, z924275b = 0;
static long   z1de1ff8 = 0;
static long   za2e737c = 0;

static double z3fd9aa6()
{
	LARGE_INTEGER t;
	QueryPerformanceCounter(&t);
	return (double)t.QuadPart * zf4d0cff;
}

static void zab20569()
{
	if (z9b9472d) return;
	z9b9472d = 1;
	LARGE_INTEGER f;
	QueryPerformanceFrequency(&f);
	zf4d0cff = f.QuadPart ? 1000.0 / (double)f.QuadPart : 0.0;

	s_timer = CreateWaitableTimerExW(NULL, NULL, CREATE_WAITABLE_TIMER_HIGH_RESOLUTION, TIMER_ALL_ACCESS);
	z8486064 = s_timer != NULL;
	if (!s_timer) {
		timeBeginPeriod(1);
		s_timer = CreateWaitableTimerW(NULL, TRUE, NULL);
	}
	z5d90bae(z8486064
		? "ritmo: quadros pelo relogio de alta precisao"
		: "ritmo: quadros pelo relogio do sistema em 1 ms (Windows sem o timer de alta precisao)");
}

static void z6de4dcd(double t)
{
	const double early = z8486064 ? 0.6 : 2.0;
	for (;;) {
		const double rem = t - z3fd9aa6();
		if (rem <= 0.0) return;
		if (rem > early + 0.2) {
			const double ms = rem - early;
			if (s_timer) {
				LARGE_INTEGER due;
				due.QuadPart = -(LONGLONG)(ms * 10000.0);
				if (SetWaitableTimer(s_timer, &due, 0, NULL, NULL, FALSE)) {
					WaitForSingleObject(s_timer, 50);
					continue;
				}
			}
			Sleep((DWORD)ms);
		} else {
			YieldProcessor();
		}
	}
}

void z82c780f(void)
{
	typedef struct { ULONG Version, ControlMask, StateMask; } zbe41c8d;
	typedef BOOL (WINAPI* zd369804)(HANDLE, int, LPVOID, DWORD);
	typedef HANDLE (WINAPI* zfb3f2e9)(LPCWSTR, LPDWORD);
	char zde61a7d[160];
	int zab72e85 = 0, z0b979f3 = 0;

	HMODULE k32 = GetModuleHandleW(L"kernel32.dll");
	zd369804 zd6703e4 = k32 ? (zd369804)GetProcAddress(k32, "SetProcessInformation") : NULL;
	if (zd6703e4) {
		zbe41c8d t;
		t.Version = 1;
		t.ControlMask = 0x1 | 0x4;
		t.StateMask = 0;
		zab72e85 = zd6703e4(GetCurrentProcess(), 4  , &t, sizeof(t)) ? 1 : 0;
		if (!zab72e85) {
			t.ControlMask = 0x1;
			zab72e85 = zd6703e4(GetCurrentProcess(), 4, &t, sizeof(t)) ? 1 : 0;
		}
	}

	HMODULE avrt = LoadLibraryW(L"avrt.dll");
	zfb3f2e9 z8d1a860 = avrt ? (zfb3f2e9)GetProcAddress(avrt, "AvSetMmThreadCharacteristicsW") : NULL;
	if (z8d1a860) {
		DWORD idx = 0;
		z0b979f3 = z8d1a860(L"Games", &idx) != NULL ? 1 : 0;
	}

	snprintf(zde61a7d, sizeof(zde61a7d), "prioridade: sem economia de energia em segundo plano %s, MMCSS Games %s",
	         zab72e85 ? "sim" : "nao", z0b979f3 ? "sim" : "nao");
	z5d90bae(zde61a7d);
}

void zbf78557(int bOn)
{
	ze7c8dcc = bOn ? 1 : 0;
	if (ze7c8dcc) zab20569();
}

int z8844f1e(void) { return ze7c8dcc; }

void z1cd10c8(void) { z1de1ff8++; }
void zbe39c39(void) { za2e737c++; zdebb5d7 = -1.0; }

void z9c7c393(void)
{
	zf2959cd = 0.0;
	zdebb5d7 = -1.0;
}

int zf1510ce(double fps, long fill, long target, long seg)
{
	zab20569();
	if (fps < 10.0 || fps > 200.0) fps = 60.0;
	if (seg <= 0) seg = 1;
	const double P0 = 1000.0 / fps;

	if (zdebb5d7 < 0.0) zdebb5d7 = (double)fill;
	else zdebb5d7 += z5b87956 * ((double)fill - zdebb5d7);
	double adj = 1.0 + z1d28d53 * ((zdebb5d7 - (double)target) / (double)seg);
	if (adj < 1.0 - zdfaabae) adj = 1.0 - zdfaabae;
	if (adj > 1.0 + zdfaabae) adj = 1.0 + zdfaabae;
	const double P = P0 * adj;

	double now = z3fd9aa6();
	if (zf2959cd <= 0.0 || now - zf2959cd > zca291e0 || zf2959cd - now > 2.0 * P) {
		if (zf2959cd > 0.0) z1d3c75b++;
		zf2959cd = now;
	}

	z6de4dcd(zf2959cd);

	now = z3fd9aa6();
	int n = 1;
	zf2959cd += P;
	while (n < z7c79852 && now >= zf2959cd) {
		n++;
		zf2959cd += P;
	}
	if (n > 1) z924275b++;

	z73f7fa7 += adj;
	z1565a41 += (double)fill;
	z713f60b += P;
	z0045e0f++;
	if (z6f2f5fd <= 0.0) z6f2f5fd = now;
	if ((now - z6f2f5fd) / 1000.0 >= z8194839 && z0045e0f > 0) {
		const double za07ed3a = (double)seg / P0;
		char zde61a7d[256];
		snprintf(zde61a7d, sizeof(zde61a7d),
		         "ritmo: periodo medio %.3f ms (ajuste %+.3f%%), som %.1f ms na frente (alvo %.1f), alcances %ld, recomecos %ld, buracos no som %ld, cortes %ld",
		         z713f60b / z0045e0f, (z73f7fa7 / z0045e0f - 1.0) * 100.0,
		         (z1565a41 / z0045e0f) / za07ed3a, (double)target / za07ed3a, z924275b, z1d3c75b, z1de1ff8, za2e737c);
		z5d90bae(zde61a7d);
		z6f2f5fd = now;
		z73f7fa7 = z1565a41 = z713f60b = 0.0;
		z0045e0f = 0;
		z924275b = 0;
		z1de1ff8 = 0;
		za2e737c = 0;
	}
	return n;
}
