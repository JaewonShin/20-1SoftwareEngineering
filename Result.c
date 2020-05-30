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
#include <ncursesw/curses.h> 
#include <pthread.h>

#include "Result.h"


/*
//스코어 내용 구조체 새로 생성
typedef struct tetris_score
{
	char name[30];
    long point;
    int year;
    int month;
    int day;
    int hour;
    int min;
} score;
*/


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
			//for(int i=0; i<59; i++)
			
			for(int i=0; i<66; i++)
				addch(ACS_HLINE);
			addch(ACS_RTEE);
			printw("\n       ");
			addch(ACS_VLINE);
			//test
			printw("  \t %s\t\t %ld\t%d. %d. %d       %d : %d \t  ", temp_result.name, temp_result.point, temp_result.year, temp_result.month, temp_result.day, temp_result.hour, temp_result.min);
			//printw("\t%s\t       %ld\t%d. %d. %d. | %d : %d    ", temp_result.name, temp_result.point, temp_result.year, temp_result.month, temp_result.day, temp_result.hour, temp_result.min);
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


	printw("\n\n\t\t       Software Engeenering");
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
	int cnt = 0;
	int i = 0, j;

	setlocale(LC_CTYPE, "ko_KR.utf-8");

	fp = fopen("result.txt", "r");

	if(fp == NULL)
		return 0;

    /////////////////////저장된 점수 동적할당 포인터 배열에 기록///////////////////

	cnt = file_count(fp);

	score *score_p;
	score_p = (score*)malloc(sizeof(score)*cnt);

	for(i=0; i<cnt; i++){
		fscanf(fp, "%s %ld %d %d %d %d %d\n", score_p[i].name, &score_p[i].point, &score_p[i].year, &score_p[i].month, &score_p[i].day, &score_p[i].hour, &score_p[i].min);
	}


	printf("\n\n%d", cnt);

	///////////////////배열 내림차순 정렬//////////////////

	score temp;
    for(i=0; i<cnt+1; i++)
    {
        for(j=0; j<cnt; j++)
        {
            if(score_p[j].point < score_p[j+1].point)
            {
                temp = score_p[j];
                score_p[j] = score_p[j+1];
                score_p[j+1] = temp;
            }
        }
    }

	/////////////////////////////////////////////////////////




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



	int k = 0;
	

//	while(!feof(fp))
	while(k < cnt)
	{
			//fscanf(fp, "%s %ld %d %d %d %d %d\n", temp_result.name, &temp_result.point, &temp_result.year, &temp_result.month, &temp_result.day, &temp_result.hour, &temp_result.min);
			printw("\n       ");
			addch(ACS_LTEE);
			for(int i=0; i<66; i++)
				addch(ACS_HLINE);
			addch(ACS_RTEE);
			printw("\n       ");
			addch(ACS_VLINE);
			//printw("  \t %s\t\t %ld\t%d. %d. %d       %d : %d \t  ", temp_result.name, temp_result.point, temp_result.year, temp_result.month, temp_result.day, temp_result.hour, temp_result.min);

			printw("  \t %s\t\t %ld\t%d. %d. %d       %d : %d \t  ", score_p[k].name, score_p[k].point, score_p[k].year, score_p[k].month, score_p[k].day, score_p[k].hour, score_p[k].min);
			addch(ACS_VLINE);

			k++;

	}

	fclose(fp);
	printw("\n       ");
	addch(ACS_LLCORNER);
	for(int i=0; i<66; i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);


	printw("\n\n\t\t\t   메뉴 화면으로 돌아가기 : M");


	printw("\n\n\t\t\t   Software Engineering");
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

int file_count(FILE *file)
{
    char str[30];
	int cnt = 0;

    while(1)
    {
        fgets(str, 30, file);
        if(feof(file)) break;
        //record = (result*)malloc(sizeof(result) * 1);
        cnt ++;
    }

    //fseek(fp,0,SEEK_SET);
    rewind(file);

	return cnt;

}


