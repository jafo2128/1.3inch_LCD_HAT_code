CC      = gcc
CFLAGS  = -g -O2 -Wall -Werror

TARGET  = lcd_test bmp_test key_test
OBJ     = bmp.o \
          font16.o \
          font20.o \
          font24.o \
          gpio.o \
          lcd.o \
          paint.o \
          spi.o

all:    $(TARGET)

clean:
	rm -f *.o $(TARGET)

lcd_test: lcd_test.o $(OBJ)
	$(CC) $(LDFLAGS) lcd_test.o $(OBJ) -o $@

bmp_test: bmp_test.o $(OBJ)
	$(CC) $(LDFLAGS) bmp_test.o $(OBJ) -o $@

key_test: key_test.o $(OBJ)
	$(CC) $(LDFLAGS) key_test.o $(OBJ) -o $@
###
bmp.o: bmp.c bmp.h paint.h fonts.h
bmp_test.o: bmp_test.c lcd.h paint.h fonts.h bmp.h
font16.o: font16.c fonts.h
font20.o: font20.c fonts.h
font24.o: font24.c fonts.h
gpio.o: gpio.c gpio.h
key_test.o: key_test.c pins.h lcd.h paint.h fonts.h gpio.h
lcd.o: lcd.c lcd.h pins.h gpio.h spi.h
lcd_test.o: lcd_test.c lcd.h paint.h fonts.h
paint.o: paint.c paint.h fonts.h lcd.h
spi.o: spi.c spi.h
