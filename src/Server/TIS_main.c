/* Yaroslav Dmitriev-Lappo */
/* Creation time:  18:41 23.11.2015 */
/* Last edit time: 18:41 23.11.2015 */

#include <stdio.h>

#include "TIS.h"

int GameSession( int players_number )
{
  GAME *game;

  LogInit();

  if ((game = AcceptPlayers(players_number)) == NULL)
  {
/*    MessageBox(NULL, strerror(GetLastError()), "Vse ochen ploho((", 0);*/
    return 0x239;
  }

  SendPropInfo(*game);

  while (1)
  {
    COMMAND command;
    RESULT result;

    command = ReadCommand(*game);
    result = CheckResult(&(game->Map), command, game);
    SendResult(*game, result);
    game->Current_player = (game->Current_player + 1) % game->Players_number;

    if (result.Result == RESULT_WINNER)
    {
      EndGame(*game);
      free(game->Players);
      free(game);
      return 0x30;
    }
  }
}

#if 0

DWORD WINAPI WinThreadGameSession( LPVOID lpThreadParameter )
{
  int players_number = *(int *)lpThreadParameter;

  GameSession(players_number);

  free(lpThreadParameter);
}

DWORD WINAPI WinThreadUserSession(LPVOID lpThreadParameter)
{
  int *players_num = malloc(sizeof(int));
  /* Kak-budto poluchaem po ssh komandy vsyakie, kartu pomenyat', nastroit' kol-vo igrokov */

  players_num = 3;

  CreateThread(NULL, 1000, GameSession, players_num, 0, NULL); /* Sozdaem thread dlya igry i kak-budto otkluchaem ssh-klienta */
  return 0;
}

int main( void )
{
  int first = 1;

  while(1)
  {
    /* Kak budto ozhidaem podkluchenia ssh-klienta */
  if (first)
    {
      CreateThread(NULL, 1000, UserSession, NULL, 0, NULL); /* sozdaem thread dlya dannogo klienta */
      first = 0;
    }
  }
} */

#endif

int main( void )
{
  GameSession(3);
}
