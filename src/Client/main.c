#include <stdio.h>
#include <conio.h>

#include <WinSock2.h>

#include "..\Common\TI_def.h"

#define HELLO "Terra Incognita Client v" VERSION "\n"

void main( void )
{
  WSADATA wsaData;
  SOCKET s;
  struct sockaddr_in serv_addr;
  int i = 0;
  int sends = 0;
  char IP[16];
  char ch;
  char hello[HELLO_LENGTH];
  int stop = 0;

  WSAStartup(0x202, &wsaData);

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
  serv_addr.sin_addr.S_un.S_addr = inet_addr(IP);
  connect(s, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

  recv(s, hello, sizeof(hello), 0);
  if (hello[0] == '\xDD')
    printf("Connected\n");
  else
  {
    MessageBox(NULL, "Vse ochen ploho((", "Error while connecting", MB_ICONERROR);
    return;
  }

  printf("%s", hello + 1);

  while (!stop)
  {
    char message[MESSAGE_LENGTH];

    recv(s, message, sizeof(message), 0);
    printf(message);
    if (message[MESSAGE_LENGTH - 1] == ASKING)
    {
      gets(message);
      send(s, message, sizeof(message), 0);
    }

    if (message[MESSAGE_LENGTH - 1] == GAME_OVER)
    {
      printf("Press any key...");
      _getch();
      stop = 1;
    }
  }
}