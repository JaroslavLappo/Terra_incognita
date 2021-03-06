/* Yaroslav Dmitriev-Lappo */
/* Creation time:  18:08 23.11.2015 */
/* Last edit time: 18:08 23.11.2015 */

#include <stdio.h>
#ifdef _WIN32
#  include <winsock2.h>
#  include <ws2tcpip.h>
#endif

#include "TIS.h"


/* connect to players */
GAME *AcceptPlayers( int players_number )
{
  int i;
  GAME *game;
  WSADATA wsa_data;
  SOCKET server_socket;
  struct addrinfo serv_addr;
  struct addrinfo *servres;
  char hello[HELLO_LENGTH] = HELLO;

  if ((game = malloc(sizeof(GAME))) == NULL)
  {
#ifdef _WIN32
    MessageBox(NULL, strerror(GetLastError()), "Vse ochen ploho((", MB_ICONERROR);
#else
	  fprintf(stderr, "Vse ochen ploho((");
#endif
    return NULL;
  }

  if ((game->Players = malloc(sizeof(PLAYER) * players_number)) == NULL)
  {
#ifdef _WIN32
	  MessageBox(NULL, strerror(GetLastError()), "Vse ochen ploho((", MB_ICONERROR);
#else
	  fprintf(stderr, "Vse ochen ploho((");
#endif
	free(game);
    return NULL;
  }

  game->Map = GenerateMap(ReadProp());

#ifdef _WIN32
  WSAStartup(0x202, &wsa_data);
#endif

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.ai_family = AF_UNSPEC;
  serv_addr.ai_socktype = SOCK_STREAM;
  serv_addr.ai_flags = AI_PASSIVE;
  getaddrinfo(NULL, PORT, &serv_addr, &servres);

  server_socket = socket(servres->ai_family, servres->ai_socktype, servres->ai_protocol);
  bind(server_socket, servres->ai_addr, servres->ai_addrlen);
  listen(server_socket, players_number);

  for (i = 0; i < players_number; i++)
  {
    char question[MESSAGE_LENGTH] = "Your name:";

    question[MESSAGE_LENGTH - 1] = ASKING;

    printf("\nCreating new connection.\n");
    struct sockaddr_in client_addr;
    int size_client_addr = sizeof(client_addr);
    game->Players[i].Socket = accept(server_socket, (struct sockaddr*)&client_addr, &size_client_addr);

    send(game->Players[i].Socket, hello, sizeof(hello), 0);
    send(game->Players[i].Socket, question, sizeof(question), 0);
  }

#ifdef _WIN32
  closesocket(server_socket);
#else linux
  close(server_socket);
#endif

  game->Players_number = players_number;

  for (i = 0; i < players_number; i++)
  {
    recv(game->Players[i].Socket, game->Players[i].Name, sizeof(game->Players[i].Name), 0);
    logprintf("Player accepted:\n  number - %i\n  name - %s\n", i, game->Players[i].Name);
    printf("Player accepted:\n  number - %i\n  name - %s\n", i, game->Players[i].Name);

    game->Players[i].Wounded = 0;
    game->Players[i].HaveTreasure = 0;
    game->Players[i].Bullets = 3;
    game->Players[i].HealthPoints = 1;
    game->Players[i].Knives = 3;
    game->Players[i].TNT = 3;

    do
    {
      game->Players[i].X = rand() % game->Map.W;
      game->Players[i].Y = rand() % game->Map.H;
    } while (game->Map.Map[game->Players[i].X + game->Map.W * game->Players[i].Y] != VOID_PLACE);

    printf("Player position: x:%i y:%i\n\n", game->Players[i].X, game->Players[i].Y);
  }

  game->Players_number = players_number;
  game->Current_player = 0;

  return game;
} /* End of 'AcceptPlayers' function */

/* send message to every player */
void InformPlayers( GAME Game, char *Message )
{
  int i;
  char Data[MESSAGE_LENGTH];

  strncpy(Data, Message, MESSAGE_LENGTH - 1);

  Data[MESSAGE_LENGTH - 1] = NO_FLAG;

  for (i = 0; i < Game.Players_number; i++)
    send(Game.Players[i].Socket, Data, MESSAGE_LENGTH, 0);
} /* End of 'InformPlayers' function */

/* send message to one player with ASKING flag and receive answer from he */
char *AskPlayer( PLAYER Player, char *Message )
{
  char *answer;
  char Data[MESSAGE_LENGTH];

  strncpy(Data, Message, MESSAGE_LENGTH - 1);

  Data[MESSAGE_LENGTH - 1] = ASKING;

  if ((answer = malloc(MESSAGE_LENGTH)) == NULL)
    return NULL;


  send(Player.Socket, Data, MESSAGE_LENGTH, 0);

  recv(Player.Socket, answer, MESSAGE_LENGTH, 0);

  return answer;
} /* End of 'AskPlayer' function */

/* send to every player message with GAME_OVER flag and close their sockets */
void EndGame( GAME Game )
{
  int i;
  int winner = -1;
  char message[MESSAGE_LENGTH];

  for (i = 0; i < Game.Players_number; i++)
    if (Game.Players[i].HaveTreasure)
    {
      winner = i;
      break;
    }

  if (winner == -1)
  {
    logprintf("No-winner error.");
    return;
  }

  sprintf(message, "%s is a winner!\n", Game.Players[winner].Name);

  message[MESSAGE_LENGTH - 1] = GAME_OVER;

  logprintf(message);

  for (i = 0; i < Game.Players_number; i++)
    send(Game.Players[i].Socket, message, sizeof(message), 0);

#ifdef _WIN32
  for (i = 0; i < Game.Players_number; i++)
  {
    closesocket(Game.Players[i].Socket);
  }
#endif

#ifdef linux
  for (i = 0; i < Game->Players_number; i++)
    close(Game.Players[i].Socket);
#endif

} /* End of 'EndGame' function */

/* END OF 'NETWORK.C' FILE */
