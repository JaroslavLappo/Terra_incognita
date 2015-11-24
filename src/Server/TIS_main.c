/* Yaroslav Dmitriev-Lappo */
/* Creation time:  18:41 23.11.2015 */
/* Last edit time: 18:41 23.11.2015 */

#include <stdio.h>

#include "TIS.h"


DWORD WINAPI GameSession(LPVOID lpThreadParameter)
{
  MAP labyrinth;
  GAME *game;
  int players_number = *(int *)lpThreadParameter;

  LogInit();

  labyrinth = GenerateMap(ReadProp());
  game = AcceptPlayers(players_number);

  SendPropInfo(*game);

  while (1)
  {
    COMMAND command;
    RESULT result;

    command = ReadCommand(*game);
    result = CheckResult(&labyrinth, command, game);
    SendResult(*game, result);
    game->Current_player = (game->Current_player + 1) % game->Players_number;

    if (result.Result == RESULT_WINNER)
    {
      EndGame(*game);
      free(game);
      free(lpThreadParameter);
      return 0;
    }
  }
}

/*DWORD WINAPI UserSession(LPVOID lpThreadParameter)
{
  int *players_num = malloc(sizeof(int));
  /* Типа получаем по ssh команды всякие, карту поменять, настроить кол-во игроков */

  /*players_num = 3;

  CreateThread(NULL, 1000, GameSession, players_num, 0, NULL); /* Создаём thread для игры и типа отключаем ssh-клиента */
  /*return 0;
}

int main( void )
{
  int first = 1;

  while(1)
  {
    /* Типа ожидаем подключения ssh-клиента */
/*  if (first)
    {
      CreateThread(NULL, 1000, UserSession, NULL, 0, NULL); /* Создаём thread для данного клиента */
/*      first = 0;
    }
  }
} */

int main( void )
{
  int *players_num = malloc(sizeof(int));

  *players_num = 3;

  GameSession(players_num);
}