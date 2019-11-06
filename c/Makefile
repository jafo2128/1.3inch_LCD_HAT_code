CC      = gcc
CFLAGS  = -g -O2 -Wall -Werror

TARGET  = lcd_test key_test
OBJ     = DEV_Config.o \
          dev_hardware_SPI.o \
          font16.o \
          font20.o \
          font24.o \
          GUI_BMP.o \
          GUI_Paint.o \
          LCD_1in3.o \
          sysfs_gpio.o

all:    $(TARGET)

clean:
	rm -f *.o $(TARGET)

lcd_test: LCD_1in3_test.o $(OBJ)
	$(CC) $(LDFLAGS) LCD_1in3_test.o $(OBJ) -o $@

key_test: KEY_1in3_test.o $(OBJ)
	$(CC) $(LDFLAGS) KEY_1in3_test.o $(OBJ) -o $@
###
DEV_Config.o: DEV_Config.c DEV_Config.h Debug.h sysfs_gpio.h \
 dev_hardware_SPI.h
dev_hardware_SPI.o: dev_hardware_SPI.c dev_hardware_SPI.h
font16.o: font16.c fonts.h
font20.o: font20.c fonts.h
font24.o: font24.c fonts.h
GUI_BMP.o: GUI_BMP.c GUI_BMP.h GUI_Paint.h Debug.h LCD_1in3.h \
 DEV_Config.h sysfs_gpio.h dev_hardware_SPI.h fonts.h
GUI_Paint.o: GUI_Paint.c GUI_Paint.h Debug.h LCD_1in3.h DEV_Config.h \
 sysfs_gpio.h dev_hardware_SPI.h fonts.h
KEY_1in3_test.o: KEY_1in3_test.c DEV_Config.h Debug.h sysfs_gpio.h \
 dev_hardware_SPI.h LCD_1in3.h GUI_Paint.h fonts.h GUI_BMP.h
LCD_1in3.o: LCD_1in3.c LCD_1in3.h DEV_Config.h Debug.h sysfs_gpio.h \
 dev_hardware_SPI.h
LCD_1in3_test.o: LCD_1in3_test.c DEV_Config.h Debug.h sysfs_gpio.h \
 dev_hardware_SPI.h LCD_1in3.h GUI_Paint.h fonts.h GUI_BMP.h
sysfs_gpio.o: sysfs_gpio.c sysfs_gpio.h
