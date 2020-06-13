#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ncursesw/curses.h>

#include "Server.h"
#include "Client.h"
#include "GameStart_Network.h"

void server() {
    // TODO: add some error checks
    host_net = 1;

    listen_sock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&listen_addr, 0, sizeof(listen_addr));
	listen_addr.sin_family = AF_INET;
	listen_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	listen_addr.sin_port = htons(23456);
	

	if(bind(listen_sock, (struct sockaddr*)&listen_addr, sizeof(listen_addr)) < 0) {
		printw("ERROR_BIND FAILED");
		return;
	}
	if(listen(listen_sock,1) < 0) {	
		printf("ERROR_소켓 리슨 실패\n");
		return;
	}

	int sockaddr_in_size = sizeof(connect_addr);
	
	server_sock = accept(listen_sock, (struct sockaddr*)&connect_addr,
	&sockaddr_in_size);	// 소켓연결이 들어올때까지 대기

	
	char msg[1024] = { 0 };	

    // how to pass f?    
    game_start_net();

    flag = 1;
    send(server_sock, (char*)&flag, sizeof(int), 0); //플래그값 클라이언트에게 쏴줌 
    lose_game == losemsg; //함수 통과시 게임 패배로 판단 
    
    close(listen_sock);
    close(server_sock);
}
