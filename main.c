#include <stdio.h>
#include <memory.h>
#include <time.h>
#include <windows.h>
#include "stgame.h"

#define P_COUNT 4
#define E_COUNT 3
#define ENEMY_FLIGHT 6
#define ENEMY_LINE 3
#define ENEMY_NUM 30
#define P_SHOT 0
#define E_SHOT 1
#define SHOT_MAX 40
#define PLAYER_LIFE 12     
#define SCORE_COUNT 8

int LifeCount = PLAYER_LIFE;
char startcursor[3] = ">>";
int SpeedCount = 1;
char pShot = '^';
char eShot = 'v';
char pUnit[P_COUNT + 1] = "<||>";
char eUnit[E_COUNT + 1] = "|*|";
char Score[SCORE_COUNT + 1] = "Score : ";
int Score2 = 0;
char PlayerLife[PLAYER_LIFE + 1] = " HP : ¢¾¢¾¢¾";

struct StartInfo {
	int x, y;
};
struct PlayerInfo {
	int x, y;
	int liveFlag;
};

struct EnemyInfo {
	int x, y;
	int liveFlag;
	int StartX;
	int StartY;
	int MoveFlag;
};

struct ShotInfo {
	int x, y;
	int Type;
	int UseFlag;
};

struct StartInfo StartGame;
struct PlayerInfo player;
struct ShotInfo shot[SHOT_MAX];
struct EnemyInfo enemy[ENEMY_NUM];



void SetStartPosition();
void DrawScore();
void PlayerAction();
void DrawShot();
void PlayerAction();
void EnemyAction();
void EnemyAction2();
void DrawPlayerInfo();
void Draw();
void DrawPlayer();
void DrawEnemy();
void PlayerInitialObject();
void EnemyInitialObject();
void CreateShot(int Type, int x, int y);
void ShotAction();
void CheckCrash();
int CheckEndGame();
int CheckClearGame();


int main() {



	SetStartPosition();
	while (1) {
		Sleep(100);
		EnemyAction();
		PlayerAction();
		ShotAction();
		CheckCrash();
		Draw();
		if (CheckClearGame() == 1)
			SetStartPosition();
		if (CheckEndGame() == 1)
			break;
	}

}


void EnemyAction() {

	int i;
	for (i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy[i].liveFlag)
		{
			if (i % 2 == 0)
			{
				if (enemy[i].MoveFlag == 1)
					enemy[i].y++;
				else if (enemy[i].MoveFlag == 2)
					enemy[i].x++;
				else if (enemy[i].MoveFlag == 3)
					enemy[i].y--;
				else if (enemy[i].MoveFlag == 4)
					enemy[i].x--;
			}

			if ((enemy[i].y - enemy[i].StartY == 1) && (enemy[i].x - enemy[i].StartX == 0))
				enemy[i].MoveFlag = 2;
			if ((enemy[i].y - enemy[i].StartY == 1) && (enemy[i].x - enemy[i].StartX == 9))
				enemy[i].MoveFlag = 3;
			if ((enemy[i].y - enemy[i].StartY == -1) && (enemy[i].x - enemy[i].StartX == 9))
				enemy[i].MoveFlag = 4;
			if ((enemy[i].y - enemy[i].StartY == -1) && (enemy[i].x - enemy[i].StartX == 0))
				enemy[i].MoveFlag = 1;
			if (rand() % 100 < 5)
				CreateShot(E_SHOT, enemy[i].x, enemy[i].y);
		}
	}
}

void EnemyAction2() {
	int i;
	for (i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy[i].liveFlag == 1)
		{
			if (i > 9 && i < 15)
			{
				if (enemy[i].MoveFlag == 1)
					enemy[i].x--;
				else
					enemy[i].x++;
			}
			else
			{
				if (enemy[i].MoveFlag == 1)
					enemy[i].x++;
				else
					enemy[i].x--;
			}
			if (abs(enemy[i].StartX - enemy[i].x) > 5)
			{
				enemy[i].MoveFlag = !enemy[i].MoveFlag;
				enemy[i].y++;
				if (enemy[i].y < 0 || enemy[i].y >= HEIGHT)
					enemy[i].liveFlag == 0;
			}
			if (rand() % 100 < 5)
				CreateShot(E_SHOT, enemy[i].x, enemy[i].y);
		}
	}
}

void PlayerAction() {

	if (player.liveFlag == 1)
	{
		if ((GetAsyncKeyState(VK_UP) & 0x8000))
		{
			if (player.y > HEIGHT - HEIGHT)
			{
				if (SpeedCount == 1)
					player.y--;
				else if (SpeedCount == 2)
					player.y--;
			}
		}
		if ((GetAsyncKeyState(VK_DOWN) & 0x8000))
		{
			if (player.y < HEIGHT - 2)
			{
				player.y++;
			}
		}
		if ((GetAsyncKeyState(VK_RIGHT) & 0x8000))
		{
			if (player.x < WIDTH - 4)
			{
				player.x += 2;
			}
		}
		if ((GetAsyncKeyState(VK_LEFT) & 0x8000))
		{
			if (player.x > WIDTH - 78)
			{
				player.x -= 2;
			}
		}
		if ((GetAsyncKeyState(VK_SPACE) & 0x8000))
		{
			CreateShot(P_SHOT, player.x, player.y);
		}
	}
}

void DrawPlayerInfo() {
	int i;
	int x = 0;
	int y = 23;

	for (i = 0; i < LifeCount; i++)
	{
		screen[y][x] = PlayerLife[i];

		x++;
	}
}

int CheckClearGame() {
	int i;
	for (i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy[i].liveFlag == 1)
		{
			return 0;
		}
	}

	return 1;
}

int CheckEndGame() {

	if (player.liveFlag == 0)
	{
		MoveCursor(36, 12);
		Sleep(1000);
		MoveCursor(0, 24);
		return 1;
	}
}



void CreateShot(int Type, int x, int y) {

	int i;
	for (i = 0; i < SHOT_MAX; i++)
	{
		if (shot[i].UseFlag == 0)
		{
			shot[i].UseFlag = 1;
			shot[i].Type = Type;
			shot[i].x = x;
			shot[i].y = y;
			return;
		}
	}
}


void PlayerInitialObject() {
	player.x = 39;
	player.y = 19;

	player.liveFlag = 1;
}



void Draw() {
	int i;
	for (i = 0; i < HEIGHT; i++)
	{
		memset(screen[i], ' ', WIDTH);
		screen[i][WIDTH - 1] = NULL;
	}

	DrawPlayer();
	DrawEnemy();
	DrawShot();
	DrawScore();

	DrawPlayerInfo();

	for (i = 0; i < HEIGHT; i++)
	{
		MoveCursor(0, i);
		printf(screen[i]);

		MoveCursor(70, 23);
		printf("%d", Score2);

	}
}


void EnemyInitialObject() {

	int i = 0;
	for (int j = 0; j < ENEMY_LINE; j++)
	{
		for (int k = 0; k < ENEMY_FLIGHT; k++)
		{
			enemy[i].x = rand() % 65 + 1;
			enemy[i].y = rand() % 10 + 1;
			enemy[i].liveFlag = 1;
			enemy[i].StartX = enemy[i].x;
			enemy[i].StartY = enemy[i].y;
			enemy[i].MoveFlag = 1;

			i++;
		}
	}
}

void DrawEnemy() {

	int i, j;
	int x, y;

	for (i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy[i].liveFlag == 1)
		{
			x = enemy[i].x - E_COUNT / 2;
			y = enemy[i].y;

			if (y < 0 || y >= HEIGHT)
				continue;

			for (j = 0; j < E_COUNT; j++)
			{
				if (x >= 0 && x < WIDTH - 1)
					screen[y][x] = eUnit[j];

				x++;
			}
		}
	}
}

void DrawPlayer() {
	int i;
	int x = player.x - P_COUNT / 2;
	int y = player.y;

	if (y < 0 || y >= HEIGHT)
		return;

	if (player.liveFlag == 1)
	{
		for (i = 0; i < P_COUNT; i++)
		{
			if (x >= 0 && x < WIDTH - 1)
				screen[y][x] = pUnit[i];

			x++;
		}
	}
}


void SetStartPosition() {
	Initial();
	PlayerInitialObject();
	EnemyInitialObject();

}

void DrawScore() {
	int i;
	int x = 60;
	int y = 23;

	for (i = 0; i < SCORE_COUNT; i++)
	{
		screen[y][x] = Score[i];
		x++;
	}
}

void DrawShot() {
	int i;
	for (i = 0; i < SHOT_MAX; i++)
	{
		if (shot[i].UseFlag == 1)
		{
			int x = shot[i].x;
			int y = shot[i].y;
			if ((x >= 0 && x < WIDTH - 1) && (y > 0 && y < 24))
			{
				if (shot[i].Type == E_SHOT)
					screen[y][x] = eShot;
				else if (shot[i].Type == P_SHOT)
				{
					screen[y][x] = pShot;
				}
			}
		}
	}
}

void ShotAction() {
	int i;
	for (i = 0; i < SHOT_MAX; i++)
	{
		if (shot[i].UseFlag == 1)
		{
			if (shot[i].Type == E_SHOT)
			{
				shot[i].y++;
				if (shot[i].y >= HEIGHT)
					shot[i].UseFlag = 0;
			}
			else if (shot[i].Type == P_SHOT)
			{
				shot[i].y--;
				if (shot[i].y <= 0)
					shot[i].UseFlag = 0;
			}
		}
	}
}

void CheckCrash() {

	int i, j;
	for (i = 0; i < SHOT_MAX; i++)
	{
		for (j = 0; j < ENEMY_NUM; j++)
		{
			if (shot[i].UseFlag == 1)
			{
				if (shot[i].Type == P_SHOT)
				{
					if (enemy[j].liveFlag == 1)
					{

						if (((shot[i].y == enemy[j].y)) && ((shot[i].x > (enemy[j].x - 2)) && (shot[i].x < (enemy[j].x + 2))))
						{
							enemy[j].liveFlag = 0;
							shot[i].UseFlag = 0;
							Score2 += 50;
							break;
						}
					}
				}
			}
		}
		if (shot[i].UseFlag == 1)
		{
			if (shot[i].Type == E_SHOT)
			{
				if (player.liveFlag == 1)
				{
					if ((shot[i].y == player.y) && ((shot[i].x >= (player.x - P_COUNT / 2)) && (shot[i].x <= (player.x + P_COUNT / 2))))
					{
						shot[i].UseFlag = 0;
						LifeCount -= 2;
						if (LifeCount == 6)
						{
							player.liveFlag = 0;
							shot[i].UseFlag = 0;
							break;
						}
					}
				}
			}
		}
	}
}