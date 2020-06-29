#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ncursesw/curses.h>

#include "Client.h"
#include "Server.h"
#include "GameStart_Network.h"

void client(void)
{
	clear();
	host_net = 2;

	// socket()
	client_sock = socket(PF_INET, SOCK_STREAM, 0);
	
	// get IP adress
	while(1) {	
		move(11, 23);
		printw("서버의 아이피주소를 입력하세요 : ");
		scanw("%s", &ip);
		
		// adress setting
		memset(&client_addr, 0, sizeof(struct sockaddr_in));
		client_addr.sin_family = AF_INET;
		client_addr.sin_addr.s_addr = inet_addr(ip);
		client_addr.sin_port = htons(23456); // temporary port num

		// connect()
		
		if (connect(client_sock, (struct sockaddr*) & client_addr, sizeof(client_addr)) != 0) { //입력한 주소로 접속
			clear();
			move(10, 21);			
			printw("연결에 실패하였습니다.");
			move(11, 19);			
			printw("다시");
			continue;
		}
		else break;

	}
	// start game as client	
	game_start_net();

	lose_game == losemsg; //함수 먼저 통과했으니 패배한것  
	flag = 1; //함수를 통과했으므로 flag 1값으로 변경후 서버에게 쏴줌으로 게임 종료를 알림 .
	send(client_sock, (char*)&flag, sizeof(int), 0);

	// close()
	close(client_sock);

	return;
}
