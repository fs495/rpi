#define CONFIG_IMAGE_TEST 0
#define CONFIG_BAR_GRAPH 1
#define CONFIG_RAW_FONT_TEST 0
#define CONFIG_FONT 1

#include "libglcd.h"

#if defined(__AVR__)
# define msleep(x) _delay_ms(x)
#elif defined(__linux__)
# define PROGMEM
# define msleep(x) usleep((x) * 1000)
#endif

#ifdef CONFIG_IMAGE_TEST
# include "toho-komakyo.c"
#endif

int main(int argc, char *argv[])
{
    hw_init();

    glcd_connect_spi();
    glcd_init();
    glcd_disconnect_spi();

    for(;;) {
	uint8_t i;
	const char *msgs[] = {
	    "One,", "Two,", "Three,", "Four,", "Five,",
	    "Six,", "Seven,", "Eight,", "Nine,", "Ten.",
	};

#if CONFIG_IMAGE_TEST
	/* イメージを表示してスクロール */
	glcd_connect_spi();
	glcd_write_block(0, 0, IMG_TOHO_KOMAKYO_WIDTH,
			 IMG_TOHO_KOMAKYO_HEIGHT / 8,
			 img_toho_komakyo);
	for(i = 0; i < GLCD_VRAM_HEIGHT; i++) {
	    glcd_set_display_row(i);
	    msleep(50);
	}
	glcd_disconnect_spi();
#endif

#if CONFIG_BAR_GRAPH
	glcd_connect_spi();
	glcd_set_display_row(0);
	for(i = 0; i < GLCD_WIDTH; i++) {
	    glcd_fill_vram(i, 1, 1, 4, 255);
	    msleep(5);
	}
	for(i = 0; i < GLCD_WIDTH; i++) {
	    glcd_fill_vram(i, 1, 1, 4, 0);
	    msleep(5);
	}
	glcd_disconnect_spi();
#endif

#if CONFIG_RAW_FONT_TEST
	/* 生のフォントデータ、ブロック転送 */
	glcd_connect_spi();
	glcd_write_blockp(64, 0, 8, 6, font8x16 + 16);
	for(i = 0; i < 16 * 3; i++) {
	    glcd_write_block(8 * (i % 16), 2 * (i / 16), 8, 2,
			     font8x16 + i * 16);
	}
	glcd_disconnect_spi();
	msleep(1000);
#endif

#if CONFIG_FONT
	/* フォント込みのスクリーン表示 */
	glcd_config_screen(FIXED8x16);
	glcd_line_wrap(1);

	glcd_connect_spi();
	glcd_clear_screen();

	/* 自動改行 */
	for(i = 0; i < sizeof(msgs) / sizeof(*msgs); i++) {
	    glcd_puts(msgs[i]);
	    msleep(500);
	}
	/* 明示的改行 */
	for(i = 0; i < sizeof(msgs) / sizeof(*msgs); i++) {
	    glcd_putchar('\n');
	    glcd_puts(msgs[i]);
	    msleep(500);
	}
	glcd_putchar('\n');

	for(i = 0; i < 100; i++) {
	    char buf[64];
	    sprintf(buf, "%d,", i);
	    glcd_puts(buf);
	    msleep(50);
	}
	glcd_disconnect_spi();
#endif
    }
}
