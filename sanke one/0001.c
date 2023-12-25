#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>	 //这是一个时间和日期库，它提供了一些函数，如time、clock、srand等，用于获取或设置系统时间，或者生成随机数。
#include <windows.h> //这是一个Windows操作系统的应用程序接口（API）库
#include <conio.h>	 //这是一个控制台输入输出库，它提供了一些函数，如getch、kbhit等，用于检测或获取键盘的输入
#include <stdlib.h>

#define WIDTH 78  // x i
#define HEIGHT 24 // y j
#define TICK 80
#define APPLE_DELAY 5000
// 枚举变量
typedef enum
{
	NORTH,
	SOUTH,
	WEST,
	EAST
} Direction;
Direction direction;
// 变量区域
int score = 0;
int size, sus = 0;
int dx, dy;
char key, sus1;

COORD *body; // COORD是一个结构体类型，它用来表示一个字符在控制台屏幕上的坐标

COORD apple, tail;

HANDLE hdl; // 句柄型变量hdl
ULONGLONG start, tick_event, apple_event, now;
boolean is_death, is_suspend;

// 函数区域
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
	// 初始化：光标啊之类的
	hdl = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci = {1, 0}; // 申明结构体变量cci，控制台光标光标大小和可见性，%
	SetConsoleCursorInfo(hdl, &cci);  // 设置控制台缓冲区的光标大小和可见性
	SetConsoleTitle("SNAKE!!!");
	srand((unsigned)time(NULL));
	// 对蛇进行初始化：在堆区创建蛇的数组
	body = (COORD *)malloc(sizeof(COORD) * HEIGHT * WIDTH); // COORD*是一个指针类型，它表示一个指向COORD结构体的指针
	init_note();

	// 绘制游戏界面
	draw_wall();
	draw_score(); // score:XXX

	// 对蛇进行绘制，苹果
	init_snake();
	init_apple();

	// 对时间进行初始化
	start = GetTickCount64(); // GetTickCount64()是一个Windows API函数，它返回自系统启动以来的毫秒数
	tick_event = start + TICK;
	apple_event = start + APPLE_DELAY;

	// 主程序
	while (1)
	{

		// 获取当前的时间，如果达到一定时间，就移动蛇
		// 对蛇本身做更改：删除尾巴，向前移动一格
		// 胜负判定、吃苹果判定
		// 按键检测
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

		// 每隔一段时间 更改苹果的位置
	}
    end_note();
    while(1){}
	free(body);
	system("cls");

	return 0;
}

// 墙绘制
void draw_wall()
{
	for (int i = 0; i <= HEIGHT; i++)
	{
		for (int j = 0; j <= WIDTH; j++)
		{
			if (i == HEIGHT && j % 2 == 0)
				printf("■");
			else if (i == 0 && j % 2 == 0)
				printf("■");
			else if (j == WIDTH)
				printf("■");
			else if (j == 0)
				printf("■");
			else
				putchar(' ');
		}
		puts("");
	}
}

// 分数绘制
void draw_score()
{
	COORD pos = {WIDTH + 5, HEIGHT / 2};
	SetConsoleCursorPosition(hdl, pos);
	printf("SCORE:");
}

// 初始化苹果
void init_apple()
{
	int flag = 1;
	while (flag)
	{
		flag = 0;

		apple.Y = rand() % (HEIGHT - 1) + 1; // rand () % (b+a-1)+a为a到b
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

// 初始化蛇
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

// 蛇更新函数
void tick()
{
	if (is_suspend)
	{
		key = _getch();
		is_suspend = FALSE;
	}

	// 判断是否撞墙
	if (body[0].X < 1 || body[0].X >= WIDTH || body[0].Y < 1 || body[0].Y >= HEIGHT)
	{
		is_death = TRUE;
		return;
	}

	// 画苹果画蛇
	update_screen();

	// 按键检测
	get_direction();

	// 判断是否撞到自己
	for (int i = 1; i < size; i++)
	{
		if (body[i].X == body[0].X && body[i].Y == body[0].Y)
		{
			is_death = TRUE;
			return;
		}
	}

	// 判断是否吃到苹果
	if (apple.X == body[0].X && apple.Y == body[0].Y)
	{
		init_apple();
		size++;
		score++;
	}

	// 更改蛇的位置
	tail = body[size - 1];
	for (int i = size - 1; i > 0; i--)
	{
		body[i] = body[i - 1];
	}
	body[0].X += dx;
	body[0].Y += dy;
}

// 定位清除函数
void remove_obj(COORD obj)
{
	SetConsoleCursorPosition(hdl, obj);
	putchar(' ');
}

// 食物随机生成函数
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
	while (_kbhit()) //_kbhit()检测键盘是否有输入
	{
		key = _getch(); // 获取输入的字符

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
			is_suspend = !is_suspend; // 按下P键时，暂停或继续游戏
			break;
		case -32: // 输入的字符是 -32 时，表示按下了方向键
			key = _getch();
			switch (key)
			{
			case 72: // 对应上
				if (direction == SOUTH)
					break;
				direction = NORTH;
				dx = 0;
				dy = -1;
				break;
			case 80: // 对应下
				if (direction == NORTH)
					break;
				direction = SOUTH;
				dx = 0;
				dy = 1;
				break;
			case 75: // 对应左
				if (direction == EAST)
					break;
				direction = WEST;
				dx = -2;
				dy = 0;
				break;
			case 77: // 对应右
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

// 健康提示函数
void init_note()
{

	CONSOLE_SCREEN_BUFFER_INFO csbi; // 这个结构体包含了很多字段，比如屏幕缓冲区的大小、当前光标的位置、当前使用的颜色等等
	int columns, rows;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi); // GetStdHandle(STD_OUTPUT_HANDLE) 是一个函数调用，它返回一个句柄，这个句柄代表了标准输出设备，也就是你的控制台窗口
	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	int advice_height = 7;						// 健康游戏忠告的行数
	int start_row = (rows - advice_height) / 2; // 开始打印的行数
	char *advice[] = {
		" 欢迎来到 贪吃蛇！",
		"《健康游戏忠告》",
		"抵制不良游戏，拒绝盗版游戏。",
		"注意自我保护，谨防受骗上当。",
		"适度游戏益脑，沉迷游戏伤身。",
		"合理安排时间，享受健康生活。",
		"Powered by Lachuan",

	};

	// 先打印空行，直到到达开始打印的行数
	for (int aa = 0; aa < start_row; aa++)
	{
		printf("\n");
	}

	// 打印每一行健康游戏忠告
	for (int aa = 0; aa < advice_height; aa++)
	{
		int start_col = (columns - strlen(advice[aa])) / 2; // 开始打印的列数
		for (int bb = 0; bb < start_col; bb++)
		{
			printf(" ");
		}
		printf("%s\n", advice[aa]);
	}

	sleep(3);	   // 暂停3秒
	system("cls"); // 清屏
}
void end_note()
{
    system("cls");

    // 获取控制台窗口的大小
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    // 计算居中显示的位置
    int center_column = columns / 2;
    int center_row = rows / 2;

    // 设置光标位置，居中显示游戏结束信息
    COORD end1 = {center_column - 10, center_row - 1};
    SetConsoleCursorPosition(hdl, end1);
    printf("===============================");

    COORD end2 = {center_column - 10, center_row};
    SetConsoleCursorPosition(hdl, end2);
    printf("           游戏结束");

    COORD end3 = {center_column - 10, center_row + 1};
    SetConsoleCursorPosition(hdl, end3);
    printf("         你的得分是：%d", score);

    COORD end4 = {center_column - 10, center_row + 2};
    SetConsoleCursorPosition(hdl, end4);
    printf("===============================");
}
