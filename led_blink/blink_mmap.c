#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "rpi_iop.h"
#include "rpi_gpio.h"

static volatile void *gpio_base = 0;

int gpio_init(void)
{
    int fd;

    if(gpio_base != 0)
	return 1;
    if((fd = open("/dev/mem", O_RDWR | O_SYNC)) < 0)
	return 1;
    gpio_base = mmap(NULL, GPIO_SEGSIZE, PROT_READ | PROT_WRITE, MAP_SHARED,
		     fd, IOP_PHYS_BASE + GPIO_OFFSET);
    if((int)gpio_base == -1)
	return 1;
    close(fd);
    return 0;
}

void gpio_fini(void)
{
    munmap((void*)gpio_base, GPIO_SEGSIZE);
    gpio_base = 0;
}

void gpio_select_func(unsigned pin, unsigned func)
{
    unsigned index = pin / 10;
    unsigned pos = (pin % 10) * 3;
    volatile uint32_t *gpfsel = IOPREGI(gpio_base, GPFSEL0, index);
    *gpfsel = (*gpfsel & ~(7 << pos)) | (func << pos);
}

#define PIN 2

int main(int argc, char *argv[])
{
    if(gpio_init()) {
	fprintf(stderr, "error: gpio_init");
	exit(1);
    }

    gpio_select_func(PIN, GPFSEL_OUTPUT);
    for(;;) {
	*IOPREGI(gpio_base, GPSET0, PIN / 32) |= 1 << (PIN % 32);
	*IOPREGI(gpio_base, GPCLR0, PIN / 32) |= 1 << (PIN % 32);
    }
}
