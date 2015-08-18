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
    case ASCII7_8x16:
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
    case ASCII7_8x16:
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

#define ISO2022_SS2 0x8e /* G2->GL */
#define ISO2022_SS3 0x8f /* G3->GL */

/**
 * 文字列表示
 */
void glcd_puts(const char *s)
{
    while(s[0]) {
	switch(font_type) {
	case ASCII7_8x16:
	    /* 常に1バイト=1文字と想定し、文字出力する */
	    glcd_putchar(s[0]);
	    s += 1;
	    break;

	case EUCJP_8x16:
	    /* 第1バイトの第7ビットが0の場合はASCII, 
	     * 第1バイトがSS2の場合はJIS X0201(半角カナ)、
	     * それ以外はJIS X0208と決め打ちする。
	     * 不正コード、補助漢字は扱わない */
	    if(s[0] & 0x80 == 0) {
		glcd_putchar(s[0]);
		s += 1;
	    } else {
		glcd_putchar((s[0] << 8) | s[1]);
		s += 2;
	    }
	    break;

	case UTF8_8x16:
	    /* UTF-8符号化に従ってデコードする。
	     * 第1バイトだけでバイト数を決め打ちし、
	     * 不正コードは特にチェックしない。
	     * UCS-2に収まらないコードポイントは単に読み捨てる。
	     * デコードされたコードポイントがすべて表示できるわけではない。*/
	    if(s[0] <= 0x7f) {
		/* 0xxxxxxx */
		glcd_putchar(s[0]);
		s += 1;
	    } else if(s[0] <= 0xdf) {
		/* 110yyyyx 10xxxxxx */
		glcd_putchar((s[0] & 0x1f) << 6 | (s[1] & 0x3f));
		s += 2;
	    } else if(s[0] <= 0xef) {
		/* 1110yyyy 10yxxxxx 10xxxxxx  */
		glcd_putchar((s[0] & 0x0f) << 12
			     | (s[1] & 0x3f) << 6 | (s[2] & 0x3f));
		s += 3;
	    } else if(s[0] <= 0xf7) {
		/* 11110yyy 10yyxxxx 10xxxxxx 10xxxxxx */
		s += 4;
	    } else if(s[0] <= 0xfb) {
		/* 111110yy 10yyyxxx 10xxxxxx 10xxxxxx 10xxxxxx */
		s += 5;
	    } else if(s[0] <= 0xfd) {
		/* 1111110y 10yyyyxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx */
		s += 6;
	    } else {
		s += 1;
	    }
	default:
	    s += 1;
	}
    }
}


uint32_t utf8tounicode(const uint8_t *p)
{
    uint32_t u;

}
