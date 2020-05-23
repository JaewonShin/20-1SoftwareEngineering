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


#include "GameStart1P.h"
char tetris_table[21][10];

extern char i_block;
extern char t_block;
extern char s_block;
extern char z_block;
extern char j_block;
extern char o_block;
extern char l_block;

int block_number = 0;  /*현재 블록 넘버 변수*/
int next_block_number = 0; /*다음 블록 넘버 변수 */
int block_state = 0; /*블록 상태, 회전함에 따라 변한다*/
int x = 3, y = 0; /*블록이 현재 테트리스판 어디에 위치해 있는지 알려주는 변수*/
int best_point = 0; /* 게임 최고 점수를 알려주는 변수*/
long point = 0; /* 현재 게임중 득점을 알려주는 변수 */

/* 게임 시작시 호출되는 함수.   game변수를 참조하여 게임을 종료하거나 시작함 . 게임 시작시 refresh()함수가 콜백함수로 설정되고 타이머를 등록함. */
int game_start(void)
{
	int _refresh(int);
	setlocale(LC_CTYPE, "ko_KR.utf-8");
	if(game == GAME_START)
	{
		initscr();
		clear();
		init_tetris_table();

		signal(SIGVTALRM,_refresh);
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
void tetris(void) //TETRIS 그림 
{
	
	printw("          ■ ■ ■   ■ ■ ■   ■ ■ ■   ■ ■ ■   ■   ■■ ■\n");
	printw("            ■     ■         ■     ■   ■   ■   ■  \n");
	printw("            ■     ■ ■ ■     ■     ■ ■     ■    ■■ \n");
	printw("            ■     ■         ■     ■  ■    ■      ■\n");
	printw("            ■     ■ ■ ■     ■     ■   ■   ■   ■ ■■\n");

}
/*테트리스판을 초기화 한다. 벽과 공간을 나눔*/
int init_tetris_table(void)
{
	int i,j;

	for(i = 0 ; i < 20 ; i++)
		for(j = 1 ; j < 9 ; j++)
			tetris_table[i][j] = 0;

	for(i = 0 ; i < 21 ; i++)
	{
		tetris_table[i][0] = 1;
		tetris_table[i][9] = 1;
	}

	for(i = 1 ; i < 9 ; i++)
		tetris_table[20][i]= 1;

	return 0;
}





/* 타이머에 콜백함수로 등록되어 계속 새로고침 하면서 호출되는 함수. 키입력 확인,  화면새로고침, 한줄완성검사등의 계속 상태가 변함을 확인해야 되는 함수를 호출한다 */
int _refresh(int signum)
{
	static int downcount = 0;
	static int setcount = 0;
	static long speedcount = 0;
	static int speed = 5;
	static int firststart = 0;

	char ch;

	srand((unsigned)time(NULL));

	if(firststart == 0)
	{
		block_number= rand()%7;
		firststart++;
	}

//	printw("\n Score : %ld | Speed : %d | Best Score : %d", point, speed, best_point);
//	refresh();	

	display_tetris_table(speed);
//	check_one_line();

//	printw("\n Stop Game : P");
	refresh();

	if(downcount == speed-1)
	{
		point += 1;
		move_block(DOWN);
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

	if(x == 3 && y == 0)
	{
		if(collision_test(LEFT) || collision_test(RIGHT) || collision_test(DOWN) || collision_test(ROTATE))
		{

			refresh();
			downcount = 0;
			setcount = 0;
			speedcount = 0;
			speed = 5;
			firststart = 0;
			game = GAME_END;
		}
	}

	if(collision_test(DOWN))
	{
		if(setcount == 9)
		{
			check_one_line();
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
		case 'a' :	move_block(LEFT);
					  		break;
		case 100 :	move_block(RIGHT);
						  	break;
		case 115 :	move_block(DOWN);
								break;
		case 119 :	move_block(ROTATE);
								break;
		case 113  :	drop();
								break;
 		case 80  :
 		case 27 :	downcount = 0;
 								setcount = 0;
 								speedcount = 0;
 								speed = 5;
 								firststart = 0;
 								game = GAME_END;
 								break;
 		default : 	break;
	}
	return 0;
}

/* 현재의 테트리스판을 보여준다. 블록이 놓이고 쌓인 현재 상태를 보여줌*/
int display_tetris_table(int speed)
{
	int i, j, yy;
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

//	system("clear");
	clear();

	yy = 7;
	move(yy,25);
	printw("\t□ □ □ □ □ □ □ □ □ □ ");
        move(yy+1,25);
	addstr("\t□    NEXT BLOCK   □ ");
   	attroff(COLOR_PAIR(1));
	
	move(yy+2,25);
	printw("\t□ □ □ □ □ □ □ □ □ □ ");
	//move(yy+3,25);
	yy = 12;
	for(i = 0 ; i < 4 ; i++)
	{
		for(int p=10; p<18; p++){ 
			move(p,50);
			printw("□ ");
			move(p,32);
			printw("□ ");
		}
		move(yy,38);
		for(j = 0 ; j < 4 ; j++)
		{
			
			if((*block_pointer)[0][i][j] == 1)
				printw("■ ");
			else if((*block_pointer)[0][i][j] == 0)
				printw("  ");
			
		}
		yy++;
	}	
	yy = 18;
	move(yy,25);
	printw("\t□ □ □ □ □ □ □ □ □ □ ");
	move(yy+1,25);
	addstr("\t□   SCORE");
	printw(": %ld",point); //현재스코어 출력 1p
	move(yy+1,50);
	addstr("□ ");
	move(yy+2,25);
	printw("\t□ □ □ □ □ □ □ □ □ □ ");
	for(i = yy+3; i<yy+9; i++)
	{
		move(i,50);
		printw("□ ");
		move(i,32);
		printw("□ ");

	}
	move(yy+4,35);
	printw("게임종료: ESC");
	move(yy+5,35);
	printw("조작키: a,s,d");
	move(yy+6,35);
	printw("블록회전: w");
	move(yy+7,35);
	printw("떨어뜨리기: q");
	move(yy+9,25);
	printw("\t□ □ □ □ □ □ □ □ □ □ ");
	
	

	yy = 7;
	for(i = 2 ; i < 21 ; i++)
	{
		move(yy,8);
		
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
		yy++;
	}
	
	move(0,0);
	tetris();
	
	//move(6,2);
	//printw("     SCORE: %ld TOP SCORE: %d",point, best_point); //현재스코어 출력 1p
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
		

	move(34,6);
	printw("20-1 Software Engineering - Copyright JaewonShin");
	move(36,23);
	printw("혼자 하기 모드");
	curs_set(0); //깜빡거리는 커서 안켜지게 수정 
	move(45,99);
	refresh();	

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
		case	LEFT :	tempx--; break;
		case	RIGHT :	tempx++; break;
		case	DOWN :	tempy++; break;
		case	ROTATE :
				temp_block_state++; temp_block_state %=  4; break;
	}

	switch(block_number)
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
			temp_tetris_table[i][j] = tetris_table[i][j]; //테이블 복사
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

/* 한줄이 완성되었는지 확인하는 함수. 완성되면 한줄을 지우고, 점수에 100점을 더한다*/
int check_one_line(void)
{
	int i, j;
	int ni, nj;
	int line_hole;

	for(i = 19 ; i > 0 ; i--)
	{
		line_hole = 0;
		for(j = 1 ; j < 9 ; j++)
		{
			if(tetris_table[i][j] == 0)
			{
				line_hole = 1;
				break;
			}
		}

		if(line_hole == 0)
		{
			point += 100;
			for(ni = i ; ni > 0 ; ni--)
			{
				for(nj = 0 ; nj < 9 ; nj++)
				{
					tetris_table[ni][nj] = tetris_table[ni-1][nj];
				}
			}
			i += 1;
		}
	}

	return 0;
}

void game_end(){
	time_t ptime;
	struct tm *t;
	FILE *fp = NULL;

	if(set_ticker(game)==-1)
		perror("set_ticker");

	// 기록 파일로 저장

	move(15,16);
		addch(ACS_ULCORNER);
		for(int i=0; i<10; i++)
			addch(ACS_HLINE);
		printw("GAME OVER");  
		for(int i=0; i<11; i++)
			addch(ACS_HLINE);
		addch(ACS_URCORNER);
		
		 for(int d=0; d<6; d++){
			move(d+16,16);
			addch(ACS_VLINE);
			printw("                              ");
			move(d+16,47);
			addch(ACS_VLINE);
		}

		
	 	
		move(21,16);
		addch(ACS_LLCORNER);
		for(int i=0; i<30; i++)
			addch(ACS_HLINE);
		addch(ACS_LRCORNER);
		move(19,44);
		printw("     ");
		move(20,17);
		printw("                          ");
		move(19,47);
		addch(ACS_VLINE);
		move(17,18);
		printw("       Final Score : %ld    ", point);
		move(18,18);
		printw("       INPUT INITIAL : ");
		refresh();
		
		curs_set(1); //커서 깜빡이 다시 켜주고
		scanw("%s%*c", temp_result.name); //사용자 이름 입력받음 
		
		while(strlen(temp_result.name)!=3){	//이니셜 세글자가 아닌경우에 사용자로부터 재입력요청	
			move(18,47);	
			addch(ACS_VLINE);
			printw("                                 "); //잘못입력한 글자를 지워주는 부분 	
			move(18,50); //초과입력으로 지워진 블록부분 다시 그려주기 
			printw("□");				
			move(18,21);
			printw("세글자만 입력하세요       ");
			move(18,40);
			scanw("%s%*c", temp_result.name);		
		}
		curs_set(0); //입력 종료시 커서 다시 꺼줌 
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
