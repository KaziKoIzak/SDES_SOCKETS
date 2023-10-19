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
#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include"FME.h"
#include "SDES.h"
#include<stdlib.h>

void send_unsigned_int(int sockfd, uint32_t num) {
    // Convert to network byte order
    uint32_t num_n = htonl(num);

    // Send the data
    send(sockfd, &num_n, sizeof(uint32_t), 0);
}

uint32_t receive_unsigned_int(int sockfd) {
    uint32_t received_num;

    // Receive the data
    recv(sockfd, &received_num, sizeof(uint32_t), 0);

    // Convert back to host byte order
    return ntohl(received_num);
}

int main(int argc , char *argv[])
{
	unsigned int e, d, p, q, n;
	p = 7;
	q = 11;
	unsigned int base = 11;
	unsigned int modulus = 257;
	unsigned int exponentPrivate = 6869;

	e = basicallyRSA(p, q);
	d = DRSA(p, q, e);
	n = PrimeN(p, q);

	printf("%u\n", e);
	printf("%u\n", d);
	
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

	unsigned int publicKey1 = FME(base, exponentPrivate, modulus);
	unsigned int publicKey = FME(publicKey1, d, n);
    send_unsigned_int(new_socket, base);
    send_unsigned_int(new_socket, modulus);
    send_unsigned_int(new_socket, publicKey);
    send_unsigned_int(new_socket, e);
    send_unsigned_int(new_socket, n);
	unsigned int AlicePublic = receive_unsigned_int(new_socket);
	unsigned int AlicE = receive_unsigned_int(new_socket);
	unsigned int AliceN = receive_unsigned_int(new_socket);

	printf("%u\n", AlicE);
	printf("%u\n", AliceN);
	printf("%u\n", AlicePublic);
	printf("%u\n", e);
	printf("%u\n", n);
	printf("%u\n", publicKey);

	unsigned int Authentication = FME(AlicePublic, AlicE, AliceN);
	unsigned int checker = FME(publicKey, e, n);

	printf("\n");

	printf("%u\n", Authentication);
	printf("%u", checker);

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
