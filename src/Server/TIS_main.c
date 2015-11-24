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
  /* ���� �������� �� ssh ������� ������, ����� ��������, ��������� ���-�� ������� */

  /*players_num = 3;

  CreateThread(NULL, 1000, GameSession, players_num, 0, NULL); /* ������ thread ��� ���� � ���� ��������� ssh-������� */
  /*return 0;
}

int main( void )
{
  int first = 1;

  while(1)
  {
    /* ���� ������� ����������� ssh-������� */
/*  if (first)
    {
      CreateThread(NULL, 1000, UserSession, NULL, 0, NULL); /* ������ thread ��� ������� ������� */
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