/* Yaroslav Dmitriev-Lappo */
/* Creation time:  17:23 23.11.2015 */
/* Last edit time: 17:23 23.11.2015 */

#ifndef __INTERFACE_H_
#define __INTERFACE_H_

#include "TIS.h"

#ifdef _WIN32
#include <winsock2.h>
#endif

#ifdef linux
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

typedef int SOCKET;

#endif

typedef struct
{
  char Name[MESSAGE_LENGTH];
  int HealthPoints, Bullets, Knives, TNT;
  int X, Y;
  int Wounded;
  int HaveTreasure;
  SOCKET Socket;
} PLAYER;

typedef struct
{
  MAP Map;
  PLAYER *Players;
  int Players_number;
  int Current_player;
} GAME;

void SendPropInfo( GAME Game );

#endif /* __INTERFACE_H_ */

/* END OF 'INTERFACE.H' FILE */
