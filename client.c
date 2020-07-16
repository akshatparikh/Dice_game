/*
Team name - MAClan
Falgun Patel - 110017108 ,
Akshat Parikh - 105182782 
*/

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h> 
#include <arpa/inet.h>
#include <time.h>
#include <signal.h>

int main(int argc, char *argv[])
{	
	srand(time(NULL));
	signal(SIGINT, SIG_IGN);		//to ignore CTRL + C
	int server, portNumber, fp;
	int pPoint[2];
	struct sockaddr_in servAdd;     // server socket address
	char message[255];
	
	if(argc != 3){
		printf("Call model: %s <IP Address> <Port Number>\n", argv[0]);
		exit(0);
	}
	
	if((server = socket(AF_INET, SOCK_STREAM, 0))<0){  //if returns -1
		fprintf(stderr, "Cannot create socket\n");
		exit(1);
	}
	
	servAdd.sin_family = AF_INET;  // IPv4 address family
	sscanf(argv[2], "%d", &portNumber);
	servAdd.sin_port = htons((uint16_t)portNumber);

	if(inet_pton(AF_INET, argv[1], &servAdd.sin_addr) < 0){ // Convert address and checkinf of failure
		fprintf(stderr, " inet_pton() has failed\n");
		exit(2);
	}
	if(connect(server, (struct sockaddr *) &servAdd, sizeof(servAdd))<0){ 	// Establish the connection to the server
		fprintf(stderr, "connect() has failed, exiting\n");
		exit(3);
	}
	
	pPoint[0] = 0;
	pPoint[1] = 0;
	
	while(1){ 
	
		if(fp=read(server, message, 50)){ 						//reads message from server 
			message[fp]= 0;

			if(!strcasecmp(message, "You can now play")){		//checks for player's turn
				printf("*** Your Turn ***\n");
				pPoint[0] = (rand() % 6 ) + 1;
				pPoint[1] +=  pPoint[0];
				printf("Points earned: %d\n", pPoint[0]);		//Displaying points
				printf("Total points earned: %d\n", pPoint[1]);
				printf("\n");	
				write(server, &pPoint, sizeof(pPoint));				//write points to the server 
			} 
			
			else if(!strcasecmp(message, "Game over: you won the game")){		//checks server message if someplayer has won
				fprintf(stderr, "%s\n","*** I won the game!!! ***");
				close(server);
				exit(3);
			}
			
			else if(!strcasecmp(message, "Game over: you lost the game")){		//checks server message if player has losen the game
				fprintf(stderr, "%s\n","*** I lost the game ***");
				close(server);
				exit(5);
			}
		}
		
	}	
}