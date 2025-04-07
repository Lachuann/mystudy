#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>	 //����һ��ʱ������ڿ⣬���ṩ��һЩ��������time��clock��srand�ȣ����ڻ�ȡ������ϵͳʱ�䣬���������������
#include <windows.h> //����һ��Windows����ϵͳ��Ӧ�ó���ӿڣ�API����
#include <conio.h>	 //����һ������̨��������⣬���ṩ��һЩ��������getch��kbhit�ȣ����ڼ����ȡ���̵�����
#include <stdlib.h>

#define WIDTH 78  // x i
#define HEIGHT 24 // y j
#define TICK 80
#define APPLE_DELAY 5000
// ö�ٱ���
typedef enum
{
	NORTH,
	SOUTH,
	WEST,
	EAST
} Direction;
Direction direction;
// ��������
int score = 0;
int size, sus = 0;
int dx, dy;
char key, sus1;

COORD *body; // COORD��һ���ṹ�����ͣ���������ʾһ���ַ��ڿ���̨��Ļ�ϵ�����

COORD apple, tail;

HANDLE hdl; // ����ͱ���hdl
ULONGLONG start, tick_event, apple_event, now;
boolean is_death, is_suspend;

// ��������
void draw_wall();
void draw_score();
void init_apple();
void init_snake();
void tick();
void remove_obj(COORD obj);
void update_screen();
void get_direction();
void init_note();
void end_note();

int main()
{
	// ��ʼ������갡֮���
	hdl = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci = {1, 0}; // �����ṹ�����cci������̨������С�Ϳɼ��ԣ�%
	SetConsoleCursorInfo(hdl, &cci);  // ���ÿ���̨�������Ĺ���С�Ϳɼ���
	SetConsoleTitle("SNAKE!!!");
	srand((unsigned)time(NULL));
	// ���߽��г�ʼ�����ڶ��������ߵ�����
	body = (COORD *)malloc(sizeof(COORD) * HEIGHT * WIDTH); // COORD*��һ��ָ�����ͣ�����ʾһ��ָ��COORD�ṹ���ָ��
	init_note();

	// ������Ϸ����
	draw_wall();
	draw_score(); // score:XXX

	// ���߽��л��ƣ�ƻ��
	init_snake();
	init_apple();

	// ��ʱ����г�ʼ��
	start = GetTickCount64(); // GetTickCount64()��һ��Windows API��������������ϵͳ���������ĺ�����
	tick_event = start + TICK;
	apple_event = start + APPLE_DELAY;

	// ������
	while (1)
	{

		// ��ȡ��ǰ��ʱ�䣬����ﵽһ��ʱ�䣬���ƶ���
		// ���߱��������ģ�ɾ��β�ͣ���ǰ�ƶ�һ��
		// ʤ���ж�����ƻ���ж�
		// �������
		now = GetTickCount64();
		if (now > tick_event)
		{
			tick();
			if (is_death == TRUE)
				break;
			tick_event += TICK;
		}
		Sleep(100);
		/*
		if (now > apple_event)
		{
			remove_obj(apple);
			init_apple();
			apple_event += APPLE_DELAY;
		}*/

		// ÿ��һ��ʱ�� ����ƻ����λ��
	}
    end_note();
    while(1){}
	free(body);
	system("cls");

	return 0;
}

// ǽ����
void draw_wall()
{
	for (int i = 0; i <= HEIGHT; i++)
	{
		for (int j = 0; j <= WIDTH; j++)
		{
			if (i == HEIGHT && j % 2 == 0)
				printf("��");
			else if (i == 0 && j % 2 == 0)
				printf("��");
			else if (j == WIDTH)
				printf("��");
			else if (j == 0)
				printf("��");
			else
				putchar(' ');
		}
		puts("");
	}
}

// ��������
void draw_score()
{
	COORD pos = {WIDTH + 5, HEIGHT / 2};
	SetConsoleCursorPosition(hdl, pos);
	printf("SCORE:");
}

// ��ʼ��ƻ��
void init_apple()
{
	int flag = 1;
	while (flag)
	{
		flag = 0;

		apple.Y = rand() % (HEIGHT - 1) + 1; // rand () % (b+a-1)+aΪa��b
		while (1)
		{
			apple.X = rand() % (WIDTH - 1) + 1;
			if (apple.X % 2 == 0)
			{
				break;
			}
		}

		for (int i = 0; i < size; i++)
		{
			if (body[i].X == apple.X && body[i].Y == apple.Y)
			{
				flag = 1;
			}
		}
	}
}

// ��ʼ����
void init_snake()
{
	is_death = FALSE;
	size = 2;
	body[0].X = (WIDTH / 2) + 1;
	body[0].Y = HEIGHT / 2;
	body[1].X = (WIDTH / 2) - 1;
	body[1].Y = HEIGHT / 2;
	direction = EAST;
	key = 'D';
	dx = 2;
	dy = 0;
}

// �߸��º���
void tick()
{
	if (is_suspend)
	{
		key = _getch();
		is_suspend = FALSE;
	}

	// �ж��Ƿ�ײǽ
	if (body[0].X < 1 || body[0].X >= WIDTH || body[0].Y < 1 || body[0].Y >= HEIGHT)
	{
		is_death = TRUE;
		return;
	}

	// ��ƻ������
	update_screen();

	// �������
	get_direction();

	// �ж��Ƿ�ײ���Լ�
	for (int i = 1; i < size; i++)
	{
		if (body[i].X == body[0].X && body[i].Y == body[0].Y)
		{
			is_death = TRUE;
			return;
		}
	}

	// �ж��Ƿ�Ե�ƻ��
	if (apple.X == body[0].X && apple.Y == body[0].Y)
	{
		init_apple();
		size++;
		score++;
	}

	// �����ߵ�λ��
	tail = body[size - 1];
	for (int i = size - 1; i > 0; i--)
	{
		body[i] = body[i - 1];
	}
	body[0].X += dx;
	body[0].Y += dy;
}

// ��λ�������
void remove_obj(COORD obj)
{
	SetConsoleCursorPosition(hdl, obj);
	putchar(' ');
}

// ʳ��������ɺ���
void update_screen()
{
	remove_obj(tail);
	for (int i = 0; i < size; i++)
	{
		SetConsoleCursorPosition(hdl, body[i]);
		if (i == 0)
			putchar('@');
		else
			putchar('*');
	}
	SetConsoleCursorPosition(hdl, apple);
	putchar('#');
	COORD pos = {WIDTH + 11, HEIGHT / 2};
	SetConsoleCursorPosition(hdl, pos);
	printf("%d", score);
}

void get_direction()
{
	while (_kbhit()) //_kbhit()�������Ƿ�������
	{
		key = _getch(); // ��ȡ������ַ�

		switch (key)
		{
		case 'W':
		case 'w':
			if (direction == SOUTH)
				break;
			direction = NORTH;
			dx = 0;
			dy = -1;
			break;
		case 'A':
		case 'a':
			if (direction == EAST)
				break;
			direction = WEST;
			dx = -2;
			dy = 0;
			break;
		case 'S':
		case 's':
			if (direction == NORTH)
				break;
			direction = SOUTH;
			dx = 0;
			dy = 1;
			break;
		case 'D':
		case 'd':
			if (direction == WEST)
				break;
			direction = EAST;
			dx = 2;
			dy = 0;
			break;
		case 'P':
		case 'p':
			is_suspend = !is_suspend; // ����P��ʱ����ͣ�������Ϸ
			break;
		case -32: // ������ַ��� -32 ʱ����ʾ�����˷����
			key = _getch();
			switch (key)
			{
			case 72: // ��Ӧ��
				if (direction == SOUTH)
					break;
				direction = NORTH;
				dx = 0;
				dy = -1;
				break;
			case 80: // ��Ӧ��
				if (direction == NORTH)
					break;
				direction = SOUTH;
				dx = 0;
				dy = 1;
				break;
			case 75: // ��Ӧ��
				if (direction == EAST)
					break;
				direction = WEST;
				dx = -2;
				dy = 0;
				break;
			case 77: // ��Ӧ��
				if (direction == WEST)
					break;
				direction = EAST;
				dx = 2;
				dy = 0;
				break;
			}
		}
	}
}

// ������ʾ����
void init_note()
{

	CONSOLE_SCREEN_BUFFER_INFO csbi; // ����ṹ������˺ܶ��ֶΣ�������Ļ�������Ĵ�С����ǰ����λ�á���ǰʹ�õ���ɫ�ȵ�
	int columns, rows;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi); // GetStdHandle(STD_OUTPUT_HANDLE) ��һ���������ã�������һ������������������˱�׼����豸��Ҳ������Ŀ���̨����
	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	int advice_height = 7;						// ������Ϸ�Ҹ������
	int start_row = (rows - advice_height) / 2; // ��ʼ��ӡ������
	char *advice[] = {
		" ��ӭ���� ̰���ߣ�",
		"��������Ϸ�Ҹ桷",
		"���Ʋ�����Ϸ���ܾ�������Ϸ��",
		"ע�����ұ�����������ƭ�ϵ���",
		"�ʶ���Ϸ���ԣ�������Ϸ����",
		"������ʱ�䣬���ܽ������",
		"Powered by Lachuan",

	};

	// �ȴ�ӡ���У�ֱ�����￪ʼ��ӡ������
	for (int aa = 0; aa < start_row; aa++)
	{
		printf("\n");
	}

	// ��ӡÿһ�н�����Ϸ�Ҹ�
	for (int aa = 0; aa < advice_height; aa++)
	{
		int start_col = (columns - strlen(advice[aa])) / 2; // ��ʼ��ӡ������
		for (int bb = 0; bb < start_col; bb++)
		{
			printf(" ");
		}
		printf("%s\n", advice[aa]);
	}

	sleep(3);	   // ��ͣ3��
	system("cls"); // ����
}
void end_note()
{
    system("cls");

    // ��ȡ����̨���ڵĴ�С
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    // ���������ʾ��λ��
    int center_column = columns / 2;
    int center_row = rows / 2;

    // ���ù��λ�ã�������ʾ��Ϸ������Ϣ
    COORD end1 = {center_column - 10, center_row - 1};
    SetConsoleCursorPosition(hdl, end1);
    printf("===============================");

    COORD end2 = {center_column - 10, center_row};
    SetConsoleCursorPosition(hdl, end2);
    printf("           ��Ϸ����");

    COORD end3 = {center_column - 10, center_row + 1};
    SetConsoleCursorPosition(hdl, end3);
    printf("         ��ĵ÷��ǣ�%d", score);

    COORD end4 = {center_column - 10, center_row + 2};
    SetConsoleCursorPosition(hdl, end4);
    printf("===============================");
}
