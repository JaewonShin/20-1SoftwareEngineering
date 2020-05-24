#include <netinet/in.h>

int flag;
int client_sock;
struct sockaddr_in client_addr;
char ip[20];

// missing from spec:
char port[10];
struct hostent* hp;     // for gethostbyname
FILE* f;                // for reading from/writing to socket

void client();