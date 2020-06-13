#include <netinet/in.h>

/*
GameStart_Network.c
*/

extern int flag;
extern int server_sock;
extern int listen_sock;
extern struct sockaddr_in listen_addr;
extern struct sockaddr_in connect_addr;
extern int lose_game;
extern int host_net;

// missing from spec:
FILE* f;        // for reading from/writing to socket

void server();


