#include "server_header.h"

// start of main

int main(int argc, char *argv[])
{
	// socket object
	WSADATA wsaData;
	SOCKET rv_sock, s_sock;
	int port_num, msg_len;
	char buffer[RCVBUFSIZE];
	//char stri[RCVBUFSIZE];
	struct sockaddr_in serv_addr;

	char * command;
	FILE * fptr;
	char filename[100];
	int length = 0;

	//Test the number of arguments
	if (argc != 2)
	{
		fprintf(stdout, "Usage: %s server_port\n", argv[0]);
		return 0;
	}
	// Initialize Winsock 
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	rv_sock = WSASocket(PF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (rv_sock == INVALID_SOCKET) ERR_EXIT;

	memset((char *)&serv_addr, 0, sizeof(serv_addr));
	//Receive the port number as argument 
	port_num = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port_num);

	if (bind(rv_sock, (SOCKADDR*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
	{         // Binding the client
		closesocket(rv_sock);
		ERR_EXIT;
	}

	// listen to the client
	if (listen(rv_sock, MAXPENDING) == SOCKET_ERROR)
	{
		closesocket(rv_sock);
		ERR_EXIT;
	}

	while (1)
	{ // Server runs continuously
		fprintf(stdout, "\nWaiting for client to connect...\n");
		s_sock = accept(rv_sock, NULL, NULL);
		if (s_sock == INVALID_SOCKET) ERR_EXIT;

		memset(buffer, 0, RCVBUFSIZE);
		//printf(buffer);
		msg_len = recv(s_sock, buffer, RCVBUFSIZE, 0);

		if (msg_len == SOCKET_ERROR) ERR_EXIT;

		// To enter the commands one by one
		command = strtok(buffer, " ");
		//  Get command
		if (strcmp(command, "GET") == 0)
		{
			command = strtok(NULL, "");

			//Check if filename is given
			if (command == NULL)
			{
				msg_len = send(s_sock, "Not given file", 20, 0);

			}
			// open the file to read the text
			fptr = fopen(command, "r");
			//Check if file is not existin
			if (fptr == NULL)
			{
				msg_len = send(s_sock, " No such file", 25, 0);

			}
			else {
				fprintf(stdout, "File name is %s\n", command);
				//Read and send file data
				while (!feof(fptr))
				{
					fgets(filename, 58, fptr);
					msg_len = send(s_sock, filename, 58, 0);

				}

				msg_len = send(s_sock, "EOF", 3, 0);



			}
		}
		// Bounce command
		else if (strcmp(command, "BOUNCE") == 0)
		{
			command = strtok(NULL, "");
			length = strlen(command);
			//Send the message back to client
			msg_len = send(s_sock, command, length, 0);
			fprintf(stdout, "Message received from client is  %s\n", command);

		}
		/*else if (strcmp(command, "BOUNCE") == 0)
		{
			
			//printf(command);
			command = strtok("BOUNCE", "");
			//printf(".....R2");
			//command = "BOUNCE";
			//msg_len = recv(s_sock, buffer, RCVBUFSIZE, 0);
			length = strlen(command);
			//printf("R3");
			//Send the message back to client
			
			fprintf(stdout, "Message received from client is  %s\n", buffer);
			msg_len = send(s_sock, command, length, 0);
		}*/
		//  Exit command
		else if (strcmp(command, "EXIT") == 0)
		{
			command = strtok(NULL, "");
			//For exit with code
			if (command != NULL)
			{
				fprintf(stdout, "Exit code is %s\n", command);
			}
			//For exit without code
			else
			{
				fprintf(stdout, "Exit without code\n");
			}
			closesocket(s_sock);
			//Server is exited
			fprintf(stdout, "Server exited\n");
		}


	}

	return 0;
}
