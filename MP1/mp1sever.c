// Name:Chetan sai Borra
// UIN: 436000947
// MP-1 Sever program

#include <stdio.h>
#include <stdlib.h> 
#include <arpa/inet.h>
#include <string.h> 
#include <unistd.h>
#define PORT 8080
#define buffer 1024

void func(int sktclient) 
{ 
	char buff[buffer]; 
	 
	// infinite loop for chat 
	for (;;) { 
		bzero(buff, buffer); 

		// read the message from client and copy it in buffer 
		read(sktclient, buff, sizeof(buff)); 
		// print buffer which contains the client contents 
		printf("Msg from client: %s\t \n", buff); 

        printf("Msg to client: %s\t \n", buff);


		// and send that buffer to client 
		write(sktclient, buff, sizeof(buff)); 

		// if msg contains "Exit" then server exit and chat ended. 
		if (strncmp("exit", buff, 4) == 0) { 
			printf("Server Exit...\n"); 
			break; 
		} 
	} 
} 


int main()
{
    int sktsever , sktclient, n;
    struct sockaddr_in addsever, addclient;


    pid_t childpid;
    
    sktsever = socket(AF_INET, SOCK_STREAM, 0); 
    if (sktsever < 0){
        printf("Scoket not Created...\n");
        exit(1);
    }
    else {
        printf("Socket creation successfully....\n");
    }

    memset(&addsever, '\0', sizeof(addsever));
    addsever.sin_family = AF_INET;
    addsever.sin_port = PORT;
    addsever.sin_addr.s_addr = inet_addr("127.0.0.1");

    //b = bind(sktsever, (struct sockadd*)&addsever, sizeof(addsever));
    if (bind(sktsever, (struct sockaddr*)&addsever, sizeof(addsever)) < 0){
        printf("bind error\n");
        exit(1);
    }
    else {
        printf("socket bind completed....\n");
    }

    if((listen(sktsever, 10)) < 0){
        printf("Listen fail....\n"); 
		exit(1); 
    }
    else {
        printf("Server listening for clients....\n");
    }

    n = sizeof(addclient);

    while(1){
        sktclient = accept(sktsever, (struct sockaddr*)&addclient, &n);
        if (sktclient < 0) { 
		printf("server accept failed...\n"); 
		exit(0); 
	}
	else{
        printf("Connection accepted from %s:%d...\n", inet_ntoa(addsever.sin_addr), ntohs(addsever.sin_port)); 
    }

    if((childpid = fork()) == 0){
			close(sktsever);


            func(sktclient);


            
        }

    } 

    close(sktsever); 
    printf("client closed!...\n");
    
    
    return 0;

}