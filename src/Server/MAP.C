/* Yaroslav Dmitriev-Lappo */
/* Creation time:  17:42 23.11.2015 */
/* Last edit time: 17:42 23.11.2015 */

#include <stdio.h>
#include "TIS.h"

MAP GenerateMap( PROPERTIES Properties )
{
  int i, j;
  MAP map;
  FILE *OF;
  map.H = Properties.H;
  map.W = Properties.W;
  map.Properties = Properties;

  if ((OF = fopen("labyrinth.map", "rt")) == NULL)
    return map;

  MessageBox(NULL, strerror(GetLastError()), "Vse ochen ploho((", MB_ICONERROR);

  if ((map.Map = malloc(map.H * map.W)) == NULL)
    return map;

  for (i = 0; i <= 2 * Properties.H; i++)
  {
    for (j = 0; j <= 2 * Properties.W; j++)
      map.Map[j + i * Properties.H] = fgetc(OF);
    fgetc(OF);
  }

  fclose(OF);

  return map;
}

PROPERTIES ReadProp( void )
{
  PROPERTIES RetValue;
  FILE *OF;

  OF = fopen("server.cfg", "rt");

  fscanf(OF, "Type:%i\n", &RetValue.Gametype);
  fscanf(OF, "Width:%i\n", &RetValue.W);
  fscanf(OF, "Height:%i\n", &RetValue.H);
  fscanf(OF, "Armory:%i\n", &RetValue.ArmoriesNumber);
  fscanf(OF, "Healer:%i\n", &RetValue.HealersNumber);
  fscanf(OF, "Holes:%*i\n");
  fscanf(OF, "River:%i\n", &RetValue.RiverLength);

  fclose(OF);

  return RetValue;
}

/* END OF 'MAP.C' FILE */