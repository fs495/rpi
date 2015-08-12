#include <stdint.h>
#include "libglcd.h"

static uint8_t line_wrap; /* 右端で行を折り返すか */
static uint8_t curx, cury; /* カーソル位置。Xはピクセル単位、Yはページ単位 */
static uint8_t dispy; /* 表示開始位置 */

/*
 * 右端を超えた時に行を折り返すか指定する
 */
void glcd_line_wrap(uint8_t enabled)
{
    line_wrap = enabled;
}

static void glcd_newline(uint8_t incry)
{
    /* カーソルを次の行の先頭に移動 */
    curx = 0;
    cury += incry;

    /* 表示可能範囲を超えてカーソルを移動したら、画面をスクロールする */
    if(cury >= (dispy + GLCD_VIEW_PAGES) % GLCD_VRAM_PAGES) {
	dispy = (dispy + incry) % GLCD_VRAM_PAGES;
	glcd_set_display_row(dispy * 8);
    }

    if(cury >= GLCD_VRAM_PAGES)
	cury -= GLCD_VRAM_PAGES;
}

/**
 * 7ビットASCIIの表示可能文字(キャラクタコードは0x20〜0x7e)を表示する
 * 8x16ドット固定
 */
void glcd_putchar(uint8_t c)
{
    if(c == '\r') {
	curx = 0;
	return;
    }

    if(c == '\n') {
	glcd_newline(2);
	return;
    }

    if(c < 0x20 || c >= 0x7f)
	return;

    /* 折り返ししない設定時に、すでにカーソルが右端に来ているときは何もしない */
    if(curx >= GLCD_WIDTH && !line_wrap)
	return;

    /* 文字イメージの表示 */
    glcd_write_block(curx, cury, 8, 2, font8x16 + (c - 0x20) * 16);
    curx += 8;

    /* 折り返しする設定時に、カーソルが右端を超えたら改行 */
    if(curx >= GLCD_WIDTH && line_wrap)
	glcd_newline(2);
}

void glcd_puts(const char *s)
{
    while(*s)
	glcd_putchar(*s++);
}
