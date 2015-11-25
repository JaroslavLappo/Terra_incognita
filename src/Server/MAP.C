/* Yaroslav Dmitriev-Lappo */
/* Creation time:  17:42 23.11.2015 */
/* Last edit time: 17:42 23.11.2015 */

#include <stdio.h>
#include "TIS.h"

CELL CharToCell( char ch )
{
  if (ch == (char)0xdb)
    return WALL;

  if (ch == '!')
    return EXIT;

  if (ch == 'A')
    return ARMORY;

  if (ch == 'H')
    return HEALER;

  if (ch == '*')
    return TREASURE;

  if (ch == 'S')
    return RIVER_START;

  if (ch == 'E')
    return RIVER_END;

  if (ch == 'U')
    return RIVER_UP;

  if (ch == 'R')
    return RIVER_RIGHT;

  if (ch == 'D')
    return RIVER_DOWN;

  if (ch == 'L')
    return RIVER_LEFT;

  if (ch == '.')
    return EMPTY_PLACE;

  if (ch == ' ')
    return VOID_PLACE;

  return 0x239;
}

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

  if ((map.Map = malloc(sizeof(CELL) * (2 * map.H + 1) * (2 * map.W + 1))) == NULL)
    return map;

  for (i = 0; i < 2 * Properties.H + 1; i++)
  {
    for (j = 0; j < 2 * Properties.W + 1; j++)
      map.Map[j + i * (2 * Properties.H + 1)] = CharToCell(getc(OF));

    getc(OF);
  }

  fclose(OF);

  return map;
}

PROPERTIES ReadProp( void )
{
  PROPERTIES RetValue;
  FILE *OF;

  if ((OF = fopen("server.cfg", "rt")) == NULL)
    exit(0x239);

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
