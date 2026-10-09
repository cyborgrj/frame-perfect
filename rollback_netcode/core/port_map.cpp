#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include "port_map.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#define za6896db      "239.255.255.250"
#define zcbcf060      1900
#define z799ec8c   1200
#define z46fcbea   2500
#define zd9ca86e       65536
#define z57b9b09      8192
#define zf259fdf  7200

static void zc1e000c(void (*z930c6f2)(const char*), const char* fmt, ...)
{
	if (!z930c6f2) return;
	char buf[400];
	va_list ap;
	va_start(ap, fmt);
	_vsnprintf(buf, sizeof(buf) - 1, fmt, ap);
	va_end(ap);
	buf[sizeof(buf) - 1] = '\0';
	z930c6f2(buf);
}

static struct {
	int            open;
	int            zfd4ce47;
	char           z67b4361[512];
	char           zadff0ab[128];
	unsigned short nPort;
} G;

static const char* z19bab83(const char* zebddd6b, const char* needle)
{
	if (!zebddd6b || !needle || !*needle) return NULL;
	size_t n = strlen(needle);
	for (const char* p = zebddd6b; *p; p++)
		if (_strnicmp(p, needle, n) == 0) return p;
	return NULL;
}

static int between(const char* src, const char* open, const char* close, char* out, int cap)
{
	const char* a = z19bab83(src, open);
	if (!a) return -1;
	a += strlen(open);
	const char* b = z19bab83(a, close);
	if (!b || b <= a) return -1;
	int n = (int)(b - a);
	if (n >= cap) n = cap - 1;
	memcpy(out, a, n);
	out[n] = '\0';
	return 0;
}

static int z70dff00(const char* url, char* host, int zb6835ae, unsigned short* port, char* path, int za953f61)
{
	if (_strnicmp(url, "http://", 7) != 0) return -1;
	const char* p = url + 7;

	const char* slash = strchr(p, '/');
	const char* colon = strchr(p, ':');
	if (colon && slash && colon > slash) colon = NULL;

	const char* z4aa56a6 = colon ? colon : (slash ? slash : p + strlen(p));
	int n = (int)(z4aa56a6 - p);
	if (n <= 0 || n >= zb6835ae) return -1;
	memcpy(host, p, n);
	host[n] = '\0';

	*port = colon ? (unsigned short)atoi(colon + 1) : 80;
	if (*port == 0) return -1;

	_snprintf(path, za953f61 - 1, "%s", slash ? slash : "/");
	path[za953f61 - 1] = '\0';
	return 0;
}

static SOCKET z38dda74(const char* host, unsigned short port)
{
	char z00fcef4[16];
	_snprintf(z00fcef4, sizeof(z00fcef4) - 1, "%u", (unsigned)port);
	z00fcef4[sizeof(z00fcef4) - 1] = '\0';

	struct addrinfo hints, *ai = NULL;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family   = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	if (getaddrinfo(host, z00fcef4, &hints, &ai) != 0 || !ai) return INVALID_SOCKET;

	SOCKET s = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
	if (s == INVALID_SOCKET) { freeaddrinfo(ai); return INVALID_SOCKET; }

	DWORD to = z46fcbea;
	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (const char*)&to, sizeof(to));
	setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, (const char*)&to, sizeof(to));

	int rc = connect(s, ai->ai_addr, (int)ai->ai_addrlen);
	freeaddrinfo(ai);
	if (rc != 0) { closesocket(s); return INVALID_SOCKET; }
	return s;
}

static int zc7cef08(SOCKET s, const char* p, int n)
{
	while (n > 0) {
		int k = send(s, p, n, 0);
		if (k <= 0) return -1;
		p += k;
		n -= k;
	}
	return 0;
}

static int z8a1932a(SOCKET s, char* out, int cap)
{
	int got = 0;
	while (got < cap - 1) {
		int k = recv(s, out + got, cap - 1 - got, 0);
		if (k <= 0) break;
		got += k;
	}
	out[got] = '\0';
	return got;
}

static int z61ebe2d(const char* host, char* out, int cap)
{
	SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s == INVALID_SOCKET) return -1;

	struct addrinfo hints, *ai = NULL;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family   = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	if (getaddrinfo(host, "9", &hints, &ai) != 0 || !ai) { closesocket(s); return -1; }

	int rc = connect(s, ai->ai_addr, (int)ai->ai_addrlen);
	freeaddrinfo(ai);
	if (rc != 0) { closesocket(s); return -1; }

	struct sockaddr_in me;
	int z876bdf8 = sizeof(me);
	if (getsockname(s, (struct sockaddr*)&me, &z876bdf8) != 0) { closesocket(s); return -1; }
	closesocket(s);

	return inet_ntop(AF_INET, &me.sin_addr, out, cap) ? 0 : -1;
}

static int z9028fd8(char* z70b995e, int cap, void (*z930c6f2)(const char*))
{
	SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s == INVALID_SOCKET) return zec8b8cf;

	struct sockaddr_in to;
	memset(&to, 0, sizeof(to));
	to.sin_family = AF_INET;
	to.sin_port   = htons(zcbcf060);
	inet_pton(AF_INET, za6896db, &to.sin_addr);

	static const char* z799a8de[] = {
		"urn:schemas-upnp-org:device:InternetGatewayDevice:1",
		"urn:schemas-upnp-org:service:WANIPConnection:1",
	};

	for (int i = 0; i < 2; i++) {
		char req[512];
		int n = _snprintf(req, sizeof(req) - 1,
			"M-SEARCH * HTTP/1.1\r\n"
			"HOST: %s:%d\r\n"
			"MAN: \"ssdp:discover\"\r\n"
			"MX: 1\r\n"
			"ST: %s\r\n\r\n",
			za6896db, zcbcf060, z799a8de[i]);
		if (n > 0) sendto(s, req, n, 0, (struct sockaddr*)&to, sizeof(to));
	}

	DWORD tStart = GetTickCount();
	while ((int)(GetTickCount() - tStart) < z799ec8c) {
		fd_set rf;
		FD_ZERO(&rf);
		FD_SET(s, &rf);
		struct timeval tv;
		tv.tv_sec  = 0;
		tv.tv_usec = 200 * 1000;
		if (select(0, &rf, NULL, NULL, &tv) <= 0) continue;

		char buf[2048];
		int n = recv(s, buf, sizeof(buf) - 1, 0);
		if (n <= 0) continue;
		buf[n] = '\0';

		if (!z19bab83(buf, "InternetGatewayDevice") && !z19bab83(buf, "WANIPConnection") &&
		    !z19bab83(buf, "WANPPPConnection"))
			continue;

		if (between(buf, "LOCATION:", "\r\n", z70b995e, cap) != 0 &&
		    between(buf, "Location:", "\r\n", z70b995e, cap) != 0)
			continue;

		char* p = z70b995e;
		while (*p == ' ' || *p == '\t') p++;
		if (p != z70b995e) memmove(z70b995e, p, strlen(p) + 1);

		closesocket(s);
		zc1e000c(z930c6f2, "upnp: router description at %s", z70b995e);
		return z5d01f23;
	}

	closesocket(s);
	zc1e000c(z930c6f2, "upnp: no router answered the search");
	return zec8b8cf;
}

static int zab87702(const char* url, char* out, int cap)
{
	char host[256], path[512];
	unsigned short port = 0;
	if (z70dff00(url, host, sizeof(host), &port, path, sizeof(path)) != 0) return -1;

	SOCKET s = z38dda74(host, port);
	if (s == INVALID_SOCKET) return -1;

	char req[1024];
	int n = _snprintf(req, sizeof(req) - 1,
		"GET %s HTTP/1.1\r\nHost: %s:%u\r\nConnection: close\r\n\r\n", path, host, (unsigned)port);
	int rc = (n > 0 && zc7cef08(s, req, n) == 0) ? 0 : -1;
	if (rc == 0) rc = z8a1932a(s, out, cap) > 0 ? 0 : -1;
	closesocket(s);
	return rc;
}

static int z7c354fc(const char* xml, char* szType, int nType, char* szControl, int nControl)
{
	const char* p = xml;
	while ((p = z19bab83(p, "<service>")) != NULL) {
		const char* end = z19bab83(p, "</service>");
		if (!end) break;

		int len = (int)(end - p);
		char block[2048];
		if (len >= (int)sizeof(block)) len = sizeof(block) - 1;
		memcpy(block, p, len);
		block[len] = '\0';

		if (z19bab83(block, "WANIPConnection") || z19bab83(block, "WANPPPConnection")) {
			if (between(block, "<serviceType>", "</serviceType>", szType, nType) == 0 &&
			    between(block, "<controlURL>", "</controlURL>", szControl, nControl) == 0)
				return 0;
		}
		p = end + 1;
	}
	return -1;
}

static int za646719(const char* z67b4361, const char* zadff0ab,
                    const char* szAction, const char* z1f5cf9a,
                    char* z30f30fc, int nReply)
{
	char host[256], path[512];
	unsigned short port = 0;
	if (z70dff00(z67b4361, host, sizeof(host), &port, path, sizeof(path)) != 0) return -1;

	char body[1400];
	int ze938635 = _snprintf(body, sizeof(body) - 1,
		"<?xml version=\"1.0\"?>"
		"<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
		"s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
		"<s:Body><u:%s xmlns:u=\"%s\">%s</u:%s></s:Body></s:Envelope>",
		szAction, zadff0ab, z1f5cf9a, szAction);
	if (ze938635 <= 0) return -1;

	char head[1024];
	int z7631798 = _snprintf(head, sizeof(head) - 1,
		"POST %s HTTP/1.1\r\n"
		"Host: %s:%u\r\n"
		"Content-Type: text/xml; charset=\"utf-8\"\r\n"
		"SOAPAction: \"%s#%s\"\r\n"
		"Content-Length: %d\r\n"
		"Connection: close\r\n\r\n",
		path, host, (unsigned)port, zadff0ab, szAction, ze938635);
	if (z7631798 <= 0) return -1;

	SOCKET s = z38dda74(host, port);
	if (s == INVALID_SOCKET) return -1;

	int rc = (zc7cef08(s, head, z7631798) == 0 && zc7cef08(s, body, ze938635) == 0) ? 0 : -1;
	if (rc == 0) rc = z8a1932a(s, z30f30fc, nReply) > 0 ? 0 : -1;
	closesocket(s);

	if (rc != 0) return -1;
	return z19bab83(z30f30fc, " 200 ") ? 0 : -1;
}

int z29b6e63(unsigned short nPort, void (*z930c6f2)(const char*))
{
	if (nPort == 0) return z17870da;
	za07fb62();

	WSADATA z2957bc2;
	if (WSAStartup(MAKEWORD(2, 2), &z2957bc2) != 0) return zec8b8cf;
	G.zfd4ce47 = 1;

	char szLocation[512] = "";
	int rc = z9028fd8(szLocation, sizeof(szLocation), z930c6f2);
	if (rc != z5d01f23) { WSACleanup(); G.zfd4ce47 = 0; return rc; }

	char host[256], path[512];
	unsigned short ze966528 = 0;
	if (z70dff00(szLocation, host, sizeof(host), &ze966528, path, sizeof(path)) != 0) {
		zc1e000c(z930c6f2, "upnp: router gave an address we cannot parse");
		return zcbe3891;
	}

	char* xml = (char*)malloc(zd9ca86e);
	if (!xml) return zcbe3891;

	if (zab87702(szLocation, xml, zd9ca86e) != 0) {
		free(xml);
		zc1e000c(z930c6f2, "upnp: could not read the router description");
		return zcbe3891;
	}

	char szType[128] = "", szControl[512] = "";
	int found = z7c354fc(xml, szType, sizeof(szType), szControl, sizeof(szControl));
	free(xml);
	if (found != 0) {
		zc1e000c(z930c6f2, "upnp: router has no WAN connection service");
		return zcbe3891;
	}

	char zfc0916b[512];
	if (_strnicmp(szControl, "http://", 7) == 0) {
		_snprintf(zfc0916b, sizeof(zfc0916b) - 1, "%s", szControl);
	} else {
		_snprintf(zfc0916b, sizeof(zfc0916b) - 1, "http://%s:%u%s%s",
		          host, (unsigned)ze966528, szControl[0] == '/' ? "" : "/", szControl);
	}
	zfc0916b[sizeof(zfc0916b) - 1] = '\0';

	char zf1fba00[64] = "";
	if (z61ebe2d(host, zf1fba00, sizeof(zf1fba00)) != 0) {
		zc1e000c(z930c6f2, "upnp: cannot tell which of our addresses faces the router");
		return zcbe3891;
	}

	char* reply = (char*)malloc(z57b9b09);
	if (!reply) return zcbe3891;

	int ok = -1;
	for (int attempt = 0; attempt < 2 && ok != 0; attempt++) {
		char args[768];
		_snprintf(args, sizeof(args) - 1,
			"<NewRemoteHost></NewRemoteHost>"
			"<NewExternalPort>%u</NewExternalPort>"
			"<NewProtocol>UDP</NewProtocol>"
			"<NewInternalPort>%u</NewInternalPort>"
			"<NewInternalClient>%s</NewInternalClient>"
			"<NewEnabled>1</NewEnabled>"
			"<NewPortMappingDescription>RBF rollback netcode</NewPortMappingDescription>"
			"<NewLeaseDuration>%d</NewLeaseDuration>",
			(unsigned)nPort, (unsigned)nPort, zf1fba00, attempt == 0 ? zf259fdf : 0);
		args[sizeof(args) - 1] = '\0';

		ok = za646719(zfc0916b, szType, "AddPortMapping", args, reply, z57b9b09);
	}
	free(reply);

	if (ok != 0) {
		zc1e000c(z930c6f2, "upnp: router refused to map udp/%u", (unsigned)nPort);
		return z1ec6598;
	}

	G.open  = 1;
	G.nPort = nPort;
	_snprintf(G.z67b4361, sizeof(G.z67b4361) - 1, "%s", zfc0916b);
	_snprintf(G.zadff0ab, sizeof(G.zadff0ab) - 1, "%s", szType);
	G.z67b4361[sizeof(G.z67b4361) - 1] = '\0';
	G.zadff0ab[sizeof(G.zadff0ab) - 1] = '\0';

	zc1e000c(z930c6f2, "upnp: udp/%u mapped to %s", (unsigned)nPort, zf1fba00);
	return z5d01f23;
}

void za07fb62(void)
{
	if (!G.open) {
		if (G.zfd4ce47) WSACleanup();
		memset(&G, 0, sizeof(G));
		return;
	}

	char args[256];
	_snprintf(args, sizeof(args) - 1,
		"<NewRemoteHost></NewRemoteHost>"
		"<NewExternalPort>%u</NewExternalPort>"
		"<NewProtocol>UDP</NewProtocol>",
		(unsigned)G.nPort);
	args[sizeof(args) - 1] = '\0';

	char* reply = (char*)malloc(z57b9b09);
	if (reply) {
		za646719(G.z67b4361, G.zadff0ab, "DeletePortMapping", args, reply, z57b9b09);
		free(reply);
	}
	if (G.zfd4ce47) WSACleanup();
	memset(&G, 0, sizeof(G));
}

int z7f9c7ee(void) { return G.open; }
