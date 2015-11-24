/* Yaroslav Dmitriev-Lappo */
/* Creation time:  17:42 23.11.2015 */
/* Last edit time: 17:42 23.11.2015 */

#include <stdio.h>

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

  return command;
} /* End of 'ReadCommand' function */

/* Execute players command */
RESULT CheckResult(MAP *Labyrinth, COMMAND Command, GAME *Game)
{
  RESULT RetValue;
  PLAYER *player = &Game->Players[Game->Current_player];

  RetValue.Player = -1;
  if (Command.Command == COMMAND_SHOOT)
  {
    if (player->Bullets == 0)
    {
      RetValue.Result = RESULT_NOT_ENOUGH_BULLETS;
      return RetValue;
    }
    else
    {
      int i, j;
      int dx, dy;
      int shooted = -1;

      player->Bullets--;
      if (Command.Destination == DESTINATION_DOWN)
      {
        dx = 0;
        dy = 1;
      }
      if (Command.Destination == DESTINATION_UP)
      {
        dx = 0;
        dy = -1;
      }
      if (Command.Destination == DESTINATION_RIGHT)
      {
        dx = 1;
        dy = 0;
      }
      if (Command.Destination == DESTINATION_LEFT)
      {
        dx = -1;
        dy = 0;
      }
      for (i = player->X + dx, j = player->Y + dy;
        i >= 0 && j >= 0 && i <= Labyrinth->W && j <= Labyrinth->H && shooted == -1;
        i += dx, j += dy)
      {
        int k;

        for (k = 0; k < Game->Players_number; k++)
        {
          if (Game->Players[k].X == i && Game->Players[k].Y == j)
          {
            Game->Players[k].HealthPoints -= 1;
            shooted = k;
            if (Game->Players[k].HaveTreasure)
            {
              Game->Players[k].HaveTreasure = 0;
              Labyrinth->Map[i + j * Labyrinth->H] = TREASURE;
            }
          }
        }
      }
      if (shooted != -1)
      {
        RetValue.Result = RESULT_SHOOTED;
        RetValue.Player = shooted;
      }
      else
      {
        RetValue.Result = RESULT_MISSED;
      }
      return RetValue;
    }
  }
  if (Command.Command == COMMAND_KNIFE)
  {
    int k;
    int hit = -1;

    for (k = 0; k < Game->Players_number; k++)
    {
      if (Game->Players[k].X == player->X && Game->Players[k].Y == player->Y)
      {
        Game->Players[k].HealthPoints -= 1;
        hit = k;
      }
    }
    if (hit != -1)
    {
      RetValue.Result = RESULT_HIT;
      RetValue.Player = hit;
    }
    else
    {
      RetValue.Result = RESULT_MISSED;
    }
    return RetValue;
  }
  if (Command.Command == COMMAND_WALK)
  {
    int dx, dy;

    if (Command.Destination == DESTINATION_DOWN)
    {
      dx = 0;
      dy = 1;
    }
    if (Command.Destination == DESTINATION_UP)
    {
      dx = 0;
      dy = -1;
    }
    if (Command.Destination == DESTINATION_RIGHT)
    {
      dx = 1;
      dy = 0;
    }
    if (Command.Destination == DESTINATION_LEFT)
    {
      dx = -1;
      dy = 0;
    }

    if (Labyrinth->Map[player->X + dx + (player->Y + dy) * Labyrinth->H] == VOID_PLACE || Labyrinth->Map[player->X + dx + (player->Y + dy) * Labyrinth->H] == EXIT)
    {
      player->X += 2 * dx;
      player->Y += 2 * dy;
      RetValue.Result = RESULT_WALK;

      if (Labyrinth->Map[player->X + player->Y * Labyrinth->H] == ARMORY)
      {
        player->Bullets++;
        player->Knives++;
        player->TNT++;
        RetValue.Result = RESULT_ARMORY_REACHED;
      }
      if (Labyrinth->Map[player->X + player->Y * Labyrinth->H] == HEALER)
      {
        player->HealthPoints = 1;
        RetValue.Result = RESULT_HEALER_REACHED;
      }
      if (Labyrinth->Map[player->X + player->Y * Labyrinth->H] == TREASURE && player->HealthPoints == 1)
      {
        player->HaveTreasure = 1;
        Labyrinth->Map[player->X + player->Y * Labyrinth->H] = EMPTY_PLACE;
        RetValue.Result = RESULT_TREASURE_REACHED;
      }
      if (Labyrinth->Map[player->X + player->Y * Labyrinth->H] == EXIT)
      {
        if (player->HaveTreasure)
          RetValue.Result = RESULT_WINNER;
        else
          RetValue.Result = RESULT_EXIT_FOUND;
      }
      return RetValue;
    }
    else
    {
      RetValue.Result = RESULT_WAY_IS_BLOCKED;
      return RetValue;
    }
  }

  RetValue.Result = RESULT_UNKNOWN_COMMAND;
  return RetValue;
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
  if (Result.Result == RESULT_MISSED)
    sprintf(message, "%s shot %s. %s now is wounded.\n\n", name, "void", "Void");
  if (Result.Result == RESULT_HIT)
    sprintf(message, "%s hit %s with a knife. %s now is wounded.\n\n", name, subject, subject);
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