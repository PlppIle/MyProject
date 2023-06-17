#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>
#include "blockModel.h"

/*Size of Gameboard*/
#define GBOARD_HEIGHT 20
#define GBOARD_WIDTH 10

/*Size of Gameboard*/
#define GBOARD_ORIGIN_Y 2
#define GBOARD_ORIGIN_X 4

// 게임보드
int gameBoardInfo[GBOARD_HEIGHT + 1][GBOARD_WIDTH + 2];

// 시작 시 게임보드 초기화
void gameBoardinit()
{
    // 게임보드 정보를 0으로 초기화
	memset(gameBoardInfo, 0, sizeof(int));

    // 외벽 정보 1로 초기화
	for (int y = 0; y < GBOARD_HEIGHT; y++)
	{
		gameBoardInfo[y][0] = 1;
		gameBoardInfo[y][GBOARD_WIDTH + 1] = 1;
	}

	for (int x = 0; x < GBOARD_WIDTH + 2; x++)
		gameBoardInfo[GBOARD_HEIGHT][x] = 1;
}

// 게임 보드 화면에 출력
void drawBoard()
{
	int x, y;

    // 게임 시작 시 초기화하며 출력
	gameBoardinit();

    // 1인 외벽 부분을 그림
	for (y = 0; y <= GBOARD_HEIGHT; y++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y + y);
		if (y == GBOARD_HEIGHT)
			printf("└");
		else
			printf("│");
	}

	for (x = 1; x <= GBOARD_WIDTH+1; x++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X + (x * 2) - 1, GBOARD_ORIGIN_Y + GBOARD_HEIGHT);
		printf("─");
	}

	for (y = 0; y <= GBOARD_HEIGHT; y++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH * 2)+2, GBOARD_ORIGIN_Y + y);
		if (y == GBOARD_HEIGHT)
			printf("┘");
		else
			printf("│");
	}
}

// main
int main()
{
	drawBoard();

		
	return 0;
}