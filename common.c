#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "common.h"


// Fill address structure
void fill_sockaddr(sa_in* saddr, in_addr_t in_addr, in_port_t in_port)
{
	memset((char*) saddr, 0, sizeof(*saddr));
	saddr->sin_family = AF_INET;
	saddr->sin_addr.s_addr = htonl(in_addr);
	saddr->sin_port = htons(in_port);
}

// Create and bind socket
int init_socket(in_port_t local_port)
{
	// Initialize address structure
	sa_in local_addr;
	fill_sockaddr(&local_addr, INADDR_ANY, local_port);

	// Attempt to bind socket
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0)
	{
		return 0;
	}
	if (bind(sock, (struct sockaddr*) &local_addr, sizeof(local_addr)) < 0) 
	{
		return 0;
	}
	return sock;
}

void destroy_socket(int sock)
{
	close(sock);
}

// Convenience wrappers
int send_data(int sock, sa_in* remote_addr, const char* data)
{
	return sendto(sock, data, strlen(data), 0, (struct sockaddr*) remote_addr, sizeof(*remote_addr));
}

int recv_data(int sock, sa_in* remote_addr, unsigned char* buf, int bufsize)
{
	socklen_t addrlen = sizeof(*remote_addr);
	return recvfrom(sock, buf, bufsize, 0, (struct sockaddr*) remote_addr, &addrlen);
	// return recvfrom(sock, buf, bufsize, 0, 0, 0);
}