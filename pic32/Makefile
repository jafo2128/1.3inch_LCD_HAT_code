CC      = gcc
CFLAGS  = -g -O2 -Wall -Werror

TARGET  = demo-pixel demo-line demo-rect demo-fill
OBJ     = gpio.o spi.o alt.o lcd.o

all:    $(TARGET)

clean:
	rm -f *.o $(TARGET)

demo-pixel: demo-pixel.o $(OBJ)
	$(CC) $(LDFLAGS) demo-pixel.o $(OBJ) -o $@

demo-line: demo-line.o $(OBJ)
	$(CC) $(LDFLAGS) demo-line.o $(OBJ) -o $@

demo-rect: demo-rect.o $(OBJ)
	$(CC) $(LDFLAGS) demo-rect.o $(OBJ) -o $@

demo-fill: demo-fill.o $(OBJ)
	$(CC) $(LDFLAGS) demo-fill.o $(OBJ) -o $@
###
alt.o: alt.c gpio.h
demo-line.o: demo-line.c lcd.h
demo-pixel.o: demo-pixel.c lcd.h
demo-rect.o: demo-rect.c lcd.h
demo-fill.o: demo-fill.c lcd.h
gpio.o: gpio.c gpio.h
lcd.o: lcd.c lcd.h gpio.h spi.h
spi.o: spi.c spi.h
