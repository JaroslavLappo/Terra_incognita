/* Yaroslav Dmitriev-Lappo */
/* Creation time:  18:08 23.11.2015 */
/* Last edit time: 18:08 23.11.2015 */

#include <stdio.h>
#include <time.h>

#ifdef _WIN32
#endif /* Linux support need */

#include "TIS.h"

void LogInit(void)
{
#ifdef _WIN32
  SYSTEMTIME systime;

  GetLocalTime(&systime); /* Linux support need */
#endif

/* 
 * Work with system time alternative
 * Syukosevn
 * I think it could work everywhere(WIN/LINUX)
 */

/*
  time_t rawtime;
  struct tm * timeinfo;

  time(&rawtime);
  timeinfo = localtime(&rawtime);
  logprintf ("New game started in %02d.%02d.%d at %02d:%02d\n", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min);
*/

  logprintf("\n\n\n\n==============================================\n");
  logprintf(HELLO);

#ifdef _WIN32
  logprintf("New game started in %02i.%02i.%i at %02d:%02d\n", systime.wDay, systime.wMonth, systime.wYear, systime.wHour, systime.wMinute);
#endif  /* Linux support need */

}

void logprintf(char *fmt, ...)
{
  FILE *Log;
  va_list ap;
  char str[MESSAGE_LENGTH];

  va_start(ap, fmt);

  vsprintf(str, fmt, ap);

  va_end(ap);

  if ((Log = fopen("game.log", "at")) == NULL)
    return;

  fprintf(Log, str);

  fclose(Log);
}
