#ifndef _rpi_gpio_h
#define _rpi_gpio_h

/*
 * GPIO
 */
#define GPIO_OFFSET 0x00200000	/* offset from I/O peripheral area */
#define GPIO_SEGSIZE 0xb4	/* mmap size */

/* convert bus address to offset within GPIO area */
#define BUS_TO_OFFSET(a)	((a) - (IOP_BUS_BASE + GPIO_OFFSET))

/*
 * GPIO register offsets
 * - based on BCM2836 datasheet
 * - not sure these can be applied to other models
 */
enum gpio_reg_offset {
    GPFSEL0 = BUS_TO_OFFSET(0x7e200000), /* GPIO Function Select 0 32 R/W */
    GPFSEL1 = BUS_TO_OFFSET(0x7e200004), /* GPIO Function Select 1 32 R/W */
    GPFSEL2 = BUS_TO_OFFSET(0x7e200008), /* GPIO Function Select 2 32 R/W */
    GPFSEL3 = BUS_TO_OFFSET(0x7e20000c), /* GPIO Function Select 3 32 R/W */
    GPFSEL4 = BUS_TO_OFFSET(0x7e200010), /* GPIO Function Select 4 32 R/W */
    GPFSEL5 = BUS_TO_OFFSET(0x7e200014), /* GPIO Function Select 5 32 R/W */
    GPSET0 = BUS_TO_OFFSET(0x7e20001c), /* GPIO Pin Output Set 0 32 W */
    GPSET1 = BUS_TO_OFFSET(0x7e200020), /* GPIO Pin Output Set 1 32 W */
    GPCLR0 = BUS_TO_OFFSET(0x7e200028), /* GPIO Pin Output Clear 0 32 W */
    GPCLR1 = BUS_TO_OFFSET(0x7e20002c), /* GPIO Pin Output Clear 1 32 W */
    GPLEV0 = BUS_TO_OFFSET(0x7e200034), /* GPIO Pin Level 0 32 R */
    GPLEV1 = BUS_TO_OFFSET(0x7e200038), /* GPIO Pin Level 1 32 R */
    GPEDS0 = BUS_TO_OFFSET(0x7e200040), /* GPIO Pin Event Detect Status 0 32 R/W */
    GPEDS1 = BUS_TO_OFFSET(0x7e200044), /* GPIO Pin Event Detect Status 1 32 R/W */
    GPREN0 = BUS_TO_OFFSET(0x7e20004c), /* GPIO Pin Rising Edge Detect Enable 0 32 R/W */
    GPREN1 = BUS_TO_OFFSET(0x7e200050), /* GPIO Pin Rising Edge Detect Enable 1 32 R/W */
    GPFEN0 = BUS_TO_OFFSET(0x7e200058), /* GPIO Pin Falling Edge Detect Enable 0 32 R/W */
    GPFEN1 = BUS_TO_OFFSET(0x7e20005c), /* GPIO Pin Falling Edge Detect Enable 1 32 R/W */
    GPHEN0 = BUS_TO_OFFSET(0x7e200064), /* GPIO Pin High Detect Enable 0 32 R/W */
    GPHEN1 = BUS_TO_OFFSET(0x7e200068), /* GPIO Pin High Detect Enable 1 32 R/W */
    GPLEN0 = BUS_TO_OFFSET(0x7e200070), /* GPIO Pin Low Detect Enable 0 32 R/W */
    GPLEN1 = BUS_TO_OFFSET(0x7e200074), /* GPIO Pin Low Detect Enable 1 32 R/W */
    GPAREN0 = BUS_TO_OFFSET(0x7e20007c), /* GPIO Pin Async. Rising Edge Detect 0 32 R/W */
    GPAREN1 = BUS_TO_OFFSET(0x7e200080), /* GPIO Pin Async. Rising Edge Detect 1 32 R/W */
    GPAFEN0 = BUS_TO_OFFSET(0x7e200088), /* GPIO Pin Async. Falling Edge Detect 0 32 R/W */
    GPAFEN1 = BUS_TO_OFFSET(0x7e20008c), /* GPIO Pin Async. Falling Edge Detect 1 32 R/W */
    GPPUD = BUS_TO_OFFSET(0x7e200094), /* GPIO Pin Pull-up/down Enable 32 R/W */
    GPPUDCLK0 = BUS_TO_OFFSET(0x7e200098), /* GPIO Pin Pull-up/down Enable Clock 0 32 R/W */
    GPPUDCLK1 = BUS_TO_OFFSET(0x7e20009c), /* GPIO Pin Pull-up/down Enable Clock 1 32 R/W */
};

#undef BUS_TO_OFFSET

/* GPFSELn register */
enum gpio_function {
    GPFSEL_INPUT = 0,
    GPFSEL_OUTPUT = 1,
    GPFSEL_ALT0 = 4,
    GPFSEL_ALT1 = 5,
    GPFSEL_ALT2 = 6,
    GPFSEL_ALT3 = 7,
    GPFSEL_ALT4 = 3,
    GPFSEL_ALT5 = 2,
};

#endif /* _rpi_gpio_h */
