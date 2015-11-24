/* Yaroslav Dmitriev-Lappo */
/* Creation time:  17:42 23.11.2015 */
/* Last edit time: 17:42 23.11.2015 */

#ifndef __MAP_H_
#define __MAP_H_

typedef struct
{
  int W, H;
  int ArmoriesNumber, HealersNumber;
  int RiverLength;
  int Gametype;
} PROPERTIES;

typedef struct
{
  PROPERTIES Properties;
  enum 
  {
    WALL = 0xdb, /* wall character */
    EXIT = '!', /* exit character */

    ARMORY = 'A',
    HEALER = 'H',
    TREASURE = '*',

    RIVER_START = 'S',
    RIVER_END = 'E',

    RIVER_UP = 'U',
    RIVER_RIGHT = 'R',
    RIVER_DOWN = 'D',
    RIVER_LEFT = 'L',

    EMPTY_PLACE = '.',
    VOID_PLACE = ' ' /* not a wall */
  } *Map;
  int H, W;
} MAP;

MAP GenerateMap( PROPERTIES properties );
PROPERTIES ReadProp( void );

#endif /* __MAP_H_ */

/* END OF 'MAP.H' FILE */