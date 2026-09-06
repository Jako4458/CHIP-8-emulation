#include <stdint.h>
#include <assert.h>
#include <stdio.h>

#include "constants.h"
#include "main.h"

uint8_t MEM[KB4];
uint8_t REG[kAmountOfRegisters];
uint16_t PC = kStartOfProgram;
bool SCREEN[kScreenWidth][kScreenHeight];

int main(int argc, char *argv [])
{

    printf("hello world!\n\r");
    printf("called with args: \n\r");

    for (int i = 0; i < argc; i++)
    {
	printf("%s\n\r", argv[i]);
    }
    clear_screen(SCREEN);

    REG[0] = 0;
    REG[1] = 1;
    REG[2] = 2;
    REG[3] = 3;
    REG[4] = 4;

    draw_to_screen(0, 0, 0xF0);
    draw_to_screen(0, 1, 0x80);
    draw_to_screen(0, 2, 0xF0);
    draw_to_screen(0, 3, 0x10);
    draw_to_screen(0, 4, 0xF0);

    // SCREEN[1][5] = 1;

    print_screen(SCREEN);

    return 0;
}

void clear_screen(bool screen[kScreenWidth][kScreenHeight])
{
    for (int k = 0; k < kScreenHeight; k++)
    {
        for (int i = 0; i < kScreenWidth; i++)
        {
	    screen[k][i] = 0;
        }
    }
}

void print_screen(bool screen[kScreenWidth][kScreenHeight])
{
    for (int i = 0; i < kScreenWidth + 2; i++)
    {
        printf("_");
    }

    printf("\n\r");

    for (int k = 0; k < kScreenHeight; k++)
    {
	printf("%c",'|');
        for (int i = 0; i < kScreenWidth; i++)
	{
	    char print_char = screen[i][k] ? '*' : ' ';
	    printf("%c", print_char);
	}
	printf("|\n\r");
    }

    for (int i = 0; i < kScreenWidth + 2; i++)
    {
        printf("_");
    }
    printf("\n\r");
}

void draw_to_screen(uint8_t x, uint8_t y, uint8_t sprite)
{
    x = REG[x];
    y = REG[y];

    assert(x >= 0);
    assert(x <= kScreenWidth);
    assert(y >= 0);
    assert(y <= kScreenHeight);

    // flag for collision -> true if sprite overwrites a 1  
    REG[kAmountOfRegisters - 1] = 0;

    for (int k = 7; k >= 0; k--)
    {
        bool val = (sprite >> k) & 1;
	x++;

        // set override flag
        if (SCREEN[x][y] && val)
        {
            REG[kAmountOfRegisters - 1]++;
    	    printf("(%d, %d)", x, y);
        }

        SCREEN[x][y] ^= val;
}
}
