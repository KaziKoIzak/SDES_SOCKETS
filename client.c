/****************************************************
*
*    Basic minimal socket client program for use
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
#include "FME.h"
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
	p = 13;
	q = 17;
	unsigned int exponentPrivate = 10137;

	e = basicallyRSA(p, q);
	d = DRSA(p, q);
	n = PrimeN(p, q);

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

	unsigned int base = receive_unsigned_int(socket_desc);
	unsigned int modulus = receive_unsigned_int(socket_desc);
	unsigned int bobPublic = receive_unsigned_int(socket_desc);
	unsigned int bobE = receive_unsigned_int(socket_desc);
	unsigned int bobN = receive_unsigned_int(socket_desc);
	unsigned int publicKey1 = FME(base, exponentPrivate, modulus);
	unsigned int publicKey = FME(publicKey1, d, n); 
	send_unsigned_int(socket_desc, publicKey);
	send_unsigned_int(socket_desc, e);
	send_unsigned_int(socket_desc, n);

	unsigned int authentication = FME(bobPublic, bobE, bobN);

	printf("%u\n", authentication);

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
			//client_message[i] = keysDecrypt(client_message[i], sharedKey);
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
