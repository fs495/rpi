TARGET	= glcd_test
SOURCES = glcd_test.c libglcd_sample_rpi.c
OBJECTS = $(SOURCES:%.c=%.o)

all:: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJECTS)

glcd_test.o: toho-komakyo.c

toho-komakyo.c: toho-komakyo.png
	ruby img2c.rb toho-komakyo.png > toho-komakyo.c

clean:
	rm -f *.o $(TARGET)

libglcd_sample.o: libglcd.c libglcd.h
