/* -----------------------------------------------------------------------
 * GGPO.net (http://ggpo.net)  -  Copyright 2009 GroundStorm Studios, LLC.
 *
 * Use of this software is governed by the MIT license that can be found
 * in the LICENSE file.
 */

#ifndef _GGPO_WINDOWS_H_
#define _GGPO_WINDOWS_H_

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdio.h>
#include "types.h"

// Local patch 3 (LOCAL-PATCHES.md): called with the assert text before the
// MessageBox, so the host can log it. NULL = nothing.
extern void (*g_rbfGgpoAssertHook)(const char *msg);

class Platform {
public:  // types
   typedef DWORD ProcessID;

public:  // functions
   static ProcessID GetProcessID() { return GetCurrentProcessId(); }
   static void AssertFailed(char *msg) {
      if (g_rbfGgpoAssertHook) g_rbfGgpoAssertHook(msg);   // local patch 3
      // Local patch 6: o texto tecnico vai so para o log (acima); o jogador ve um
      // aviso nosso, e nao a caixa "GGPO Assertion Failed" com o arquivo e a linha.
      MessageBoxW(NULL,
                  L"A conexão da partida teve um erro e o jogo vai fechar.\n\n"
                  L"Os detalhes ficaram no rbf-netplay.log.",
                  L"Frame Perfect", MB_OK | MB_ICONWARNING | MB_SETFOREGROUND | MB_TOPMOST);
   }
   static uint32 GetCurrentTimeMS() { return timeGetTime(); }
   static int GetConfigInt(const char* name);
   static bool GetConfigBool(const char* name);
};

#endif
