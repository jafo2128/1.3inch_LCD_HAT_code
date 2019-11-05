# USELIB = USE_BCM2835_LIB
# USELIB = USE_WIRINGPI_LIB
USELIB = USE_DEV_LIB

CC      = gcc
CFLAGS  = -g -O2 -Wall -Werror -D$(USELIB)

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

ifeq ($(USELIB), USE_BCM2835_LIB)
    LIB = -lbcm2835 -lm
else ifeq ($(USELIB), USE_WIRINGPI_LIB)
    LIB = -lwiringPi -lm -lpthread -lcrypt -lrt
else
    LIB =
endif

all:    $(TARGET)

clean:
	rm -f *.o $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) $(LIB) -o $@
