CC      = gcc
CFLAGS  = -g -O -Wall -Werror -I.

TARGET  = demo-pixel demo-line demo-rect demo-fill demo-circle demo-color \
          demo-font demo-speed flappy
OBJ     = gpio.o spi.o alt.o lcd.o
FONTS   = 5x7.o 6x9.o digits20.o digits32.o lucidasans11.o lucidasans15.o \
          lucidasans28.o lucidasans7.o lucidasans9.o verdana7.o

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

demo-circle: demo-circle.o $(OBJ)
	$(CC) $(LDFLAGS) demo-circle.o $(OBJ) -o $@

demo-color: demo-color.o $(OBJ)
	$(CC) $(LDFLAGS) demo-color.o $(OBJ) -o $@

demo-font: demo-font.o $(OBJ) $(FONTS)
	$(CC) $(LDFLAGS) demo-font.o $(OBJ) $(FONTS) -o $@

demo-speed: demo-speed.o $(OBJ) lucidasans15.o
	$(CC) $(LDFLAGS) demo-speed.o $(OBJ) lucidasans15.o -o $@

flappy: flappy.o $(OBJ) lucidasans15.o lucidasans28.o
	$(CC) $(LDFLAGS) flappy.o $(OBJ) lucidasans15.o lucidasans28.o -o $@

5x7.o:  fonts/5x7.c
	$(CC) $(CFLAGS) -o $@ -c $<

6x9.o:  fonts/6x9.c
	$(CC) $(CFLAGS) -o $@ -c $<

digits20.o: fonts/digits20.c
	$(CC) $(CFLAGS) -o $@ -c $<

digits32.o: fonts/digits32.c
	$(CC) $(CFLAGS) -o $@ -c $<

lucidasans11.o: fonts/lucidasans11.c
	$(CC) $(CFLAGS) -o $@ -c $<

lucidasans15.o: fonts/lucidasans15.c
	$(CC) $(CFLAGS) -o $@ -c $<

lucidasans28.o: fonts/lucidasans28.c
	$(CC) $(CFLAGS) -o $@ -c $<

lucidasans7.o: fonts/lucidasans7.c
	$(CC) $(CFLAGS) -o $@ -c $<

lucidasans9.o: fonts/lucidasans9.c
	$(CC) $(CFLAGS) -o $@ -c $<

verdana7.o: fonts/verdana7.c
	$(CC) $(CFLAGS) -o $@ -c $<
###
alt.o: alt.c gpio.h
demo-circle.o: demo-circle.c lcd.h
demo-color.o: demo-color.c lcd.h
demo-fill.o: demo-fill.c lcd.h
demo-font.o: demo-font.c lcd.h
demo-line.o: demo-line.c lcd.h
demo-pixel.o: demo-pixel.c lcd.h
demo-rect.o: demo-rect.c lcd.h
demo-speed.o: demo-speed.c lcd.h
gpio.o: gpio.c gpio.h
lcd.o: lcd.c lcd.h gpio.h spi.h
spi.o: spi.c spi.h
5x7.o: fonts/5x7.c lcd.h
6x9.o: fonts/6x9.c lcd.h
convbdf.o: fonts/convbdf.c
digits20.o: fonts/digits20.c lcd.h
digits32.o: fonts/digits32.c lcd.h
lucidasans11.o: fonts/lucidasans11.c lcd.h
lucidasans15.o: fonts/lucidasans15.c lcd.h
lucidasans28.o: fonts/lucidasans28.c lcd.h
lucidasans7.o: fonts/lucidasans7.c lcd.h
lucidasans9.o: fonts/lucidasans9.c lcd.h
verdana7.o: fonts/verdana7.c lcd.h
