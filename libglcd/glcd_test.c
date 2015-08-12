#define CONFIG_IMAGE_TEST 0
#define CONFIG_FONT_TEST 0
#define CONFIG_SCREEN 1

#include "libglcd.h"

#ifdef CONFIG_IMAGE_TEST
# ifndef __AVR__
#  define PROGMEM
# endif
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

#if CONFIG_IMAGE_TEST
	glcd_connect_spi();
	glcd_write_block(0, 0, IMG_TOHO_KOMAKYO_WIDTH,
			 IMG_TOHO_KOMAKYO_HEIGHT / 8,
			 img_toho_komakyo);
	glcd_disconnect_spi();
	sleep(1);
#endif

#if CONFIG_FONT_TEST
	glcd_connect_spi();
	glcd_write_blockp(64, 0, 8, 6, font8x16 + 16);
	for(i = 0; i < 16 * 3; i++) {
	    glcd_write_block(8 * (i % 16), 2 * (i / 16), 8, 2,
			     font8x16 + i * 16);
	}
	glcd_disconnect_spi();
	sleep(1);
#endif

#if CONFIG_SCREEN
	for(i = 0; i < 100; i++) {
	    glcd_connect_spi();
	    glcd_line_wrap(1);
	    glcd_puts("\nHello, ");
	    glcd_putchar(i / 10 + '0');
	    glcd_putchar(i % 10 + '0');
	    glcd_disconnect_spi();
	    usleep(500 * 1000);
	}
#endif
    }
}
