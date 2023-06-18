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

int curPosX, curPosY;	// 현재 커서 위치 저장 변수
int block_id;			// 내려올 블록의 id
int speed = 1;			// block 속도 조정
int score = 0;			// 점수 추가

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

// 블록끼리의 충돌을 판별
int DetectCollision(int _x, int _y, char blockInfo[4][4])
{
	int x, y;
	int arrX = (_x-GBOARD_ORIGIN_X) / 2;
	int arrY = _y-GBOARD_ORIGIN_Y;

	for (x = 0; x < 4; x++)
	{
		for (y = 0; y < 4; y++)
		{
			if (blockInfo[y][x] == 1 && gameBoardInfo[arrY+y][arrX+x] == 1)
				return 0;
		}
	}

	return 1;
}

// 블록을 왼쪽으로 움직임
// 기호는 문자 2칸 차지 +-2
void ShiftLeft()
{
	// 충돌 코드 추가
	if (!DetectCollision(curPosX-2, curPosY, blockModel[block_id]))
		return;

	deleteBlock(blockModel[block_id]);
	curPosX -= 2;
	SetCurrentCursorPos(curPosX, curPosY);
	showBlock(blockModel[block_id]);
}

// 블록을 오른쪽으로 움직임
void ShiftRight()
{
	if (!DetectCollision(curPosX + 2, curPosY, blockModel[block_id]))
		return;

	deleteBlock(blockModel[block_id]);
	curPosX += 2;
	SetCurrentCursorPos(curPosX, curPosY);
	showBlock(blockModel[block_id]);
}

// 일정 시간마다 블록으로 아래쪽으로 내림
// 계속 내려오다가 바닥에 닿으면 정지해야 하므로 반환형 변형
int BlockDown()
{
	if (!DetectCollision(curPosX, curPosY + 1, blockModel[block_id]))
		return 0;
	
	deleteBlock(blockModel[block_id]);
	curPosY += 1;
	SetCurrentCursorPos(curPosX, curPosY);
	showBlock(blockModel[block_id]);

	return 1;
}

// 게임보드에 블록 정보를 저장
void AddBlockToBoard()
{
	// 4*4 영역을 돌면서 게임보드의 0인 부분을 1로 변경하여 블록 정보 저장
	int x, y, arrCurX, arrCurY;
	for (y = 0; y < 4; y++)
	{
		for (x = 0; x < 4; x++)
		{
			arrCurX = (curPosX - GBOARD_ORIGIN_X) / 2;
			arrCurY = curPosY - GBOARD_ORIGIN_Y;

			if (blockModel[block_id][y][x] == 1)
				gameBoardInfo[arrCurY + y][arrCurX + x] = 1;
		}
	}
}

// 추가된 블록 정보를 포함하여 게임보드를 다시 그림
void RedrawBlocks()
{
	int x, y;
	int cursX, cursY;

	for (y = 0; y < GBOARD_HEIGHT; y++)
	{
		for (x = 1; x < GBOARD_WIDTH + 1; x++)
		{
			cursX = x * 2 + GBOARD_ORIGIN_X;
			cursY = y + GBOARD_ORIGIN_Y;
			SetCurrentCursorPos(cursX, cursY);
			if (gameBoardInfo[y][x] == 1)
				printf("■");
			else
				printf("  ");
		}
	}
}

// 한 라인이 모두 블록으로 가득 찼을 경우 해당 라인 제거
void RemoveFillUpLine()
{
	int y, x, line;
	
	// 게임보드 전체를 돔
	for (y = GBOARD_HEIGHT - 1; y > 0; y--)
	{
		for (x = 1; x < GBOARD_WIDTH + 1; x++)
		{
			if (gameBoardInfo[y][x] != 1)
				break;
		}

		// 전체를 돌다가 어느 한 줄이 처음부터 마지막까지 1일 경우
		// 한 줄이 완성됐다고 판단
 		if (x == GBOARD_WIDTH + 1)
		{
			// 해당 줄을 초기화
			for (line = 0; y - line > 0; line++)
				memcpy(&gameBoardInfo[y - line][1], &gameBoardInfo[(y - line) - 1][1], GBOARD_WIDTH * sizeof(int));
			// 줄을 하나씩 아래로 내림
			y += 1;
			score += 10;	// 줄 제거 시 점수 획득
		}
	}
	RedrawBlocks();
}

// 블록 회전
int RotateBlock()
{
	int block_base = block_id / 4;
	int block_rotated = (block_base * 4) + ((block_id + 1) % 4);

	// 방향 전환 시 블록과 충돌의 경우
	if (!DetectCollision(curPosX, curPosY, blockModel[block_rotated]))
		return 0;

	deleteBlock(blockModel[block_id]);
	block_id = (block_base * 4) + ((block_id + 1) % 4);
	SetCurrentCursorPos(curPosX, curPosY);
	showBlock(blockModel[block_id]);

	return 1;
}

// 스페이스를 누르면 바로 밑으로 내려옴
void SpaceDown()
{
	while (BlockDown());
}

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

// 키 입력을 받음
void ProcessKeyInput()
{

	for (int i = 0; i < 20; i++)
	{
		int key;
		if (_kbhit() != 0)
		{
			key = _getch();

			switch (key)
			{
			case LEFT:
				ShiftLeft();
				break;
			case RIGHT:
				ShiftRight();
				break;
			case UP:
				RotateBlock();
				break;
			case SPACE:
				SpaceDown();
				break;
			}
		}	

		Sleep(speed);
	}
}

// 현재 점수를 보여주는 인터페이스
void showScore()
{
	SetCurrentCursorPos(30, 10);
	printf("Score : %d", score);
	SetCurrentCursorPos(curPosX, curPosY);
}

// main
int main()
{
	srand(time(NULL));

	while (1)
		{
			// 블록 랜덤 생성
			block_id = rand() % 28;
			curPosX = GBOARD_ORIGIN_X + 6;
			curPosY = GBOARD_ORIGIN_Y;

			drawBoard();

			while(1)
			{
				showScore();

				if(BlockDown()==0)	// 테트리스에서 블록은 계속 내려옴
				{
					// 이후 충돌 시 0을 반환
					// 블록을 현재 위치에 저장
					// 게임보드를 다시 그림
					AddBlockToBoard();
					RemoveFillUpLine();
					break;
				}
				ProcessKeyInput();	// 키보드 입력을 받음
			}
		}
		
	

	return 0;
}