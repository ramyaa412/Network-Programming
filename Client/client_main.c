//#define _CRT_SECURE_NO_WARNINGS
//#define _WINSOCK_DEPRECATED_NO_WARNINGS
//#define __STDC_WANT_LIB_EXT1__ 1
#include "client_header.h"

int main(int argc, char *argv[])
{
	//Initialize the socket
	WSADATA wsaData;
	int c_sock, port_num, msg_len;
	struct sockaddr_in serv_addr;
	struct hostent *serverIP;
	//Buffer Initialization
	char buffer[RCVBUFSIZE];
	char msg[64];
	int i = 1;
	// File pointer
	FILE * fptr;
	char * text;
	char * command;
	char ch[50], str[50];


	//Test number of arguments
	if (argc != 3)
	{
		fprintf(stdout, "Usage: %s server_name server_port\n", argv[0]);
		return 0;
	}

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	memset((char *)&serv_addr, 0, sizeof(serv_addr));
	//ip address for server
	serverIP = gethostbyname(argv[1]);
	if (serverIP == NULL)
	{
		fprintf(stdout, "ERROR, server host name unknown\n");
		return 0;
	}
	//Get port number as argument
	port_num = atoi(argv[2]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(port_num);



	// Commands - Start the client and ask for the command
	while (i)
	{
		c_sock = WSASocket(PF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

		if (c_sock == INVALID_SOCKET) ERR_EXIT;

		if (connect(c_sock, (SOCKADDR*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
		{
			closesocket(c_sock);
			ERR_EXIT;
		}
		memset(buffer, 0, RCVBUFSIZE);
		fprintf(stdout, "Enter either GET, BOUNCE or EXIT ");

		text = gets(ch);
		strcpy(str, text);
		command = strtok(str, " ");
		// GET command
		if (strcmp(command, "GET") == 0)
		{

			msg_len = send(c_sock, text, 50, 0);
			if (msg_len == SOCKET_ERROR) ERR_EXIT;

			//Create a file in the CLient side 
			fptr = fopen("word.txt", "w");

			if (fptr == NULL)
			{
				fprintf(stdout, "Could not create file\n");
				return 0;
			}

			memset(buffer, 0, RCVBUFSIZE);
			//Get data and write in file
			msg_len = recv(c_sock, buffer, RCVBUFSIZE, 0);
			while (msg_len>0 && strcmp(buffer, "EOF") != 0)
			{
				fputs(buffer, fptr);
				fprintf(stdout, "%s", buffer);
				memset(buffer, 0, RCVBUFSIZE);
				msg_len = recv(c_sock, buffer, RCVBUFSIZE, 0);
			}
			closesocket(c_sock);
			fprintf(stdout, "\n");
		}
		//  BOUNCE command
		else if (strcmp(command, "BOUNCE") == 0)
		{
			//command = strtok(NULL, " ");

			printf("\nSending...\n\n");
			//Send the message to the server
			msg_len = send(c_sock, text, 50, 0);

			//Receive echoed message from server
			msg_len = recv(c_sock, buffer, RCVBUFSIZE, 0);

			//Print the message  
			fprintf(stdout, "Server message : %s\n",command);
			closesocket(c_sock);
		}
		// EXIT command
		else if (strcmp(command, "EXIT") == 0)
		{
			msg_len = send(c_sock, str, 50, 0);

			closesocket(c_sock);
			i = 0;
			fprintf(stdout, "Client Exit\n");
		}
		else
		{
			printf("Use GET or BOUNCE or EXIT commands only\n");
		}
	}
	return 0;
}
