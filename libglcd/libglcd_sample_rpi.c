/*
 * libglcdのRaspberry Pi用のサンプル実装
 * 以下のピンアサインを想定している
 *
 * P1 name   AQM1248
 * -- ------ -------
 * 13 GPIO27 RS
 * 15 GPIO22 CS#
 * 17 3.3V   VDD
 * 19 MOSI   SDI
 * 21 n/c    n/c
 * 23 SCLK   SCLK
 * 25 GND    GND
 */
#include "libglcd.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include <string.h>
#include <stdio.h>

#include "sysfs_gpio.h"

/* 液晶モジュールのCS#信号とRS信号に接続するGPIOピン番号 */
#define GPIO_CS_PIN 22
#define GPIO_RS_PIN 27

#define SPI_SPEED (1000 * 1000)
#define SPI_BITS 8
#define SPI_DELAY 0

#define HAVE_BLOCK_TRANSFER

static int gpio_cs_fd = -1;
static int gpio_rs_fd = -1;
static int spi_fd = -1;

/*----------------------------------------------------------------------*/

static int hw_spi_init()
{
    int fd, ret;
    uint32_t mode = SPI_MODE_1 | SPI_NO_CS;
    uint8_t bits = SPI_BITS;
    uint32_t speed = SPI_SPEED;

    if((fd = open("/dev/spidev0.0", O_RDWR)) < 0) {
	fprintf(stderr, "Error: hw_spi_init: open(spidev)\n");
	return -1;
    }

    ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
    if(ret < 0) {
	fprintf(stderr, "Error: ioctl(SPI_IOC_WR_MODE)\n");
	return -1;
    }

    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if(ret < 0) {
	fprintf(stderr, "Error: ioctl(SPI_IOC_WR_BITS_PER_WORD)\n");
	return -1;
    }
    ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);

    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if(ret < 0) {
	fprintf(stderr, "Error: ioctl(SPI_IOC_WR_MAX_SPEED_HZ)\n");
	return -1;
    }
    ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);

    fprintf(stderr, "Info: SPI: speed=%d, bits=%d\n", speed, bits);
    return fd;
}

void hw_init(void)
{
    gpio_cs_fd = sysfs_gpio_open("out", GPIO_CS_PIN);
    if(gpio_cs_fd < 0) {
	fprintf(stderr, "Error: hw_gpio_init(GPIO_CS_PIN)\n");
	exit(1);
    }

    gpio_rs_fd = sysfs_gpio_open("out", GPIO_RS_PIN);
    if(gpio_rs_fd < 0) {
	fprintf(stderr, "Error: hw_gpio_init(GPIO_RS_PIN)\n");
	exit(1);
    }

    spi_fd = hw_spi_init();
    if(spi_fd < 0) {
	fprintf(stderr, "Error: hw_spi_init()\n");
	exit(1);
    }
}

void hw_fini(void)
{
    if(gpio_cs_fd >= 0)
	sysfs_gpio_close(gpio_cs_fd, GPIO_CS_PIN);
    if(gpio_rs_fd >= 0)
	sysfs_gpio_close(gpio_rs_fd, GPIO_RS_PIN);
    if(spi_fd >= 0)
	close(spi_fd);

    gpio_cs_fd = gpio_rs_fd = spi_fd = -1;
}

/*----------------------------------------------------------------------*/

void glcd_connect_spi(void)
{
    if(sysfs_gpio_clr(gpio_cs_fd))
	fprintf(stderr, "glcd_connect_spi: write error\n");
}

void glcd_disconnect_spi(void)
{
    if(sysfs_gpio_set(gpio_cs_fd))
	fprintf(stderr, "glcd_disconnect_spi: write error\n");
}

void glcd_select_cmd(void)
{
    sysfs_gpio_clr(gpio_rs_fd);
}

void glcd_select_data(void)
{
    sysfs_gpio_set(gpio_rs_fd);
}

void glcd_send_byte(uint8_t byte)
{
    struct spi_ioc_transfer tr;

    memset(&tr, 0, sizeof(tr));
    tr.tx_buf = (unsigned int) &byte;
    tr.rx_buf = (unsigned int) NULL;
    tr.len    = 1;

    int ret = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &tr);
    if(ret < 0)
	fprintf(stderr, "Warn: glcd_send_byte: ioctl(SPI_IOC_MESSAGE(1))\n");
}

void glcd_send_block(const uint8_t *p, unsigned len)
{
    struct spi_ioc_transfer tr;

    memset(&tr, 0, sizeof(tr));
    tr.tx_buf = (unsigned int) p;
    tr.rx_buf = (unsigned int) NULL;
    tr.len    = len;

    int ret = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &tr);
    if(ret < 0)
	fprintf(stderr, "Warn: glcd_send_block: ioctl(SPI_IOC_MESSAGE(1))\n");
}

#include "libglcd.c"
