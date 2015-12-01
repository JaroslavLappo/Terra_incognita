/* Yaroslav Dmitriev-Lappo */
/* Creation time:  17:12 23.11.2015 */
/* Last edit time: 17:12 23.11.2015 */

#ifndef __COMMAND_H_
#define __COMMAND_H_

#include "TIS.h"

typedef struct
{
  enum
  {
    COMMAND_UNKNOVN, /* unknown command */
    COMMAND_WALK, /* walking*/
    COMMAND_SHOOT, /* gun shooting */
    COMMAND_KNIFE /* hit with a knife */
  } Command;
  enum
  {
    DESTINATION_UP,
    DESTINATION_RIGHT,
    DESTINATION_DOWN,
    DESTINATION_LEFT, 
    DESTINATION_NULL
  } Destination; /* destination (shoot and walk)*/
} COMMAND;

typedef struct
{
  enum
  {
    RESULT_UNKNOWN_COMMAND, /* nothing */

    RESULT_WEAK, /* too wounded to shoot or hit */

    RESULT_NOT_ENOUGH_BULLETS, /* not enough bullets */
    RESULT_SHOOTED, /* succesful shot */
    RESULT_SHOT_MISSED, /* failure shot*/

    RESULT_NOT_ENOUGH_KNIVES, /* not enough bullets */
    RESULT_HIT, /* succesful knife hit */
    RESULT_HIT_MISSED, /* failure knife shot */

    RESULT_WAY_IS_BLOCKED, /* failure walking */
    RESULT_WALK, /* succesful walking */

    RESULT_ARMORY_REACHED, /* reached armory */
    RESULT_HEALER_REACHED, /* reached healer */
    RESULT_TREASURE_REACHED, /* found treasure */

    RESULT_EXIT_FOUND, /* found exit(without treasure) */
    RESULT_WINNER /* found exit(with treasure) */
  } Result;
  int Player; /* object of result. he was shooted, for example */
} RESULT;

/* Get another command */
COMMAND ReadCommand(GAME Game);

/* Execute players command */
RESULT CheckResult(GAME *Game, COMMAND Command);

/* Send result to all players */
void SendResult(GAME Game, RESULT Result);

#endif /* __COMMAND_H_ */

/* END OF 'COMMAND.H' FILE */
