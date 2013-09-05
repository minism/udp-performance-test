#include <sys/time.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "common.h"


// Compute usleep interval given packet size and rate of transfer
int get_sleep_interval(int packet_size, int kbps)
{
	return 1000 * 1000 * packet_size / (kbps * 1000);
}

void test_transmit(int sock, sa_in* remote_addr, const char* sink, int sink_len, int kbps, int interval)
{
	int packet_size = COM_TEST_PACKET_SIZE;
	int sleep_interval = get_sleep_interval(packet_size, 1);
	int iterations = (interval * 1000 * 1000) / sleep_interval;
	int sink_ptr = 0;
	printf("Transmitting at %d kbps for %d seconds\n", kbps, interval);
	int i;
	for(i = 0; i < iterations; ++i)
	{
		send_data(sock, remote_addr, sink + sink_ptr, packet_size);
		usleep(sleep_interval);
		sink_ptr = (sink_ptr + packet_size) % (sink_len - packet_size);
	}
}

// Transmit data from a garbage `sink` at a rate of `kbps` for `interval` seconds

int main()
{
	// Initialize socket
	int sock = init_socket(0);
	if (sock == 0) 
	{
		printf("Failed to bind socket");
		return 1;
	}

	// Generate 10K of garbage data
	int garbage_len = 1024 * 10; 
	char garbage[garbage_len];
	int fd = open("/dev/urandom", O_RDONLY);
	read(fd, garbage, garbage_len);

	// Initialize the server address structure
	sa_in remote_addr;
	fill_sockaddr(&remote_addr, COM_SERVER_ADDRESS, COM_SERVER_PORT);

	// 1 kbps for 10 seconds
	test_transmit(sock, &remote_addr, garbage, garbage_len, 1, 10);

	destroy_socket(sock);
	return 0;
}
