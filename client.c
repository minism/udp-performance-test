#include <stdio.h>
#include "common.h"

int main()
{
	// Initialize socket
	int sock = init_socket(0);
	if (sock == 0) 
	{
		printf("Failed to bind socket");
		return 1;
	}

	// Initialize server address structure
	sa_in remote_addr;
	fill_sockaddr(&remote_addr, COM_SERVER_ADDRESS, COM_SERVER_PORT);

	// Test message
	const char* msg = "Hello";
	if (send_data(sock, &remote_addr, msg) < 0)
	{
		printf("Failed to send data");
	}
	else
	{
		printf("Sent %s", msg);
	}

	destroy_socket(sock);
	return 0;
}
