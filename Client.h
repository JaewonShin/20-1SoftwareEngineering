#include <netinet/in.h>

/*
GameStart_Network.c
*/
extern int flag;
extern int client_sock;
extern struct sockaddr_in client_addr;
extern char ip[];
extern int host_net;
extern int lose_game;
extern int menu;

void client();
