#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "common.h"


// Get difference between two time values in microseconds
long get_delta_usec(const struct timeval* t0, const struct timeval* t1)
{
	return (t1->tv_sec - t0->tv_sec) * 1000000 + 
  		    t1->tv_usec - t0->tv_usec;
}


// Transmit data from a garbage `sink` at a rate of `kbps` for `duration` seconds
void test_transmit(int sock, sa_in* remote_addr, const char* sink, int sink_len, int kbps, int duration)
{
	int packet_size = COM_TEST_PACKET_SIZE;
	long delivery_interval_usec = 1000 * 1000 * packet_size / (kbps * 1000);
	int sink_ptr = 0;
	struct timeval start_time, send_time, cur_time;
	gettimeofday(&start_time, NULL);
	printf("Transmitting at %d kbps for %d seconds\n", kbps, duration);
	int count = 0;
	while (1)
	{
		// Send packet
		gettimeofday(&send_time, NULL);
		send_data(sock, remote_addr, sink + sink_ptr, packet_size);

		// Sleep the rest of the expected interval
		gettimeofday(&cur_time, NULL);
		long delta = get_delta_usec(&send_time, &cur_time); 
		usleep(delivery_interval_usec - delta);

		// Cycle through data sink
		sink_ptr = (sink_ptr + packet_size) % (sink_len - packet_size);

		// Loop until duration reached
		long total_delta = get_delta_usec(&start_time, &cur_time);
		if (total_delta >= duration * 1000 * 1000) 
			break;
	}
}


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
