/****************************************************
*
*    Basic minimal socket server program for use
*    in CSc 487 final projects.  You will have to
*    enhance this for your projects!!
*
*                                  RSF    11/14/20
*
****************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include"FME.h"
#include "SDES.h"
#include<stdlib.h>

int main(int argc , char *argv[])
{
	unsigned int e, d, p, q, n;
	printf("Choose a prime! ");
	scanf("%u", &p);
	printf("Choose another prime! ");
	scanf("%u", &q);

	e = basicallyRSA(p, q);
	printf("%u\n", e);
	d = DRSA(p, q);
	printf("%u\n", d);
	n = PrimeN(p, q);
	printf("%u\n", n);
	
	int socket_desc , new_socket , c, read_size, i;
	struct sockaddr_in server , client;
	char *message, client_message[100];

	char *list;	
	list = "ls -l\n";

	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 8421 );                 // Random high (assumed unused) port
	
	//Bind
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		printf(" unable to bind\n");
		return 1;
	}
	printf(" socket bound, ready for and waiting on a client\n");
	
	//Listen
	listen(socket_desc , 3);
	
	//Accept incoming connection
	printf(" Waiting for incoming connections... \n");
	
	
	c = sizeof(struct sockaddr_in);
	new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
	if (new_socket<0)
	{
		perror("accept failed");
		return 1;
	}
	
	printf("Connection accepted\n");

	//Reply to the client
	message = "You have located Server X at our undisclosed location.  What would you like to say?\n";
	//write(new_socket , message , strlen(message));
	unsigned int base = 7;
	unsigned int modulus = 257;
	unsigned int exponent = 127883921;

    unsigned int publicKey1 = FME(base, exponent, modulus);

	unsigned char buffer[sizeof(unsigned int)];
	for(int i = 0; i < sizeof(unsigned int); i++) {
    buffer[i] = (base >> (i * 8)) & 0xFF;
	}

	send(new_socket, buffer, sizeof(unsigned int), 0);
	
	for(int i = 0; i < sizeof(unsigned int); i++) {
    buffer[i] = (modulus >> (i * 8)) & 0xFF;
	}

	send(new_socket, buffer, sizeof(unsigned int), 0);

	unsigned int publicKey = FME(publicKey1, d, n);

	for(int i = 0; i < sizeof(unsigned int); i++) {
    buffer[i] = (publicKey >> (i * 8)) & 0xFF;
	}
	send(new_socket, buffer, sizeof(unsigned int), 0);

	for(int i = 0; i < sizeof(unsigned int); i++) {
    buffer[i] = (e >> (i * 8)) & 0xFF;
	}
	send(new_socket, buffer, sizeof(unsigned int), 0);

	for(int i = 0; i < sizeof(unsigned int); i++) {
    buffer[i] = (n >> (i * 8)) & 0xFF;
	}
	send(new_socket, buffer, sizeof(unsigned int), 0);

	unsigned int recieved_value;
	recv(new_socket, buffer, sizeof(unsigned int), 0);
	for(int i = 0; i < sizeof(unsigned int); i++) {
		recieved_value |= ((unsigned int)buffer[i] << (i * 8));
	}

	unsigned int AliceE;
	recv(new_socket, buffer, sizeof(unsigned int), 0);
	for(int i = 0; i < sizeof(unsigned int); i++) {
		recieved_value |= ((unsigned int)buffer[i] << (i * 8));
	}

	unsigned int AliceN;
	recv(new_socket, buffer, sizeof(unsigned int), 0);
	for(int i = 0; i < sizeof(unsigned int); i++) {
		recieved_value |= ((unsigned int)buffer[i] << (i * 8));
	}

	unsigned int AuthenticationA = FME(recieved_value, AliceE, AliceN);

	printf("%u\n", AuthenticationA);

    //unsigned int sharedKey = FME(recieved_value, exponent, modulus);

	//Receive a message from client
	while( (read_size = recv(new_socket , client_message , 100 , 0)) > 0 )
	{

		printf("\n Client sent %2i byte message:  %.*s\n",read_size, read_size ,client_message);

		if(!strncmp(client_message,"showMe",6)) 
		{
			printf("\nFiles in this directory: \n");
			system(list);
			printf("\n\n");
		}

		

		//Send the message back to client
		for(i=0;i< read_size;i++)
		{
			//client_message[i] = keysDecrypt(client_message[i], sharedKey);
		}

        printf(" Sending back decrypted message:  %.*s \n", read_size ,client_message);

		//write(new_socket, client_message , strlen(client_message));
		write(new_socket, client_message , read_size);
	}
	
	if(read_size == 0)
	{
		printf("client disconnected\n");
		fflush(stdout);
	}
	else if(read_size == -1)
	{
		perror("receive failed");
	}
		
	//Free the socket pointer
	close(socket_desc);
	return 0;
}
