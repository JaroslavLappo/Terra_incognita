/* Yaroslav Dmitriev-Lappo */
/* Creation time:  18:41 23.11.2015 */
/* Last edit time: 18:41 23.11.2015 */

#include <stdio.h>

#include "TIS.h"

/* Send every player information about map */
void SendPropInfo(GAME Game)
{
  char PropInfo[MESSAGE_LENGTH];
  PROPERTIES properties = Game.Map.Properties;

  sprintf(
	  PropInfo,
	  "Gametype:%s\nSize:%iX%i\nLenght of River:%i\n"
      "Number of Armoryies:%i\nNumber of Healers:%i\nNumber of Wormholes:%i\n",
      /*GTfromType(properties.gametype)*/"Normal game", 
	  properties.W, 
	  properties.H, 
	  properties.RiverLength, 
	  properties.ArmoriesNumber,
	  properties.HealersNumber, 
	  0);

  InformPlayers(Game, PropInfo);
  logprintf(PropInfo);
} /* End of 'SendPropInfo' function */

/* END OF 'INTERFACE.C' FILE */
