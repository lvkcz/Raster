#ifndef COLOR_HAL_H
#define COLOR_HAL_H

#include <stdio.h>

// 统一颜色枚举
typedef enum {
    COLOR_DEFAULT = 0,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_YELLOW,
    COLOR_BLUE,
    COLOR_DIM,
    COLOR_GREEN_LIGHT,
    COLOR_GREEN_MEDIUM,
    COLOR_GREEN_DARK
    
} AppColor;

static void set_color(AppColor color);
static void reset_color(void);

#if defined(_WIN32)
    #include <windows.h>
    static void set_color(AppColor color) {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        WORD value = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        switch (color) {
            case COLOR_RED: value = FOREGROUND_RED | FOREGROUND_INTENSITY;
                break;
            case COLOR_GREEN:
            case COLOR_GREEN_LIGHT: value = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
                break;
            case COLOR_GREEN_MEDIUM: value = FOREGROUND_GREEN | FOREGROUND_BLUE;
                break;
            case COLOR_GREEN_DARK: value = FOREGROUND_GREEN;
                break;
            case COLOR_YELLOW: value = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
                break;
            case COLOR_BLUE: value = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
                break;
            case COLOR_DIM: value = FOREGROUND_INTENSITY;
                break;
            default:
                break;
        }
        SetConsoleTextAttribute(h, value);
    }
    static void reset_color(void) {
        set_color(COLOR_DEFAULT);
    }

#elif defined(__MSDOS__) || defined(__DOS__)
    // DOS 8086 / 386 环境：通过 conio 或 BIOS / 显存属性直接修改
    #include <conio.h>
    static void set_color(AppColor color) {
        switch (color) {
            case COLOR_RED: textcolor(12);
                break;
            case COLOR_GREEN:
            case COLOR_GREEN_LIGHT: textcolor(10);
                break;
            case COLOR_GREEN_MEDIUM: textcolor(2);
                break;
            case COLOR_GREEN_DARK: textcolor(2);
                break;
            case COLOR_YELLOW: textcolor(14);
                break;
            case COLOR_BLUE: textcolor(9);
                break;
            case COLOR_DIM: textcolor(8);
                break;
            default: textcolor(7);
                break;
        }
    }
    static void reset_color(void) {
        textcolor(7);
    }

#elif defined(EMBEDDED_MCU)
    // 51 单片机 / 裸机串口 / 液晶屏环境：关闭转义码输出，保持纯文本防乱码
    static void set_color(AppColor color) {
        (void)color; // 默认环境不做处理，或在此处调用 TFT_SetTextColor()
    }
    void reset_color(void) {
    }

#else
    // 现代 Linux / macOS / POSIX 系统
    #include <unistd.h>
    static void set_color(AppColor color) {
        // 如果输出被重定向到文件（例如 ./todo list > out.txt），自动降级为不着色，防止污染文件
        if (!isatty(fileno(stdout))) return;

        switch (color) {
            case COLOR_RED: printf("\033[1;31m");
                break;
            case COLOR_GREEN: printf("\033[1;32m");
                break;
            case COLOR_YELLOW: printf("\033[1;33m");
                break;
            case COLOR_BLUE: printf("\033[1;34m");
                break;
            case COLOR_DIM: printf("\033[38;5;236m");
                break;
            case COLOR_GREEN_LIGHT: printf("\033[38;5;83m");
                break;
            case COLOR_GREEN_MEDIUM: printf("\033[38;5;34m");
                break;
            case COLOR_GREEN_DARK: printf("\033[38;5;22m");
                break;
            default: printf("\033[0m");
                break;
        }
    }
     void reset_color(void) {
        if (!isatty(fileno(stdout)))
            return;
        printf("\033[0m");
    }

#endif
#endif
