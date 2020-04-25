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

/* 타이머  */
#define CCHAR 0
#ifdef CTIME
#undef CTIME
#endif
#define CTIME 1

/* 방향키, 회전키 설정*/
#define LEFT 0
#define RIGHT 1
#define DOWN 2
#define ROTATE 3

/*블록넘버 설정*/
#define I_BLOCK 0
#define	T_BLOCK 1
#define S_BLOCK 2
#define Z_BLOCK 3
#define L_BLOCK 4
#define J_BLOCK 5
#define O_BLOCK 6

/* 게임 시작, 종료 설정*/
#define GAME_START 0
#define GAME_END 1

//블럭모양에따라 삼중배열로 선언 

char i_block[4][4][4] = //막대기 모양 블럭 
	{
			1, 1, 1, 1,    0, 0, 0, 0,    0, 0, 0, 0,    0,0,0,0,
			0, 0, 0, 1,   0, 0, 0, 1,    0, 0, 0, 1,    0,0,0,1,
			0, 0, 0, 0,    0, 0, 0, 0,   0, 0, 0, 0,   1, 1, 1, 1,
			1, 0, 0, 0,   1, 0, 0, 0,    1, 0, 0, 0,    1,0,0,0
	};

char t_block[4][4][4] = //ㅏ 모양 블럭 
	{
			1, 0, 0, 0,   1, 1, 0, 0,    1, 0, 0, 0,   0, 0, 0, 0,
			1, 1, 1, 0,   0, 1, 0, 0,   0, 0, 0, 0,    0, 0, 0, 0,
			0, 0, 1, 0,   0, 1, 1, 0,   0, 0, 1, 0,   0, 0, 0, 0,
			0, 0, 0, 0,   0, 1, 0, 0,   1, 1, 1, 0,   0, 0, 0, 0
	};

char s_block[4][4][4] = //z 모양 블럭 
	{
			1, 0, 0, 0,    1, 1, 0, 0,    0, 1, 0, 0,   0, 0, 0, 0,
			0, 1, 1, 0,    1, 1, 0, 0,    0, 0, 0, 0,   0, 0, 0, 0,
			0, 1, 0, 0,    0, 1, 1, 0,    0, 0, 1, 0,   0, 0, 0, 0,
			0, 0, 0, 0,   0, 1, 1, 0,    1, 1, 0, 0,    0, 0, 0, 0
	};

char z_block[4][4][4] = //반대모양 z 블럭 
	{
			0, 1, 0, 0,    1, 1, 0, 0,   1, 0, 0, 0,    0, 0,0, 0,
			1, 1, 0, 0,    0, 1, 1, 0,   0, 0, 0, 0,    0, 0, 0, 0,
			0, 0, 1, 0,    0, 1, 1, 0,   0, 1, 0, 0,    0, 0, 0, 0,
			0, 0, 0, 0,    1, 1, 0, 0,    0, 1, 1, 0,   0, 0, 0, 0
	};

char l_block[4][4][4] =//L 모양 블럭 
	{
			1, 0, 0, 0,    1, 0, 0, 0,    1, 1, 0, 0,   0, 0, 0, 0,
			1, 1, 1, 0,    1, 0, 0, 0,    0, 0, 0, 0,    0, 0, 0, 0,
			0, 1, 1, 0,    0, 0, 1, 0,    0, 0, 1, 0,    0, 0, 0, 0,
			0, 0, 0, 0,    0, 0, 1, 0,    1, 1, 1, 0,    0, 0, 0, 0
	};

char j_block[4][4][4] =//반대모양 L 블럭 
	{
			0, 1, 0, 0,    0, 1, 0, 0,    1, 1, 0, 0,     0, 0, 0, 0,
			1, 0, 0, 0,    1, 1, 1, 0,    0, 0, 0, 0,    0, 0, 0, 0,
			1, 1, 0, 0,    1, 0, 0, 0,    1, 0, 0, 0,    0, 0, 0, 0,
			1, 1, 1, 0,    0, 0, 1, 0,    0, 0, 0, 0,    0, 0, 0, 0
	};

char o_block[4][4][4] =//ㅁ 모양 블럭 
	{
			1, 1, 0, 0,    1, 1, 0, 0,    0, 0, 0, 0,    0, 0, 0, 0,
			1, 1, 0, 0,    1, 1, 0, 0,    0, 0, 0, 0,    0, 0, 0, 0,
			1, 1, 0, 0,    1, 1, 0, 0,    0, 0, 0, 0,    0, 0, 0, 0,
			1, 1, 0, 0,    1, 1, 0, 0,    0, 0, 0, 0,    0, 0, 0, 0,
	};

/* 테트리스 블럭이 움직일 수 있는 공간*/
char tetris_table[21][10];
/* 게임 기록이 저장되는 구조체 선언*/
static struct result
{
	char name[30];
	long point;
	int year;
	int month;
	int day;
	int hour;
	int min;
	int rank;
}temp_result;

int block_number = 0;  /*현재 블록 넘버 변수*/
int next_block_number = 0; /*다음 블록 넘버 변수 */
int block_state = 0; /*블록 상태, 회전함에 따라 변한다*/
int x = 3, y = 0; /*블록이 현재 테트리스판 어디에 위치해 있는지 알려주는 변수*/
int game = GAME_END; /*게임 상태 변수, 게임이 시작되거나 종료됨에 따라 변한다*/
int best_point = 0; /* 게임 최고 점수를 알려주는 변수*/
long point = 0; /* 현재 게임중 득점을 알려주는 변수 */
int set_ticker(int);/*타이머 등록*/
int display_menu(void); /* 메뉴를 보여줌 */
int init_tetris_table(void); /*테트리스판을 초기화 한다. 벽과 공간을 나눔*/
int display_tetris_table(void); /* 현재의 테트리스판을 보여준다. 블록이 놓이고 쌓인 현재 상태를 보여줌*/
int game_start(void); /* 게임 시작시 호출되는 함수.   game변수를 참조하여 게임을 종료하거나 시작함 . 게임 시작시 refresh()함수가 콜백함수로 설정되고 타이머를 등록함. */
int _refresh(int);/* 타이머에 콜백함수로 등록되어 계속 새로고침 하면서 호출되는 함수. 키입력 확인,  화면새로고침, 한줄완성검사등의 계속 상태가 변함을 확인해야 되는 함수를 호출한다 */
int _refresh2(int);/* 타이머에 콜백함수로 등록되어 계속 새로고침 하면서 호출되는 함수. 키입력 확인,  화면새로고침, 한줄완성검사등의 계속 상태가 변함을 확인해야 되는 함수를 호출한다 */
int move_block(int);/*이동, 회전키가 입력되면, 충돌검사후 이동시킨다*/
int drop(void);/* 충돌되기 전까지 블록을 다운시킨다.*/
int collision_test(int); /* 블록이 이동, 회전하기 전에 충돌되는 블록이나 벽이 없는지 확인하는 함수*/
int check_one_line(void);/* 한줄이 완성되었는지 확인하는 함수. 완성되면 한줄을 지우고, 점수에 1000점을 더한다*/
int print_result(void);/* 메뉴에서 기록출력시 호출되어 기록을 출력하는 함수*/
int search_result(void); /*메뉴에서 기록검색시 호출되어 기록을 검색하는 함수*/
int _getch(void);/*문자를 바로 입력 받을 수 있는 함수*/
void game_end(void);/*게임 종료시 호출되는 기록저장 및 게임오버 화면 출력하는 함수*/
void tetris(void);/*테트리스게임 중 블럭모양 TETRIS 글자를 그려주는 함수*/
void tetris2(void);/*게임 시작 시 블럭모양 TETRIS 글자를 그려주는 함수*/

int main(void)
{
	int menu = 1;
	setlocale(LC_CTYPE, "ko_KR.utf-8");//한국어 및 특수문자 출력을 위한 설정 

	while(menu)
	{
		menu = display_menu();

		if(menu == 1) //1p 게임 시작 
		{
			game = GAME_START;
			menu = game_start();
		}
		else if(menu == 2)//기록 검색 
		{
			search_result();
		}
		else if(menu == 3)//기록 출력 
		{
			print_result();
		}
		else if(menu == 4)//종료 
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
 //메인메뉴 화면 그려주기 
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
	move(11,31);	
	printw("1. Play\n");
	move(13,31);
	printw("2. Search\n");
	move(15,31);
	printw("3. Ranking\n");
	move(17,31);
	printw("4. Exit\n");
	move(22,23);
	printw("system_programming_group9");
	move(19,27);
	printw("선택 : ");

	scanw("%d",&menu); //사용자로부터 메뉴선택 입력받음 
		if(menu < 1 || menu > 5)
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
/* 게임 시작시 호출되는 함수.   game변수를 참조하여 게임을 종료하거나 시작함 . 게임 시작시 refresh()함수가 콜백함수로 설정되고 타이머를 등록함. */
int game_start(void)
{

	int _refresh(int);
	static struct sigaction sa;
	
	setlocale(LC_CTYPE, "ko_KR.utf-8"); //NCURSES.H에서 한글과 특수문자를 사용하기 위한 로케일 설정 
	if(game == GAME_START)
	{
		initscr();
		init_tetris_table();
		
		memset(&sa,0,sizeof(sa));
		sa.sa_handler = (void*)&_refresh;
		sigaction(SIGVTALRM,&sa,NULL);

		if(set_ticker(game)==-1)
			perror("set_ticker");

		while(1)
		{
			if(game == GAME_END)
			{
				game_end();
				endwin();
				return 1;
			}
		}
	}

  return 0;
}

int set_ticker(int game_state)
{
        struct itimerval new_timeset;
	
	if(game_state==GAME_START){
		new_timeset.it_interval.tv_sec  = 0;        
		new_timeset.it_interval.tv_usec = 1;      
		new_timeset.it_value.tv_sec     = 0  ;      
		new_timeset.it_value.tv_usec    = 1 ;    
	}
	else if(game_state==GAME_END){
		new_timeset.it_interval.tv_sec  = 0;        
		new_timeset.it_interval.tv_usec = 0;      
		new_timeset.it_value.tv_sec     = 0  ;      
		new_timeset.it_value.tv_usec    = 0 ;   
	}
	else return 1;

	return setitimer(ITIMER_VIRTUAL, &new_timeset, NULL); //타이머 리턴 
}
/* 현재의 테트리스판을 보여준다. 블록이 놓이고 쌓인 현재 상태를 보여줌*/
int display_tetris_table()
{
	int i, j;
	char (*block_pointer)[4][4][4] = NULL;

	switch(next_block_number)
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

	move(7,2);
	for(i = 2 ; i < 21 ; i++)
	{
		printw("\t");
		for(j = 0 ; j < 10 ; j++)
		{
			if(j == 0 || j == 9|| (i == 20 && (j > 1 || j < 8)))
			{
				printw("□ ");
			
			}
			
			else if(tetris_table[i][j] == 1)
				printw("■ ");
			else if(tetris_table[i][j] == 0)
				printw("  ");
		}
		addstr("\n");
	}
	move(0,0);
	tetris();
	
	move(6,2);
	printw("     SCORE: %ld TOP SCORE: %d",point, best_point); //현재스코어와 최고점수를 출력 
	
	move(7,8);

	for(int z=0; z<10; z++) //테트리스판뚜껑 
	printw("□ ");
	
	/*다음나올 블럭 출력 부분 */
	move(26,2);
	printw(" \n\t     Next Block\n\n\t");
	printw("□ □ □ □ □ □ □ □ □ □  \n\n");

	for(i = 0 ; i < 4 ; i++)
	{
		addstr("\n\t       ");
		for(j = 0 ; j < 4 ; j++)
		{
			
			if((*block_pointer)[0][i][j] == 1)
				printw("■ ");
			else if((*block_pointer)[0][i][j] == 0)
				printw("");
			
		}
	}	
	printw("\n\t□ □ □ □ □ □ □ □ □ □  \n");
	printw("      System_programming_Group9\n\n");	
	refresh();	

	return 0;
}

void tetris(void) //TETRIS 그림 
{
	
	printw("■ ■ ■   ■ ■ ■   ■ ■ ■   ■ ■ ■   ■   ■■ ■\n");
	printw("  ■     ■         ■     ■   ■   ■   ■  \n");
	printw("  ■     ■ ■ ■     ■     ■ ■     ■    ■■ \n");
	printw("  ■     ■         ■     ■  ■    ■      ■\n");
	printw("  ■     ■ ■ ■     ■     ■   ■   ■   ■ ■■\n");

}
void tetris2(void) //칸에 맞춰 출력하기 위한 메인화면 TETRIS 그림 
{
	move(4,13);
	printw("■ ■ ■   ■ ■ ■   ■ ■ ■   ■ ■ ■   ■   ■■ ■");
	move(5,13);
	printw("  ■     ■         ■     ■   ■   ■   ■  \n");
	move(6,13);	
	printw("  ■     ■ ■ ■     ■     ■ ■     ■    ■■ \n");
	move(7,13);	
	printw("  ■     ■         ■     ■  ■    ■      ■\n");
	move(8,13);	
	printw("  ■     ■ ■ ■     ■     ■   ■   ■   ■ ■■\n");

}
/*테트리스판을 초기화 한다. 벽과 공간을 나눔*/
int init_tetris_table(void)
{
	int i = 0, j = 0;

	for(i = 0 ; i < 20 ; i++)
		for(j = 1 ; j < 9 ; j++)
			tetris_table[i][j] = 0;

	for(i = 0 ; i < 21 ; i++)
	{
		tetris_table[i][0] = 1;
		tetris_table[i][9] = 1;
	}

	for(j = 1 ; j < 9 ; j++)
		tetris_table[20][j]= 1;

	return 0;
}

void game_end(){
	time_t ptime;
	struct tm *t;
	FILE *fp = NULL;

	if(set_ticker(game)==-1)
		perror("set_ticker");

	// 기록 저장 및 게임오버화면 출력 
	
	move(15,2);
	addch(ACS_ULCORNER);
	for(int i=0; i<10; i++)
		addch(ACS_HLINE);
	printw("GAME OVER");  
	for(int i=0; i<11; i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	
	printw("\n\n           Final Score : %ld    ", point);
	printw("\n\n          INPUT INITIAL : ");
	
 	 for(int d=0; d<6; d++){
		move(d+16,2);
		addch(ACS_VLINE);
		move(d+16,33);
		addch(ACS_VLINE);
	}
	move(21,2);
	addch(ACS_LLCORNER);
	for(int i=0; i<30; i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
	move(19,26);
	printw("     ");
	move(20,3);
	printw("                          ");
	move(19,26);
	
	
	refresh();
	
		scanw("%s%*c", temp_result.name);
	if(strlen(temp_result.name)!=3){	//이니셜 세글자가 아닌경우에 사용자로부터 한번만 다시 입력받음 	
		move(18,7);
		printw("세글자만 입력하세요");
		move(19,26);
		scanw("%s%*c", temp_result.name);		
	}
	if (!strcmp(temp_result.name, ""))
		strcpy(temp_result.name, "NULL");
	temp_result.point = point;
	//endwin();	

	if(temp_result.point >= best_point)
		best_point = temp_result.point;


	ptime = time(NULL); // 현재 시각을 초 단위로 얻기
	t = localtime(&ptime); // 초 단위의 시간을 분리하여 구조체에 넣기

	temp_result.year = t->tm_year + 1900;
	temp_result.month = t->tm_mon + 1;
	temp_result.day = t->tm_mday;
	temp_result.hour = t->tm_hour;
	temp_result.min = t->tm_min;

	fp = fopen("result.txt", "a");
	fseek(fp, 1, SEEK_END);
	//fwrite(&temp_result, sizeof(struct result), 1, fp);
	fprintf(fp, "%s %ld %d %d %d %d %d\n", temp_result.name, temp_result.point, temp_result.year, temp_result.month, temp_result.day, temp_result.hour, temp_result.min);
	fclose(fp);

	x = 3, y =0;
	point = 0;
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

/* 타이머 콜백함수*/ 
int _refresh(int signum)
{
	static int downcount = 0;
	static int setcount = 0;
	static long speedcount = 0;
	static int countrange = 5;
	static int firststart = 0;
	

	char ch;

	srand((unsigned)time(NULL));

	if(firststart == 0)
	{
		block_number= rand()%7;
		if(firststart == 0)
			firststart++;
	}
	
	
	display_tetris_table();
	check_one_line();


	if(downcount == countrange-1)
	{
		point += 1;
		move_block(DOWN);
	}

	if(speedcount == 499)
	{
		if(countrange != 1)
			countrange--;
	}

	downcount++;
	downcount %= countrange;
	speedcount++;
	speedcount %= 500;

	if(x == 3 && y == 0)
	{
		if(collision_test(LEFT) || collision_test(RIGHT) || collision_test(DOWN) || collision_test(ROTATE))
		{
			printw("\n Game End! \n");
			downcount = 0;
			setcount = 0;
			speedcount = 0;
			countrange = 5;
			firststart = 0;
			game = GAME_END;
		}
	}

	if(collision_test(DOWN))
	{
		if(setcount == 9)
		{
			block_number= next_block_number;
			next_block_number = rand()%7;
			block_state = 0;
			x = 3;
			y = 0;
		}
		setcount++;
		setcount %= 10;
	}

	ch = _getch();

	switch(ch)
	{
		case 97 :	move_block(LEFT);
					  		break;
		case 100 :	move_block(RIGHT);
						  	break;
		case 115 :	move_block(DOWN);
								break;
		case 119 :	move_block(ROTATE);
								break;
		case 106 :	move_block(LEFT);
					  		break;
		case 108 :	move_block(RIGHT);
						  	break;
		case 107 :	move_block(DOWN);
								break;
		case 105 :	move_block(ROTATE);
								break;
		case 113  :	drop();
								break;
 		case 27 :	downcount = 0;
 								setcount = 0;
 								speedcount = 0;
 								countrange = 5;
 								firststart = 0;
 								game = GAME_END;
 								break;
 		default : 	break;
	}
	return 0;
}


/*이동, 회전키가 입력되면, 충돌검사후 이동시킨다*/
int move_block(int command)
{
	int i, j;
	int newx, newy;
	int oldx, oldy;
	int old_block_state;
	char (*block_pointer)[4][4][4] = NULL;

	newx = x;
	newy = y;

	old_block_state = block_state;

	if(collision_test(command) == 0)
	{
		switch(command)
		{
			case	LEFT :	newx--;
										break;
			case	RIGHT :	newx++;
										break;
			case	DOWN :	newy++;
										break;
			case ROTATE :	block_state++;
										block_state %= 4;
										break;
		}
	}
	else
	{
		return 1;
	}

	switch(block_number)
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

	for(i = 0, oldy = y ; i < 4 ; i++, oldy++)
	{
		for(j = 0, oldx = x ; j < 4 ; j++, oldx++)
		{
			if(oldx > 0 && oldx < 9 && oldy < 20 && oldy > 0)
				if((*block_pointer)[old_block_state][i][j] == 1)
						tetris_table[oldy][oldx] = 0;

		}
	}

	x = newx;
	y = newy;

	for(i = 0, newy = y ; i < 4 ; i++, newy++)
	{
		for(j = 0, newx = x ; j < 4 ; j++, newx++)
		{
			if(newx > 0 && newx < 9 && newy < 20 && newy > 0)
				if((*block_pointer)[block_state][i][j] == 1)
					tetris_table[newy][newx] = (*block_pointer)[block_state][i][j];
		}
	}

	return 0;
}

/* 블록이 이동, 회전하기 전에 충돌되는 블록이나 벽이 없는지 확인하는 함수*/
int collision_test(int command)
{
	int i, j;
	int tempx, tempy;
	int oldx, oldy;
	int temp_block_state;
	char (*block_pointer)[4][4][4];
	char temp_tetris_table[21][10];

	oldx = tempx = x;
	oldy = tempy = y;
	temp_block_state = block_state;

	switch(command)
	{
		case	LEFT :	tempx--;
									break;
		case	RIGHT :	tempx++;
									break;
		case	DOWN :	tempy++;
									break;
		case ROTATE : temp_block_state++;
									temp_block_state %=  4;
									break;
	}

	switch(block_number)
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

	for(i = 0 ; i < 21 ; i++)
	{
		for(j = 0 ; j < 10 ; j++)
		{
			temp_tetris_table[i][j] = tetris_table[i][j];
		}
	}

	for(i = 0, oldy = y ; i < 4 ; i++, oldy++)
	{
		for(j = 0, oldx = x ; j < 4 ; j++, oldx++)
		{
			if(oldx > 0 && oldx < 9 && oldy < 20 && oldy > 0)
			{
				if((*block_pointer)[block_state][i][j] == 1)
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

/* 충돌되기 전까지 블록을 다운시킨다.*/
int drop(void)
{
	while(!collision_test(DOWN))
		move_block(DOWN);

	return 0;
}

/* 한줄이 완성되었는지 확인하는 함수. 완성되면 한줄을 지우고, 점수에 1000점을 더한다*/
int check_one_line(void)
{
	int i, j;
	int ti, tj;
	int line_hole;

	for(i = 19 ; i > 0 ; i--)
	{
		line_hole = 0;
		for(j = 1 ; j < 9 ; j++)
		{
			if(tetris_table[i][j] == 0)
			{
				line_hole = 1;
			}
		}

		if(line_hole == 0)
		{
			point += 1000;
			for(ti = i ; ti > 0 ; ti--)
			{
				for(tj = 0 ; tj < 9 ; tj++)
				{
					tetris_table[ti][tj] = tetris_table[ti-1][tj];
				}
			}
		}
	}

	return 0;
}

/* 기록을 검색하는 함수*/
int search_result(void)
{
	FILE *fp = NULL;
	char name[30];
	char ch = 1;
	int find = 0;
	setlocale(LC_CTYPE, "ko_KR.utf-8");

	fp = fopen("result.txt", "r");

	if(fp == NULL)
		return 0;

	initscr();
	clear();
	move(10,0);
	printw("\n\n\t\t기록을 찾고싶은 이니셜을 입력하세요  : ");
	scanw("%s%*c", name);
	move(10,0);
	printw("\n\t\t\t\tTetris");
	printw("\n\n\t\t\t      게임  기록\n\n");
	printw("\n       ");
	addch(ACS_ULCORNER);
		for(int i=0; i<59; i++)
			addch(ACS_HLINE);
	addch(ACS_URCORNER);
	printw("\n       ");
	addch(ACS_VLINE);
	printw("\t이름\t     점수\t   날짜\t\t 시간");
	printw("      ");
	addch(ACS_VLINE);


	while(!feof(fp))
	{
		//fread(&temp_result, sizeof(struct result), 1, fp);
		fscanf(fp, "%s %ld %d %d %d %d %d\n", temp_result.name, &temp_result.point,&temp_result.year, &temp_result.month, &temp_result.day, &temp_result.hour, &temp_result.min);
		if(!strcmp(temp_result.name, name))
		{
			find = 1;
			printw("\n       ");
			addch(ACS_LTEE);
			for(int i=0; i<59; i++)
			addch(ACS_HLINE);
			addch(ACS_RTEE);
			printw("\n       ");
			addch(ACS_VLINE);
			printw("\t%s\t       %ld\t%d. %d. %d. | %d : %d     ", temp_result.name, temp_result.point, temp_result.year, temp_result.month, temp_result.day, temp_result.hour, temp_result.min);
			addch(ACS_VLINE);
		}
	}
	printw("\n       ");
	addch(ACS_LLCORNER);
		for(int i=0; i<59; i++)
			addch(ACS_HLINE);
	addch(ACS_LRCORNER);

	if(find == 0)
		printw("\n\n\n\t\t        검색된 이름이 없습니다.");

	printw("\n\n\n\t\t      메뉴 화면으로 돌아가기 : M");

	
	printw("\n\n\t\t       system_programming_group9");
	refresh();
	while(1)
	{
		ch = _getch();
		if(ch == 77 || ch == 109)
		{
			break;
		}
	}
	return 0;
}

/* 기록을 출력하는 함수*/
int print_result(void)
{
	FILE *fp = NULL;
	char ch = 1 ;
	setlocale(LC_CTYPE, "ko_KR.utf-8");
	
	fp = fopen("result.txt", "r");

	if(fp == NULL)
		return 0;

	initscr();
	clear();
	move(2,36);
	printw("Tetris");
	move(3,35);
	printw("게임 기록");
	printw("\n       ");
	addch(ACS_ULCORNER);
		for(int i=0; i<66; i++)
			addch(ACS_HLINE);
	addch(ACS_URCORNER);
	
	move(5,5);
	printw("  ");
	addch(ACS_VLINE);
	printw("\t 이름\t\t점수\t    날짜\t 플레이시간");
	printw("   \t  ");
	addch(ACS_VLINE);

	while(!feof(fp))
	{
			fscanf(fp, "%s %ld %d %d %d %d %d\n", temp_result.name, &temp_result.point, &temp_result.year, &temp_result.month, &temp_result.day, &temp_result.hour, &temp_result.min);
			printw("\n       ");
			addch(ACS_LTEE);
			for(int i=0; i<66; i++)
				addch(ACS_HLINE);
			addch(ACS_RTEE);
			printw("\n       ");
			addch(ACS_VLINE);
			printw("  \t %s\t\t %ld\t%d. %d. %d       %d : %d \t  ", temp_result.name, temp_result.point, temp_result.year, temp_result.month, temp_result.day, temp_result.hour, temp_result.min);
			addch(ACS_VLINE);
	}

	fclose(fp);
	printw("\n       ");
	addch(ACS_LLCORNER);
	for(int i=0; i<66; i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
	
	
	printw("\n\n\t\t\t   메뉴 화면으로 돌아가기 : M");

	
	printw("\n\n\t\t\t   system_programming_group9");
	refresh();
	while(1)
	{
		ch = _getch();
		if(ch == 77 || ch == 109)
			break;
	}
	endwin();
	return 1;

}