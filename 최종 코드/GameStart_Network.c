#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <time.h>
#include <locale.h>
#include <ncursesw/curses.h> //NCURSES.h
#include <pthread.h>
#define PORT 12345

#include "GameStart_Network.h"
#include "Client.h"
#include "Server.h"

extern char i_block;
extern char t_block;
extern char s_block;
extern char z_block;
extern char j_block;
extern char o_block;
extern char l_block;

int block_number3 = 0;  /*현재 블록 넘버 변수*/
int next_block_number3 = 0; /*다음 블록 넘버 변수 */
int block_state3 = 0; /*블록 상태, 회전함에 따라 변한다*/
int x3 = 3, y3 = 0; /*블록이 현재 테트리스판 어디에 위치해 있는지 알려주는 변수*/
int best_point3 = 0; /* 게임 최고 점수를 알려주는 변수*/
long point3 = 0; /* 현재 게임중 득점을 알려주는 변수 */

/* 테트리스 블럭이 움직일 수 있는 공간*/
char tetris_table3[21][10];
char tetris_table4[21][10];

struct sockaddr_in listen_addr, connect_addr, client_addr;
int listen_sock, server_sock, client_sock; //통신관련 변수 선언 
int lo= GAME_END;
char ip[20];
int flag = 0;
int host_net;
int lose_game;

/* 현재의 테트리스판을 보여준다. 블록이 놓이고 쌓인 현재 상태를 보여줌*/
int display_tetris_table_net(int speed, int host) {
	int i, j, yy;
	char (*block_pointer)[4][4][4] = NULL;
	char (*block_pointer2)[4][4][4] = NULL;
	char block_pointer3[4][4][4];
	switch(next_block_number3)
	{
		case I_BLOCK :	block_pointer = &i_block; break;
		case T_BLOCK :	block_pointer = &t_block; break;
		case S_BLOCK :  block_pointer = &s_block; break;
		case Z_BLOCK : 	block_pointer = &z_block; break;
		case L_BLOCK : 	block_pointer = &l_block; break;
		case J_BLOCK : 	block_pointer = &j_block; break;
		case O_BLOCK :	block_pointer = &o_block; break;
	}
	
	clear();

	yy = 7;
	for(i = 2 ; i < 21 ; i++) {
		move(yy,8);
		
		for(j = 0 ; j < 10 ; j++) {
			if(j == 0 || j == 9|| (i == 20 && (j > 1 || j < 8))) {
				printw("□ ");
			
			}
			
			else if(tetris_table4[i][j] == 1)
				printw("■ ");
			else if(tetris_table4[i][j] == 0)
				printw("  ");
		}
		yy++;
	}
	
	move(0,0);
	tetris();
	
	move(6,2);
	printw("      SPEED: %d SCORE: %d ",speed,point3); //현재스코어 출력 1p
	move(7,8);

	for(int z=0; z<10; z++) //테트리스판뚜껑 
	printw("□ ");
	
	/*다음나올 블럭 출력 부분 */
	attron(A_BOLD);
	attron(COLOR_PAIR(2));
	move(26,2);
	printw(" \t□    \t 1P  \t  □");
	move(27,2);
	printw("\t□ □ □ □ □ □ □ □ □ □ ");
        move(28,2);
	addstr("\t□    NEXT BLOCK   □ ");
   	attroff(COLOR_PAIR(1));
	
	move(29,2);
	printw("\t□ □ □ □ □ □ □ □ □ □ ");
	yy = 32;
	for(i = 0 ; i < 4 ; i++)
	{
		for(int p=30; p<37; p++){ 
			move(p,8);
			printw("□ ");
			move(p,26);
			printw("□ ");
		}
		move(yy,14);
		//printw("\t       ");
		for(j = 0 ; j < 4 ; j++)
		{
			
			if((*block_pointer)[0][i][j] == 1)
				printw("■ ");
			else if((*block_pointer)[0][i][j] == 0)
				printw("  ");
			
		}
		yy++;
	}	
	move(37,2);
	printw("\t□ □ □ □ □ □ □ □ □ □ ");	

	move(0,0);
	tetris2p();	//테트리스 그림
	char pt_char[50];
	int point_buf,point_buf2;
	int recv_cnt, recv_cnt2;
	if(host_net ==1) { //호스트인경우			
		send(server_sock,(char*)&flag,sizeof(int),0);
		recv(server_sock,&flag, sizeof(int), 0);
		if(flag == 1){ //중간에 flag 1을 받으면 상대방이 져서 먼저 함수를 통과한것 
			lose_game = winmsg; //이기 때문에 이긴것이라 판단하고 소켓 닫음 
			close(server_sock);	
			close(listen_sock);
		}
		int point_buf2 =0;
		send(server_sock, tetris_table4, sizeof(tetris_table4), 0);
		recv_cnt = recv(server_sock,&tetris_table3,sizeof(tetris_table3),0);
		if(recv_cnt <0)
			game= win2P;
		
		yy = 7;
		move(yy,32);
		for(i = 2 ; i < 21 ; i++) {
		move(yy,32);
			printw("\t");
			for(j = 0 ; j < 10 ; j++) {
				if(j == 0 || j == 9|| (i == 20 && (j > 1 || j < 8))) {
					printw("□ ");
				}
					
				else if(tetris_table3[i][j] == 1)
					printw("■ ");
				else if(tetris_table3[i][j] == 0)
					printw("  ");
			}
			yy++;
		}
		move(6,34);
					
		send(server_sock,(char*)&point3,sizeof(int),0);
		recv_cnt = recv(server_sock,&point_buf2, sizeof(int), 0);
		if(recv_cnt <0)
			game= win2P;
		printw("      SPEED: %d SCORE: %d ",speed,point_buf2); //2p스코어 출력 
		
		move(7,40);
		for(int z=0; z<10; z++) //테트리스판뚜껑 
		printw("□ ");
		yy = 32;
		for(int p=30; p<37; p++){ 
			move(p,40);
			printw("□ ");
			move(p,58);
			printw("□ ");
		}
		
		send(server_sock, *block_pointer, sizeof(*block_pointer), 0);
		recv_cnt = recv(server_sock,&block_pointer3,sizeof(block_pointer3),0);
		if(recv_cnt <0)
			game= win2P;

		for(i = 0 ; i < 4 ; i++)
		{
			for(int p=30; p<37; p++){ 
			move(p,40);
			printw("□ ");
			move(p,58);
			printw("□ ");
			}
		move(yy,46);

		
			for(j = 0 ; j < 4 ; j++) {				
				if(block_pointer3[0][i][j] == 1)
					printw("■ ");
				else if(block_pointer3[0][i][j] == 0)
					printw("  ");
			}
		yy++;
		}	
	}

	if(host_net==2) { //클라이언트인경우
		recv(client_sock,&flag,sizeof(int),0);
		send(client_sock,(char*)&flag, sizeof(int), 0);
		if(flag ==1){ //중간에 끊기는 경우 이긴것으로 판단 
			lose_game = winmsg;
			close(client_sock);		
		}
		int point_buf =0;
		recv_cnt2 = recv(client_sock,&tetris_table3,sizeof(tetris_table3),0);
		if(recv_cnt2 <0)
			game= win2P;
		send(client_sock, tetris_table4, sizeof(tetris_table4), 0);
		yy = 7;

		move(yy,32);
		for(i = 2 ; i < 21 ; i++)
		{
		move(yy,32);
			printw("\t");
				for(j = 0 ; j < 10 ; j++)
			{
				if(j == 0 || j == 9|| (i == 20 && (j > 1 || j < 8)))
				{
					printw("□ ");
				
				}
				
				else if(tetris_table3[i][j] == 1)
					printw("■ ");
				else if(tetris_table3[i][j] == 0)
					printw("  ");
			}
			yy++;
		}
		move(6,34);

		recv_cnt2 = recv(client_sock,&point_buf,sizeof(int),0);
	
		if (recv_cnt2 < 0) {
			game = win2P;
		}		

		send(client_sock,(char*)&point3, sizeof(int), 0);
		printw("      SPEED: %d SCORE: %d ",speed,point_buf); 
		move(7,40);
		for(int z=0; z<10; z++) //테트리스판뚜껑 
		printw("□ ");
		yy = 32;
		for(int p=30; p<37; p++){ 
			move(p,40);
			printw("□ ");
			move(p,58);
			printw("□ ");
		}

		recv_cnt2 = recv(client_sock,&block_pointer3,sizeof(block_pointer3),0);
		if(recv_cnt2 <0)
			game= win2P;
		send(client_sock, *block_pointer, sizeof(*block_pointer), 0);

		for(i = 0 ; i < 4 ; i++)
		{
			for(int p=30; p<37; p++){ 
			move(p,40);
			printw("□ ");
			move(p,58);
			printw("□ ");
			}
		move(yy,46);

		
			for(j = 0 ; j < 4 ; j++)
			{
			
			if(block_pointer3[0][i][j] == 1)
				printw("■ ");
			else if(block_pointer3[0][i][j] == 0)
				printw("  ");
			
			}
		yy++;
		}
	}

	attron(A_BOLD);
	attron(COLOR_PAIR(2));
	move(26,32);
	printw(" \t□    \t 2P  \t  □");
	move(27,32);
	printw("\t□ □ □ □ □ □ □ □ □ □ ");
		 move(28,32);
	addstr("\t□    NEXT BLOCK   □ ");
	attroff(COLOR_PAIR(1));
	move(29,32);
	printw("\t□ □ □ □ □ □ □ □ □ □ ");
	move(37,32);
	printw("\t□ □ □ □ □ □ □ □ □ □ ");
	
	move(42,25);
	printw("네트워크 대전 모드");
	curs_set(0); //커서 종료 
	move(45,99);
	refresh();
	move(45,99);
	return 0;
}

/* 게임 시작시 호출되는 함수.   game변수를 참조하여 게임을 종료하거나 시작함 . 게임 시작시 refresh()함수가 콜백함수로 설정되고 타이머를 등록함. */
int game_start_net(void) {
	int _refresh_net(int);
	static struct sigaction sa;
	
	setlocale(LC_CTYPE, "ko_KR.utf-8"); //NCURSES.H에서 한글과 특수문자를 사용하기 위한 로케일 설정 
	
	if(game == GAME_START)
	{
		initscr();
		init_tetris_table_net();
		
		memset(&sa,0,sizeof(sa));
		sa.sa_handler = (void*)&_refresh_net;
		sigaction(SIGVTALRM,&sa,NULL);

		if(set_ticker(game)==-1)
			perror("set_ticker");

		while(1) {
			if(game == win1P && lose_game==0)
			{
				game_end_net(win1P); 
				endwin();
				return 1;
			}
			else if(game == win2P && lose_game ==0)
			{
				game_end_net(win2P);  
				endwin();
				return 1;
			}
			else if(game ==win1P && lose_game == winmsg) 
			{	
				game_end_net(winmsg);
				endwin();
				return 1;
			}
			else if(game ==win1P && lose_game ==losemsg)
			{	
				game_end_net(losemsg);
				endwin();
				return 1;
			}
			else if(game == win2P &&lose_game ==winmsg)
			{	
				game_end_net(winmsg);
				endwin();
				return 1;
			}
			else if(game == win2P &&lose_game ==losemsg)
			{	
				game_end_net(losemsg);
				endwin();
				return 1;
			}
		}
	}
  return 0;
}
int init_tetris_table_net(void)
{
	int i,j;

	for(i = 0 ; i < 21 ; i++){
		for(j = 0 ; j < 10 ; j++){
			if(j==0 || j==9||i==20){
				tetris_table3[i][j] = 1;
				tetris_table4[i][j] = 1;
			}
			else{
				tetris_table3[i][j] = 0;
				tetris_table4[i][j] = 0;
			}
		}
	}
	return 0;
}
/*이동, 회전키가 입력되면, 충돌검사후 이동시킨다*/
int move_block_net(int command)
{
	int i, j;
	int newx, newy;
	int oldx, oldy;
	int old_block_state;
	char (*block_pointer)[4][4][4] = NULL;

	newx = x3;
	newy = y3;

	old_block_state = block_state3;

	if(collision_test_net(command) == 0)
	{
		switch(command)
		{
			case	LEFT :	newx--;
										break;
			case	RIGHT :	newx++;
										break;
			case	DOWN :	newy++;
										break;
			case ROTATE :	block_state3++;
										block_state3 %= 4;
										break;
		}
	}
	else
	{
		return 1;
	}

	switch(block_number3)
	{
		case I_BLOCK :	block_pointer = &i_block;
								  	break;
		case T_BLOCK :	block_pointer = &t_block;
										break;
		case S_BLOCK :  block_pointer = &s_block;
										break;
		case Z_BLOCK : 	block_pointer = &z_block;
										break;
		case L_BLOCK : 	block_pointer = &l_block;
										break;
		case J_BLOCK : 	block_pointer = &j_block;
										break;
		case O_BLOCK :	block_pointer = &o_block;
										break;
	}

	for(i = 0, oldy = y3 ; i < 4 ; i++, oldy++)
	{
		for(j = 0, oldx = x3 ; j < 4 ; j++, oldx++)
		{
			if(oldx > 0 && oldx < 9 && oldy < 20 && oldy > 0)
				if((*block_pointer)[old_block_state][i][j] == 1)
						tetris_table4[oldy][oldx] = 0;

		}
	}

	x3 = newx;
	y3 = newy;

	for(i = 0, newy = y3 ; i < 4 ; i++, newy++)
	{
		for(j = 0, newx = x3 ; j < 4 ; j++, newx++)
		{
			if(newx > 0 && newx < 9 && newy < 20 && newy > 0)
				if((*block_pointer)[block_state3][i][j] == 1)
					tetris_table4[newy][newx] = (*block_pointer)[block_state3][i][j];
		}
	}

	return 0;
}
int collision_test_net(int command)
{
	int i, j;
	int tempx, tempy;
	int oldx, oldy;
	int temp_block_state;
	char (*block_pointer)[4][4][4];
	char temp_tetris_table[21][10];

	oldx = tempx = x3;
	oldy = tempy = y3;
	temp_block_state = block_state3;

	switch(command)
	{
		case	LEFT :	tempx--; break;
		case	RIGHT :	tempx++; break;
		case	DOWN :	tempy++; break;
		case	ROTATE :
				temp_block_state++; temp_block_state %=  4; break;
	}

	switch(block_number3)
	{
		case I_BLOCK :	block_pointer = &i_block; break;
		case T_BLOCK :	block_pointer = &t_block; break;
		case S_BLOCK :  block_pointer = &s_block; break;
		case Z_BLOCK : 	block_pointer = &z_block; break;
		case L_BLOCK : 	block_pointer = &l_block; break;
		case J_BLOCK : 	block_pointer = &j_block; break;
		case O_BLOCK :	block_pointer = &o_block; break;
	}

	for(i = 0 ; i < 21 ; i++)
	{
		for(j = 0 ; j < 10 ; j++)
		{
			temp_tetris_table[i][j] = tetris_table4[i][j]; //테이블 복사
		}
	}

	for(i = 0, oldy = y3 ; i < 4 ; i++, oldy++)
	{
		for(j = 0, oldx = x3 ; j < 4 ; j++, oldx++)
		{
			if(oldx > 0 && oldx < 9 && oldy < 20 && oldy > 0)
			{
				if((*block_pointer)[block_state3][i][j] == 1)
						temp_tetris_table[oldy][oldx] = 0;
			}
		}
	}

	for(i = 0 ; i < 4 ; i++)
	{
		for(j = 0 ; j < 4 ; j++)
		{

			if(temp_tetris_table[tempy+i][tempx+j] == 1 && (*block_pointer)[temp_block_state][i][j] == 1)
					return 1;
		}
	}

	return 0;
}
int drop_net(void)
{
	while(!collision_test_net(DOWN))
		move_block_net(DOWN);

	return 0;
}
int check_one_line_net(void)
{
	int i, j;
	int ni, nj;
	int line_hole;

	for(i = 19 ; i > 0 ; i--)
	{
		line_hole = 0;
		for(j = 1 ; j < 9 ; j++)
		{
			if(tetris_table4[i][j] == 0)
			{
				line_hole = 1;
				break;
			}
		}

		if(line_hole == 0)
		{
			point3 += 100;
			for(ni = i ; ni > 0 ; ni--)
			{
				for(nj = 0 ; nj < 9 ; nj++)
				{
					tetris_table4[ni][nj] = tetris_table4[ni-1][nj];
				}
			}
			i += 1;
		}
	}

	return 0;
}
void tetris2(void) { //칸에 맞춰 출력하기 위한 메인화면 TETRIS 그림 
	move(4,14);
	printw("■ ■ ■   ■ ■ ■   ■ ■ ■   ■ ■ ■   ■   ■■ ■");
	move(5,14);
	printw("  ■     ■         ■     ■   ■   ■   ■  \n");
	move(6,14);	
	printw("  ■     ■ ■ ■     ■     ■ ■     ■    ■■ \n");
	move(7,14);	
	printw("  ■     ■         ■     ■  ■    ■      ■\n");
	move(8,14);	
	printw("  ■     ■ ■ ■     ■     ■   ■   ■   ■ ■■\n");
}

void game_end_net(int aword) {
	if(set_ticker(game)==-1)
		perror("set_ticker");

	// 기록 저장 및 게임오버화면 출력 	
	if(aword==3) {
		move(15,2);
		addch(ACS_ULCORNER);
		for(int i=0; i<30; i++)
			addch(ACS_HLINE);
		printw("GAME OVER");  
		for(int i=0; i<30; i++)
			addch(ACS_HLINE);
		addch(ACS_URCORNER);
		
		printw("                              \n");
		printw("\t\t\t\t  YOU WIN!");
		printw("           \n                                                               ");
		printw("\n\t\t\t      CONGRATURATION!");
		printw("\n                                                               ");
		 for(int d=0; d<6; d++){
			move(d+16,2);
			addch(ACS_VLINE);
			move(d+16,72);
			addch(ACS_VLINE);
		}
		move(21,2);
		addch(ACS_LLCORNER);
		for(int i=0; i<69; i++)
			addch(ACS_HLINE);
		addch(ACS_LRCORNER);
		move(99,99);
		refresh();
		x3 = 3, y3 =0;
		point3 = 0;
		point3 = 0;
		sleep(2);
	}
	if(aword==4)
	{
		move(15,2);
		addch(ACS_ULCORNER);
		for(int i=0; i<30; i++)
			addch(ACS_HLINE);
		printw("GAME OVER");  
		for(int i=0; i<30; i++)
			addch(ACS_HLINE);
		addch(ACS_URCORNER);
		printw("\n\n\t\t\t\t YOU LOSE!");
		printw("\n                                                               ");
		printw("\n\t\t\t       YOU DEFEATED");
		printw("\n                                                               ");
		 for(int d=0; d<6; d++){
			move(d+16,2);
			addch(ACS_VLINE);
			move(d+16,72);
			addch(ACS_VLINE);
		}
		move(21,2);
		addch(ACS_LLCORNER);
		for(int i=0; i<69; i++)
			addch(ACS_HLINE);
		addch(ACS_LRCORNER);
		move(99,99);
		refresh();
		x3 = 3, y3 =0;
		point3 = 0;
		point3 = 0;
		sleep(2);
	}
}

/* 타이머 콜백함수*/ 
int _refresh_net(int signum) {
	static int downcount = 0;
	static int setcount = 0;
	static long speedcount = 0;
	static int speed = 5;
	static int firststart = 0;
	static int downcount2 = 0;
	static int setcount2 = 0;
	static long speedcount2 = 0;
	static int speed2 = 5;
	static int firststart2 = 0;

	char ch;

	srand((unsigned)time(NULL));

	if(firststart == 0)
	{
		block_number3= rand()%7;
		//if(firststart == 0)
			firststart++;
	}
	
	display_tetris_table_net(speed, host_net);
	check_one_line_net();

	if(downcount == speed-1)
	{
		point3 += 1;
		move_block_net(DOWN);
	}
	if(speedcount == 499)
	{
		if(speed != 1)
			speed--;
	}
	downcount++;
	downcount %= speed;
	speedcount++;
	speedcount %= 500;

	if(x3 == 3 && y3 == 0) {
		if(collision_test_net(LEFT) || collision_test_net(RIGHT) || collision_test_net(DOWN) || collision_test_net(ROTATE))
		{
			//printw("\n Game End! \n");
			downcount = 0;
			setcount = 0;
			speedcount = 0;
			speed = 5;
			firststart = 0;
			game = win2P;
			lose_game = losemsg;
		}
	}

	if(collision_test_net(DOWN)) {
		if(setcount == 9)
		{
			block_number3= next_block_number3;
			next_block_number3 = rand()%7;
			block_state3 = 0;
			x3 = 3;
			y3 = 0;
		}
		setcount++;
		setcount %= 10;
	}

	ch = _getch();
	switch(ch) {
		case 97 :	move_block_net(LEFT);
					  		break;
		case 100 :	move_block_net(RIGHT);
						  	break;
		case 115 :	move_block_net(DOWN);
								break;
		case 119 :	move_block_net(ROTATE);
								break;
		case 113  :	drop_net();
								break;
 		case 27 :	downcount = 0; //엔터로 quit
 								setcount = 0;
 								speedcount = 0;
 								speed = 5;
 								firststart = 0;
 								break;
 		default : 	break;
	}
	return 0;
}
