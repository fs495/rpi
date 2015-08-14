TARGET	= glcd_test
SOURCES = glcd_test.c \
	libglcd_sample_rpi.c sysfs_gpio.c \
	libglcd_font.c font8x16.c
OBJECTS = $(SOURCES:%.c=%.o)

all:: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJECTS)

glcd_test.o: glcd_test.c toho-komakyo.c

libglcd_sample_rpi.o: libglcd_sample_rpi.c libglcd_impl.c libglcd.h

toho-komakyo.c: toho-komakyo.png
	ruby img2c.rb toho-komakyo.png > toho-komakyo.c

clean:
	rm -f *.o $(TARGET)

