#include <stdio.h>
#include <string.h>
#include "common.h"


int main()
{
	// Initialize socket
	int sock = init_socket(COM_SERVER_PORT);
	if (sock == 0) 
	{
		printf("Failed to bind socket");
		return 1;
	}

	sa_in remote_addr;
	unsigned char buf[COM_BUFSIZE];
	int recvlen;
	printf("Listening on %d\n", COM_SERVER_PORT);
	printf("b");
	while(1)
	{
		recvlen = recv_data(sock, &remote_addr, buf, COM_BUFSIZE);
		printf("Recieve message from %d: %s\n", remote_addr.sin_addr.s_addr, buf);
	}

	destroy_socket(sock);
	return 0;
}