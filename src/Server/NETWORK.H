/* Yaroslav Dmitriev-Lappo */
/* Creation time:  17:02 23.11.2015 */
/* Last edit time: 17:03 23.11.2015 */

#ifndef __NETWORK_H_
#define __NETWORK_H_

#include "TIS.h"

#if defined(_WIN32) && defined(_LINUX)
#error "Ты впариваешь мне какую-то дичь."
#endif

#if !defined(_WIN32) && !defined(_LINUX)
#error "You need windows or linux to compile this program"
#endif

#ifdef _WIN32
#include <winsock2.h>
#endif

#ifdef _LINUX
#include <sys/socket.h>
#endif

/* connect to players */
GAME *AcceptPlayers( int players_number );

/* send message to every player */
void InformPlayers( GAME Game, char *Message );

/* send message to one player with ASKING flag and receive answer from he */
char *AskPlayer( PLAYER Player, char *Message );

/* send message to every player with GAME_OVER flag and close their sockets */
void EndGame( GAME Game );

#endif /* __NETWORK_H_ */

/* END OF 'NETWORK.H' FILE */