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
  int width = Game->Map.W * 2 + 1;
  int height = Game->Map.H * 2 + 1;

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

    Game->Players[Game->Current_player].X += 2 * dx; /* we step over */
    Game->Players[Game->Current_player].Y += 2 * dy;

    switch(Game->Map.Map[step_pos])
    {
    case ARMORY:
      result.Result = RESULT_ARMORY_REACHED;
      Game->Players[Game->Current_player].Bullets = 3;
      Game->Players[Game->Current_player].Knives = 3;
      Game->Players[Game->Current_player].TNT = 3;
      break;
    case HEALER:
      result.Result = RESULT_HEALER_REACHED;
      Game->Players[Game->Current_player].HealthPoints = 1;
      Game->Players[Game->Current_player].Wounded = 0;
      break;
    case TREASURE:
      result.Result = RESULT_TREASURE_REACHED;
      Game->Players[Game->Current_player].HaveTreasure = 1;
      Game->Map.Map[step_pos] = EMPTY_PLACE;
      break;
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

    if (Game->Players[Game->Current_player].Knives <= 0)
    {
      result.Result = RESULT_NOT_ENOUGH_KNIVES;

      return result;
    }

    if (victim != -1)
    {
      if (Game->Players[victim].HealthPoints > 0)
        Game->Players[victim].HealthPoints--;

      if (Game->Players[victim].HealthPoints == 0)
        Game->Players[victim].Wounded = 1;

      Game->Players[Game->Current_player].Knives--;

      result.Player = victim;
      result.Result = RESULT_HIT;

      return result;
    }

    result.Result = RESULT_HIT_MISSED;

    return result;
  }

  if (Command.Command == COMMAND_SHOOT)
  {
    //TODO
  }
} /* End of 'CheckResult' function */

/* Send result to all players */
void SendResult(GAME Game, RESULT Result)
{
  char name[MESSAGE_LENGTH];
  char subject[MESSAGE_LENGTH];
  char message[MESSAGE_LENGTH];

  sprintf(name, Game.Players[Game.Current_player].Name);

  if (Result.Player != -1)
    sprintf(subject, Game.Players[Result.Player].Name);

  if (Result.Result == RESULT_UNKNOWN_COMMAND)
    sprintf(message, "Unknown Command\n\n");
  if (Result.Result == RESULT_NOT_ENOUGH_BULLETS)
    sprintf(message, "%s tried to shoot, but he/she haven't enough bullets.\n\n", name);
  if (Result.Result == RESULT_SHOOTED)
    sprintf(message, "%s shot %s. %s now is wounded.\n\n", name, subject, subject);
  if (Result.Result == RESULT_SHOT_MISSED)
    sprintf(message, "%s shot %s. %s now is wounded.\n\n", name, "void", "Void");
  if (Result.Result == RESULT_HIT)
    sprintf(message, "%s hit %s with a knife. %s now is wounded.\n\n", name, subject, subject);
  if (Result.Result == RESULT_HIT_MISSED)
    sprintf(message, "%s hit %s with a knife. %s now is wounded.\n\n", name, "void", "Void");
  if (Result.Result == RESULT_WAY_IS_BLOCKED)
    sprintf(message, "Wall!\n\n");
  if (Result.Result == RESULT_WALK)
    sprintf(message, "Succesful walk.\n\n");
  if (Result.Result == RESULT_ARMORY_REACHED)
    sprintf(message, "Succesful walk and armory reached!\n\n");
  if (Result.Result == RESULT_HEALER_REACHED)
    sprintf(message, "Succesful walk and healer reached!\n\n");
  if (Result.Result == RESULT_TREASURE_REACHED)
    sprintf(message, "Treasure reached!\n\n");
  if (Result.Result == RESULT_EXIT_FOUND)
    sprintf(message, "Exit!\n\n");
  if (Result.Result == RESULT_WINNER)
    sprintf(message, "%s wins!\n\n", name);

  InformPlayers(Game, message);
} /* End of 'SendResult' function */

/* END OF 'COMMAND.C' FILE */
