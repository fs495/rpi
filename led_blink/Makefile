
TARGETS	= blink_mmap blink_sysfs
CFLAGS	= -DCONFIG_RASPBERRY_PI2 -O2

all: $(TARGETS)

blink_mmap: blink_mmap.c
	$(CC) $(CFLAGS) -o $@ blink_mmap.c

blink_sysfs: blink_sysfs.c
	$(CC) $(CFLAGS) -o $@ blink_sysfs.c

clean:
	rm -f $(TARGETS)
