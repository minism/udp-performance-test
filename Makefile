CC=clang

all:
	$(CC) client.c common.c -o bin/client
	$(CC) server.c common.c -o bin/server
