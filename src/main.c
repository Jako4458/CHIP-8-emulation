#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "constants.h"
#include "main.h"

uint8_t MEM[KB4];
uint8_t REG[kAmountOfRegisters];
uint16_t PC = kStartOfProgram;
uint16_t I = kStartOfProgram;
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
    REG[1] = SIZE_OF_CHAR_SPRITE;
    REG[2] = SIZE_OF_CHAR_SPRITE * 2;
    REG[3] = SIZE_OF_CHAR_SPRITE * 3;
    REG[4] = 4;

    load_sprite_to_mem(0x00B, ONE, SIZE_OF_CHAR_SPRITE);
    load_sprite_to_mem(0x00B + SIZE_OF_CHAR_SPRITE + 1, TWO, SIZE_OF_CHAR_SPRITE);
    load_sprite_to_mem(0x00B + (SIZE_OF_CHAR_SPRITE + 1) * 2, THREE, SIZE_OF_CHAR_SPRITE);
    load_sprite_to_mem(0x00B + (SIZE_OF_CHAR_SPRITE + 1) * 3, FOUR, SIZE_OF_CHAR_SPRITE);
    load_sprite_to_mem(0x00B + (SIZE_OF_CHAR_SPRITE + 1) * 4, FIVE, SIZE_OF_CHAR_SPRITE);
    Annn(0x00B);

    Dxyn(0, 0, SIZE_OF_CHAR_SPRITE*6);

    load_sprite_to_mem(0x10B, SIX, SIZE_OF_CHAR_SPRITE);
    load_sprite_to_mem(0x10B + (SIZE_OF_CHAR_SPRITE + 1) * 1, SEVEN, SIZE_OF_CHAR_SPRITE);
    load_sprite_to_mem(0x10B + (SIZE_OF_CHAR_SPRITE + 1) * 2, EIGHT, SIZE_OF_CHAR_SPRITE);
    load_sprite_to_mem(0x10B + (SIZE_OF_CHAR_SPRITE + 1) * 3, NINE, SIZE_OF_CHAR_SPRITE);
    load_sprite_to_mem(0x10B + (SIZE_OF_CHAR_SPRITE + 1) * 4, ZERO, SIZE_OF_CHAR_SPRITE);
    Annn(0x10B);

    Dxyn(1, 0, SIZE_OF_CHAR_SPRITE*6);
    //
    // draw_to_screen(0, 0, 0xF0);
    // draw_to_screen(0, 1, 0x80);
    // draw_to_screen(0, 2, 0xF0);
    // draw_to_screen(0, 3, 0x10);
    // draw_to_screen(0, 4, 0xF0);

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

/*
 * load a sprite of size n into MEM[addr]
 */
void load_sprite_to_mem(uint16_t addr, const uint8_t sprite[], uint8_t n)
{
    for (int i = 0; i < n; i++)
    {
        MEM[addr + i] = sprite[i];
    }
}

/*
 * Annn - LD I, addr
 * Set I = nnn.
 *
 * The value of register I is set to nnn.
 */
void Annn(uint16_t nnn)
{
    assert(nnn < 0xF000 && "Out of Bound MEM addr. only uses 12 bit so max i 0x0FFF");
    I = 0x0FFF & nnn; 
}

/*
 * Dxyn - DRW Vx, Vy, nibble
 * Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
 * 
 * The interpreter reads n bytes from memory, starting at the address stored in I. These bytes are then displayed as sprites on screen at coordinates (Vx, Vy). Sprites are XORed onto the existing screen. If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0. If the sprite is positioned so part of it is outside the coordinates of the display, it wraps around to the opposite side of the screen. See instruction 8xy3 for more information on XOR, and section 2.4, Display, for more information on the Chip-8 screen and sprites.
 */
void Dxyn(uint8_t x, uint8_t y, uint8_t n)
{
    x = REG[x];
    y = REG[y];

    for (int i = 0; i < n; i++)
    {
        draw_to_screen(x, y + i, MEM[I + i]);
    }
}

void draw_to_screen(uint8_t x, uint8_t y, uint8_t sprite)
{
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
