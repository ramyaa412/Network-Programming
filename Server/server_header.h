#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>                  /* for all WinSock functions */
#include <ws2tcpip.h>
#include <conio.h>


#define MAXPENDING 5                             /* Max outstanding connection requests */ 
#define RCVBUFSIZE 256                                  /* Size of receive buffer */ 
#define ERR_EXIT { \
	fprintf(stderr, "ERROR: %ld\n", WSAGetLastError()); \
	WSACleanup(); return 0; }


#pragma comment(lib, "Ws2_32.lib")
