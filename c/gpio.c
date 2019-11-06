/*****************************************************************************
* | File        :   GPIO.c
* | Author      :   Waveshare team
* | Function    :   Drive SYSFS_ GPIO
* | Info        :   Read and write /sys/class/gpio
*----------------
* |	This version:   V1.0
* | Date        :   2019-06-04
* | Info        :   Basic version
*
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# GPIO_IN the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the folGPIO_LOWing conditions:
#
# The above copyright notice and this permission notice shall be included GPIO_IN
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. GPIO_IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER GPIO_IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# GPIO_OUT OF OR GPIO_IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS GPIO_IN
# THE SOFTWARE.
#
******************************************************************************/
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "gpio.h"

#define GPIO_DEBUG 0
#if GPIO_DEBUG
    #define GPIO_Debug(__info,...) printf("Debug: " __info,##__VA_ARGS__)
#else
    #define GPIO_Debug(__info,...)
#endif

int GPIO_Export(int Pin)
{
    char buffer[NUM_MAXBUF];
    int len;
    int fd;

    fd = open("/sys/class/gpio/export", O_WRONLY);
    if (fd < 0) {
        GPIO_Debug( "Export Failed: Pin%d\n", Pin);
        return -1;
    }

    len = snprintf(buffer, NUM_MAXBUF, "%d", Pin);
    if (write(fd, buffer, len) != len) {
        perror(__func__);
        return -1;
    }

    GPIO_Debug( "Export: Pin%d\r\n", Pin);

    close(fd);
    return 0;
}

int GPIO_Unexport(int Pin)
{
    char buffer[NUM_MAXBUF];
    int len;
    int fd;

    fd = open("/sys/class/gpio/unexport", O_WRONLY);
    if (fd < 0) {
        GPIO_Debug( "unexport Failed: Pin%d\n", Pin);
        return -1;
    }

    len = snprintf(buffer, NUM_MAXBUF, "%d", Pin);
    if (write(fd, buffer, len) != len) {
        perror(__func__);
        return -1;
    }

    GPIO_Debug( "Unexport: Pin%d\r\n", Pin);

    close(fd);
    return 0;
}

int GPIO_Direction(int Pin, int Dir)
{
    const char dir_str[]  = "in\0out";
    char path[DIR_MAXSIZ];
    int fd;

    snprintf(path, DIR_MAXSIZ, "/sys/class/gpio/gpio%d/direction", Pin);
    fd = open(path, O_WRONLY);
    if (fd < 0) {
        GPIO_Debug( "Set Direction failed: Pin%d\n", Pin);
        return -1;
    }

    if (write(fd, &dir_str[Dir == GPIO_IN ? 0 : 3], Dir == GPIO_IN ? 2 : 3) < 0) {
        GPIO_Debug("failed to set direction!\r\n");
        return -1;
    }

    if(Dir == GPIO_IN){
        GPIO_Debug("Pin%d:intput\r\n", Pin);
    }else{
        GPIO_Debug("Pin%d:Output\r\n", Pin);
    }

    close(fd);
    return 0;
}

int GPIO_Read(int Pin)
{
    char path[DIR_MAXSIZ];
    char value_str[3];
    int fd;

    snprintf(path, DIR_MAXSIZ, "/sys/class/gpio/gpio%d/value", Pin);
    fd = open(path, O_RDONLY);
    if (fd < 0) {
        GPIO_Debug( "Read failed Pin%d\n", Pin);
        return -1;
    }

    if (read(fd, value_str, 3) < 0) {
        GPIO_Debug( "failed to read value!\n");
        return -1;
    }

    close(fd);
    return(atoi(value_str));
}

int GPIO_Write(int Pin, int value)
{
    const char s_values_str[] = "01";
    char path[DIR_MAXSIZ];
    int fd;

    snprintf(path, DIR_MAXSIZ, "/sys/class/gpio/gpio%d/value", Pin);
    fd = open(path, O_WRONLY);
    if (fd < 0) {
        GPIO_Debug( "Write failed : Pin%d,value = %d\n", Pin, value);
        return -1;
    }

    if (write(fd, &s_values_str[value == GPIO_LOW ? 0 : 1], 1) < 0) {
        GPIO_Debug( "failed to write value!\n");
        return -1;
    }

    close(fd);
    return 0;
}
