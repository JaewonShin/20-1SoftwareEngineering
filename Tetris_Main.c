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
#include "Tetris_GameStart.h"


int game = GAME_END; /*게임 상태 변수, 게임이 시작되거나 종료됨에 따라 변한다*/

int display_menu(void); /* 메뉴를 보여줌 */

int main(void)
{
	int menu = 1;

	while(1)
	{
		menu = display_menu();

		if(menu == 1)
		{
			game = GAME_START;
			game_start();
		}
		else if(menu == 2)
		{
			search_result();
		}
		else if(menu == 3)
		{
			print_result();
		}
		else if(menu == 4)
		{
			system("clear");
			exit(0);
		}
	}

	return 0;
}

/* 메뉴를 보여줌 */
int display_menu(void)
{
	int menu = 0;

	while(1)
	{
		initscr();
		clear();
		move(3,10);
		addch(ACS_ULCORNER);
		for(int i=0; i<48; i++)
			addch(ACS_HLINE);
		addch(ACS_URCORNER);
		move(4,10);
		addch(ACS_VLINE);
		move(4,59);
		addch(ACS_VLINE);
		move(5,10);
		addch(ACS_VLINE);
		move(5,0);
		tetris2(); //네모칸사이에 TETRIS 그림 그려주기 
		move(5,59);
		addch(ACS_VLINE);
		move(6,10);
		addch(ACS_VLINE);
		move(6,59);
		addch(ACS_VLINE);
		move(7,10);
		addch(ACS_VLINE);
		move(7,59);
		addch(ACS_VLINE);
	  	move(8,10);
		addch(ACS_VLINE);
		move(8,59);
		addch(ACS_VLINE);
	  	move(9,10);
		addch(ACS_LLCORNER);

		for(int i=0; i<48; i++)
			addch(ACS_HLINE);

		addch(ACS_LRCORNER);
		move(11,30);	
		printw("1. 혼자하기\n");
		move(13,30);
		printw("2. 같이하기\n");
		move(15,30);
		printw("3. 네트워크 대전\n");
		move(17,30);
		printw("4. 순위검색\n");
		move(19,30);
		printw("5. 순위보기 \n");
		move(21,30);
		printw("6. 끝내기\n");
		
		move(27,25);
		printw("v3.0 2020-1 KNU project");

		move(23,30);
		addch(ACS_ULCORNER);
		for(int i=0; i<10; i++)
			addch(ACS_HLINE);
		addch(ACS_URCORNER);
		move(24,30);
		addch(ACS_VLINE);
		
		move(24,41);
		addch(ACS_VLINE);
		move(25,30);
		addch(ACS_LLCORNER);
		for(int i=0; i<10; i++)
			addch(ACS_HLINE);
		addch(ACS_LRCORNER);
		
		move(24,31);	
		printw(" 선택 : ");
	
	
	

		scanf("%d",&menu);
		if(menu < 1 || menu > 4)
		{
			continue;
		}
		else
		{
			return menu;
		}
	}
	return 0;
}
