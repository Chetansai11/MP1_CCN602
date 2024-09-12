# Variables
CC = gcc
CFLAGS = -Wall


# Targets
all: server.out client.out

server.out: mp1sever.c
	$(CC) $(CFLAGS) -o server.out mp1sever.c

client.out: mp1client.c
	$(CC) $(CFLAGS) -o client.out mp1client.c

echos: server.out
	./server.out $(PORT)

echo: client.out
	./client.out $(SERVER_ADDR) $(PORT)

clean:
	rm -f server.out client.out
