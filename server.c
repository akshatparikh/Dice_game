/*

Team name - MAClan
Falgun Patel - 110017108 ,
Akshat Parikh - 105182782 

*/

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

void servicePlayers(int, int);

void servicePlayers(int p1, int p2){  //function

	int p1points[2], p2points[2];
  	p1points[0]=0;
  	p1points[1]=0;
  	p2points[0]=0;
  	p2points[1]=0;
	
	while(1){
    
		printf("Referee: TOTO plays:\n");
		write(p1, "You can now play", 50);							//sends message to Player 1 to play its turn
		if(read(p1, &p1points, sizeof(p1points)) < 0){		//reads points from Player 1
			fprintf(stderr, "read() error\n");
			exit(3);
		}
		
		printf("Points earned: %d\n", p1points[0]);					//prints points earned by Player 1
		printf("Total points earned: %d\n\n", p1points[1]);				//prints total points earned by Player 1 so far
		
		if(p1points[1] >= 100){		
			printf("TOTO won\n");
			write(p1, "Game over: you won the game",30);			//Message to player1
			close(p1);										//closing socket
			write(p2, "Game over: you lost the game",30);	//Message to player2	
			close(p2);										//closing socket
			exit(5);												//verification error
		}
		sleep(2);

		printf("Referee: TITI plays:\n");
		write(p2, "You can now play", 50);							//sends message to Player 2 to play its turn
		if(read(p2, &p2points, sizeof(p2points)) < 0){		//reads points from Player 2
			fprintf(stderr, "read() error\n");
			exit(3);										 //I/O error
		}
		
		printf("Points earned: %d\n", p2points[0]);		
		printf("Total points earned: %d\n\n", p2points[1]);		
		
		if(p2points[1] >= 100){		
			printf("TITI won\n");
			write(p2, "Game over: you won the game",30);						//	
			close(p2);															//sends message to second player		
			write(p1, "Game over: you lost the game",30);						//				
			close(p1);		
			exit(5);
		}
		sleep(2);		
  }
    exit(1); 
}

int main(int argc, char *argv[]){  
	int sd, portNumber, p1, p2;
	struct sockaddr_in servAdd;     // server socket address
	
	if(argc != 2){					// test for correct no. of argument
		printf("Call model: %s <Port Number>\n", argv[0]);
		exit(0);
  }

	if((sd = socket(AF_INET, SOCK_STREAM, 0))<0){   // Creates a reliable stream and checks if it returns -1
		fprintf(stderr, "Could not create socket\n");
		exit(1);
  }
	servAdd.sin_family = AF_INET; 						//Domain for Internet, AF -> Address Family
	servAdd.sin_addr.s_addr = htonl(INADDR_ANY); 		//to accept a client connection on any interface 
	sscanf(argv[1], "%d", &portNumber);					
	servAdd.sin_port = htons((uint16_t)portNumber);
	
	bind(sd, (struct sockaddr *) &servAdd, sizeof(servAdd));
	listen(sd, 6);
	
	while(1){
		p1=accept(sd,(struct sockaddr*)NULL,NULL);			// accept a connection on a socket
		p2=accept(sd,(struct sockaddr*)NULL,NULL);
		
			
			printf("Two Players joined\n");
		
		
		if(!fork())
		{
		  servicePlayers(p1, p2);
		}
	}
	
	return 0;
}

