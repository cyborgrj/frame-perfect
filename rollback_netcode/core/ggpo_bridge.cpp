#if defined(_WIN32)
#include <winsock2.h>
#endif

#include "ggpo_bridge.h"
#include "state_ring.h"

#include "ggponet.h"

#include <stdio.h>
#include <string.h>

#if defined(_WIN32)
static int z77def17 = 0;
static void z4cc668e(void)
{
	if (!z77def17) {
		WSADATA z2957bc2;
		if (WSAStartup(MAKEWORD(2, 2), &z2957bc2) == 0) z77def17 = 1;
	}
}
static void zeef0821(void)
{
	if (z77def17) { WSACleanup(); z77def17 = 0; }
}
#else
static void z4cc668e(void) {}
static void zeef0821(void) {}
#endif

#if defined(_WIN32)
extern void (*g_rbfGgpoAssertHook)(const char* msg);
void z8441e84(void (*z930c6f2)(const char*)) { g_rbfGgpoAssertHook = z930c6f2; }
#else
void z8441e84(void (*)(const char*)) {}
#endif

static GGPOSession*      z621d134 = NULL;
static GGPOSessionCallbacks z4833d6f;
static z4238888  z94aaa38;
static zd7729d0    z75a2617;

static GGPOPlayerHandle  z57d68da[z897cf46];
static GGPOPlayerHandle  z1147efa = GGPO_INVALID_HANDLE;

#define z864fc22 64
static int z5fcc7d4 = 0;
static int zf10a59f = 0;
static int z166e35d[z864fc22];
static int z37e94ee = 0;

#define ze390a15 3
#define zccefa11    12
static int        za68b6b4 = 0;
static int        z8840a56   = 0;
static long long  zda7eec6  = 0;

static int        z18703bb    = 0;
static int        z1e19f4f   = 0;
static int        z32cace1  = GGPO_OK;
static long long  za76ac89 = 0;

static long long  z6479ec9 = 0;
static int        ze3f9ee7    = 0;
#define z1b20a60 900

static unsigned char zdc1f98d[z897cf46 * z102275f];
static unsigned char z557e2c7[z897cf46 * z102275f];

#define ze953f65 4
#define z00258e2 60
#define z1b123e3  32
#define z3fce952    256
static int           zc1500b5   = 0;
static int           zf8ba340     = 0;
static int           z74559bc[z3fce952];
static unsigned int  zd52ad69[z3fce952];
static unsigned char z24744ae[ze953f65];
static int           z969ed94 = -1;
static int           zcdaf137[z897cf46];
static int           z40204b1  = 0;
static int           z69285ac   = 0;
static int           zd1aa6ef = -1;

static unsigned int  zb61378f = 0x2545F491u;
static unsigned char zb99b38b[z897cf46][z102275f];

extern int g_rbfSyncTestErrors;

#define z0a8dcd9   (GGPO_MAX_PREDICTION_FRAMES + 2)
#define z2713695  32

static unsigned char z0e73798[z2713695][z897cf46 * z102275f];
static int  z0970765[z2713695];
static int  z54113e9     = 0;
static int  z6ce543c = -1;

static int z82899c9(void)
{
	return zc1500b5 * z94aaa38.z0d29982;
}

static int z92dc8a0(void)
{
	return z94aaa38.z9df8312 * z94aaa38.z0d29982;
}

static void z715df01(const unsigned char* src, unsigned char* dst)
{
	for (int p = 0; p < z94aaa38.z0d29982; p++)
		memcpy(dst + p * z94aaa38.z9df8312, src + p * zc1500b5, z94aaa38.z9df8312);
}

static unsigned int z474800f(unsigned int c)
{
	return (c ^ (c >> 16)) & 0xFFFF;
}

static void z399002f(zf1d453f code, int player, int a, int b)
{
	if (!z75a2617.on_event) return;
	z0d60a9e ev;
	ev.code = code;
	ev.player = player;
	ev.a = a;
	ev.b = b;
	z75a2617.on_event(&ev, z75a2617.user);
}

static void zf483cce(int frame)
{
	if (frame < 0) return;
	const int i = frame & (z2713695 - 1);
	z0970765[i] = frame;
	memcpy(z0e73798[i], zdc1f98d, z82899c9());
}

static void z21e0c15(int f, const unsigned char* in)
{
	const int n = z94aaa38.z9df8312;
	for (int p = 0; p < z94aaa38.z0d29982; p++) {
		if (p + 1 == z94aaa38.ze08b6ae) continue;
		const unsigned char* c = in + (size_t)p * zc1500b5 + n;
		const int tag = c[0] | (c[1] << 8);
		if (!(tag & 0x8000) || tag == zcdaf137[p]) continue;
		zcdaf137[p] = tag;

		int t = (f & ~0x7FFF) | (tag & 0x7FFF);
		if (t > f) t -= 0x8000;
		if (t < 0) continue;
		const int k = t & (z3fce952 - 1);
		if (z74559bc[k] != t) continue;

		const unsigned int mine   = z474800f(zd52ad69[k]);
		const unsigned int zd16d743 = (unsigned int)(c[2] | (c[3] << 8));
		z40204b1++;
		if (mine != zd16d743) {
			z69285ac++;
			if (zd1aa6ef < 0) {
				zd1aa6ef = t;
				z399002f(z4647cc7, p + 1, t, (int)((mine << 16) | zd16d743));
			}
		}
	}
}

static void z9e34bdf(int z2a1ba6c)
{
	if (!z75a2617.z2ecdd27 && !zf8ba340) return;
	const int zca199b8 = z2a1ba6c - z0a8dcd9;
	for (int f = z6ce543c + 1; f <= zca199b8; f++) {
		const int i = f & (z2713695 - 1);
		if (z0970765[i] != f) break;
		if (zf8ba340) z21e0c15(f, z0e73798[i]);
		if (z75a2617.z2ecdd27) {
			unsigned char out[z897cf46 * z102275f];
			z715df01(z0e73798[i], out);
			z75a2617.z2ecdd27(f, out, z92dc8a0(), z75a2617.user);
		}
		z6ce543c = f;
	}
}

static void zf7f6e87(void)
{
	const int F = z54113e9;
	if (F < z00258e2) return;
	if (z969ed94 >= 0 && F - z969ed94 < z00258e2) return;

	const int t = F - z1b123e3;
	const int k = t & (z3fce952 - 1);
	if (z74559bc[k] != t) return;

	const int tag = 0x8000 | (t & 0x7FFF);
	const unsigned int v = z474800f(zd52ad69[k]);
	z24744ae[0] = (unsigned char)(tag & 0xFF);
	z24744ae[1] = (unsigned char)(tag >> 8);
	z24744ae[2] = (unsigned char)(v & 0xFF);
	z24744ae[3] = (unsigned char)(v >> 8);
	z969ed94 = F;
}

static void z6af0f92(int p, unsigned char* out)
{
	if ((z54113e9 % 6) == 0) {
		for (int b = 0; b < z94aaa38.z9df8312; b++) {
			zb61378f = zb61378f * 1103515245u + 12345u;
			unsigned int r1 = zb61378f >> 16;
			zb61378f = zb61378f * 1103515245u + 12345u;
			unsigned int r2 = zb61378f >> 16;
			zb99b38b[p][b] = (unsigned char)(r1 & r2);
		}
		for (int g = 0; g < z9107916; g++) {
			int any = 0, all = 1, last = -1;
			for (int b = 0; b < z94aaa38.z9df8312; b++) {
				const unsigned char m = z94aaa38.za42b774[g][b];
				if (!m) continue;
				any = 1;
				last = b;
				if ((zb99b38b[p][b] & m) != m) all = 0;
			}
			if (any && all) {
				const unsigned char m = z94aaa38.za42b774[g][last];
				unsigned char top = 0x80;
				while (!(m & top)) top >>= 1;
				zb99b38b[p][last] &= (unsigned char)~top;
			}
		}
	}
	memset(out, 0, z102275f);
	memcpy(out, zb99b38b[p], z94aaa38.z9df8312);
}

static int z7723aab(int bRollback)
{
	int zc666ab5 = 0;
	GGPOErrorCode r = ggpo_synchronize_input(z621d134, zdc1f98d, z82899c9(), &zc666ab5);
	if (!GGPO_SUCCEEDED(r)) {
		z32cace1 = r;
		return z888e3aa;
	}

	const int z2a1ba6c = z54113e9;
	if (!bRollback) zf483cce(z2a1ba6c);

	z715df01(zdc1f98d, z557e2c7);
	int rc = z75a2617.z24c6e73(z557e2c7, z94aaa38.z0d29982, zc666ab5, bRollback, z75a2617.user);
	if (rc != 0) return z46e894c;

	ggpo_advance_frame(z621d134);

	if (bRollback) {
		zf483cce(z54113e9 - 1);
	} else {
		za76ac89++;
		ze3f9ee7 = 1;
		z9e34bdf(z2a1ba6c);
	}
	return z30e34bc;
}

static bool __cdecl z4bb5b65(const char*  )
{
	return true;
}

static bool __cdecl z7c4dc7b(unsigned char** buffer, int* len, int* checksum, int frame)
{
	if (!zb9193bb()) {
		fprintf(stderr, "[ggpo_bridge] WARNING: state_ring not initialised before session; late init.\n");
		if (zc8e48ed(z94aaa38.z7f20ee6) != z20fb1dd) return false;
	}

	z54113e9 = frame;

	unsigned int chk = 0;
	int rc = z2726b67(frame, (void**)buffer, len, &chk);
	if (rc != z20fb1dd) {
		z32cace1 = rc;
		fprintf(stderr, "[ggpo_bridge] StateRingSave(frame=%d) failed: %d\n", frame, rc);
		return false;
	}
	if (checksum) *checksum = (int)chk;

	const int k = frame & (z3fce952 - 1);
	z74559bc[k] = frame;
	zd52ad69[k]   = chk;

	if (z75a2617.z01e689f) z75a2617.z01e689f(frame, z75a2617.user);
	return true;
}

static bool __cdecl zc52eb94(unsigned char* buffer, int len)
{
	int rc = z4dac140(0  , buffer, len);
	if (rc != z20fb1dd) {
		z32cace1 = rc;
		fprintf(stderr, "[ggpo_bridge] StateRingLoad failed: %d\n", rc);
		return false;
	}
	if (z75a2617.z5f96894) z75a2617.z5f96894(zf580c77(buffer), z75a2617.user);
	return true;
}

static bool __cdecl z1b91d04(char* filename, unsigned char* buffer, int len)
{
	if (z75a2617.z6a769fe) z75a2617.z6a769fe(filename, buffer, len, z75a2617.user);
	return true;
}

static void __cdecl zcf97874(void* buffer)
{
	zaf99fbe(buffer);
}

static bool __cdecl zce40cc3(int  )
{
	z5fcc7d4++;
	z7723aab( 1);
	return true;
}

static bool __cdecl zf0b2aea(GGPOEvent* info)
{
	switch (info->code) {
	case GGPO_EVENTCODE_CONNECTED_TO_PEER:
		z399002f(z06e5e34, info->u.connected.player, 0, 0);
		break;
	case GGPO_EVENTCODE_SYNCHRONIZING_WITH_PEER:
		z399002f(z722c814, info->u.synchronizing.player,
		          info->u.synchronizing.count, info->u.synchronizing.total);
		break;
	case GGPO_EVENTCODE_SYNCHRONIZED_WITH_PEER:
		z399002f(z6d4531c, info->u.synchronized.player, 0, 0);
		break;
	case GGPO_EVENTCODE_RUNNING:
		z399002f(ze1dba0a, -1, 0, 0);
		break;
	case GGPO_EVENTCODE_DISCONNECTED_FROM_PEER:
		z399002f(ze1cca8e, info->u.disconnected.player, 0, 0);
		break;
	case GGPO_EVENTCODE_TIMESYNC:
		za68b6b4 = info->u.timesync.frames_ahead;
		if (za68b6b4 > zccefa11) za68b6b4 = zccefa11;
		if (za68b6b4 < 0) za68b6b4 = 0;
		z399002f(z95efe8a, -1, info->u.timesync.frames_ahead, 0);
		break;
	case GGPO_EVENTCODE_CONNECTION_INTERRUPTED:
		z399002f(z14a1d04, info->u.connection_interrupted.player,
		          info->u.connection_interrupted.disconnect_timeout, 0);
		break;
	case GGPO_EVENTCODE_CONNECTION_RESUMED:
		z399002f(z5a7fbc9, info->u.connection_resumed.player, 0, 0);
		break;
	default:
		break;
	}
	return true;
}

static void zf37704b(void)
{
	memset(&z4833d6f, 0, sizeof(z4833d6f));
	z4833d6f.begin_game      = z4bb5b65;
	z4833d6f.save_game_state = z7c4dc7b;
	z4833d6f.load_game_state = zc52eb94;
	z4833d6f.log_game_state  = z1b91d04;
	z4833d6f.free_buffer     = zcf97874;
	z4833d6f.advance_frame   = zce40cc3;
	z4833d6f.on_event        = zf0b2aea;
}

static int z64ad6ca(const z4238888* cfg, const zd7729d0* host)
{
	if (!cfg || !host)                                       return z7c2e9de;
	if (!host->z9ffd6e7 || !host->z24c6e73)        return z7c2e9de;
	if (cfg->z0d29982 < 2 || cfg->z0d29982 > z897cf46) return z7c2e9de;
	if (cfg->z9df8312 < 1 || cfg->z9df8312 > z102275f) return z7c2e9de;
	if (cfg->ze08b6ae < 1 || cfg->ze08b6ae > cfg->z0d29982) return z7c2e9de;
	return z30e34bc;
}

static int z0b8f191(void)
{
	for (int i = 0; i < z94aaa38.z0d29982; i++) {
		GGPOPlayer p;
		memset(&p, 0, sizeof(p));
		p.size = sizeof(GGPOPlayer);
		p.player_num = i + 1;

		if ((i + 1) == z94aaa38.ze08b6ae) {
			p.type = GGPO_PLAYERTYPE_LOCAL;
		} else {
			p.type = GGPO_PLAYERTYPE_REMOTE;
			strncpy(p.u.remote.ip_address, z94aaa38.z37f80f0, sizeof(p.u.remote.ip_address) - 1);
			p.u.remote.port = z94aaa38.zbf4b4be;
		}

		GGPOErrorCode r = ggpo_add_player(z621d134, &p, &z57d68da[i]);
		if (!GGPO_SUCCEEDED(r)) {
			z32cace1 = r;
			return z888e3aa;
		}

		if (p.type == GGPO_PLAYERTYPE_LOCAL) {
			z1147efa = z57d68da[i];
			if (z94aaa38.z3b2036b > 0) {
				ggpo_set_frame_delay(z621d134, z1147efa, z94aaa38.z3b2036b);
			}
		}
	}

	if (!z1e19f4f) {
		if (z94aaa38.z535990a > 0)
			ggpo_set_disconnect_timeout(z621d134, z94aaa38.z535990a);
		if (z94aaa38.z40c7152 > 0)
			ggpo_set_disconnect_notify_start(z621d134, z94aaa38.z40c7152);
	}
	return z30e34bc;
}

static void z752fc69(const z4238888* cfg, const zd7729d0* host)
{
	z94aaa38  = *cfg;
	z75a2617 = *host;
	for (int i = 0; i < z897cf46; i++) z57d68da[i] = GGPO_INVALID_HANDLE;
	z1147efa = GGPO_INVALID_HANDLE;
	z32cace1 = GGPO_OK;
	za76ac89 = 0;
	z6479ec9 = 0;
	ze3f9ee7 = 0;
	z5fcc7d4 = zf10a59f = z37e94ee = 0;
	memset(z166e35d, 0, sizeof(z166e35d));
	za68b6b4 = z8840a56 = 0;
	zda7eec6  = 0;

	zc1500b5   = cfg->z9df8312;
	zf8ba340     = 0;
	z969ed94 = -1;
	z40204b1  = z69285ac = 0;
	zd1aa6ef = -1;
	memset(z24744ae, 0, sizeof(z24744ae));
	memset(zcdaf137, 0, sizeof(zcdaf137));
	for (int i = 0; i < z3fce952; i++) z74559bc[i] = -1;
	memset(zb99b38b, 0, sizeof(zb99b38b));
	zb61378f = 0x2545F491u;
	g_rbfSyncTestErrors = 0;

	z54113e9     = 0;
	z6ce543c = -1;
	for (int i = 0; i < z2713695; i++) z0970765[i] = -1;
}

int ze0e3d01(const z4238888* cfg, const zd7729d0* host)
{
	if (z18703bb) z30d5c32();

	int v = z64ad6ca(cfg, host);
	if (v != z30e34bc) return v;

	z752fc69(cfg, host);
	z1e19f4f = 0;
	zf37704b();
	z4cc668e();

	if (z94aaa38.z9df8312 + ze953f65 <= z102275f) {
		zc1500b5 = z94aaa38.z9df8312 + ze953f65;
		zf8ba340   = 1;
	}

	GGPOErrorCode r = ggpo_start_session(&z621d134, &z4833d6f, z94aaa38.z91da805,
	                                     z94aaa38.z0d29982, zc1500b5, z94aaa38.zb3dbf67);
	if (!GGPO_SUCCEEDED(r)) {
		z32cace1 = r;
		z621d134 = NULL;
		return z888e3aa;
	}

	int w = z0b8f191();
	if (w != z30e34bc) {
		ggpo_close_session(z621d134);
		z621d134 = NULL;
		return w;
	}

	z18703bb = 1;
	return z30e34bc;
}

int zc0f640d(const z4238888* cfg, const zd7729d0* host, int zb9843b9)
{
	if (z18703bb) z30d5c32();

	int v = z64ad6ca(cfg, host);
	if (v != z30e34bc) return v;
	if (zb9843b9 < 1) zb9843b9 = 1;

	z752fc69(cfg, host);
	z1e19f4f = 1;
	zf37704b();
	z4cc668e();

	GGPOErrorCode r = ggpo_start_synctest(&z621d134, &z4833d6f, (char*)z94aaa38.z91da805,
	                                      z94aaa38.z0d29982, zc1500b5, zb9843b9);
	if (!GGPO_SUCCEEDED(r)) {
		z32cace1 = r;
		z621d134 = NULL;
		return z888e3aa;
	}

	int w = z0b8f191();
	if (w != z30e34bc) {
		ggpo_close_session(z621d134);
		z621d134 = NULL;
		return w;
	}

	z18703bb = 1;
	return z30e34bc;
}

static void z8568293(void);

void z30d5c32(void)
{
	z8568293();
	if (z621d134) {
		ggpo_rbf_goodbye(z621d134);
		ggpo_close_session(z621d134);
		z621d134 = NULL;
	}
	z18703bb  = 0;
	z1e19f4f = 0;
	z1147efa = GGPO_INVALID_HANDLE;
	zeef0821();
}

#define za597b7b       8
#define z2fccc5f 300
#define z9f591f9     40
#define z9b47d1d      64
static struct { int id, len, tries; unsigned int at; char data[z67a7ed4]; } z393ae09[za597b7b];
static int          zb976334 = 1;
static int          z7d268b5[z9b47d1d];
static int          z1cc3408  = 0;
static int          z08447af = 0, zabefa69 = 0;
static unsigned int z2042a0b = 0;

static void z8568293(void)
{
	memset(z393ae09, 0, sizeof(z393ae09));
	zb976334 = 1;
	z1cc3408 = 0;
	z08447af = zabefa69 = 0;
}

int z77d22b7(const char* utf8, int len)
{
	if (!z18703bb || !z621d134 || z1e19f4f || !utf8 || len <= 0 || len > z67a7ed4) return 0;
	for (int i = 0; i < za597b7b; i++) {
		if (z393ae09[i].id) continue;
		const int id = zb976334++;
		if (zb976334 > 0x7FFF) zb976334 = 1;
		z393ae09[i].id = id;
		z393ae09[i].len = len;
		z393ae09[i].tries = 1;
		z393ae09[i].at = GetTickCount();
		memcpy(z393ae09[i].data, utf8, len);
		ggpo_rbf_chat_send(z621d134, 0, id, utf8, len);
		return id;
	}
	return 0;
}

void z018147a(int zd7ce357)
{
	if (zd7ce357 <= z08447af && zabefa69 == 0) return;
	if (zd7ce357 > z08447af) z08447af = zd7ce357;
	zabefa69 = 3;
	z2042a0b = 0;
}

void z9949c17(zd9eb82a fn, void* user)
{
	if (!z18703bb || !z621d134 || z1e19f4f) return;
	const unsigned int now = GetTickCount();
	for (int i = 0; i < za597b7b; i++) {
		if (!z393ae09[i].id || now - z393ae09[i].at < z2fccc5f) continue;
		if (z393ae09[i].tries >= z9f591f9) { z393ae09[i].id = 0; continue; }
		z393ae09[i].tries++;
		z393ae09[i].at = now;
		ggpo_rbf_chat_send(z621d134, 0, z393ae09[i].id, z393ae09[i].data, z393ae09[i].len);
	}
	if (zabefa69 > 0 && now - z2042a0b >= z2fccc5f) {
		zabefa69--;
		z2042a0b = now;
		ggpo_rbf_chat_send(z621d134, 2, z08447af, NULL, 0);
	}
	int kind = 0, id = 0, len = 0;
	char buf[z67a7ed4];
	while (ggpo_rbf_chat_poll(z621d134, &kind, &id, buf, &len)) {
		if (kind == 0) {
			ggpo_rbf_chat_send(z621d134, 1, id, NULL, 0);
			int seen = 0;
			for (int k = 0; k < z1cc3408; k++) if (z7d268b5[k] == id) { seen = 1; break; }
			if (seen) continue;
			if (z1cc3408 == z9b47d1d) { memmove(z7d268b5, z7d268b5 + 1, sizeof(int) * (z9b47d1d - 1)); z1cc3408--; }
			z7d268b5[z1cc3408++] = id;
			if (fn) fn(0, id, buf, len, user);
		} else if (kind == 1) {
			for (int i = 0; i < za597b7b; i++) if (z393ae09[i].id == id) z393ae09[i].id = 0;
			if (fn) fn(1, id, NULL, 0, user);
		} else if (kind == 2) {
			if (fn) fn(2, id, NULL, 0, user);
		}
	}
}

int ze41ba77(void)
{
	return z18703bb;
}

static void ze1c8346(void)
{
	zf10a59f = z5fcc7d4;
	z166e35d[z37e94ee] = z5fcc7d4;
	z37e94ee = (z37e94ee + 1) & (z864fc22 - 1);
}

int zeba63f1(void)
{
	if (!z18703bb || !z621d134) return zb3e0836;

	if (!ze3f9ee7 && ++z6479ec9 > z1b20a60) {
		z32cace1 = GGPO_ERRORCODE_NOT_SYNCHRONIZED;
		return z888e3aa;
	}

	z5fcc7d4 = 0;

	if (z1e19f4f) {
		ggpo_idle(z621d134, 0);
		for (int i = 0; i < z94aaa38.z0d29982; i++) {
			unsigned char v[z102275f];
			z6af0f92(i, v);
			GGPOErrorCode ar = ggpo_add_local_input(z621d134, z57d68da[i], v, zc1500b5);
			if (!GGPO_SUCCEEDED(ar)) {
				z32cace1 = ar;
				ze1c8346();
				return zdebf434;
			}
		}
		int st = z7723aab( 0);
		ze1c8346();
		return st == z888e3aa ? zdebf434 : st;
	}

	if (za68b6b4 > 0 && ze3f9ee7 && (++z8840a56 % ze390a15) == 0) {
		za68b6b4--;
		zda7eec6++;
		ggpo_idle(z621d134, z94aaa38.z293ec00);
		ze1c8346();
		return zdebf434;
	}

	unsigned char local[z102275f];
	memset(local, 0, sizeof(local));
	z75a2617.z9ffd6e7(local, z94aaa38.z9df8312, z75a2617.user);
	if (zf8ba340) {
		zf7f6e87();
		memcpy(local + z94aaa38.z9df8312, z24744ae, ze953f65);
	}

	ggpo_idle(z621d134, z94aaa38.z293ec00);

	GGPOErrorCode r = ggpo_add_local_input(z621d134, z1147efa, local, zc1500b5);
	if (!GGPO_SUCCEEDED(r)) {
		z32cace1 = r;
		ze1c8346();
		return zdebf434;
	}

	int rc = z7723aab( 0);
	ze1c8346();
	if (rc == z888e3aa) return zdebf434;
	return rc;
}

int z7430f6e(int zf3a465a, zc3f6a60* out)
{
	if (!out) return z7c2e9de;
	memset(out, 0, sizeof(*out));
	if (!z18703bb || !z621d134) return zb3e0836;

	GGPONetworkStats s;
	memset(&s, 0, sizeof(s));
	GGPOErrorCode r = ggpo_get_network_stats(z621d134, zf3a465a, &s);
	if (!GGPO_SUCCEEDED(r)) {
		z32cace1 = r;
		return z888e3aa;
	}

	out->valid                = 1;
	out->zd534a2f              = s.network.ping;
	out->send_queue_len       = s.network.send_queue_len;
	out->recv_queue_len       = s.network.recv_queue_len;
	out->kbps_sent            = s.network.kbps_sent;
	out->local_frames_behind  = s.timesync.local_frames_behind;
	out->remote_frames_behind = s.timesync.remote_frames_behind;
	return z30e34bc;
}

int z70d4a8a(zc3f6a60* out)
{
	if (!out) return z7c2e9de;
	memset(out, 0, sizeof(*out));
	if (!z18703bb || !z621d134) return zb3e0836;

	for (int i = 0; i < z94aaa38.z0d29982 && i < z897cf46; i++) {
		if (z57d68da[i] == GGPO_INVALID_HANDLE || z57d68da[i] == z1147efa) continue;
		if (z7430f6e(z57d68da[i], out) == z30e34bc) return z30e34bc;
	}
	return zb3e0836;
}

long long zdd43f35(void) { return zda7eec6; }
int       zc77e2be(void)   { return z40204b1; }
int       zab20c35(void)    { return zd1aa6ef; }
int       zbff6c98(void) { return g_rbfSyncTestErrors; }
int       z3186ab5(void)  { return z32cace1; }
long long za0ce261(void) { return za76ac89; }
int       zcb8e9fd(void) { return z18703bb ? z94aaa38.z3b2036b : 0; }
int       zfc9bc19(void) { return zf10a59f; }

int z57e44c7(void)
{
	int peak = 0;
	for (int i = 0; i < z864fc22; i++)
		if (z166e35d[i] > peak) peak = z166e35d[i];
	return peak;
}
