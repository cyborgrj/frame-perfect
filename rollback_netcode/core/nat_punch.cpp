#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include "nat_punch.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define zfcd9de7   250

#define zdf5ef98         2500
#define za5d7e68     100
#define za3c5a83       5000
#define z4dc4e87   250

static int (*z7b15357)(int, int) = NULL;

void z73b2207(int (*zb6e470c)(int z1af252e, int zf13fff5))
{
    z7b15357 = zb6e470c;
}

static void z2e91ce7(void (*z930c6f2)(const char*), const char* fmt, ...)
{
    if (!z930c6f2) return;
    char buf[256];
    va_list ap;
    va_start(ap, fmt);
    _vsnprintf(buf, sizeof(buf) - 1, fmt, ap);
    va_end(ap);
    buf[sizeof(buf) - 1] = '\0';
    z930c6f2(buf);
}

int z82369e2(const char* zf785ae5, char* szOut, int nOut)
{
    if (!zf785ae5 || !*zf785ae5 || !szOut || nOut < 16) return zbc061ae;

    WSADATA z2957bc2;
    int z0b00021 = (WSAStartup(MAKEWORD(2, 2), &z2957bc2) == 0);

    struct addrinfo hints, *ai = NULL;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    int rc = za5b59f5;
    if (getaddrinfo(zf785ae5, NULL, &hints, &ai) == 0 && ai != NULL) {
        struct sockaddr_in* sin = (struct sockaddr_in*)ai->ai_addr;
        if (inet_ntop(AF_INET, &sin->sin_addr, szOut, (size_t)nOut) != NULL) rc = z10fc2a7;
        freeaddrinfo(ai);
    }

    if (z0b00021) WSACleanup();
    return rc;
}

static int z385c1f9(const struct sockaddr_in* a, const struct sockaddr_in* b)
{
    return a->sin_addr.s_addr == b->sin_addr.s_addr && a->sin_port == b->sin_port;
}

static void z43d2e83(const struct sockaddr_in* a, char* z20c0c1d, int z2c76d3c, unsigned* pPort)
{
    if (!inet_ntop(AF_INET, (void*)&a->sin_addr, z20c0c1d, (size_t)z2c76d3c)) z20c0c1d[0] = '\0';
    *pPort = ntohs(a->sin_port);
}

#define z08ac216     64
#define zc3ee0aa    3000
#define za578b35  8
#define zd88337f    400
#define ze528625   25

typedef struct { int n; int a[z08ac216]; } Zs;

static LARGE_INTEGER z244b958;

static unsigned z1363da3(void)
{
    LARGE_INTEGER c;
    if (!z244b958.QuadPart) QueryPerformanceFrequency(&z244b958);
    if (!z244b958.QuadPart) return GetTickCount();
    QueryPerformanceCounter(&c);
    return (unsigned)((c.QuadPart / z244b958.QuadPart) * 1000 + (c.QuadPart % z244b958.QuadPart) * 1000 / z244b958.QuadPart);
}

static void z702da46(Zs* z, unsigned t)
{
    const int d = (int)(z1363da3() - t);
    if (d < 0 || d > zc3ee0aa || z->n >= z08ac216) return;
    z->a[z->n++] = d;
}

static int zbc882b0(const Zs* z)
{
    if (z->n <= 0) return -1;
    int b[z08ac216];
    for (int i = 0; i < z->n; i++) {
        int v = z->a[i], j = i;
        while (j > 0 && b[j - 1] > v) { b[j] = b[j - 1]; j--; }
        b[j] = v;
    }
    return (z->n & 1) ? b[z->n / 2] : (b[z->n / 2 - 1] + b[z->n / 2] + 1) / 2;
}

static void zf9f1a5a(SOCKET s, int m)
{
    int ms = m >= 0 ? m + m / 2 + 40 : 120;
    if (ms < 60)  ms = 60;
    if (ms > 600) ms = 600;
    DWORD t0 = GetTickCount();
    while ((int)(GetTickCount() - t0) < ms) {
        fd_set rf;
        FD_ZERO(&rf);
        FD_SET(s, &rf);
        struct timeval tv = { 0, 10 * 1000 };
        if (select(0, &rf, NULL, NULL, &tv) <= 0) continue;
        char buf[256];
        struct sockaddr_in from;
        int z7c61b00 = sizeof(from);
        recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr*)&from, &z7c61b00);
    }
}

static void z4ad172f(SOCKET s, const struct sockaddr_in* to, const char* zb306bef, int nSide)
{
    char b[160];
    int n = _snprintf(b, sizeof(b) - 1, "RBF1 PUNCH %s %d %u\n", zb306bef, nSide, z1363da3());
    if (n > 0) sendto(s, b, n, 0, (const struct sockaddr*)to, sizeof(*to));
}

static void z12300ac(SOCKET s, const struct sockaddr_in* to, int zd227c9f, const char* zb306bef, int nSide,
                 int z9eb1eeb, unsigned t)
{
    char b[160];
    int n = z9eb1eeb ? _snprintf(b, sizeof(b) - 1, "RBF1 PACK %s %d %u\n", zb306bef, nSide, t)
               : _snprintf(b, sizeof(b) - 1, "RBF1 PACK %s %d\n", zb306bef, nSide);
    if (n > 0) sendto(s, b, n, 0, (const struct sockaddr*)to, zd227c9f);
}

static int z4e1173f(const char* buf, const char* zb306bef, int nSide, unsigned* pT, int* pT1)
{
    char kind[8], mid[64];
    int side = 0;
    unsigned t = 0;
    const int k = sscanf(buf, "RBF1 %7s %63s %d %u", kind, mid, &side, &t);
    if (k < 3) return 0;
    if (strcmp(mid, zb306bef) != 0 || side == nSide) return 0;
    *pT  = t;
    *pT1 = (k == 4);
    if (strcmp(kind, "PUNCH") == 0) return 1;
    if (strcmp(kind, "PACK")  == 0) return 2;
    return 0;
}

static int z202e320(SOCKET s, const struct sockaddr_in* peer, const char* zb306bef, int nSide,
                     struct sockaddr_in* heard, Zs* z)
{
    int z8910c81 = 0, z0f3467b = 0, z9eb1eeb = 0;
    DWORD tStart = GetTickCount(), z34b2b3a = 0, zbdf5b19 = 0;
    int z85f7d84 = 0;

    while ((int)(GetTickCount() - tStart) < zdf5ef98) {
        DWORD now = GetTickCount();
        if (z0f3467b && (!z9eb1eeb || z->n >= za578b35 || (int)(now - zbdf5b19) >= zd88337f)) break;
        if (!z85f7d84 || (int)(now - z34b2b3a) >= (z0f3467b ? ze528625 : za5d7e68)) {
            z4ad172f(s, peer, zb306bef, nSide);
            if (z8910c81 && !z385c1f9(heard, peer)) z4ad172f(s, heard, zb306bef, nSide);
            z34b2b3a = now;
            z85f7d84 = 1;
        }

        fd_set rf;
        FD_ZERO(&rf);
        FD_SET(s, &rf);
        struct timeval tv = { 0, (z0f3467b ? 5 : 30) * 1000 };
        if (select(0, &rf, NULL, NULL, &tv) <= 0) continue;

        char buf[256];
        struct sockaddr_in from;
        int z7c61b00 = sizeof(from);
        int n = recvfrom(s, buf, sizeof(buf) - 1, 0, (struct sockaddr*)&from, &z7c61b00);
        if (n <= 0) continue;
        buf[n] = '\0';

        unsigned t = 0;
        int zf75ca5c = 0;
        int kind = z4e1173f(buf, zb306bef, nSide, &t, &zf75ca5c);
        if (kind == 0) continue;
        if (!z8910c81) { *heard = from; z8910c81 = 1; }
        if (kind == 1) {
            z12300ac(s, &from, z7c61b00, zb306bef, nSide, zf75ca5c, t);
        } else {
            if (!z0f3467b) { z0f3467b = 1; zbdf5b19 = GetTickCount(); }
            if (zf75ca5c) { z9eb1eeb = 1; z702da46(z, t); }
        }
    }
    return z8910c81;
}

static int z9d4a926(SOCKET s, const struct sockaddr_in* srv,
                      const struct sockaddr_in* relay, const char* zca72890, int z8860887,
                      const char* zb306bef, int nSide, int z9e7d179, Zs* z, int* pD)
{
    *pD = -1;
    DWORD tStart = GetTickCount(), z34b2b3a = 0;
    int z85f7d84 = 0;

    while ((int)(GetTickCount() - tStart) < za3c5a83) {
        DWORD now = GetTickCount();
        if (!z85f7d84 || (int)(now - z34b2b3a) >= z4dc4e87) {
            char zbe25e4c[160];
            const int m = z9e7d179 ? zbc882b0(z) : -1;
            int z7908632 = m >= 0
                ? _snprintf(zbe25e4c, sizeof(zbe25e4c) - 1, "RBF1 CHECK %s %d ok %d\n", zb306bef, nSide, m)
                : _snprintf(zbe25e4c, sizeof(zbe25e4c) - 1, "RBF1 CHECK %s %d %s\n",
                            zb306bef, nSide, z9e7d179 ? "ok" : "fail 0");
            if (z7908632 > 0) sendto(s, zbe25e4c, z7908632, 0, (struct sockaddr*)srv, sizeof(*srv));
            if (zca72890) sendto(s, zca72890, z8860887, 0, (struct sockaddr*)relay, sizeof(*relay));
            z34b2b3a = now;
            z85f7d84 = 1;
        }

        fd_set rf;
        FD_ZERO(&rf);
        FD_SET(s, &rf);
        struct timeval tv = { 0, 50 * 1000 };
        if (select(0, &rf, NULL, NULL, &tv) <= 0) continue;

        char buf[256];
        struct sockaddr_in from;
        int z7c61b00 = sizeof(from);
        int n = recvfrom(s, buf, sizeof(buf) - 1, 0, (struct sockaddr*)&from, &z7c61b00);
        if (n <= 0) continue;
        buf[n] = '\0';

        char mid[64], mode[16];
        int d = -1;
        const int k = sscanf(buf, "RBF1 GO %63s %15s %d", mid, mode, &d);
        if (k >= 2 && strcmp(mid, zb306bef) == 0) {
            if (strcmp(mode, "relay") == 0) return za95c540;
            if (strcmp(mode, "none") == 0)  return z7199250;
            if (k == 3 && strcmp(mode, "direct") == 0 && d >= 0 && d <= 4) *pD = d;
            return zaf33281;
        }

        unsigned t = 0;
        int zf75ca5c = 0;
        const int kind = z4e1173f(buf, zb306bef, nSide, &t, &zf75ca5c);
        if (kind == 1)                z12300ac(s, &from, z7c61b00, zb306bef, nSide, zf75ca5c, t);
        else if (kind == 2 && zf75ca5c && z9e7d179) z702da46(z, t);
    }
    return -1;
}

int z416a606(const char*      z12df4dc,
                        unsigned short   z2ad330e,
                        unsigned short   zaece3c9,
                        const char*      zb306bef,
                        int              nSide,
                        unsigned short   zb3dbf67,
                        zf74d5eb*  out,
                        int              zf13fff5,
                        void           (*z930c6f2)(const char*))
{
    if (!z12df4dc || !*z12df4dc || !zb306bef || !*zb306bef ||
        !out || z2ad330e == 0)
        return zbc061ae;

    memset(out, 0, sizeof(*out));
    out->mode = zaf33281;
    out->z5c6ee43  = -1;
    out->zf3e1f1a  = -1;
    Zs z;
    z.n = 0;

    WSADATA z2957bc2;
    int z0b00021 = (WSAStartup(MAKEWORD(2, 2), &z2957bc2) == 0);

    struct addrinfo hints, *ai = NULL;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    char z00fcef4[16];
    _snprintf(z00fcef4, sizeof(z00fcef4) - 1, "%u", (unsigned)z2ad330e);
    z00fcef4[sizeof(z00fcef4) - 1] = '\0';

    if (getaddrinfo(z12df4dc, z00fcef4, &hints, &ai) != 0 || ai == NULL) {
        z2e91ce7(z930c6f2, "nat punch: could not resolve rendezvous %s", z12df4dc);
        if (z0b00021) WSACleanup();
        return za5b59f5;
    }
    struct sockaddr_in srv = *(struct sockaddr_in*)ai->ai_addr;
    freeaddrinfo(ai);

    SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s == INVALID_SOCKET) {
        if (z0b00021) WSACleanup();
        return z3b663a4;
    }
    int on = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char*)&on, sizeof(on));

    struct sockaddr_in local;
    memset(&local, 0, sizeof(local));
    local.sin_family      = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    local.sin_port        = htons(zb3dbf67);
    if (bind(s, (struct sockaddr*)&local, sizeof(local)) == SOCKET_ERROR) {
        z2e91ce7(z930c6f2, "nat punch: cannot bind local port %u (err %d)", zb3dbf67, WSAGetLastError());
        closesocket(s);
        if (z0b00021) WSACleanup();
        return z1e2b454;
    }

    struct sockaddr_in relay = srv;
    relay.sin_port = htons(zaece3c9);

    char zfd298cf[160];
    int zed97df0 = _snprintf(zfd298cf, sizeof(zfd298cf) - 1, "RBF1 REG %s %d\n", zb306bef, nSide);
    if (zed97df0 < 0) zed97df0 = 0;

    char zca72890[160];
    int z8860887 = _snprintf(zca72890, sizeof(zca72890) - 1, "RBF1 GREG %s %d\n", zb306bef, nSide);
    if (z8860887 < 0) z8860887 = 0;

    char z10e203a[64] = "";
    unsigned int zea7b53c = 0;
    int za0fcda0 = 0;
    char ze15ce46[64] = "";
    unsigned int zad178a8 = 0;
    int zfb060a6 = 0;

    DWORD tStart    = GetTickCount();
    DWORD z99e41ec = 0;
    int   zb1e2e23 = 0;

    while (!za0fcda0 && (int)(GetTickCount() - tStart) < zf13fff5) {
        DWORD now = GetTickCount();
        if (!zb1e2e23 || (int)(now - z99e41ec) >= zfcd9de7) {
            sendto(s, zfd298cf, zed97df0, 0, (struct sockaddr*)&srv, sizeof(srv));
            if (zaece3c9) sendto(s, zca72890, z8860887, 0, (struct sockaddr*)&relay, sizeof(relay));
            z99e41ec = now;
            zb1e2e23 = 1;
        }

        if (z7b15357 && z7b15357((int)(now - tStart), zf13fff5)) {
            z2e91ce7(z930c6f2, "nat punch: wait cancelled after %d ms", (int)(now - tStart));
            break;
        }

        fd_set rf;
        FD_ZERO(&rf);
        FD_SET(s, &rf);
        struct timeval tv;
        tv.tv_sec  = 0;
        tv.tv_usec = 50 * 1000;

        if (select(0, &rf, NULL, NULL, &tv) <= 0) continue;

        char buf[256];
        struct sockaddr_in from;
        int z7c61b00 = sizeof(from);
        int n = recvfrom(s, buf, sizeof(buf) - 1, 0, (struct sockaddr*)&from, &z7c61b00);
        if (n <= 0) continue;
        buf[n] = '\0';

        char ip[64];
        char z041e3da[16] = "";
        unsigned int port = 0;
        if (sscanf(buf, "RBF1 PEER %63s %u %15s", ip, &port, z041e3da) >= 2 && port != 0) {
            strncpy(z10e203a, ip, sizeof(z10e203a) - 1);
            z10e203a[sizeof(z10e203a) - 1] = '\0';
            zea7b53c = port;
            za0fcda0  = 1;
            if      (strcmp(z041e3da, "relay") == 0) out->mode = za95c540;
            else if (strcmp(z041e3da, "lan")   == 0) out->mode = z1b155da;
            else if (strcmp(z041e3da, "none")  == 0) out->mode = z7199250;
            else                                   out->mode = zaf33281;
        } else if (sscanf(buf, "RBF1 GSELF %63s %u", ip, &port) == 2) {
            if (!zfb060a6) {
                zfb060a6 = 1;
                z2e91ce7(z930c6f2, "nat punch: game relay answered from our side (%s:%u)", ip, port);
            }
        } else if (sscanf(buf, "RBF1 SELF %63s %u", ip, &port) == 2) {
            out->ze3a737a = 1;
            if (strcmp(ze15ce46, ip) != 0 || zad178a8 != port) {
                z2e91ce7(z930c6f2, "nat punch: our public endpoint is %s:%u", ip, port);
                strncpy(ze15ce46, ip, sizeof(ze15ce46) - 1);
                ze15ce46[sizeof(ze15ce46) - 1] = '\0';
                zad178a8 = port;
            }
        }
    }

    if (zaece3c9 && !zfb060a6)
        z2e91ce7(z930c6f2, "nat punch: WARNING game relay udp/%u never answered - "
                          "symmetric NAT cannot be detected (port closed on the server?)",
                  (unsigned)zaece3c9);

    if (za0fcda0 && out->mode == zaf33281 &&
        ze15ce46[0] && strcmp(ze15ce46, z10e203a) == 0)
        out->mode = z1b155da;

    if (za0fcda0 && out->mode == z7199250) {
        z2e91ce7(z930c6f2, "nat punch: rendezvous: none");
    } else if (za0fcda0 && out->mode == z1b155da) {
        z2e91ce7(z930c6f2, "nat punch: peer is behind the same NAT as us (%s) - staying on the LAN", ze15ce46);
    } else if (za0fcda0 && out->mode == za95c540) {
        z2e91ce7(z930c6f2, "nat punch: NAT simetrico - a partida vai pelo relay udp/%u", (unsigned)zaece3c9);
    } else if (za0fcda0) {
        z2e91ce7(z930c6f2, "nat punch: peer public endpoint %s:%u - opening NAT", z10e203a, zea7b53c);

        strncpy(out->z10e203a, z10e203a, sizeof(out->z10e203a) - 1);
        out->z10e203a[sizeof(out->z10e203a) - 1] = '\0';
        out->zea7b53c = (unsigned short)zea7b53c;

        struct sockaddr_in peer;
        memset(&peer, 0, sizeof(peer));
        peer.sin_family = AF_INET;
        peer.sin_port   = htons((unsigned short)zea7b53c);
        if (inet_pton(AF_INET, z10e203a, &peer.sin_addr) == 1) {
            struct sockaddr_in heard;
            memset(&heard, 0, sizeof(heard));
            int z9e7d179 = z202e320(s, &peer, zb306bef, nSide, &heard, &z);

            if (z9e7d179) {
                char z20c0c1d[64];
                unsigned nPort = 0;
                z43d2e83(&heard, z20c0c1d, sizeof(z20c0c1d), &nPort);
                if (z20c0c1d[0] && !z385c1f9(&heard, &peer)) {
                    z2e91ce7(z930c6f2, "nat punch: peer answers from %s:%u, not %s:%u - using that",
                              z20c0c1d, nPort, z10e203a, zea7b53c);
                    strncpy(out->z10e203a, z20c0c1d, sizeof(out->z10e203a) - 1);
                    out->z10e203a[sizeof(out->z10e203a) - 1] = '\0';
                    out->zea7b53c = (unsigned short)nPort;
                } else {
                    z2e91ce7(z930c6f2, "nat punch: peer answered directly");
                }
            } else {
                z2e91ce7(z930c6f2, "nat punch: nothing came back from the peer in %d ms", zdf5ef98);
            }

            int z2c3d532 = -1;
            int z860360a = z9d4a926(s, &srv, &relay, zaece3c9 ? zca72890 : NULL, z8860887,
                                     zb306bef, nSide, z9e7d179, &z, &z2c3d532);
            if (z860360a == za95c540) {
                out->mode = za95c540;
                z2e91ce7(z930c6f2, "nat punch: no direct path between us - the match goes through the relay udp/%u",
                          (unsigned)zaece3c9);
            } else if (z860360a == z7199250) {
                out->mode = z7199250;
                z2e91ce7(z930c6f2, "nat punch: rendezvous: none");
            } else if (z860360a == zaf33281) {
                out->zf3e1f1a = z2c3d532;
                z2e91ce7(z930c6f2, "nat punch: rendezvous confirmed the direct path");
            } else {
                z2e91ce7(z930c6f2, "nat punch: rendezvous gave no ruling (old server?) - trying direct");
            }
            if (z860360a != za95c540 && z860360a != z7199250) zf9f1a5a(s, zbc882b0(&z));
        }
    } else {
        z2e91ce7(z930c6f2, "nat punch: peer never registered within %d ms", zf13fff5);
    }

    out->z5c6ee43 = zbc882b0(&z);
    out->zae7efac = z.n;

    closesocket(s);
    if (z0b00021) WSACleanup();

    return za0fcda0 ? z10fc2a7 : z95c4b69;
}
