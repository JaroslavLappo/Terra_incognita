/* Yaroslav Dmitriev-Lappo */
/* Creation time:  17:42 23.11.2015 */
/* Last edit time: 17:42 23.11.2015 */

#include <stdio.h>
#include <stdlib.h>

#include "TIS.h"

/* Get another command */
COMMAND ReadCommand( GAME Game )
{
  int i = 0;
  char ch = '\0';
  char message[MESSAGE_LENGTH];
  char question[MESSAGE_LENGTH];
  char *answer;
  COMMAND command;

  sprintf(question, "%s, your actions:", Game.Players[Game.Current_player].Name);
  answer = AskPlayer(Game.Players[Game.Current_player], question);

  sprintf(message, "%s: %s\n", Game.Players[Game.Current_player].Name, answer);
  InformPlayers(Game, message);

  logprintf(message);

  command.Command = COMMAND_UNKNOVN;
  command.Destination = DESTINATION_NULL;

  if (strcmp(answer, "Move up") == 0)
  {
    command.Command = COMMAND_WALK;
    command.Destination = DESTINATION_UP;
  }
  if (strcmp(answer, "Move right") == 0)
  {
    command.Command = COMMAND_WALK;
    command.Destination = DESTINATION_RIGHT;
  }
  if (strcmp(answer, "Move down") == 0)
  {
    command.Command = COMMAND_WALK;
    command.Destination = DESTINATION_DOWN;
  }
  if (strcmp(answer, "Move left") == 0)
  {
    command.Command = COMMAND_WALK;
    command.Destination = DESTINATION_LEFT;
  }
  if (strcmp(answer, "Shoot up") == 0)
  {
    command.Command = COMMAND_SHOOT;
    command.Destination = DESTINATION_UP;
  }
  if (strcmp(answer, "Shoot right") == 0)
  {
    command.Command = COMMAND_SHOOT;
    command.Destination = DESTINATION_RIGHT;
  }
  if (strcmp(answer, "Shoot down") == 0)
  {
    command.Command = COMMAND_SHOOT;
    command.Destination = DESTINATION_DOWN;
  }
  if (strcmp(answer, "Shoot left") == 0)
  {
    command.Command = COMMAND_SHOOT;
    command.Destination = DESTINATION_LEFT;
  }
  if (strcmp(answer, "Hit with knife") == 0)
  {
    command.Command = COMMAND_KNIFE;
  }

  free(answer);

  return command;
} /* End of 'ReadCommand' function */

/* Execute players command */
RESULT CheckResult(GAME *Game, COMMAND Command)
{
  RESULT result;
  int width = Game->Map.W;
  int height = Game->Map.H;

  result.Result = RESULT_UNKNOWN_COMMAND;
  result.Player = -1;

  if (Command.Command == COMMAND_UNKNOVN)
    return result;

  if (Command.Command == COMMAND_WALK)
  {
    int x = Game->Players[Game->Current_player].X;
    int y = Game->Players[Game->Current_player].Y;
    int dx, dy;
    int wall_pos, step_pos;

    switch (Command.Destination)
    {
    case DESTINATION_UP:
      dx = 0;
      dy = -1;
      break;
    case DESTINATION_DOWN:
      dx = 0;
      dy = 1;
      break;
    case DESTINATION_RIGHT:
      dx = 1;
      dy = 0;
      break;
    case DESTINATION_LEFT:
      dx = -1;
      dy = 0;
      break;
    default:
      dx = 0;
      dy = 0;
    }

    wall_pos = (x + dx) + (y + dy) * width; /* Cell that we step over */
    step_pos = (x + 2 * dx) + (y + 2 * dy) * width; /* Cell that we step on */

    if (Game->Map.Map[wall_pos] == EXIT && Game->Players[Game->Current_player].HaveTreasure) /* if we have treasure and stepped over EXIT, we are winners */
    {
      result.Result = RESULT_WINNER;

      return result;
    }

    if (Game->Map.Map[wall_pos] != VOID_PLACE) /* if we cannot step over this */
    {
      result.Result = RESULT_WAY_IS_BLOCKED;

      return result;
    }

    x += 2 * dx; /* we step over */
    y += 2 * dy;

    switch(Game->Map.Map[step_pos])
    {
    case ARMORY:
      result.Result = RESULT_ARMORY_REACHED;
      break;
    case HEALER:
      result.Result = RESULT_HEALER_REACHED;
    default:
      result.Result = RESULT_WALK;
    }

    return result;
  }

  if (Command.Command == COMMAND_KNIFE)
  {
    int i;
    int victim = -1;

    for (i = 0; i < Game->Players_number; i++)
      if (Game->Players[i].X == Game->Players[Game->Current_player].X && Game->Players[i].Y == Game->Players[Game->Current_player].Y && i != Game->Current_player)
      {
        victim = i;
        break;
      }

#ifdef SELF_HIT_WEAK
    if (Game->Players[Game->Current_player].Wounded)
      victim = Game->Current_player;
#endif

#ifdef SELF_HIT_RAND
    if (victim == -1)
    {
      if (SELF_HIT_LONELY >= (double)rand() / RAND_MAX)
        victim = Game->Current_player;
    } else
    {
      if (SELF_HIT_NOT_LONELY >= (double)rand() / RAND_MAX)
        victim = Game->Current_player;
    }
#endif

#ifndef SELF_HIT_WEAK
    if (Game->Players[Game->Current_player].Wounded)
    {
      result.Player = -1;
      result.Result = RESULT_WEAK;
    }
#endif

    if (victim != -1)
    {
      if (Game->Players[victim].HealthPoints > 0)
        Game->Players[victim].HealthPoints--;

      if (Game->Players[victim].HealthPoints == 0)
        Game->Players[victim].Wounded = 1;

      result.Player = victim;
      result.Result = RESULT_HIT;

      return result;
    }

    result.Result = RESULT_HIT_MISSED;

    return;
  }

  if (Command.Command == COMMAND_SHOOT)
  {
    //TODO
  }
} /* End of 'CheckResult' function */

/* END OF 'COMMAND.C' FILE */
