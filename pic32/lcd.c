/*
 * Interface to a color LCD display based on ST7789 controller.
 *
 * Copyright (C) 2019 Serge Vakulenko
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *   3. The name of the author may not be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "lcd.h"
#include "gpio.h"
#include "spi.h"

//
// Hardware pins on GPIO connector of PIC32MZ DA Starter Kit.
//
// Outputs
#define PIN_LCD_CS      GPIO_PIN('D', 0)    // p8  - RD0 - j24
#define PIN_LCD_RST     GPIO_PIN('B', 8)    // p27 - RB8 - j13
#define PIN_LCD_DC      GPIO_PIN('H', 6)    // p25 - RH6 - j22
#define PIN_LCD_BL      GPIO_PIN('H', 4)    // p24 - RH4 - j18

// Inputs
#define PIN_KEY_UP      GPIO_PIN('K', 2)    // p6  - RK2  - j31
#define PIN_KEY_DOWN    GPIO_PIN('B', 0)    // p19 - RB0  - j35
#define PIN_KEY_LEFT    GPIO_PIN('K', 1)    // p5  - RK1  - j29
#define PIN_KEY_RIGHT   GPIO_PIN('H', 7)    // p26 - RH7  - j37
#define PIN_KEY_PRESS   GPIO_PIN('G', 9)    // p13 - RG9  - j33
#define PIN_KEY1        GPIO_PIN('D', 15)   // p21 - RD15 - j40
#define PIN_KEY2        GPIO_PIN('H', 12)   // p20 - RH12 - j38
#define PIN_KEY3        GPIO_PIN('B', 15)   // p16 - RB15 - j36

static int lcd_width, lcd_height;

//
// Hardware reset
//
static void lcd_reset()
{
    gpio_write(PIN_LCD_RST, 1);
    usleep(10000);
    gpio_write(PIN_LCD_RST, 0);
    usleep(10000);
    gpio_write(PIN_LCD_RST, 1);
    usleep(10000);
}

//
// Send command
//
static void lcd_send_command(uint8_t reg)
{
    gpio_write(PIN_LCD_DC, 0);
    //gpio_write(PIN_LCD_CS, 0);
    spi_transfer(reg);
    //gpio_write(PIN_LCD_CS, 1);
}

//
// Send data
//
static int lcd_send_data(uint8_t data)
{
    gpio_write(PIN_LCD_DC, 1);
    return spi_transfer(data);
}

//
// Initialize the display.
//
void lcd_start_st7789(int vertical_flag)
{
    // Turn on the backlight
    gpio_write(PIN_LCD_BL, 1);

    // Hardware reset
    lcd_reset();

    // Set the resolution and scanning method of the screen
    lcd_send_command(0x36);          // MX, MY, RGB mode
    if (vertical_flag) {
        lcd_send_data(0x00);
    } else {
        lcd_send_data(0x70);
    }

    // Initialize the LCD registers
    lcd_send_command(0x3A);
    lcd_send_data(0x05);

    lcd_send_command(0xB2);
    lcd_send_data(0x0C);
    lcd_send_data(0x0C);
    lcd_send_data(0x00);
    lcd_send_data(0x33);
    lcd_send_data(0x33);

    lcd_send_command(0xB7);  // Gate Control
    lcd_send_data(0x35);

    lcd_send_command(0xBB);  // VCOM Setting
    lcd_send_data(0x19);

    lcd_send_command(0xC0);  // LCM Control
    lcd_send_data(0x2C);

    lcd_send_command(0xC2);  // VDV and VRH Command Enable
    lcd_send_data(0x01);
    lcd_send_command(0xC3);  // VRH Set
    lcd_send_data(0x12);
    lcd_send_command(0xC4);  // VDV Set
    lcd_send_data(0x20);

    lcd_send_command(0xC6);  // Frame Rate Control in Normal Mode
    lcd_send_data(0x0F);

    lcd_send_command(0xD0);  // Power Control 1
    lcd_send_data(0xA4);
    lcd_send_data(0xA1);

    lcd_send_command(0xE0);  // Positive Voltage Gamma Control
    lcd_send_data(0xD0);
    lcd_send_data(0x04);
    lcd_send_data(0x0D);
    lcd_send_data(0x11);
    lcd_send_data(0x13);
    lcd_send_data(0x2B);
    lcd_send_data(0x3F);
    lcd_send_data(0x54);
    lcd_send_data(0x4C);
    lcd_send_data(0x18);
    lcd_send_data(0x0D);
    lcd_send_data(0x0B);
    lcd_send_data(0x1F);
    lcd_send_data(0x23);

    lcd_send_command(0xE1);  // Negative Voltage Gamma Control
    lcd_send_data(0xD0);
    lcd_send_data(0x04);
    lcd_send_data(0x0C);
    lcd_send_data(0x11);
    lcd_send_data(0x13);
    lcd_send_data(0x2C);
    lcd_send_data(0x3F);
    lcd_send_data(0x44);
    lcd_send_data(0x51);
    lcd_send_data(0x2F);
    lcd_send_data(0x1F);
    lcd_send_data(0x1F);
    lcd_send_data(0x20);
    lcd_send_data(0x23);

    lcd_send_command(0x21);  // Display Inversion On
    lcd_send_command(0x11);  // Sleep Out
    lcd_send_command(0x29);  // Display On
}

//
// Set the start position and size of the display area
//
static void lcd_set_window(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end)
{
    // set the X coordinates
    lcd_send_command(0x2A);
    lcd_send_data((uint8_t) (x_start >> 8));
    lcd_send_data((uint8_t) x_start);
    lcd_send_data((uint8_t) (x_end >> 8));
    lcd_send_data((uint8_t) x_end);

    // set the Y coordinates
    lcd_send_command(0x2B);
    lcd_send_data((uint8_t) (y_start >> 8));
    lcd_send_data((uint8_t) y_start);
    lcd_send_data((uint8_t) (y_end >> 8));
    lcd_send_data((uint8_t) y_end);

    lcd_send_command(0x2c);
}

//
// Paint a pixel.
//
void lcd_pixel(int color, int x, int y)
{
    if (x < 0 || x >= lcd_width || y < 0 || y >= lcd_height)
        return;

    lcd_set_window(x, y, x, y);
    lcd_send_data((uint8_t) (color >> 8));
    lcd_send_data((uint8_t) color);
}

//
// Clear screen.
//
void lcd_clear(int color)
{
    lcd_fill(color, 0, 0, lcd_width - 1, lcd_height - 1);
}

//
// Fill rectangle.
//
void lcd_fill(int color, int x0, int y0, int x1, int y1)
{
    unsigned npixels = x1 + 1 - x0;
    uint16_t data[npixels];
    int j;

    color = ((uint8_t)color << 8) | (uint8_t)(color >> 8);

    for (j = 0; j < npixels; j++) {
        data[j] = color;
    }

    lcd_set_window(x0, y0, x1, y1);
    gpio_write(PIN_LCD_DC, 1);
    for (j = y0; j <= y1; j++) {
        spi_bulk_write((uint8_t*) &data[0], npixels * 2);
    }
}

//
// Initialize the pins.
//
static void lcd_setup_gpio_spi()
{
    // Re-map U2RX pin from p19 to p3, temporarily.
    gpio_set_mode(GPIO_PIN('F', 8), MODE_U2RX);

    // Output pins.
    gpio_set_mode(PIN_LCD_CS,  MODE_OUTPUT);
    gpio_set_mode(PIN_LCD_RST, MODE_OUTPUT);
    gpio_set_mode(PIN_LCD_DC,  MODE_OUTPUT);
    gpio_set_mode(PIN_LCD_BL,  MODE_OUTPUT);
    gpio_write(PIN_LCD_CS, 1);

    // Input pins.
    gpio_set_mode(PIN_KEY_UP,    MODE_INPUT);
    gpio_set_mode(PIN_KEY_DOWN,  MODE_INPUT);
    gpio_set_mode(PIN_KEY_LEFT,  MODE_INPUT);
    gpio_set_mode(PIN_KEY_RIGHT, MODE_INPUT);
    gpio_set_mode(PIN_KEY_PRESS, MODE_INPUT);
    gpio_set_mode(PIN_KEY1,      MODE_INPUT);
    gpio_set_mode(PIN_KEY2,      MODE_INPUT);
    gpio_set_mode(PIN_KEY3,      MODE_INPUT);

    //
    // Open the SPI port.
    //
    spi_init("/dev/spidev2.0", 20000000);
}

//
// Initialize the display.
//
void lcd_init(int rotate, int color, int *xsize, int *ysize)
{
    lcd_width = 240;
    lcd_height = 240;

    lcd_setup_gpio_spi();
    lcd_start_st7789(rotate);
    lcd_clear(color);

    *xsize = lcd_width;
    *ysize = lcd_height;
}

//
// Close the display.
//
void lcd_close()
{
    spi_close();

    // Restore U2RX, U2TX pins.
    gpio_set_mode(GPIO_PIN('G', 9), MODE_U2TX);
    gpio_set_mode(GPIO_PIN('B', 0), MODE_U2RX);
}

/*
 * Draw a line.
 */
void lcd_line(int color, int x0, int y0, int x1, int y1)
{
    int dx, dy, stepx, stepy, fraction;

    if (x0 == x1 || y0 == y1) {
        lcd_fill(color, x0, y0, x1, y1);
        return;
    }

    /* Use Bresenham's line algorithm. */
    dy = y1 - y0;
    if (dy < 0) {
        dy = -dy;
        stepy = -1;
    } else {
        stepy = 1;
    }
    dx = x1 - x0;
    if (dx < 0) {
        dx = -dx;
        stepx = -1;
    } else {
        stepx = 1;
    }
    dy <<= 1;                           /* dy is now 2*dy */
    dx <<= 1;                           /* dx is now 2*dx */
    lcd_pixel(color, x0, y0);
    if (dx > dy) {
        fraction = dy - (dx >> 1);      /* same as 2*dy - dx */
        while (x0 != x1) {
            if (fraction >= 0) {
                y0 += stepy;
                fraction -= dx;         /* same as fraction -= 2*dx */
            }
            x0 += stepx;
            fraction += dy;             /* same as fraction -= 2*dy */
            lcd_pixel(color, x0, y0);
        }
    } else {
        fraction = dx - (dy >> 1);
        while (y0 != y1) {
            if (fraction >= 0) {
                x0 += stepx;
                fraction -= dy;
            }
            y0 += stepy;
            fraction += dx;
            lcd_pixel(color, x0, y0);
        }
    }
}
