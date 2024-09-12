// Name:Chetan sai Borra
// UIN: 436000947
// MP-1 Sever program
// sai's code

#include <stdio.h>
#include <stdlib.h> 
#include <arpa/inet.h>
#include <string.h> 
#include <errno.h>
#include <unistd.h>
#define buffer 100
//#define PORT 8080



void chatcli(int sktsever)
{
	char buff[buffer];
	int n;
	for (;;) {
		bzero(buff, sizeof(buff));
		printf("Msg to server: ");
		n = 0;
		while ((buff[n++] = getchar()) != '\n');
        
		write(sktsever, buff, sizeof(buff));
		bzero(buff, sizeof(buff));
		read(sktsever, buff, sizeof(buff));
		printf("Msg from Server : %s", buff);
		if ((strncmp(buff, "exit", 4)) == 0) {
			printf("Client Exit...\n");
			break;
		}
	}
}

void handle_error(const char *msg){
	perror(msg);
}


int main(int arg_cnt,char** numarg){
    int sktsever , sktclient;
    struct sockaddr_in addsever, addclient;

    sktsever = socket(AF_INET, SOCK_STREAM, 0); 
    if (sktsever < 0){
        handle_error("Scoket not Created...\n");
        exit(1);
    }
    else {
        printf("Socket creation successfully....\n");
    }

    memset(&addsever, '\0', sizeof(addsever));
    addsever.sin_family = AF_INET;
    addsever.sin_port = htons(atoi(numarg[2]));
    addsever.sin_addr.s_addr = inet_addr(numarg[1]);

    // connect the client socket to server socket
	if (connect(sktsever, (struct sockaddr*)&addsever, sizeof(addsever)) != 0) {
		handle_error("connection with the server failed...\n");
		exit(0);
	}
	else{
        printf("connected to the server..\n");
    }

    chatcli(sktsever);

	// close the socket
	close(sktsever);

	return 0;
}