#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

typedef struct sockaddr_in sa_in;

#define COM_TEST_PACKET_SIZE	200
#define COM_BUFSIZE				1024
#define COM_SERVER_ADDRESS		INADDR_LOOPBACK
#define COM_SERVER_PORT			13002

int init_socket(in_port_t local_port);
void destroy_socket(int sock);
void fill_sockaddr(sa_in* saddr, in_addr_t in_addr, in_port_t in_port);
int send_data(int sock, sa_in* remote_addr, const char* data, int datalen);
int recv_data(int sock, sa_in* remote_addr, unsigned char* buf, int bufsize);