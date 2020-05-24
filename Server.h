#include <netinet/in.h>

int flag;
int server_sock;
int listen_sock;
struct sockaddr_in listen_addr;
struct sockaddr_in connect_addr;

// missing from spec:
FILE* f;        // for reading from/writing to socket

void server();