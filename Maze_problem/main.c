#include <stdio.h>
#include <stdlib.h>

#define ROWS 8
#define COLS 8

typedef struct
{
  int x;
  int y;
} Point;

typedef struct
{
  Point *points;
  int top;
} Stack;

void initMaze(int maze[][COLS])
{
  for (int i = 0; i < ROWS; i++)
  {
    for (int j = 0; j < COLS; j++)
    {
      maze[i][j] = rand() % 2; // Randomly fill with 0 or 1
    }
  }
}

void printMaze(int maze[][COLS])
{
  for (int i = 0; i < ROWS; i++)
  {
    for (int j = 0; j < COLS; j++)
    {
      printf("%d ", maze[i][j]);
    }
    printf("\n");
  }
}

int isValid(int maze[][COLS], Point p)
{
  return (p.x >= 0 && p.x < ROWS && p.y >= 0 && p.y < COLS && maze[p.x][p.y] == 1);
}

void initStack(Stack *stack)
{
  stack->points = malloc(sizeof(Point) * 100); // Allocate memory for points
  stack->top = 0;
}

void push(Stack *stack, Point p)
{
  stack->points[stack->top++] = p;
}

Point pop(Stack *stack)
{
  if (stack->top == 0)
  {
    printf("Stack is empty!\n");
    exit(1);
  }
  return stack->points[--stack->top];
}

int dfs(int maze[][COLS], Stack *stack, Point start, Point end)
{
  push(stack, start);

  if (start.x == end.x && start.y == end.y)
  {
    printf("Path found:\n");
    for (int i = stack->top - 1; i >= 0; i--)
    {
      printf("(%d, %d)\n", stack->points[i].x, stack->points[i].y);
    }
    return 1; // Path found
  }

  // push(stack, start);

  int dx[] = {0, 1, 0, -1}; // Directions: up, right, down, left
  int dy[] = {-1, 0, 1, 0};

  for (int i = 0; i < 4; i++)
  {
    int newX = start.x + dx[i];
    int newY = start.y + dy[i];

    Point newPoint = {newX, newY};

    if (isValid(maze, newPoint) && !isVisited(stack, newPoint))
    {
      if (dfs(maze, stack, newPoint, end))
      {
        return 1; // Path found
      }
    }
  }

  pop(stack);
  return 0; // No path found at this point
}

int isVisited(Stack *stack, Point p)
{
  for (int i = 0; i < stack->top; i++)
  {
    if (stack->points[i].x == p.x && stack->points[i].y == p.y)
    {
      return 1;
    }
  }
  return 0;
}

int main()
{
  int maze[ROWS][COLS];

  initMaze(maze);

  Point start = {0, 0}; // Set start point
  Point end = {7, 7};   // Set end point

  if (!isValid(maze, start)) {
    printf("No path found!\n");
    return 0;
  }

  Stack stack;
  initStack(&stack);

  int found = dfs(maze, &stack, start, end);

  if (!found)
  {
    printf("No path found!\n");
  }

  free(stack.points); // Free memory allocated for stack

  return 0;
}