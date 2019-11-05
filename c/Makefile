CC      = gcc
CFLAGS  = -g -O2 -Wall -Werror

TARGET  = demo
OBJ     = DEV_Config.o \
          dev_hardware_SPI.o \
          font16.o \
          font20.o \
          font24.o \
          GUI_BMP.o \
          GUI_Paint.o \
          KEY_1in3_test.o \
          KEY_APP.o \
          LCD_1in3.o \
          LCD_1in3_test.o \
          main.o \
          sysfs_gpio.o

all:    $(TARGET)

clean:
	rm -f *.o $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o $@
