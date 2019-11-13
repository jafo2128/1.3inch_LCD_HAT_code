CC      = gcc
CFLAGS  = -g -O2 -Wall -Werror

TARGET  = lcd-demo
OBJ     = gpio.o spi.o alt.o lcd.o

all:    $(TARGET)

clean:
	rm -f *.o $(TARGET)

lcd-demo: lcd-demo.o $(OBJ)
	$(CC) $(LDFLAGS) lcd-demo.o $(OBJ) -o $@
###
