#include <stdio.h>
#include <memory.h>

#ifdef _WIN32
#include <WinSock2.h>
#endif

#ifdef linux
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

typedef int SOCKET;

#endif


#ifdef _WIN32
#include "..\Common\TI_def.h"
#elif linux
#include "../Common/TI_def.h"
#endif

#define HELLO "Terra Incognita Client v" VERSION "\n"

void main( void )
{
#ifdef _WIN32
  WSADATA wsaData;
#endif
  SOCKET s;
  struct sockaddr_in serv_addr;
  int i = 0;
  int sends = 0;
  char IP[16];
  char ch;
  char hello[HELLO_LENGTH];
  int stop = 0;

#ifdef _WIN32
  WSAStartup(0x202, &wsaData);
#endif

  printf(HELLO);

  printf("IP:");
  while ((ch = getchar()) != '\n')
    IP[i++] = ch;
  IP[i] = 0;

  printf("Connecting to IP:%s\n", IP);

  s = socket(AF_INET, SOCK_STREAM, 0);
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);
  serv_addr.sin_addr.s_addr = inet_addr(IP);
  connect(s, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

  recv(s, hello, sizeof(hello), 0);
  if (hello[0] == '\xDD')
    printf("Connected\n");
  else
  {
#ifdef _WIN32
    MessageBox(NULL, "Vse ochen ploho((", "Error while connecting", MB_ICONERROR);
#endif
    printf("Vse ochen ploho((\n");
    return;
  }

  printf("%s", hello + 1);

  while (!stop)
  {
    char message[MESSAGE_LENGTH];

    recv(s, message, sizeof(message), 0);
    printf("%s", message);
    if (message[MESSAGE_LENGTH - 1] == ASKING)
    {
      gets(message);
      send(s, message, sizeof(message), 0);
    }

    if (message[MESSAGE_LENGTH - 1] == GAME_OVER)
    {
      printf("Press any key...");
      getchar();
      stop = 1;
    }
  }
}