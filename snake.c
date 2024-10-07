#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define WIDTH 30
#define HEIGHT 20

typedef struct
{
    int x, y;
    int fruitX, fruitY;
    int score, gameover, speed, level, count;
    int *tailX, *tailY;
    int nTail;
    int flag;
    char dir;
} SnakeGame;

void setup(SnakeGame *snake)
{
    snake->gameover = 0;
    snake->dir = 'd';
    snake->x = WIDTH / 2;
    snake->y = HEIGHT / 2;
    snake->fruitX = rand() % WIDTH;
    snake->fruitY = rand() % HEIGHT;
    snake->score = 0;
    snake->level = 1;
    snake->speed = 200;
    snake->count = 0;
    snake->nTail = 0;
    snake->flag = 1;
    snake->tailX = (int *)malloc(sizeof(int) * 100);
    snake->tailY = (int *)malloc(sizeof(int) * 100);
}

void displaySnakeHead(char dir)
{
    switch (dir)
    {
    case 'w':
        printf("^ ");
        break;
    case 'a':
        printf("< ");
        break;
    case 'd':
        printf("> ");
        break;
    case 's':
        printf("v ");
        break;
    }
}

void displayScoreLevel(SnakeGame *snake)
{
    if (snake->count < 5)
        printf("Level %d\n", snake->level);
    else
    {
        printf("======> Level Up <======\n");
        snake->level++;
        snake->count = 0;
        snake->speed -= 50;
        getch();
    }
    printf("Score : %d\n", snake->score);
}

void draw(SnakeGame *snake)
{
    system("cls");

    for (int i = 0; i < WIDTH + 2; i++)
        printf("# ");
    printf("\n");

    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if (j == 0)
                printf("# ");

            if (i == snake->y && j == snake->x)
                displaySnakeHead(snake->dir);

            else if (i == snake->fruitY && j == snake->fruitX)
                printf("* ");
            else
            {
                int print = 0;
                for (int k = 0; k < snake->nTail; k++)
                {
                    if (snake->tailX[k] == j && snake->tailY[k] == i)
                    {
                        printf("o ");
                        print = 1;
                    }
                }
                if (!print)
                    printf("  ");
            }
            if (j == WIDTH - 1)
                printf("# ");
        }
        printf("\n");
    }

    for (int i = 0; i < WIDTH + 2; i++)
        printf("# ");
    printf("\n");

    displayScoreLevel(snake);
}

void input(SnakeGame *snake)
{
    if (kbhit())
    {
        switch (getch())
        {
        case 'w':
            if (snake->dir != 's' || snake->level == 1)
                snake->dir = 'w';
            break;
        case 'a':
            if (snake->dir != 'd' || snake->level == 1)
                snake->dir = 'a';
            break;
        case 's':
            if (snake->dir != 'w' || snake->level == 1)
                snake->dir = 's';
            break;
        case 'd':
            if (snake->dir != 'a' || snake->level == 1)
                snake->dir = 'd';
            break;
        case 'k':
            snake->flag = 0;
            break;
        }
    }
}

void run(SnakeGame *snake)
{
    int prevX = snake->tailX[0];
    int prevY = snake->tailY[0];
    int prev2X, prev2Y;
    snake->tailX[0] = snake->x;
    snake->tailY[0] = snake->y;

    for (int i = 1; i < snake->nTail; i++)
    {
        prev2X = snake->tailX[i];
        prev2Y = snake->tailY[i];
        snake->tailX[i] = prevX;
        snake->tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (snake->dir)
    {
    case 'w':
        snake->y--;
        break;
    case 'a':
        snake->x--;
        break;
    case 's':
        snake->y++;
        break;
    case 'd':
        snake->x++;
        break;
    }

    if (snake->level >= 3)
    {
        if (snake->x < 0 || snake->x >= WIDTH || snake->y < 0 || snake->y >= HEIGHT)
        {
            draw(snake);
            snake->gameover = 1;
        }

        for (int i = 0; i < snake->nTail; i++)
        {
            if (snake->tailX[i] == snake->x && snake->tailY[i] == snake->y)
            {
                draw(snake);
                snake->gameover = 1;
            }
        }
    }
    else
    {
        if (snake->x < 0)
            snake->x = WIDTH - 1;
        else if (snake->x >= WIDTH)
            snake->x = 1;
        else if (snake->y < 0)
            snake->y = HEIGHT - 1;
        else if (snake->y == HEIGHT)
            snake->y = 1;
    }

    if (snake->x == snake->fruitX && snake->y == snake->fruitY)
    {
        snake->score += 10;
        snake->fruitX = rand() % WIDTH;
        snake->fruitY = rand() % HEIGHT;
        snake->nTail++;
        snake->count++;
    }
}

int main()
{
    srand(time(NULL));
    SnakeGame snake;
    setup(&snake);

    while (!snake.gameover)
    {
        if (snake.flag)
        {
            draw(&snake);
            input(&snake);
            run(&snake);
            Sleep(snake.speed);
        }
        else
        {
            getch();
            snake.flag = 1;
        }
    }
    printf("Game Over! Final Score: %d\n", snake.score);
    return 0;
}
