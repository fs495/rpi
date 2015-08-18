/**
 * グラフィック液晶モジュールAQM1248のテンプレートライブラリ
 *
 * 実装時には、システムごとに以下の低水準関数を定義する。
 * (1) glcd_connect_spi()	AQM1248に対してSPI接続する。SS信号をアサート。
 * (2) glcd_disconnect_spi()	AQM1248に対してSPI接続を解除する
 * (3) glcd_select_cmd()	コマンド送信状態(RS=L)にする
 * (4) glcd_select_data()	データ送信状態(RS=H)にする
 * (5) glcd_send_byte()		データ/コマンドを出力する
 * (6) glcd_send_block()	データ/コマンドを出力する(オプション)
 *
 * SPI通信を行なうAPI関数はglcd_connect_spi()とglcd_disconnect_spi()の
 * 呼び出しの間に呼びださなければならない。
 * 
 * ライブラリ内ではデータ送信とコマンド送信を随時切り替える。
 * ユーザ側はデータ送信状態にして、glcd_send_byte()でデータやコマンドを送信しても
 * 構わないが、API関数呼び出し時はコマンド送信状態でなければならない。
 */
#ifndef __LIBGLCD_H__
#define __LIBGLCD_H__

#include <stdint.h>

/*
 * 液晶の表示サイズ=128x48ドット
 * VRAMのサイズ=128x64ドット
 * 縦方向に8ドットずつ、8つのエリアに分割し、それぞれを「ページ」とする。
 */
#define GLCD_WIDTH 128
#define GLCD_VRAM_HEIGHT 64
#define GLCD_VIEW_HEIGHT 48
#define GLCD_VRAM_PAGES (GLCD_VRAM_HEIGHT / 8)
#define GLCD_VIEW_PAGES (GLCD_VIEW_HEIGHT / 8)

/*
 * ハードウェア制御関数。使用側で実装を提供する。
 */
void glcd_connect_spi(void);
void glcd_disconnect_spi(void);
void glcd_select_cmd(void);
void glcd_select_data(void);
void glcd_send_byte(uint8_t byte);

/*
 * 低レベルAPI
 */
/* 初期化 */
void glcd_init(void);
/* 表示を有効化 */
void glcd_display_on(void);
/* 表示を停止 */
void glcd_display_off(void);
/* 表示を開始する縦位置を設定(ドット単位) */
void glcd_set_display_row(uint8_t row);
/* 書き込みを開始する縦位置を設定(ページ単位) */
void glcd_set_addr_page(uint8_t page);
/* 書き込みを開始する横位置を設定(ドット単位) */
void glcd_set_addr_col(uint8_t col);
/* コントラストを設定 */
void glcd_set_resistor_ratio(uint8_t val);
/* コントラストを設定 */
void glcd_set_contrast(uint8_t val);
/* スリープモードに入る */
void glcd_set_sleep_mode(void);
/* スリープモードから抜ける */
void glcd_leave_sleep_mode(void);

/*
 * ブロック書き込み・ブロックフィルAPI
 * ブロックデータを(sx,sy)の位置から、横wドット、縦hページ分書き込む。
 * sxとwの単位はドット、syとhの単位はページ。
 */
void glcd_write_block(uint8_t sx, uint8_t sy, uint8_t w, uint8_t h,
		      const uint8_t *p);
void glcd_write_blockp(uint8_t sx, uint8_t sy, uint8_t w, uint8_t h,
		       const uint8_t *p);
void glcd_fill_vram(uint8_t sx, uint8_t sy, uint8_t w, uint8_t h, uint8_t ptn);
void glcd_clear_vram(void);

/*
 * フォントAPI
 */
enum font_types {
    FIXED8x16,
};

/* フォントを設定する */
uint8_t glcd_config_font(uint8_t ft);
/* 右端を超えた時に行を折り返すか指定する */
void glcd_line_wrap(uint8_t enabled);
/* 画面クリアし、文字表示位置を初期化する */
void glcd_clear_screen(void);
/* 文字を現在の位置に表示する */
void glcd_putchar(uint16_t c);
/* 文字列表示 */
void glcd_puts(const char *s);

extern const unsigned char font8x16[];

#endif /* __LIBGLCD_H__ */
