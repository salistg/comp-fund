//////////////////////////  Server2.c ////////////////////////
////////////////////Salma Tamer Fathy Ahmed Elsaid////////////
/////////////////////////////20050927////////////////////////

#include <io.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#define MY_PORT		8989
#define MAXBUF		256

int main(int argc , char *argv[])
{
    WSADATA wsa;
    SOCKET sockfd , clientfd;
    struct sockaddr_in self;
	char buffer[MAXBUF], reversed[MAXBUF];

    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        return 1;
    }

    printf("Initialised.\n");

	/*---create streaming socket---*/
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		perror("Socket");
		exit(errno);
	}

        printf("Socket created.\n");

	/*---initialize address/port structure---*/
	/* bzero(&self, sizeof(self));*/
	self.sin_family = AF_INET;
	self.sin_port = htons(MY_PORT);
	self.sin_addr.s_addr = INADDR_ANY;

	/*---assign a port number to the socket---*/
    if ( bind(sockfd, (struct sockaddr*)&self, sizeof(self)) != 0 )
	{
		perror("socket--bind");
		exit(errno);
	}

        puts("Bind done");

	/*---make it a "listening socket"---*/
	if ( listen(sockfd, 20) != 0 )
	{
		perror("socket--listen");
		exit(errno);
	}

        puts("Waiting for incoming connections...");

	/*---forever... ---*/
	while (1)
	{	struct sockaddr_in client_addr;
		int addrlen=sizeof(client_addr);

		int length;
		int i, j;

		/*---accept a connection (creating a data pipe)---*/
		clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);


		while (1) //while loop so the server keeps accepting messages
		{
			length = recv(clientfd, buffer, MAXBUF, 0); //receiving a request from the client and assigning that to 'length'


			//this if statement is used to prevent the length of the message from being displayed if its an enter click
               if (buffer[0]!='\r'){
                //printing the information of the connection
                printf("\nInformation of this connection:\n");
				printf("IP address\n");
				printf("%s\n", inet_ntoa(client_addr.sin_addr)); //this will print the IP address of the connection
				printf("Port number\n");
				printf("%d\n", ntohs(client_addr.sin_port)); //this will print the port number of the connection
				printf("\nThe length of the message: %d \n", length); //printing the length of the message
               }

            //loop to reverse messages and uppercase them
            //the characters will be changed to uppercase then they'll be placed in the reversed buffer
			for(i=length-1, j=0; i>=0; i--)
            {
                reversed[j++] = toupper(buffer[i]);
            }


            //sending the response back to the client
			send(clientfd, reversed, length, 0);

			// close the connection
			close(clientfd);
		}
	}

	/*---clean up (should never get here!)---*/
	close(sockfd);
    WSACleanup();

	return 0;
}
