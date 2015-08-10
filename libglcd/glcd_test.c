#include "libglcd.h"

#ifndef __AVR__
# define PROGMEM
#endif

#define CONFIG_IMAGE_TEST 1
#define CONFIG_FONT_TEST 1

#ifdef CONFIG_IMAGE_TEST
# include "toho-komakyo.c"
#endif
#ifdef CONFIG_FONT_TEST
#include "../font/font8x16.c"
#endif

int main(int argc, char *argv[])
{
    hw_init();

    glcd_connect_spi();
    glcd_init();
    glcd_disconnect_spi();

    for(;;) {
	uint8_t i;

#ifdef CONFIG_IMAGE_TEST
	glcd_connect_spi();
	glcd_write_block(0, 0, IMG_TOHO_KOMAKYO_WIDTH,
			 IMG_TOHO_KOMAKYO_HEIGHT / 8,
			 img_toho_komakyo);
	glcd_disconnect_spi();
	sleep(1);
#endif

#ifdef CONFIG_FONT_TEST
	glcd_connect_spi();
	glcd_write_blockp(64, 0, 8, 6, font8x16 + 16);
	for(i = 0; i < 16 * 3; i++) {
	    glcd_write_block(8 * (i % 16), 2 * (i / 16), 8, 2,
			     font8x16 + i * 16);
	}
	glcd_disconnect_spi();
	sleep(1);
#endif
    }
}
