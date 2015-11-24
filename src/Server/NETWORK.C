/* Yaroslav Dmitriev-Lappo */
/* Creation time:  18:08 23.11.2015 */
/* Last edit time: 18:08 23.11.2015 */

#include <stdio.h>

#include "TIS.h"


/* connect to players */
GAME *AcceptPlayers( int players_number )
{
  int i;
  GAME *game;
  WSADATA wsa_data;
  SOCKET server_socket;
  struct sockaddr_in serv_addr;
  char hello[HELLO_LENGTH] = HELLO;
  char name[MESSAGE_LENGTH] = "Your name:";

  LogInit();

  name[MESSAGE_LENGTH - 1] = ASKING;

  if ((game = malloc(sizeof(GAME))) == NULL)
    return NULL;

  if ((game->Players = malloc(sizeof(PLAYER) * players_number)) == NULL)
  {
    free(game);
    return NULL;
  }

  WSAStartup(0x202, &wsa_data);

  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);
  serv_addr.sin_addr.s_addr = 0;
  bind(server_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
  listen(server_socket, players_number);

  for (i = 0; i < players_number; i++)
  {
    printf("\nCreating new connection.\n");
    struct sockaddr_in client_addr;
    int size_client_addr = sizeof(client_addr);
    game->Players[i].Socket = accept(server_socket, (struct sockaddr*)&client_addr, &size_client_addr);

    send(game->Players[i].Socket, hello, sizeof(hello), 0);
    send(game->Players[i].Socket, name, sizeof(name), 0);
  }

  game->Players_number = players_number;

  for (i = 0; i < players_number; i++)
  {
    recv(game->Players[i].Socket, game->Players[i].Name, sizeof(game->Players[i].Name), 0);
    logprintf("Player accepted:\n  number - %i\n  name - %s\n", i, game->Players[i].Name);

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
    } while (game->Map.Map[game->Players[i].X + game->Map.W * game->Players[i].Y]);
  }

  game->Players_number = players_number;
  game->Current_player = 0;

  return game;
} /* End of 'AcceptPlayers' function */

/* send message to every player */
void InformPlayers( GAME Game, char *Message )
{
  int i;

  Message[MESSAGE_LENGTH - 1] = NO_FLAG;

  for (i = 0; i < Game.Players_number; i++)
    send(Game.Players[i].Socket, Message, MESSAGE_LENGTH, 0);
} /* End of 'InformPlayers' function */

/* send message to one player with ASKING flag and receive answer from he */
char *AskPlayer( PLAYER Player, char *Message )
{
  char *answer;

  Message[MESSAGE_LENGTH - 1] = ASKING;

  send(Player.Socket, Message, MESSAGE_LENGTH, 0);

  if ((answer = malloc(MESSAGE_LENGTH)) == NULL)
    return NULL;

  recv(Player.Socket, answer, MESSAGE_LENGTH, 0);

  return answer;
} /* End of 'AskPlayer' function */

/* send message to every player with GAME_OVER flag and close their sockets */
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
/*    shutdown(Game->Players[i].Socket, ???); */ /* I even don't know do we need this.*/
    closesocket(Game.Players[i].Socket);
  }
#endif

#ifdef _LINUX
  for (i = 0; i < Game->Players_number; i++)
    close(Game.Players[i].Socket);
#endif /* Linux support need */

} /* End of 'EndGame' function */

/* END OF 'NETWORK.C' FILE */