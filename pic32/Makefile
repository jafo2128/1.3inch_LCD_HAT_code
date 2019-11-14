CC      = gcc
CFLAGS  = -g -O2 -Wall -Werror

TARGET  = demo-pixel
OBJ     = gpio.o spi.o alt.o lcd.o

all:    $(TARGET)

clean:
	rm -f *.o $(TARGET)

demo-pixel: demo-pixel.o $(OBJ)
	$(CC) $(LDFLAGS) demo-pixel.o $(OBJ) -o $@
###
alt.o: alt.c gpio.h
demo-pixel.o: demo-pixel.c lcd.h
gpio.o: gpio.c gpio.h
lcd.o: lcd.c lcd.h gpio.h spi.h
spi.o: spi.c spi.h
