#ifndef ze7c71a1
#define ze7c71a1

#ifdef __cplusplus
extern "C" {
#endif

#define z5cee153 3

#define z87df4c7 64

enum zb9cebb9 {
	za4ce288           =  0,
	z58c624b   = -1,
	z1d7d469   = -2
};

#define z77b92c2 8

typedef struct z21b0d90 {
	int zc4ac0ca;
	int zd1372c9;
	int zdb52f45;
} z21b0d90;

typedef struct z6ac2385 {
	int zc4ac0ca[z5cee153];
	int zd1372c9[z5cee153];
	int zdafe8ef;
	int zdebfa6d;
	int z726041c;
	int zae1a069;
	int zdb52f45;
	int nFrames;
	int zfb4df8e;
	int zb21a368;
	int       zd29aa1b;
	z21b0d90 za9df5d5[z77b92c2];
} z6ac2385;

#define z2f4b4cc     0
#define z4ca563e 1
#define z4d63468    2

typedef struct za78fa53 {
	char      szGame[32];
	int       z2ed2c96;
	int       zd61d2b1;
	int       z45e0144;
	int       z78ca99a;
	int       z1903fc4;
	int       z726041c;
	int       zae1a069;
	int       z37051f2;
	int       zc4ac0ca[z5cee153];
	int       zd1372c9[z5cee153];
	int       zdafe8ef;
	int       zdebfa6d;
	int          z535cef6;
	int          z219e1e5;
	z6ac2385 z5400403[z87df4c7];
	int       z1f29c92;
	long long nStartFrame;
	long long nEndFrame;
} za78fa53;

int  z5ebf79e(int ze6bfbdc, void (*z930c6f2)(const char*));

void zea0d2fd(int bOn);

void z97e971e(int bOn);

void z65fd730(void);

#define z5149dd1  3600
#define z6fc34b9 10
#define z1d4bc00 300
#define zb2570fd 35500
void z9fff242(int bOn);
int  ze2aa305(void);
int  zb139002(void);
int  z969ea27(int* pSide, int* z5a4156e, int* z335f6f7);
int  zf7cdc79(void);
int  z719f6d2(int nSide);
int  z26c0539(int nSide);
void z3440738(int nSide);

void z770c6b6(int bOn);
void zb4c9a9b(void);

#define z69eb802  1
#define z5ea2195 2
#define zafb1e7e   3
#define z93d035c    4
typedef void (*z309d1ab)(int nType, long long nFrame, int zdb52f45);
void za069b92(z309d1ab pfn);

int  zf480b00(void);
void z5eacaeb(void* pOut);
void zdf72ff8(const void* pIn);

void z21b472f(void);

int  z5e8792b(za78fa53* out);

int  ze949c69(void);
void zd82e415(int* ze8821d1, int* z0f82ab8);
int  z4f974fb(int* ze8821d1, int* z0f82ab8);

void z9e6bdfe(void (*z930c6f2)(const char*));

int  z9ae99d0(void);

int  zb31ef22(const char* zb306bef, int ze6bfbdc, const char* z303e5ed,
                           void (*z930c6f2)(const char*));

void z5126edb(const char* zb306bef);

#ifdef __cplusplus
}
#endif

#endif
