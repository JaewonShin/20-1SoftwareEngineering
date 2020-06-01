#include <netinet/in.h>

int flag;
int client_sock;
struct sockaddr_in client_addr;
char ip[20];

extern int host_net;

void client();
