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

int z82369e2(const char* zf785ae5, char* szOut, int z58ab65b)
{
    if (!zf785ae5 || !*zf785ae5 || !szOut || z58ab65b < 16) return zbc061ae;

    WSADATA z2957bc2;
    int z0b00021 = (WSAStartup(MAKEWORD(2, 2), &z2957bc2) == 0);

    struct addrinfo hints, *ai = NULL;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    int rc = za5b59f5;
    if (getaddrinfo(zf785ae5, NULL, &hints, &ai) == 0 && ai != NULL) {
        struct sockaddr_in* sin = (struct sockaddr_in*)ai->ai_addr;
        if (inet_ntop(AF_INET, &sin->sin_addr, szOut, (size_t)z58ab65b) != NULL) rc = z10fc2a7;
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

static int z4e1173f(const char* buf, const char* zb306bef, int nSide)
{
    char kind[8], mid[64];
    int side = 0;
    if (sscanf(buf, "RBF1 %7s %63s %d", kind, mid, &side) != 3) return 0;
    if (strcmp(mid, zb306bef) != 0 || side == nSide) return 0;
    if (strcmp(kind, "PUNCH") == 0) return 1;
    if (strcmp(kind, "PACK")  == 0) return 2;
    return 0;
}

static int z202e320(SOCKET s, const struct sockaddr_in* peer, const char* zb306bef, int nSide,
                     struct sockaddr_in* heard)
{
    char z29ecf34[160], zd8c1604[160];
    int zdf124c2 = _snprintf(z29ecf34, sizeof(z29ecf34) - 1, "RBF1 PUNCH %s %d\n", zb306bef, nSide);
    int z7bf521b   = _snprintf(zd8c1604,   sizeof(zd8c1604) - 1,   "RBF1 PACK %s %d\n",  zb306bef, nSide);
    if (zdf124c2 < 0 || z7bf521b < 0) return 0;

    int z8910c81 = 0, z0f3467b = 0;
    DWORD tStart = GetTickCount(), z34b2b3a = 0;
    int z85f7d84 = 0;

    while (!z0f3467b && (int)(GetTickCount() - tStart) < zdf5ef98) {
        DWORD now = GetTickCount();
        if (!z85f7d84 || (int)(now - z34b2b3a) >= za5d7e68) {
            sendto(s, z29ecf34, zdf124c2, 0, (struct sockaddr*)peer, sizeof(*peer));
            if (z8910c81 && !z385c1f9(heard, peer))
                sendto(s, z29ecf34, zdf124c2, 0, (struct sockaddr*)heard, sizeof(*heard));
            z34b2b3a = now;
            z85f7d84 = 1;
        }

        fd_set rf;
        FD_ZERO(&rf);
        FD_SET(s, &rf);
        struct timeval tv = { 0, 30 * 1000 };
        if (select(0, &rf, NULL, NULL, &tv) <= 0) continue;

        char buf[256];
        struct sockaddr_in from;
        int z7c61b00 = sizeof(from);
        int n = recvfrom(s, buf, sizeof(buf) - 1, 0, (struct sockaddr*)&from, &z7c61b00);
        if (n <= 0) continue;
        buf[n] = '\0';

        int kind = z4e1173f(buf, zb306bef, nSide);
        if (kind == 0) continue;
        if (!z8910c81) { *heard = from; z8910c81 = 1; }
        if (kind == 1) sendto(s, zd8c1604, z7bf521b, 0, (struct sockaddr*)&from, z7c61b00);
        else           z0f3467b = 1;
    }
    return z8910c81;
}

static int z9d4a926(SOCKET s, const struct sockaddr_in* z2e0c9f4,
                      const struct sockaddr_in* relay, const char* zca72890, int z8860887,
                      const char* zb306bef, int nSide, int z9e7d179)
{
    char zbe25e4c[160], zd8c1604[160];
    int z7908632 = _snprintf(zbe25e4c, sizeof(zbe25e4c) - 1, "RBF1 CHECK %s %d %s\n",
                           zb306bef, nSide, z9e7d179 ? "ok" : "fail");
    int z7bf521b   = _snprintf(zd8c1604, sizeof(zd8c1604) - 1, "RBF1 PACK %s %d\n", zb306bef, nSide);
    if (z7908632 < 0 || z7bf521b < 0) return -1;

    DWORD tStart = GetTickCount(), z34b2b3a = 0;
    int z85f7d84 = 0;

    while ((int)(GetTickCount() - tStart) < za3c5a83) {
        DWORD now = GetTickCount();
        if (!z85f7d84 || (int)(now - z34b2b3a) >= z4dc4e87) {
            sendto(s, zbe25e4c, z7908632, 0, (struct sockaddr*)z2e0c9f4, sizeof(*z2e0c9f4));
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
        if (sscanf(buf, "RBF1 GO %63s %15s", mid, mode) == 2 && strcmp(mid, zb306bef) == 0)
            return strcmp(mode, "relay") == 0 ? za95c540 : zaf33281;

        if (z4e1173f(buf, zb306bef, nSide) == 1)
            sendto(s, zd8c1604, z7bf521b, 0, (struct sockaddr*)&from, z7c61b00);
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
    struct sockaddr_in z2e0c9f4 = *(struct sockaddr_in*)ai->ai_addr;
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

    struct sockaddr_in relay = z2e0c9f4;
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
            sendto(s, zfd298cf, zed97df0, 0, (struct sockaddr*)&z2e0c9f4, sizeof(z2e0c9f4));
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

    if (za0fcda0 && out->mode == z1b155da) {
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
            int z9e7d179 = z202e320(s, &peer, zb306bef, nSide, &heard);

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

            int z860360a = z9d4a926(s, &z2e0c9f4, &relay, zaece3c9 ? zca72890 : NULL, z8860887,
                                     zb306bef, nSide, z9e7d179);
            if (z860360a == za95c540) {
                out->mode = za95c540;
                z2e91ce7(z930c6f2, "nat punch: no direct path between us - the match goes through the relay udp/%u",
                          (unsigned)zaece3c9);
            } else if (z860360a == zaf33281) {
                z2e91ce7(z930c6f2, "nat punch: rendezvous confirmed the direct path");
            } else {
                z2e91ce7(z930c6f2, "nat punch: rendezvous gave no ruling (old server?) - trying direct");
            }
        }
    } else {
        z2e91ce7(z930c6f2, "nat punch: peer never registered within %d ms", zf13fff5);
    }

    closesocket(s);
    if (z0b00021) WSACleanup();

    return za0fcda0 ? z10fc2a7 : z95c4b69;
}
