#ifndef Tetris_H
#define Tetris_H

int _getch(void);/*문자를 바로 입력 받을 수 있는 함수*/

/* 게임 종료때 마다
 * 이름과 득점점수와
 * 날짜, 시간이저장되는 구조체
 * */
static struct result
{
	char name[30];
	long point;
	int year;
	int month;
	int day;
	int hour;
	int min;
}temp_result;

#endif
