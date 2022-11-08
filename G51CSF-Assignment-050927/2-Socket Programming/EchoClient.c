//////////////////////////  EchoClient.c ////////////////////////
////////////////////Salma Tamer Fathy Ahmed Elsaid////////////
/////////////////////////////20050927////////////////////////

#include<io.h>
#include<stdio.h>
#include<winsock2.h>
#include<ctype.h>

#define MY_PORT		8989
#define MAXBUF		256

int main(int argc , char *argv[])
{
    WSADATA wsa;
    SOCKET sockfd , clientfd;
   	struct sockaddr_in self;
   	struct sockaddr_in serv_addr;
	char buffer[MAXBUF];

	printf("\nInitialising Winsock...");

    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Initialisation failed.");
        printf("Error Code : %d\n", WSAGetLastError());
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
	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(MY_PORT);
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");


// a function to connect with the client
if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        //if there is no connection it should print this message
        printf("Connection failed.\n");
        //then the client should terminate
        return -1;
    }

    for(;;) //an infinte loop so the client can handle multiple requests
    {
            memset(buffer,0,MAXBUF); //to clear the buffer and avoid junk values

            printf("Message:\n"); //asking the user to enter a message
            gets(buffer); //getting the user input

            //if the user types in QUIT or quit
            if( strcmp(buffer,"QUIT")==0 || strcmp(buffer,"quit")==0 )
            {
                printf("Client terminated.\n"); //it should display this message
                return -1; //and the client should terminate
            }
            else
            {
                send(sockfd, buffer, strlen(buffer), 0); //else it should send the message from the client to the server
                recv(sockfd, buffer, MAXBUF, 0); //and receive response from the server
                printf("Server's response : %s\n", buffer); //then it should display the response from the server on the server's window

            }
    }

    close(sockfd);
	WSACleanup();

	return 0;
}
