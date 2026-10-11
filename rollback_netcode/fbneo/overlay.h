#ifndef zaa20559
#define zaa20559

#ifdef __cplusplus
extern "C" {
#endif

void z5c0c8b7(const char* z733a844, const char* zd5e0939, int ze6bfbdc);

void z8fa5b4b(void);
int  z4b7d267(void);

void z1c35054(void);
int  zfe2b00f(void);

void zf9658e7(unsigned char* pImage, int nWidth, int nHeight, int nBpp, int nPitch);

typedef struct z1fa3283 {
	int  nMode;
	char z733a844[40], zfad56d7[8], z5ea9399[12], z561be8d[8], zd5e0939[40];
	unsigned int z03b5823, z20dcc8d, ze871279, z73d250a, z02d18e2;
	int  ze326e54;
} z1fa3283;

int z8e16a23(z1fa3283* out);

typedef struct z2c2b1a7 {
	char z0f6d336[48];
	char z6af1ebd[48];
	unsigned int zd4c6117, z0456974;
	int  z70f9d25;
	int  nSide;
	int  z430cab8;
	int  z8b6ca0e;
} z2c2b1a7;
int z70213e0(z2c2b1a7* out);

void z6987e0c(int z5cddbe5);

#define z0452cf9 8
typedef struct z9bafc5a {
	int            mine, read, sys;
	const wchar_t* text;
} z9bafc5a;
typedef struct z1ea8be6 {
	int            z822f0cf, zcf8a8e5, z92c652b, nInput, nMax, z19ef79a;
	float          fOpen;
	const wchar_t* z1e5d86a;
	const wchar_t* zd8f5856;
	const wchar_t* ze6affeb;
	z9bafc5a   lines[z0452cf9];
} z1ea8be6;
int z8ad8d82(z1ea8be6* v);

typedef void (*zdcfcee4)(void* ctx, int x0, int y0, int x1, int y1, unsigned int rgb, int alpha);
void z8eca64b(zdcfcee4 fn, void* ctx, int x, int y, int w, int h, int r, unsigned int rgb, int alpha);
void zb52af7e(zdcfcee4 fn, void* ctx, int x, int y, int w, int h, int alpha,
                    int z92c652b, int z822f0cf, int* z924dd86, int* ze97eb52, int* pbr);

const char* z5c1451e(void);

const char* zdff8e52(void);

void z81ef24e(void);
int  zf92b87e(const char* s, int nSize, int* pHeight);
int  z11f9d81(unsigned char* img, int w, int h, int bpp, int pitch,
                    int x, int y, const char* s, unsigned int rgb, int nSize);
void z5bbbc0c(unsigned char* img, int w, int h, int bpp, int pitch,
                     int x0, int y0, int x1, int y1, unsigned int rgb, int alpha);

void z73bbb8d(int z056997a);
int  z9534659(void);

void zcf14204(int* pRight, int* pBottom);

#ifdef __cplusplus
}
#endif

#endif
