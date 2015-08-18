#include <stdint.h>
#include "libglcd.h"

static uint8_t font_type; /* フォントのタイプ */
static uint8_t base_height; /* ベースハイト(ページ単位) */
static uint8_t curx, cury; /* カーソル位置。Xはピクセル単位、Yはページ単位 */
static uint8_t dispy; /* 表示開始位置(ページ単位) */
static uint8_t line_wrap; /* 右端で行を折り返すか */

/**
 * フォントを設定
 */
uint8_t glcd_config_font(uint8_t ft)
{
    switch(ft) {
    case FIXED8x16:
	base_height = 2;
	break;

    default:
	return -1;
    }

    font_type = ft;
    return 0;
}

/**
 * 右端を超えた時に行を折り返すか指定する
 */
void glcd_line_wrap(uint8_t enabled)
{
    line_wrap = enabled;
}

/**
 * 画面クリアし、文字表示位置を初期化する
 */
void glcd_clear_screen(void)
{
    glcd_clear_vram();
    curx = cury = dispy = 0;
    glcd_set_display_row(0);
}

/**
 * 改行の内部処理
 */
static void glcd_newline(void)
{
    /* カーソルを次の行の先頭に移動。
     * スクロール処理のため、まだ境界超え処理しない */
    curx = 0;
    cury += base_height;

    /* 表示可能範囲を超えてカーソルを移動したら、画面をスクロールする */
    if(cury >= (dispy + GLCD_VIEW_PAGES) % GLCD_VRAM_PAGES) {
	dispy = (dispy + base_height) % GLCD_VRAM_PAGES;
	glcd_set_display_row(dispy * 8);
    }

    /* ここで境界処理超え */
    if(cury >= GLCD_VRAM_PAGES)
	cury -= GLCD_VRAM_PAGES;

    /* 新しい行をクリアしておく */
    glcd_fill_vram(0, cury, GLCD_WIDTH, base_height, 0);
}

/**
 * 文字を現在の位置に表示する。
 * カーソル移動・改行も処理される。
 */
void glcd_putchar(uint16_t c)
{
    if(c == '\r') {
	curx = 0;
	return;
    }

    if(c == '\n') {
	glcd_newline();
	return;
    }

    /* 折り返ししない設定時に、すでにカーソルが右端に来ているときは何もしない */
    if(curx >= GLCD_WIDTH && !line_wrap)
	return;

    /* 文字イメージの表示 */
    switch(font_type) {
    case FIXED8x16:
	if(c < 0x20 || c >= 0x7f)
	    return;
	glcd_write_block(curx, cury, 8, base_height,
			 font8x16 + (c - 0x20) * base_height * 8);
	curx += 8;
    }

    /* 折り返しする設定時に、カーソルが右端を超えたら改行 */
    if(curx >= GLCD_WIDTH && line_wrap)
	glcd_newline();
}

/**
 * 文字列表示
 */
void glcd_puts(const char *s)
{
    /* TODO: 文字符号化の処理 */
    while(*s)
	glcd_putchar(*s++);
}
