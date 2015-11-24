/* Yaroslav Dmitriev-Lappo */
/* Creation time:  18:08 23.11.2015 */
/* Last edit time: 18:08 23.11.2015 */

#include <stdio.h>

#ifdef _WIN32
#endif /* Linux support need */

#include "TIS.h"

void LogInit(void)
{
#ifdef _WIN32
  SYSTEMTIME time;

  GetLocalTime(&time); /* Linux support need */
#endif

  logprintf("\n\n\n\n==============================================\n");
  logprintf(HELLO);

#ifdef _WIN32
  logprintf("New game started in %02i.%02i.%i at %02d:%02d\n", time.wDay, time.wMonth, time.wYear, time.wHour, time.wMinute);
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
