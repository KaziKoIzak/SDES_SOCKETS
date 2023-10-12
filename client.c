/****************************************************
*
*    Basic minimal socket client program for use
*    in CSc 487 final projects.  You will have to
*    enhance this for your projects!!
*
*                                  RSF    11/14/20
*
****************************************************/
#include<stdio.h>      // used printf/scanf for demo (puts/getchar would be leaner)
#include<string.h>	
#include<sys/socket.h>
#include<arpa/inet.h>  // for inet_addr and sockaddr_in structs
#include "FME.h"
#include "SDES.h"
#include<stdlib.h>

int main(int argc , char *argv[])
{
	int socket_desc;    // file descripter returned by socket command
	int read_size;
	struct sockaddr_in server;    // in arpa/inet.h
	char  server_reply[100], client_message[100];   // will need to be bigger
	
	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);

	printf("Trying to create socket\n");
	if (socket_desc == -1)
	{
		printf("Unable to create socket\n");
	}
		
// *********** This is the line you need to edit ****************
	server.sin_addr.s_addr = inet_addr("0.0.0.0");  // doesn't like localhost?
	server.sin_family = AF_INET;
	server.sin_port = htons( 8421 );    // random "high"  port number

	//Connect to remote server
	if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		printf(" connect error");
		return 1;
	}

	unsigned int exponentPrivate = 1012437;

	unsigned char received_buffer[sizeof(unsigned int)];
	recv(socket_desc, received_buffer, sizeof(unsigned int), 0);

	unsigned int base;
	for(int i = 0; i < sizeof(unsigned int); i++) {
		base |= ((unsigned int)received_buffer[i] << (i * 8));
	}

	unsigned int modulus;
	recv(socket_desc, received_buffer, sizeof(unsigned int), 0);
	for(int i = 0; i < sizeof(unsigned int); i++) {
		modulus |= ((unsigned int)received_buffer[i] << (i * 8));
	}

	unsigned int recieved_value;
	recv(socket_desc, received_buffer, sizeof(unsigned int), 0);
	for(int i = 0; i < sizeof(unsigned int); i++) {
		recieved_value |= ((unsigned int)received_buffer[i] << (i * 8));
	}

    unsigned int publicKey = FME(base, exponentPrivate, modulus);

	for(int i = 0; i < sizeof(unsigned int); i++) {
    received_buffer[i] = (publicKey >> (i * 8)) & 0xFF;
	}
	send(socket_desc, received_buffer, sizeof(unsigned int), 0);


    unsigned int sharedKey = FME(recieved_value, exponentPrivate, modulus);


	//Get data from keyboard and send  to server
	printf("What do you want to send to the server. (b for bye)\n");
	while(strncmp(client_message,"b",1))      // quit on "b" for "bye"
	{
		memset(client_message,'\0',100);

		fgets(client_message, sizeof(client_message), stdin);
		
		// Remove the newline character at the end (if present)
		client_message[strcspn(client_message, "\n")] = '\0';
		
		// Iterate through each character and convert to uppercase
		for(int i = 0; i < strlen(client_message); i++) {
			client_message[i] = keysDecrypt(client_message[i], sharedKey);
		}

		if( send(socket_desc , &client_message, strlen(client_message) , 0) < 0)
		{
			printf("Send failed");
			return 1;
		}

		printf("\nSending Message: %.*s\n", (int) strlen(client_message),client_message);
	
		//Receive a reply from the server
		if( (read_size = recv(socket_desc, server_reply , 100 , 0)) < 0)
		{
			printf("recv failed");
		}
		printf("Server  Replies: %.*s\n\n", read_size,server_reply);
	}

	
	return 0;
}
