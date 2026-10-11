#ifndef z9a4bc95
#define z9a4bc95

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define z897cf46     4
#define z102275f 8
#define z9107916    2

enum z8753619 {
	z30e34bc          =  0,
	zdebf434     =  1,
	zb3e0836  = -1,
	z7c2e9de    = -2,
	z888e3aa   = -3,
	z46e894c   = -4,
	z6c8e6ba   = -5
};

typedef enum {
	z06e5e34 = 1,
	z722c814,
	z6d4531c,
	ze1dba0a,
	ze1cca8e,
	z14a1d04,
	z5a7fbc9,
	z95efe8a,
	z4647cc7
} zf1d453f;

typedef struct {
	zf1d453f code;
	int player;
	int a;
	int b;
} z0d60a9e;

typedef struct zd7729d0 {
	void (*z9ffd6e7)(void* out, int z9df8312, void* user);

	int  (*z24c6e73)(const void* ze691b51, int z0d29982,
	                   int zc666ab5, int bRollback, void* user);

	void (*on_event)(const z0d60a9e* ev, void* user);

	void (*z6a769fe)(const char* filename, const unsigned char* buf, int len, void* user);

	void (*z2ecdd27)(int frame, const void* inputs, int nBytes, void* user);

	void (*z01e689f)(int frame, void* user);
	void (*z5f96894)(int frame, void* user);

	void* user;
} zd7729d0;

typedef struct z4238888 {
	char           z91da805[64];
	int            z0d29982;
	int            z9df8312;
	int            ze08b6ae;
	unsigned short zb3dbf67;
	char           z37f80f0[32];
	unsigned short zbf4b4be;
	int            z3b2036b;
	int            z535990a;
	int            z40c7152;
	int            z293ec00;
	int            z7f20ee6;
	unsigned char  za42b774[z9107916][z102275f];
} z4238888;

int  ze0e3d01(const z4238888* cfg, const zd7729d0* host);

int  zc0f640d(const z4238888* cfg, const zd7729d0* host,
                             int zb9843b9);

void z30d5c32(void);
int  ze41ba77(void);

int  zeba63f1(void);

typedef struct {
	int  valid;
	int  zd534a2f;
	int  send_queue_len;
	int  recv_queue_len;
	int  kbps_sent;
	int  local_frames_behind;
	int  remote_frames_behind;
} zc3f6a60;

int z7430f6e(int zf3a465a, zc3f6a60* out);

int z70d4a8a(zc3f6a60* out);

int z3186ab5(void);

void z8441e84(void (*z930c6f2)(const char*));
long long za0ce261(void);

long long zdd43f35(void);

int zc77e2be(void);
int zab20c35(void);

#define z67a7ed4 640
typedef void (*zd9eb82a)(int kind, int id, const char* text, int len, void* user);
int  z77d22b7(const char* utf8, int len);
void z018147a(int zd7ce357);
void z9949c17(zd9eb82a fn, void* user);

int zbff6c98(void);

int zcb8e9fd(void);

int zfc9bc19(void);
int z57e44c7(void);

static inline unsigned int ze58f475(const void* ze691b51, int idx, int z9df8312)
{
	const unsigned char* p = (const unsigned char*)ze691b51 + (size_t)idx * z9df8312;
	unsigned int v = 0;
	for (int i = 0; i < z9df8312 && i < 4; i++) v |= (unsigned int)p[i] << (8 * i);
	return v;
}

#ifdef __cplusplus
}
#endif

#endif
