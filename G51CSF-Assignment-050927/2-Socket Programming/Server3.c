//////////////////////////  Server3.c ////////////////////////
////////////////////Salma Tamer Fathy Ahmed Elsaid////////////
/////////////////////////////20050927////////////////////////

#include<io.h>
#include<stdio.h>
#include<winsock2.h>
#include<ctype.h>
#include<string.h>
#include<time.h>

#define MY_PORT		8989
#define MAXBUF		256

int main(int argc , char *argv[])
{
    WSADATA wsa;
    SOCKET sockfd , clientfd;

    char recbuff[MAXBUF], sendbuff[MAXBUF], stringt[MAXBUF];
    int i, length;

    struct sockaddr_in self;
    struct tm * timeptr; //the structure tm which has the time information in it
                        //and to which we are adding a pointer timeptr
    time_t rawtime; //a data type that stores system time values and is capable of representing times





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

	/*--forever... ---*/
	while (1)
	{	struct sockaddr_in client_addr;
		int addrlen=sizeof(client_addr);

		/*---accept a connection (creating a data pipe)---*/
		clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);

		//printing the information of the connection
		printf("\nInformation of this connection:\n");
		printf("IP address\n"); //printing the IP address
		printf("%s\n", inet_ntoa(client_addr.sin_addr));
		printf("Port number\n"); //printing the port number
		printf("%d", ntohs(client_addr.sin_port));

        //this will reset the buffer and helps avoid junk values
		memset(sendbuff, 0, MAXBUF);
		memset(recbuff, 0, MAXBUF);

		// a loop while the length of the message is greater than 0
		while(length > 0)
		{

			length = recv(clientfd, recbuff, MAXBUF, 0); //receiving a request from the client and assigning it to 'length'

            //to prevent the server from displaying the length of the enter click
			if (recbuff[0]!='\r'){
            //to display the length of the message on the server
            printf("\nThe length of the message: %d\n", length);
            }



			time(&rawtime); //time function that gets the current time
			timeptr = gmtime(&rawtime); //this is done so the time displayed from the commands below is expressed according to GMT

            //if the received buffer has "TIME" in it then
			if(strncmp(recbuff, "TIME", 4) == 0){

				//if the user enters the command TIME PST
				//compares the received buffer with 'TIME PST' (compares a maximum of 8 characters)
				if(strncmp(recbuff, "TIME PST", 8)== 0){

					timeptr->tm_hour -= 8; //the pointer timeptr from the structure tm points to the current hour - 8
                                            //because the offset hours from GMT is -8
					mktime(timeptr);
					send(clientfd, asctime(timeptr), (int)strlen(asctime(timeptr)), 0); //sending the response back to the client
					//using asctime converts the time into a string so that it is in a form readable by the user

				} //if the user enters the command TIME MST
				//compares the received buffer with 'TIME MST' (compares a maximum of 8 characters)
				else if(strncmp(recbuff, "TIME MST", 8)== 0){

					timeptr->tm_hour -= 7;//the pointer timeptr from the structure tm points to the current hour - 7
                                            //because the offset hours from GMT is -7
					mktime(timeptr); //this function is used as it converts the structure pointed to by
                                    //timeptr into a time_t value according to the local time zone
					send(clientfd, asctime(timeptr), (int)strlen(asctime(timeptr)), 0); //sending the response back to the client

				} //if the user enters the command TIME CST
				//compares the received buffer with 'TIME CST' (compares a maximum of 8 characters)
				else if(strncmp(recbuff, "TIME CST", 8)== 0){

					timeptr->tm_hour -= 6; //the pointer timeptr from the structure tm points to the current hour - 6
                                            //because the offset hours from GMT is -6
					mktime(timeptr);
					send(clientfd, asctime(timeptr), (int)strlen(asctime(timeptr)), 0); //sending the response back to the client

				} //if the user enters the command TIME EST
				//compares the received buffer with 'TIME EST' (compares a maximum of 8 characters)
				else if(strncmp(recbuff, "TIME EST", 8)== 0){

					timeptr->tm_hour -= 5; //the pointer timeptr from the structure tm points to the current hour - 5
                                            //because the offset hours from GMT is -5
					mktime(timeptr);
					send(clientfd, asctime(timeptr), (int)strlen(asctime(timeptr)), 0); //sending the response back to the client

				}//if the user enters the command TIME GMT
				//compares the received buffer with 'TIME GMT' (compares a maximum of 8 characters)
				else if(strncmp(recbuff, "TIME GMT", 8)== 0){
                    //it will just display the current time in GMT
                    //no conversions needed as we already set the time earlier to be displayed in GMT
					send(clientfd, asctime(timeptr), (int)strlen(asctime(timeptr)), 0); //sending the response back to the client

				}////if the user enters the command TIME CET
				//compares the received buffer with 'TIME CET' (compares a maximum of 8 characters)
				else if(strncmp(recbuff, "TIME CET", 8)== 0){

					timeptr->tm_hour += 1; //the pointer timeptr from the structure tm points to the current hour + 1
                                            //because the offset hours from GMT is +1
					mktime(timeptr);
					send(clientfd, asctime(timeptr), (int)strlen(asctime(timeptr)), 0); //sending the response back to the client

				} 	//if the user enters the command TIME MSK
				//compares the received buffer with 'TIME MSK' (compares a maximum of 8 characters)
				else if(strncmp(recbuff, "TIME MSK", 8)== 0){

					timeptr->tm_hour += 3; //the pointer timeptr from the structure tm points to the current hour + 3
                                            //because the offset hours from GMT is +3
					mktime(timeptr);
					send(clientfd, asctime(timeptr), (int)strlen(asctime(timeptr)), 0); //sending the response back to the client

				} 	//if the user enters the command TIME JST
				//compares the received buffer with 'TIME JST' (compares a maximum of 8 characters)
				else if(strncmp(recbuff, "TIME JST", 8)== 0){

					timeptr->tm_hour += 9; //the pointer timeptr from the structure tm points to the current hour + 9
                                            //because the offset hours from GMT is +9
					mktime(timeptr);
					send(clientfd, asctime(timeptr), (int)strlen(asctime(timeptr)), 0); //sending the response back to the client

				} 	//if the user enters the command TIME AEST
				//compares the received buffer with 'TIME AEST' (compares a maximum of 9 characters)
				else if(strncmp(recbuff, "TIME AEST", 9)== 0){

					timeptr->tm_hour += 10; //the pointer timeptr from the structure tm points to the current hour +9
                                            //because the offset hours from GMT is +10
					mktime(timeptr);
					send(clientfd, asctime(timeptr), (int)strlen(asctime(timeptr)), 0); //sending the response back to the client

				} 	//if the user enters the command TIME
				//compares the received buffer with 'TIME'
				else if((strcmp(recbuff, "TIME") == 0)||(strcmp(recbuff, "TIME ") == 0)){

					timeptr = localtime(&rawtime); //it will show the current local time
					send(clientfd, asctime(timeptr), (int)strlen(asctime(timeptr)), 0); //sending the response back to the client

				} //if any other code is displayed with 'TIME', it will show ERROR
				else{

					strcpy(sendbuff, "ERROR"); //copies the word ERROR into sendbuff which is what is sent to the client
					send(clientfd, sendbuff, (int)strlen(sendbuff), 0); //sending the response back to the client

				}
			}
			//else it will reverse the buffer, uppercase it and display the message
			else{

				for(i = 0;recbuff[i] != '\0';++i){

					sendbuff[length - 1 - i] = toupper(recbuff[i]);

				}

				send(clientfd, sendbuff, length, 0); //sends the response back to the client (which is the message that
                                                    //is reversed and uppercased)

			}

            //to refresh the buffer before accepting a new message
			memset(sendbuff, 0, MAXBUF);
			memset(recbuff, 0, MAXBUF);

		} //end of the while loop

		/*---close connection---*/
		close(clientfd);
	}

	/*---clean up (should never get here!)---*/
	close(sockfd);
        WSACleanup();
	return 0;
}
