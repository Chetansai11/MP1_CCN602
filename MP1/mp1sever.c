// Name:Chetan sai Borra
// UIN: 436000947
// MP-1 Sever program

#include <stdio.h>
#include <stdlib.h> 
#include <arpa/inet.h>
#include <string.h> 
#include <errno.h>
#include <unistd.h>
//#define PORT 8080
#define buffer 100
// defining the port number and the buffer size of the message.

// this function is to send echo to client and close the client when the "exit" massage is received.
void chatser(int sktclient,int cnt) 
{ 
	char buf[buffer]; 
	 
	// starting the loop
	for (;;) { 
		bzero(buf, buffer); 

		// message is read from the client and stored in the buf.
		read(sktclient, buf, sizeof(buf)); 
		// prints the received msg. 
		printf("Msg from client (%d): %s\t \n",cnt, buf); 
        // echos the same msg back to client.
		write(sktclient, buf, sizeof(buf)); 
        printf("Msg to client (%d): %s\t \n",cnt, buf);

		// if the msg contains "exit" text it closes the chat.  
		if (strncmp("exit", buf, 4) == 0) { 
			printf("Client (%d) Exited ...\n",cnt);
			break; 
		} 
	} 
} 


//for error handling 
void handle_error(const char *msg){
	perror(msg);
}


int main(int arg_cnt,char** numarg)
{
    //creating the socket varibles for server and client.
    int sktsever , sktclient, n;
    //creating the varibles for storing the address of  server and client.
    struct sockaddr_in addsever, addclient;

    //counting the no of clients that are connected.
    int cnt = 0;
    pid_t childpid;
    //Creating the server Socket with IPv4 and TCP connection with socket() and handling the errors.
    sktsever = socket(AF_INET, SOCK_STREAM, 0); 
    if (sktsever < 0)
    {
        handle_error("Scoket not Created...\n");
        exit(1);
    }
    else 
    {
        printf("Socket creation successfully....\n");
    }

    //Defining the IPv4 address as localhost and defining the port.

    memset(&addsever, '\0', sizeof(addsever));
    addsever.sin_family = AF_INET;
    addsever.sin_port = htons(atoi(numarg[1]));
    addsever.sin_addr.s_addr = inet_addr("127.0.0.1");

    //binding the server socket to the address of the server with bind(). 
    if (bind(sktsever, (struct sockaddr*)&addsever, sizeof(addsever)) < 0)
    {
        handle_error("bind error\n");
        exit(1);
    }
    else 
    {
        printf("socket bind completed....\n");
    }

    // listening to the clients using listen() function.
    if((listen(sktsever, 10)) < 0)
    {
        handle_error("Listen fail....\n"); 
		exit(1); 
    }
    else 
    {
        printf("Server listening for clients....\n");
    }

    n = sizeof(addclient);
    //starting the loop to wait and accept the clients.
    while(1)
    {
        //accepting the clients using accept().
        sktclient = accept(sktsever, (struct sockaddr*)&addclient, &n);
        if (sktclient < 0) 
        { 
            handle_error("server accept failed..."); 
            
		    exit(0); 
	    }
	    else
        {
            printf("Connection accepted from %s:(%d).\n", inet_ntoa(addsever.sin_addr), ntohs(addsever.sin_port));
            cnt = cnt+1; 
        }

        // creating the child process using fork while accepting other clients.
        if((childpid = fork()) == 0)
        {
			close(sktsever); 


            chatser(sktclient,cnt);
              
        }
        //close(client_socket);

    } 

    close(sktsever); 
    printf("server closed!...\n");
    
    
    return 0;

}