/* -----------------------------------------------------------------------
 * GGPO.net (http://ggpo.net)  -  Copyright 2009 GroundStorm Studios, LLC.
 *
 * Use of this software is governed by the MIT license that can be found
 * in the LICENSE file.
 */

#include "types.h"
#include "udp.h"

SOCKET
CreateSocket(uint16 bind_port, int retries)
{
   SOCKET s;
   sockaddr_in sin;
   uint16 port;
   int optval = 1;

   s = socket(AF_INET, SOCK_DGRAM, 0);
   setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char *)&optval, sizeof optval);
   setsockopt(s, SOL_SOCKET, SO_DONTLINGER, (const char *)&optval, sizeof optval);

   // non-blocking...
   u_long iMode = 1;
   ioctlsocket(s, FIONBIO, &iMode);

#if defined(_WIN32)
   // Local patch 6 (LOCAL-PATCHES.md): no Windows o "port unreachable" de um
   // pacote antigo volta como WSAECONNRESET no sendto/recvfrom seguinte - quando o
   // outro jogador fecha a partida, o envio seguinte daqui falhava e o ASSERT
   // fechava este emulador tambem. Sem isso, o socket UDP nao recebe esse aviso.
#  ifndef SIO_UDP_CONNRESET
#    define SIO_UDP_CONNRESET _WSAIOW(IOC_VENDOR, 12)
#  endif
   BOOL connReset = FALSE;
   DWORD ioBytes = 0;
   WSAIoctl(s, SIO_UDP_CONNRESET, &connReset, sizeof connReset, NULL, 0, &ioBytes, NULL, NULL);
#endif

   sin.sin_family = AF_INET;
   sin.sin_addr.s_addr = htonl(INADDR_ANY);
   for (port = bind_port; port <= bind_port + retries; port++) {
      sin.sin_port = htons(port);
      if (bind(s, (sockaddr *)&sin, sizeof sin) != SOCKET_ERROR) {
         Log("Udp bound to port: %d.\n", port);
         return s;
      }
   }
   closesocket(s);
   return INVALID_SOCKET;
}

Udp::Udp() :
   _socket(INVALID_SOCKET),
   _callbacks(NULL)
{
}

Udp::~Udp(void)
{
   if (_socket != INVALID_SOCKET) {
      closesocket(_socket);
      _socket = INVALID_SOCKET;
   }
}

void
Udp::Init(uint16 port, Poll *poll, Callbacks *callbacks)
{
   _callbacks = callbacks;

   _poll = poll;
   _poll->RegisterLoop(this);

   Log("binding udp socket to port %d.\n", port);
   _socket = CreateSocket(port, 0);
}

void
Udp::SendTo(char *buffer, int len, int flags, struct sockaddr *dst, int destlen)
{
   struct sockaddr_in *to = (struct sockaddr_in *)dst;

   int res = sendto(_socket, buffer, len, flags, dst, destlen);
   if (res == SOCKET_ERROR) {
      DWORD err = WSAGetLastError();
      Log("unknown error in sendto (erro: %d  wsaerr: %d).\n", res, err);
      // Local patch 6: um envio que falha e um pacote perdido, nao o fim do jogo.
      // O GGPO ja reenvia o que nao foi confirmado, e um adversario que sumiu de
      // verdade encerra a partida pelo disconnect timeout. Era ASSERT: o buffer
      // cheio (WSAEWOULDBLOCK, socket nao bloqueante), o Wi-Fi oscilando
      // (WSAENETUNREACH/WSAEHOSTUNREACH) ou o "port unreachable" de quem fechou o
      // jogo (WSAECONNRESET) fechavam o emulador com a caixa "GGPO Assertion Failed".
#if defined(_WIN32)
      static int s_reported = 0;
      if (g_rbfGgpoAssertHook && s_reported < 5) {
         char m[160];
         snprintf(m, sizeof m, "ggpo: sendto falhou (wsaerr %lu) - pacote perdido, a partida segue%s",
                  (unsigned long)err, s_reported == 4 ? " (as proximas falhas nao vao para o log)" : "");
         g_rbfGgpoAssertHook(m);
      }
      s_reported++;
#endif
      return;
   }
   char dst_ip[1024];
   Log("sent packet length %d to %s:%d (ret:%d).\n", len, inet_ntop(AF_INET, (void *)&to->sin_addr, dst_ip, ARRAY_SIZE(dst_ip)), ntohs(to->sin_port), res);
}

bool
Udp::OnLoopPoll(void *cookie)
{
   uint8          recv_buf[MAX_UDP_PACKET_SIZE];
   sockaddr_in    recv_addr;
   int            recv_addr_len;

   for (;;) {
      recv_addr_len = sizeof(recv_addr);
      int len = recvfrom(_socket, (char *)recv_buf, MAX_UDP_PACKET_SIZE, 0, (struct sockaddr *)&recv_addr, &recv_addr_len);

      // TODO: handle len == 0... indicates a disconnect.

      if (len == -1) {
         int error = WSAGetLastError();
         if (error != WSAEWOULDBLOCK) {
            Log("recvfrom WSAGetLastError returned %d (%x).\n", error, error);
         }
         break;
      } else if (len > 0) {
         char src_ip[1024];
         Log("recvfrom returned (len:%d  from:%s:%d).\n", len, inet_ntop(AF_INET, (void*)&recv_addr.sin_addr, src_ip, ARRAY_SIZE(src_ip)), ntohs(recv_addr.sin_port) );
         UdpMsg *msg = (UdpMsg *)recv_buf;
         _callbacks->OnMsg(recv_addr, msg, len);
      } 
   }
   return true;
}


void
Udp::Log(const char *fmt, ...)
{
   char buf[1024];
   size_t offset;
   va_list args;

   strcpy_s(buf, "udp | ");
   offset = strlen(buf);
   va_start(args, fmt);
   vsnprintf(buf + offset, ARRAY_SIZE(buf) - offset - 1, fmt, args);
   buf[ARRAY_SIZE(buf)-1] = '\0';
   ::Log(buf);
   va_end(args);
}
