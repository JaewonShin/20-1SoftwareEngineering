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
#include<locale.h>
#include<ncursesw/curses.h>
#include "GameStart2P.h"
#include "GameStart.h"



int game = GAME_END; /*게임 상태 변수, 게임이 시작되거나 종료됨에 따라 변한다*/
char i_block[4][4][4] =
	{
			{0, 0, 0, 0,
			 0, 0, 0, 0,
			 1, 1, 1, 1,
			 0, 0, 0, 0},

			{0, 0, 1, 0,
			 0, 0, 1, 0,
			 0, 0, 1, 0,
			 0, 0, 1, 0},

			{0, 0, 0, 0,
			 0, 0, 0, 0,
			 1, 1, 1, 1,   
			 0, 0, 0, 0},

			{0, 0, 1, 0,
			 0, 0, 1, 0,
			 0, 0, 1, 0,
			 0, 0, 1, 0}
	};

char t_block[4][4][4] =

	{
			{0, 1, 0, 0,
			 0, 1, 1, 0,
			 0, 1, 0, 0,
			 0, 0, 0, 0},

			{0, 0, 0, 0,
			 1, 1, 1, 0,
			 0, 1, 0, 0,
			 0, 0, 0, 0},

			{0, 1, 0, 0,
			 1, 1, 0, 0,
			 0, 1, 0, 0,
			 0, 0, 0, 0},

			{0, 1, 0, 0,
			 1, 1, 1, 0,
			 0, 0, 0, 0,
			 0, 0, 0, 0}
	};

char s_block[4][4][4] =
	{
			{1, 0, 0, 0,
			 1, 1, 0, 0,
			 0, 1, 0, 0,
			 0, 0, 0, 0},

			{0, 1, 1, 0,
			 1, 1, 0, 0,
			 0, 0, 0, 0,
			 0, 0, 0, 0},

			{0, 1, 0, 0,
			 0, 1, 1, 0,
			 0, 0, 1, 0,
			 0, 0, 0, 0},

			{0, 0, 0, 0,
			 0, 1, 1, 0,
			 1, 1, 0, 0,
			 0, 0, 0, 0}
	};

char z_block[4][4][4] =
	{
			{0, 1, 0, 0,
			 1, 1, 0, 0,
			 1, 0, 0, 0,
			 0, 0,0, 0},

			{1, 1, 0, 0,
			 0, 1, 1, 0,
			 0, 0, 0, 0,
			 0, 0, 0, 0},

			{0, 0, 1, 0,
			 0, 1, 1, 0,
			 0, 1, 0, 0,
			 0, 0, 0, 0},

			{0, 0, 0, 0,
			 1, 1, 0, 0,
			 0, 1, 1, 0,
			 0, 0, 0, 0}
	};

char l_block[4][4][4] =
	{
			{0,1, 0, 0, 
			 0,1, 0, 0, 
			 0,1, 1, 0, 
			 0,0, 0, 0},

			{0, 0, 0, 0,
			 1, 1, 1, 0,
			 1, 0, 0, 0,
			 0, 0, 0, 0},

			{1, 1, 0,0,
			 0, 1, 0,0,
			 0, 1, 0,0,
			 0, 0, 0,0},

			{0, 0, 1, 0,
			 1, 1, 1, 0,
			 0, 0, 0, 0,
			 0, 0, 0, 0}
	};

char j_block[4][4][4] =
	{
			{0, 1, 0, 0,
			 0, 1, 0, 0,
			 1, 1, 0, 0,
			 0, 0, 0, 0},

			{1, 0, 0, 0,
			 1, 1, 1, 0,
			 0, 0, 0, 0,
			 0, 0, 0, 0},

			{0,1, 1, 0, 
			 0,1, 0, 0, 
			 0,1, 0, 0, 
			 0,0, 0, 0},

			{0, 0, 0, 0,
			 1, 1, 1, 0,
			 0, 0, 1, 0,
			 0, 0, 0, 0}
	};

char o_block[4][4][4] =
	{
			{1, 1, 0, 0,
			 1, 1, 0, 0,
			 0, 0, 0, 0,
			 0, 0, 0, 0},

			{1, 1, 0, 0,
			 1, 1, 0, 0,
			 0, 0, 0, 0,
			 0, 0, 0, 0},

			{1, 1, 0, 0,
			 1, 1, 0, 0,
			 0, 0, 0, 0,
			 0, 0, 0, 0},

			{1, 1, 0, 0,
			 1, 1, 0, 0,
			 0, 0, 0, 0,
			 0, 0, 0, 0}
	};
int display_menu(void); /* 메뉴를 보여줌 */
void tetris_plate(void);/*게임 시작 시 블럭모양 TETRIS 글자를 그려주는 함수*/


int main(void)
{
	int menu = 1;
	setlocale(LC_CTYPE, "ko_KR.utf-8");

	while(menu)
	{
		menu = display_menu();

		if(menu == 1)
		{
			game = GAME_START;
			menu = game_start();
		}
		else if(menu == 2)
		{
			game = GAME_START;
			menu = game_start_2p();
		}
		else if(menu == 4)
		{
			search_result();
		}
		else if(menu == 5)
		{
			print_result();
		}
		else if(menu == 6)
		{
			clear();
			endwin();
			exit(0);
		}
	}

	return 0;
}

/* 메뉴를 보여줌 */
int display_menu(void)
{
	int menu = 0;
	curs_set(1);
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
		tetris_plate(); //네모칸사이에 TETRIS 그림 그려주기 
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
	
	
	

		scanw("%d",&menu);
		if(menu != 4) /*검색시에만 커서 살려두기*/
			curs_set(0);
		if(menu < 1 || menu > 6)
		{
			endwin();
			continue;
		}
		else
		{
			endwin();
			return menu;
		}
	}
	return 0;
}

/*문자를 바로 입력 받을 수 있는 함수*/
int _getch(void)
{
             char   ch;
             int   error;
             static struct termios Otty, Ntty;

             fflush(stdout);
             tcgetattr(0, &Otty);
             Ntty = Otty;
             Ntty.c_iflag  =  0;
             Ntty.c_oflag  =  0;
             Ntty.c_lflag &= ~ICANON;
#if 1
            Ntty.c_lflag &= ~ECHO;
#else
            Ntty.c_lflag |=  ECHO;
#endif
            Ntty.c_cc[VMIN]  = CCHAR;
            Ntty.c_cc[VTIME] = CTIME;

#if 1
#define FLAG TCSAFLUSH
#else
#define FLAG TCSANOW
#endif

            if (0 == (error = tcsetattr(0, FLAG, &Ntty)))
            {
                       error  = read(0, &ch, 1 );
                       error += tcsetattr(0, FLAG, &Otty);
            }

            return (error == 1 ? (int) ch : -1 );
}


int set_ticker(int game_state)
{
        struct itimerval new_timeset;
	
	if(game_state==GAME_START){
		new_timeset.it_interval.tv_sec  = 0;        /* set reload  */
		new_timeset.it_interval.tv_usec = 1;      /* new ticker value */
		new_timeset.it_value.tv_sec     = 0  ;      /* store this   */
		new_timeset.it_value.tv_usec    = 1 ;     /* and this     */
	}
	else if(game_state==GAME_END){
		new_timeset.it_interval.tv_sec  = 0;        /* set reload  */
		new_timeset.it_interval.tv_usec = 0;      /* new ticker value */
		new_timeset.it_value.tv_sec     = 0  ;      /* store this   */
		new_timeset.it_value.tv_usec    = 0 ;     /* and this     */
	}
	else return 1;

	return setitimer(ITIMER_VIRTUAL, &new_timeset, NULL);
}


///

void tetris_plate(void) //칸에 맞춰 출력하기 위한 메인화면 TETRIS 그림 
{
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
