#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ncursesw/curses.h>

#include "Client.h"

void client(void)
{
	// socket()
	client_sock = socket(PF_INET, SOCK_STREAM, 0);

	// get IP adress
	clear();
	move(11, 23);
	printw("서버 IP 주소 입력 : ");
	scanw("%s", &ip);

	// adress setting
	memset(&client_addr, 0, sizeof(struct sockaddr_in));
	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = inet_addr(ip);
	client_addr.sin_port = htons(3456); // temporary port num

	// connect()
	connect(client_sock, (struct sockaddr*)&client_addr, sizeof(struct sockaddr_in));

	// start game as client
	host_net = 2;
	menu = game_start_net();
	
	// close()
	close(client_sock);

	return;
}
