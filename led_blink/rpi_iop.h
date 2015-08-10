#ifndef _rpi_iop_h
#define _rpi_iop_h

/*
 * bus and physical base address of I/O peripherals
 */
#define IOP_BUS_BASE 0x7e000000
#if defined(CONFIG_RASPBERRY_PI2)
# define IOP_PHYS_BASE 0x3f000000
#elif defined(CONFIG_RASPBERRY_PI)
# define IOP_PHYS_BASE 0x20000000
#else
# error "must define CONFIG_RASPBERRY_PI or CONFIG_RASPBERRY_PI2"
#endif

/* macros for register access */
#define IOPREG(base, regoff)	((volatile uint32_t *)((base) + (regoff)))
#define IOPREGI(base, regoff, idxoff)	(IOPREG((base), (regoff)) + (idxoff))

#endif /* _rpi_iop_h */
