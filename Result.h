#include "Tetris.h"

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

int search_result(void);
int print_result(void);
int file_count(FILE *file);

