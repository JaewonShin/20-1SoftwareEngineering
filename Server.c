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

    server_sock = socket(AF_INET, SOCK_STREAM, 0);    
    bzero((void*)&listen_addr, sizeof(listen_addr));
    
    listen_addr.sin_family = AF_INET;
    listen_addr.sin_addr.s_addr = INADDR_ANY;
    listen_addr.sin_port = htons(3456);     // port

    bind(server_sock, (struct sockaddr*)&listen_addr, sizeof(listen_addr));
    listen(server_sock, 1);

    listen_sock = accept(server_sock, NULL, NULL);
    f = fdopen(listen_sock, "r+");

    // how to pass f?    
    game_start_net();

    flag = 1;
    send(server_sock, (char*)&flag, sizeof(int), 0); //플래그값 클라이언트에게 쏴줌 
    lose_game == losemsg; //함수 통과시 게임 패배로 판단 
    fclose(f);
    close(listen_sock);
    close(server_sock);
}
