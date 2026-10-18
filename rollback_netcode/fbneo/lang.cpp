#include <windows.h>
#include <string.h>
#include <wchar.h>

#include "lang.h"

static int zf31e054 = -1;

int z70d1f01(void)
{
	if (zf31e054 < 0) {
		char v[16];
		const DWORD n = GetEnvironmentVariableA("RBF_LANG", v, sizeof(v));
		zf31e054 = (n >= 2 && n < sizeof(v) && (v[0] | 32) == 'e' && (v[1] | 32) == 's' &&
		        (v[2] == 0 || v[2] == '-' || v[2] == '_')) ? 1 : 0;
	}
	return zf31e054;
}

struct z0058415 { const wchar_t* pt; const wchar_t* es; };
struct z65e3829 { const char* pt; const char* es; };

static const z0058415 zc3207ca[] = {
	{ L"voltando para o lobby em %d...", L"volviendo al lobby en %d..." },
	{ L"FT encerrada: o chat fica aberto por %d s.", L"FT terminada: el chat queda abierto por %d s." },

	{ L"Esperando o advers\x00E1rio", L"Esperando al rival" },
	{ L"O advers\x00E1rio n\x00E3o conectou", L"El rival no se conect\x00F3" },
	{ L"Carregando", L"Cargando" },
	{ L"Carregando partida", L"Cargando partida" },
	{ L"FT%d encerrada", L"FT%d terminada" },
	{ L"Partida encerrada por inatividade", L"Partida terminada por inactividad" },
	{ L"Adversário inativo", L"Rival inactivo" },
	{ L"você ficou sem jogar", L"dejaste de jugar" },
	{ L"partida encerrada", L"partida terminada" },
	{ L"Sem conexão com o adversário", L"Sin conexión con el rival" },
	{ L"Partida encerrada", L"Partida terminada" },
	{ L"Fim da demonstração", L"Fin de la demostración" },
	{ L"Transmissão encerrada", L"Transmisión terminada" },
	{ L"N\x00E3o foi poss\x00EDvel conectar direto com o seu advers\x00E1rio.",
	  L"No fue posible conectar directo con tu rival." },
	{ L"A conex\x00E3o pelo servidor deixaria a partida lenta demais para voc\x00EAs dois.",
	  L"La conexi\x00F3n por el servidor dejar\x00ED" L"a la partida demasiado lenta para los dos." },

	{ L"\x00cdndice da FT", L"\x00cdndice de la FT" },
	{ L"As partidas e os rounds, para ir direto", L"Las partidas y los rounds, para ir directo" },
	{ L"Favoritar esta partida", L"Agregar a favoritos" },
	{ L"Fica em Replays salvos (s\x00f3 neste computador)", L"Queda en Replays guardados (solo en esta PC)" },
	{ L"Tomar o controle: %d s", L"Tomar el control: %d s" },
	{ L"Quanto o trecho dura (2 bot\x00f5" L"es juntos; com esquerda, volta antes)",
	  L"Cu\x00e1nto dura el tramo (2 botones juntos; con izquierda, vuelve antes)" },
	{ L"Voltar ao replay", L"Volver al replay" },
	{ L"Desativado", L"Desactivado" },
	{ L"Esquerda/direita escolhe", L"Izquierda/derecha elige" },
	{ L"Cima/baixo escolhe  -  soco/chute fraco confirma  -  Start fecha",
	  L"Arriba/abajo elige  -  pu\x00f1o/patada d\x00e9" L"bil confirma  -  Start cierra" },
	{ L"\x2039  Voltar", L"\x2039  Volver" },
	{ L"n\x00e3o terminou (%d x %d em rounds)", L"no termin\x00f3 (%d x %d en rounds)" },
	{ L"%ls venceu, %d x %d em rounds", L"%ls gan\x00f3, %d x %d en rounds" },
	{ L"ningu\x00e9m", L"nadie" },
	{ L"Analisando o replay... %d%%", L"Analizando el replay... %d%%" },
	{ L"Nenhuma partida completa neste replay.", L"Ninguna partida completa en este replay." },
	{ L"n\x00e3o terminou", L"no termin\x00f3" },
	{ L"%ls venceu", L"%ls gan\x00f3" },
	{ L"Tomar o controle", L"Tomar el control" },
	{ L"o de agora", L"el actual" },
	{ L"Salvo em Replays salvos (s\x00f3 neste computador).", L"Guardado en Replays guardados (solo en esta PC)." },
	{ L"N\x00e3o deu para salvar esta partida.", L"No se pudo guardar esta partida." },
	{ L"Tomar o controle: %d segundos.", L"Tomar el control: %d segundos." },

	{ L"A ou B", L"A o B" },
	{ L"Soco ou chute fraco", L"Pu\x00f1o o patada d\x00e9" L"bil" },
	{ L"C ou D", L"C o D" },
	{ L"Soco ou chute m\x00e9" L"dio", L"Pu\x00f1o o patada media" },
	{ L"Esquerda / direita seguradas", L"Izquierda / derecha mantenidas" },
	{ L"Cima / baixo", L"Arriba / abajo" },
	{ L"2 bot\x00f5" L"es juntos", L"2 botones juntos" },
	{ L"Esquerda + 2 bot\x00f5" L"es", L"Izquierda + 2 botones" },
	{ L"Pausa e continua", L"Pausa y contin\x00fa" L"a" },
	{ L"Velocidade: 1x, 2x, 4x, 8x", L"Velocidad: 1x, 2x, 4x, 8x" },
	{ L"Volta / avan\x00e7" L"a r\x00e1pido (com pausa: um quadro)", L"Retrocede / avanza r\x00e1pido (en pausa: un frame)" },
	{ L"Pr\x00f3ximo round / round anterior", L"Siguiente round / round anterior" },
	{ L"Tomar o controle a partir daqui", L"Tomar el control desde aqu\x00ed" },
	{ L"Tomar o controle voltando antes", L"Tomar el control volviendo antes" },
	{ L"Menu: \x00edndice da FT, favoritar, tempo do tomar o controle",
	  L"Men\x00fa: \x00edndice de la FT, favoritos, tiempo de tomar el control" },
	{ L"Come\x00e7" L"a em %d s  -  qualquer bot\x00e3o come\x00e7" L"a agora",
	  L"Empieza en %d s  -  cualquier bot\x00f3n empieza ya" },
	{ L"Comandos do replay", L"Comandos del replay" },

	{ L"FIM DO REPLAY", L"FIN DEL REPLAY" },
	{ L"VOLTANDO", L"REBOBINANDO" },
	{ L"PAUSADO", L"EN PAUSA" },
	{ L"AVAN\x00c7" L"ANDO %dx", L"AVANZANDO %dx" },

	{ L"Este \x00E9 o chat da partida. T ou F1 exibe/oculta o chat. F2 desativa o chat nesta partida.",
	  L"Este es el chat de la partida. T o F1 muestra/oculta el chat. F2 desactiva el chat en esta partida." },
	{ L"Espere um instante", L"Espera un momento" },
	{ L"N\x00E3o deu para enviar agora", L"No se pudo enviar ahora" },
	{ L"Chat desativado nesta partida. F2 ativa de novo.", L"Chat desactivado en esta partida. F2 lo activa de nuevo." },
	{ L"Chat ativado. T escreve, F1 mostra ou esconde.", L"Chat activado. T escribe, F1 muestra u oculta." },
	{ L"No m\x00E1ximo 200 caracteres", L"M\x00E1ximo 200 caracteres" },
	{ L"Advers\x00E1rio", L"Rival" },

	{ L"ESTA\x00C7\x00C3O %d", L"ESTACI\x00D3N %d" },
	{ L"Continuar na esta\x00E7\x00E3o?", L"Continuar en la estaci\x00F3n?" },
	{ L"Na fila:", L"En la fila:" },
	{ L"fecha em %d s \x00B7 sem resposta = n\x00E3o", L"se cierra en %d s \x00B7 sin respuesta = no" },
	{ L"Recusar", L"Rechazar" },
	{ L"Aceitar", L"Aceptar" },
	{ L"\x2191 vai para os bot\x00F5" L"es \x00B7 \x2190 \x2192 escolhe \x00B7 Enter ou A confirma",
	  L"\x2191 va a los botones \x00B7 \x2190 \x2192 elige \x00B7 Enter o A confirma" },
	{ L"\x2190 \x2192 escolhe \x00B7 Enter ou A confirma", L"\x2190 \x2192 elige \x00B7 Enter o A confirma" },
	{ L"Cima: bot\x00F5" L"es - Esq./Dir.: escolhe - Enter ou A: confirma",
	  L"Arriba: botones - Izq./Der.: elige - Enter o A: confirma" },
	{ L"Esq./Dir.: escolhe - Enter ou A: confirma", L"Izq./Der.: elige - Enter o A: confirma" },
	{ L"Voc\x00EA aceitou: voc\x00EA continua na esta\x00E7\x00E3o depois desta partida.",
	  L"Aceptaste: sigues en la estaci\x00F3n despu\x00E9s de esta partida." },
	{ L"Voc\x00EA recusou: depois desta partida voc\x00EA sai da esta\x00E7\x00E3o.",
	  L"Rechazaste: despu\x00E9s de esta partida sales de la estaci\x00F3n." },
	{ L"Sem resposta: depois desta partida voc\x00EA sai da esta\x00E7\x00E3o.",
	  L"Sin respuesta: despu\x00E9s de esta partida sales de la estaci\x00F3n." },
	{ L"Pr\x00F3xima luta? T, F1 ou Start+Select", L"\x00BFPr\x00F3xima pelea? T, F1 o Start+Select" },

	{ L"N\x00e3o encontramos a ROM de ", L"No encontramos la ROM de " },
	{ L" (faltam os arquivos ", L" (faltan los archivos " },
	{ L" (falta o arquivo ", L" (falta el archivo " },
	{ L".\n\nBaixe a ROM pelo Frame Perfect e tente de novo.",
	  L".\n\nDescarga la ROM desde Frame Perfect e int\x00e9ntalo de nuevo." },
	{ L"A ROM de ", L"La ROM de " },
	{ L" est\x00e1 incompleta ou \x00e9 de outra vers\x00e3o.\n\n", L" est\x00e1 incompleta o es de otra versi\x00f3n.\n\n" },
	{ L"Apague este arquivo e baixe a ROM de novo pelo Frame Perfect:\n",
	  L"Borra este archivo y descarga la ROM de nuevo desde Frame Perfect:\n" },
	{ L"Apague o arquivo dela e baixe a ROM de novo pelo Frame Perfect.",
	  L"Borra su archivo y descarga la ROM de nuevo desde Frame Perfect." },
	{ L"Esta vers\x00e3o do emulador n\x00e3o tem o jogo ", L"Esta versi\x00f3n del emulador no tiene el juego " },
	{ L".\n\nAtualize o Frame Perfect e tente de novo.", L".\n\nActualiza Frame Perfect e int\x00e9ntalo de nuevo." },
	{ L"N\x00e3o foi poss\x00edvel abrir ", L"No se pudo abrir " },
	{ L".\n\nO motivo est\x00e1 em rbf-netplay.log, ao lado do emulador.",
	  L".\n\nEl motivo est\x00e1 en rbf-netplay.log, junto al emulador." },

	{ L"TREINO", L"ENTRENAMIENTO" },
	{ L"Pressione F6 para o treino avançado", L"Presiona F6: entrenamiento avanzado" },
	{ L"hitboxes, gravação do adversário e menu de opções", L"hitboxes, grabación del rival y menú de opciones" },
	{ L"fecha sozinho em %d", L"se cierra en %d" },
	{ L"Ver mais", L"Ver más" },
	{ L"Fechar", L"Cerrar" },

	{ L"Como usar o treino", L"Cómo usar el entrenamiento" },
	{ L"O treino avançado começa desligado. Ligue no F6 quando quiser.",
	  L"El entrenamiento avanzado empieza desactivado. Actívalo con F6 cuando quieras." },
	{ L"liga o treino avançado; com ele ligado, abre e fecha o menu de opções",
	  L"activa el entrenamiento avanzado; activado, abre y cierra el menú de opciones" },
	{ L"liga e desliga os hitboxes (começam desligados)", L"activa y desactiva los hitboxes (empiezan desactivados)" },
	{ L"troca o filtro de CRT (no M\x00E9" L"dio e no Avan\x00E7" L"ado)", L"cambia el filtro CRT (en el Medio y el Avanzado)" },
	{ L"volta ao começo do treino", L"vuelve al inicio del entrenamiento" },
	{ L"salva / carrega o seu ponto do treino", L"guarda / carga tu punto del entrenamiento" },
	{ L"sai do treino", L"sale del entrenamiento" },
	{ L"repete a gravação", L"repite la grabación" },
	{ L"começa e termina uma gravação", L"empieza y termina una grabación" },
	{ L"troca de lado: você controla o P2, para gravar o que ele faz", L"cambia de lado: controlas al P2 para grabar lo que hace" },
	{ L"(ou segurar) abre o menu", L"(o mantener) abre el menú" },
	{ L"escolhe a opção", L"elige la opción" },
	{ L"Botão A", L"Botón A" },
	{ L"Botão B", L"Botón B" },
	{ L"Botão C", L"Botón C" },
	{ L"explica a opção escolhida", L"explica la opción elegida" },
	{ L"volta", L"vuelve" },
	{ L"volta ao jogo", L"vuelve al juego" },
	{ L"posição do HUD, tempo entre os toques do Coin, atraso das entradas, estado salvo, direção que o P2 segura, hitboxes",
	  L"posición del HUD, tiempo entre toques del Coin, retraso de las entradas, estado guardado, dirección que sostiene el P2, hitboxes" },
	{ L"vida e barra do P1 e do P2: recarregar sempre, depois do combo ou nunca, e o valor máximo",
	  L"vida y barra del P1 y del P2: recargar siempre, después del combo o nunca, y el valor máximo" },
	{ L"slot, repetir em loop, ordem e início aleatórios, cortar o começo e o fim, quem é gravado, tocar ao apanhar ou ao carregar o estado, virar sozinho, salvar e carregar, editor",
	  L"slot, repetir en bucle, orden e inicio aleatorios, recortar el inicio y el final, a quién se graba, reproducir al recibir golpes o al cargar el estado, girar solo, guardar y cargar, editor" },
	{ L"COIN, COM O TREINO AVANÇADO LIGADO", L"COIN, EN EL ENTRENAMIENTO AVANZADO" },
	{ L"NO MENU", L"EN EL MENÚ" },
	{ L"Nos jogos da Capcom, A, B e C são o soco fraco, o médio e o forte.",
	  L"En los juegos de Capcom, A, B y C son el puño débil, el medio y el fuerte." },
	{ L"OPÇÕES DO MENU", L"OPCIONES DEL MENÚ" },

	{ L"Voc\x00EA", L"T\x00FA" },
	{ L"  (lido)", L"  (le\x00EDdo)" },
	{ L"F2 ativa", L"F2 activa" },
	{ L"T escreve \x00B7 F1 esconde \x00B7 F2 desativa", L"T escribe \x00B7 F1 oculta \x00B7 F2 desactiva" },
	{ L"Nenhuma mensagem ainda.", L"Todav\x00EDa no hay mensajes." },
	{ L"Enter envia \x00B7 Esc fecha", L"Enter env\x00EDa \x00B7 Esc cierra" },
	{ L"TOMANDO O CONTROLE", L"TOMANDO EL CONTROL" },
	{ L"Esquerda / direita escolhe o lado  -  qualquer bot\u00e3o confirma",
	  L"Izquierda / derecha elige el lado  -  cualquier bot\u00f3n confirma" },
	{ L"SEGUIR COM O REPLAY", L"SEGUIR CON EL REPLAY" },
	{ L"TENTAR DE NOVO", L"INTENTAR DE NUEVO" },
	{ L"F4 ou 2 bot\x00f5" L"es - Tomar o controle", L"F4 o 2 botones - Tomar el control" },

	{ L"Nao foi possivel entrar na transmissao desta partida.\n\n"
	  L"O motivo esta em rbf-netplay.log, ao lado do emulador.",
	  L"No se pudo entrar en la transmisi\x00F3n de esta partida.\n\n"
	  L"El motivo est\x00E1 en rbf-netplay.log, junto al emulador." },
	{ L"RBF - assistir", L"RBF - ver" },
};

static const z65e3829 z9ac68b5[] = {
	{ "CONTROLE DESCONECTADO", "CONTROL DESCONECTADO" },
	{ "RECONECTE PARA CONTINUAR", "RECONECTA PARA CONTINUAR" },
	{ "CONTROLE RECONECTADO", "CONTROL RECONECTADO" },
	{ "VOLUME", "VOLUMEN" },
	{ "ENCERRANDO A PARTIDA", "TERMINANDO LA PARTIDA" },
	{ "HAVERA PERDA DE PONTOS...", "VAS A PERDER PUNTOS..." },
	{ "ENCERRANDO", "TERMINANDO" },
	{ "A FT JA TERMINOU", "LA FT YA TERMINO" },
	{ "FT ENCERRADA", "FT TERMINADA" },
	{ "FECHANDO EM", "CERRANDO EN" },
	{ "ESC SAI SEM PERDER PONTOS", "ESC SALE SIN PERDER PUNTOS" },
	{ "ESC PARA SAIR AGORA", "ESC PARA SALIR AHORA" },
	{ "FECHANDO O JOGO", "CERRANDO EL JUEGO" },
	{ "SOLTE PARA CONTINUAR", "SUELTA PARA CONTINUAR" },
	{ "RECOMECAR A PARTIDA?", "REINICIAR LA PARTIDA?" },
	{ "AGUARDANDO O ADVERSARIO", "ESPERANDO AL RIVAL" },
	{ "APERTE F3 PARA ACEITAR", "PRESIONA F3 PARA ACEPTAR" },
	{ "POSSIVEL MACRO", "POSIBLE MACRO" },
	{ "POSSIVEL TURBO", "POSIBLE TURBO" },
	{ "INATIVIDADE DO %s", "INACTIVIDAD DEL %s" },
	{ "P1 E DO P2", "P1 Y DEL P2" },
	{ "A PARTIDA SERA ENCERRADA", "LA PARTIDA VA A TERMINAR" },
	{ "ENCERRANDO EM %d", "TERMINA EN %d" },

	{ "Nenhuma mensagem ainda.", "A\xFAn no hay mensajes." },
	{ "Voc\xEA", "T\xFA" },
	{ " (lido)", " (le\xED" "do)" },
	{ "Enter envia - Esc fecha   %d/%d", "Enter env\xED" "a - Esc cierra   %d/%d" },
};

const wchar_t* z6f2968c(const wchar_t* pt)
{
	if (!pt || !z70d1f01()) return pt;
	for (size_t i = 0; i < sizeof(zc3207ca) / sizeof(zc3207ca[0]); i++)
		if (wcscmp(zc3207ca[i].pt, pt) == 0) return zc3207ca[i].es;
	return pt;
}

const char* z34ba319(const char* pt)
{
	if (!pt || !z70d1f01()) return pt;
	for (size_t i = 0; i < sizeof(z9ac68b5) / sizeof(z9ac68b5[0]); i++)
		if (strcmp(z9ac68b5[i].pt, pt) == 0) return z9ac68b5[i].es;
	return pt;
}
