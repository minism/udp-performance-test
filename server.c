#include <stdio.h>
#include <string.h>
#include <unistd.h>
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
	int packet_num = 0;
	int recvlen = 0;
	printf("Listening on %d\n", COM_SERVER_PORT);
	while(1)
	{
		recvlen = recv_data(sock, &remote_addr, buf, COM_BUFSIZE);
		packet_num++;
		printf("Recieve packet %d\n", packet_num);
	}

	destroy_socket(sock);
	return 0;
}