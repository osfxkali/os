#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdint.h>
#include "main.h" // 包含 draw_pixel 和 clear_screen 函数

// 猜数字游戏
void guess_number_game() {
    srand(time(NULL)); // 应该在程序开始时只调用一次
    int number = rand() % 100 + 1;
    int guess;
    printf("Guess the number between 1 and 100:\n");

    while (1) {
        printf("Your guess: ");
        if (scanf("%d", &guess) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // 清空输入缓冲区
            continue;
        }
        while (getchar() != '\n'); // 清空输入缓冲区

        if (guess < number) {
            printf("Too low! Try again.\n");
        } else if (guess > number) {
            printf("Too high! Try again.\n");
        } else {
            printf("Congratulations! You guessed the number.\n");
            break;
        }
    }
}

// 蛇形游戏
void snake_game() {
    volatile uint32_t *fb = (uint32_t *)FRAMEBUFFER_BASE;
    clear_screen(fb, 0x000000); // 黑色

    int snake_x[] = {100, 99, 98};
    int snake_y[] = {100, 100, 100};
    int direction = 1; // 1: right, 2: down, 3: left, 4: up
    int last_direction = 1; // 记录上一次的方向
    int food_x = 200;
    int food_y = 200;
    int score = 0;

    draw_pixel(fb, food_x, food_y, 0x00FF00); // 绿色食物

    while (1) {
        // 绘制蛇
        for (int i = 0; i < 3; i++) {
            draw_pixel(fb, snake_x[i], snake_y[i], 0xFFFFFF); // 白色蛇身
        }

        // 处理输入
        char input;
        if (scanf("%c", &input) == 1) {
            while (getchar() != '\n'); // 清空输入缓冲区
            if ((input == 'w' && last_direction != 2) || (input == 's' && last_direction != 4) ||
                (input == 'a' && last_direction != 1) || (input == 'd' && last_direction != 3)) {
                direction = (input == 'w') ? 4 : (input == 's') ? 2 : (input == 'a') ? 3 : 1;
                last_direction = direction;
            }
        }

        // 移动蛇
        int old_x = snake_x[0];
        int old_y = snake_y[0];
        for (int i = 2; i > 0; i--) {
            snake_x[i] = snake_x[i - 1];
            snake_y[i] = snake_y[i - 1];
        }

        switch (direction) {
            case 1: snake_x[0]++; break; // 右
            case 2: snake_y[0]++; break; // 下
            case 3: snake_x[0]--; break; // 左
            case 4: snake_y[0]--; break; // 上
        }

        // 检查碰撞食物
        if (snake_x[0] == food_x && snake_y[0] == food_y) {
            score++;
            snake_x[2] = old_x; // 增加蛇的长度
            snake_y[2] = old_y;
            food_x = rand() % WIDTH;
            food_y = rand() % HEIGHT;
            while (food_x == old_x || food_y == old_y) { // 确保食物不在蛇身上
                food_x = rand() % WIDTH;
                food_y = rand() % HEIGHT;
            }
            draw_pixel(fb, food_x, food_y, 0x00FF00); // 新的食物
        }

        // 检查边界和自身碰撞
        if (snake_x[0] < 0 || snake_x[0] >= WIDTH || snake_y[0] < 0 || snake_y[0] >= HEIGHT ||
            snake_x[0] == old_x && snake_y[0] == old_y) { // 撞到自己
            printf("Game Over! Your score: %d\n", score);
            break;
        }

        // 延迟
        for (volatile int i = 0; i < 1000000; i++);
    }
}