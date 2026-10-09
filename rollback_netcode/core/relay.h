#ifndef z76244db
#define z76244db

#ifdef __cplusplus
extern "C" {
#endif

enum z6da75e9 {
	zd3e2c04           =  0,
	z1cd7dae      = -1,
	z63d00a0   = -2,
	zeeb14a8  = -3,
	zc62b008  = -4,
	z3f960f0 = -5,
	z0001497  = -6,
	z81c8baf   = -7,
	zf2605d7   = -8
};

#define zfb7dd07 32

typedef struct zac814b9 {
	char szGame[64];
	char z733a844[zfb7dd07];
	char zd5e0939[zfb7dd07];
	int  z0d29982;
	int  z9df8312;
	int  z0f2ef1e;
	char z4b40456[40];
	int  zc16580c;
} zac814b9;

int  zf71597a(const char* zf785ae5, unsigned short nPort,
                       const char* zb306bef, const zac814b9* info,
                       const void* pState, int z0f2ef1e,
                       void (*z930c6f2)(const char*));

void z59d3a98(int nFrame, const void* pInputs, int nBytes);

void z4568e55(void);
int  zcd7ed90(void);

int  zef7c603(void);
int  z99502a6(int nFrame, const void* z7106887, int z15b1a6d,
                            const void* z9ae6e3a, int nScore, unsigned int z4570d0f);

int  z0244df4(const char* zf785ae5, unsigned short nPort,
                     const char* zb306bef, int zf13fff5, const char* szKey,
                     void (*z930c6f2)(const char*));

int  z3283911(const char* szDir, void (*z930c6f2)(const char*));
int  z67bbfd7(int nStart, double fps);
int  z64b5a35(void);
int  z18d4eb5(int k, void* pOut, int nBytes);

int         z7e412e1(zac814b9* out);
const void* ze055a3e(int* pnLen);
int         zef6e7c8(void);
int         zd26e5b7(void);

int  z2e8f5ef(void* pOut, int nBytes);

void z3dfe63b(void);

int  zc4093bf(const char* zf785ae5, unsigned short nPort,
                       const char* zb306bef, int zf13fff5, const char* szKey,
                       void (*z930c6f2)(const char*));
int  zb8a7e4f(const void** ppData, int* pnLen, int* z9d2014b);
int  z70247ba(void (*z930c6f2)(const char*));

#ifdef __cplusplus
}
#endif

#endif
