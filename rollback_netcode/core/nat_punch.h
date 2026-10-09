#ifndef zed41f88
#define zed41f88

#ifdef __cplusplus
extern "C" {
#endif

enum zda44996 {
    z10fc2a7          =  0,
    zbc061ae     = -1,
    za5b59f5 = -2,
    z3b663a4  = -3,
    z1e2b454    = -4,
    z95c4b69 = -5
};

typedef enum {
    zaf33281 = 0,
    z1b155da    = 1,
    za95c540  = 2
} z0b7780b;

typedef struct zf74d5eb {
    z0b7780b   mode;
    char           z10e203a[64];
    unsigned short zea7b53c;
    int            ze3a737a;
} zf74d5eb;

void z73b2207(int (*zb6e470c)(int z1af252e, int zf13fff5));

int z416a606(const char*      z12df4dc,
                        unsigned short   z2ad330e,
                        unsigned short   zaece3c9,
                        const char*      zb306bef,
                        int              nSide,
                        unsigned short   zb3dbf67,
                        zf74d5eb*  out,
                        int              zf13fff5,
                        void           (*z930c6f2)(const char*));

int z82369e2(const char* zf785ae5, char* szOut, int z58ab65b);

#ifdef __cplusplus
}
#endif

#endif
