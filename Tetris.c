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

int curPosX, curPosY;	// 현재 커서 위치 저장 변수
int block_id;			// 내려올 블록의 id

/* 키보드 키값 정의 */
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define SPACE 32

// 현재 커서 위치 x, y로 변경
void SetCurrentCursorPos(int x, int y)
{
	COORD pos = { x, y };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// 현재 커서 위치 받아오기
COORD GetCurrentCursorPos()
{
	CONSOLE_SCREEN_BUFFER_INFO	curInfo;
	COORD curPoint;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curPoint.X = curInfo.dwCursorPosition.X;
	curPoint.Y = curInfo.dwCursorPosition.Y;

	return curPoint;
}

// 블록을 그림
void showBlock(char blockInfo[4][4])
{
	// 현재 커서의 위치 반환
	COORD pos = GetCurrentCursorPos();		

	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			// 커서를 움직이며 4*4 공간에 블록을 그림
			SetCurrentCursorPos(pos.X + x * 2, pos.Y + y);
			if (blockInfo[y][x] == 1)
				printf("■");
		}
		SetCurrentCursorPos(pos.X, pos.Y);
		// 처음 커서 위치로 복귀
	}
}

// 블록을 지움
void deleteBlock(char blockInfo[4][4])
{
	COORD pos = GetCurrentCursorPos();

	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			// 커서를 움직이며 4*4 공간에 그렸던 블록을 지움
			SetCurrentCursorPos(pos.X + x * 2, pos.Y + y);
			if (blockInfo[y][x] == 1)
				printf("  ");
		}
		SetCurrentCursorPos(pos.X, pos.Y);
		// 처음 커서 위치로 복귀
	}
}

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