#include "Turboc.h"
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define LevelMax 3

const char* Level[LevelMax] = { "StageLevel1.txt","StageLevel2.txt","StageLevel3.txt" };
char* Map;
char* test;
int Map_Size = 0;
int LevelUp = 0;

enum MapPoint
{
	space = '0', wall, goal, box, player
};

struct PlayerPos
{
	int x = 0;
	int y = 0;
	int Player = 0;
	int Size = 0;
	int Bool = 0;
}PlayerPosition;

void InitOnMap(FILE* StageMap,int stageLevel);
void Paint();
void SetMap(char Mapdata);
void MoveMap(int Move);
void GameReset();
void Message(const char * Message);
int crashMove(int x,int y);
int Stageclear();



int main()
{
	// ���ڹ� ���� 
	FILE* StageMap = NULL;
	int Move;
	setcursortype(NOCURSOR);

	while (1)
	{
		

		InitOnMap(StageMap, LevelUp);

		PlayerPosition.x = PlayerPosition.Player % PlayerPosition.Size;
		PlayerPosition.y = PlayerPosition.Player / PlayerPosition.Size;
		
		while (1)
		{
			if (Stageclear())
			{
				clrscr();
				LevelUp++;
				GameReset();
				break;
			}

			Paint();
			gotoxy(PlayerPosition.x, PlayerPosition.y);
			putchar('@');

			Move = _getch();

			if (_kbhit())
			{
				if (Move == 224)
				{
					Move = _getch();
					if (Move == 72 || Move == 80 || Move == 75 || Move == 77)
						MoveMap(Move);
				}
				else
				{
					Message("�߸� �Է��ϼ̽��ϴ�.");
				}
				
			}
			else
			{
				if (Move == 'q')
				{
					free(Map);
					clrscr();
					exit(1);
				}
				else if (Move == 'w')
				{
					if (LevelUp == LevelMax - 1)
					{
						Message("���� ���������� �����ϴ�.");
						continue;
					}
						
					else
					{
						LevelUp++;
						GameReset();
					}
					break;
				}
				else if (Move == 'e')
				{
					if (LevelUp == 0)
					{
						Message("���� ���������� �����ϴ�.");
						continue;
					}
					
					else
					{
						LevelUp--;
						GameReset();
					}
					
					break;
				}

				else if (Move == 'r')
				{
					GameReset();
					break;
				}
				else
				{
					Message("�߸� �Է��ϼ̽��ϴ�.");
				}

			}
		}
	}

	return 0;
}


void InitOnMap(FILE* StageMap,int stageLevel) // ���Ͽ��� �� �׸���
{
	char  paint;
	fopen_s(&StageMap, Level[stageLevel], "r"); // �� fopen_s �̰Ÿ� ��ߵ��� ???

	if (StageMap == NULL)
	{
		Message("������ ���µ� ������ ������ϴ�.");
		return;
	}

	while (1) { // feof�Լ��� ������  eof�� �о �ѹ� �� ������� �ʳ� ???

		if (feof(StageMap))
			break;

		paint = fgetc(StageMap);

		switch (paint)
		{
		case space:
			paint = ' ';
			break;
		case wall:
			paint = '#';
			break;
		case goal:
			paint = '*';
			break;
		case box:
			paint = 'O';
			break;
		case player:
			paint = ' ';
			PlayerPosition.Player = Map_Size;
			break;
		default:
			if (PlayerPosition.Bool == 1)
				break;
			PlayerPosition.Size = Map_Size+1;
			PlayerPosition.Bool = 1;
		}

		if (Map_Size > 0)
			SetMap(paint);
		else
		{
			Map = (char*)malloc(sizeof(char) * 1);
			Map[0] = paint;
			Map_Size++;
		}

		putchar(paint);

	}



	fclose(StageMap);
}

void SetMap(char Mapdata) // ������ ũ�⸸ŭ �޸� �����Ҵ� \n���� �� �Ǵܰ���
{

	test = (char*)realloc(Map,sizeof(char)*(Map_Size+1)); // �޸� ���� �����ϱ�
	
	if (test != NULL)
	{
		Map = test;
	}
	Map[Map_Size] = Mapdata;
	Map_Size++;
}


void Paint()
{
	clrscr();
	int i = 0;
	while (i < Map_Size)
	{
		putchar(Map[i]);
		i++;
	}

	gotoxy(PlayerPosition.Size + 3, 0);
	puts("q: ��������    w: ���� �������� �̵�");
	gotoxy(PlayerPosition.Size + 3, 1);
	puts("r: �ٽ��ϱ�    e: ���� �������� �̵�");
}

void MoveMap(int Move)
{
	int x = 0;
	int y = 0;

	switch (Move) {

	case LEFT:
		x = -1;
		break;

	case RIGHT:
		x = 1;
		break;
	case UP:
		y = -1;
		break;
	case DOWN:
		y = 1;
		break;
	default:
		Message("���α׷� ����");
		break;
	}
	if (crashMove(x, y))
	{
		PlayerPosition.x += x;
		PlayerPosition.y += y;
	}
}


int crashMove(int x,int y)
{
	int objectPos1,objectPos2;
	objectPos1 = (PlayerPosition.x+x) + (PlayerPosition.y+y) * PlayerPosition.Size;
	
	if (Map[objectPos1] == '#')
		return 0;

	else
	{
		if (Map[objectPos1] == 'O')
		{
			objectPos2 = (PlayerPosition.x + x*2) + (PlayerPosition.y + y* 2) * PlayerPosition.Size;
			if (Map[objectPos2] == 'O' || Map[objectPos2] == '#')
				return 0;
			else 
			{
				if (Map[objectPos2] == '*')
				{
					Map[objectPos2] = 'X';
					Map[objectPos1] = ' ';
				}
				else
				{
					Map[objectPos1] = Map[objectPos2];
					Map[objectPos2] = 'O';
				}
			}
		}
	
	}

	return 1;
}

int Stageclear()
{

	int i = 0;
	while (i < Map_Size)
	{
		if (Map[i] == '*')
			return 0;
		i++;
	}
	return 1;
}

void GameReset()
{
	free(Map);
	PlayerPosition.Bool = 0;
	PlayerPosition.x = 0;
	PlayerPosition.y = 0;
	PlayerPosition.Player = 0;
	PlayerPosition.Size = 0;
	Map_Size = 0;
	if (LevelUp == LevelMax)
	{
		Message("��� �������� Clear!!!");
		exit(1);
	}
}


void Message(const char* Message)
{
	clrscr();
	gotoxy(40, 12);
	puts(Message);
	delay(1000);
}