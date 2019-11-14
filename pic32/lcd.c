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
// Cursor position for text output.
//
static int _col, _row;

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
    if (x0 < 0) x0 = 0;
    if (y0 < 0) x0 = 0;
    if (x1 < 0) x1 = 0;
    if (y1 < 0) x1 = 0;
    if (x0 >= lcd_width) x0 = lcd_width-1;
    if (x1 >= lcd_width) x1 = lcd_width-1;
    if (y0 >= lcd_height) y0 = lcd_height-1;
    if (y1 >= lcd_height) y1 = lcd_height-1;

    if (x1 < x0) {
        int t = x0;
        x0 = x1;
        x1 = t;
    }
    if (y1 < y0) {
        int t = y0;
        y0 = y1;
        y1 = t;
    }

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
    gpio_set_pull(PIN_KEY_UP,    PULL_UP);
    gpio_set_pull(PIN_KEY_DOWN,  PULL_UP);
    gpio_set_pull(PIN_KEY_LEFT,  PULL_UP);
    gpio_set_pull(PIN_KEY_RIGHT, PULL_UP);
    gpio_set_pull(PIN_KEY_PRESS, PULL_UP);
    gpio_set_pull(PIN_KEY1,      PULL_UP);
    gpio_set_pull(PIN_KEY2,      PULL_UP);
    gpio_set_pull(PIN_KEY3,      PULL_UP);

    //
    // Open the SPI port.
    //
    spi_init("/dev/spidev2.0", 10000000);
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

//
// Draw a line.
//
void lcd_line(int color, int x0, int y0, int x1, int y1)
{
    int dx, dy, stepx, stepy, fraction;

    if (x0 == x1 || y0 == y1) {
        lcd_fill(color, x0, y0, x1, y1);
        return;
    }

    // Use Bresenham's line algorithm.
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
    dy <<= 1;                           // dy is now 2*dy
    dx <<= 1;                           // dx is now 2*dx
    lcd_pixel(color, x0, y0);
    if (dx > dy) {
        fraction = dy - (dx >> 1);      // same as 2*dy - dx
        while (x0 != x1) {
            if (fraction >= 0) {
                y0 += stepy;
                fraction -= dx;         // same as fraction -= 2*dx
            }
            x0 += stepx;
            fraction += dy;             // same as fraction -= 2*dy
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

//
// Draw a rectangular frame.
//
void lcd_rect(int color, int x0, int y0, int x1, int y1)
{
    lcd_fill(color, x0, y0, x1, y0);
    lcd_fill(color, x0, y1, x1, y1);
    lcd_fill(color, x0, y0, x0, y1);
    lcd_fill(color, x1, y0, x1, y1);
}

//
// Draw a circle.
//
void lcd_circle(int color, int x0, int y0, int radius)
{
    int f = 1 - radius;
    int ddF_x = 0;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;

    lcd_pixel(color, x0, y0 + radius);
    lcd_pixel(color, x0, y0 - radius);
    lcd_pixel(color, x0 + radius, y0);
    lcd_pixel(color, x0 - radius, y0);
    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x + 1;
        lcd_pixel(color, x0 + x, y0 + y);
        lcd_pixel(color, x0 - x, y0 + y);
        lcd_pixel(color, x0 + x, y0 - y);
        lcd_pixel(color, x0 - x, y0 - y);
        lcd_pixel(color, x0 + y, y0 + x);
        lcd_pixel(color, x0 - y, y0 + x);
        lcd_pixel(color, x0 + y, y0 - x);
        lcd_pixel(color, x0 - y, y0 - x);
    }
}

//
// Draw a glyph of one symbol.
//
static void lcd_glyph(const struct lcd_font_t *font,
    int color, int background, int x, int y, int width,
    const unsigned short *bits)
{
    int h, w, c;
    unsigned bitmask = 0;

    if (x + width > lcd_width || y + font->height > lcd_height)
        return;

    if (background >= 0) {
        //
        // Clear background.
        //
        lcd_set_window(x, y, x + width - 1, y + font->height - 1);

        // Loop on each glyph row.
        for (h=0; h<font->height; h++) {
            // Loop on every pixel in the row (left to right).
            for (w=0; w<width; w++) {
                if ((w & 15) == 0)
                    bitmask = *bits++;
                else
                    bitmask <<= 1;

                c = (bitmask & 0x8000) ? color : background;
                lcd_send_data(c >> 8);
                lcd_send_data(c);
            }
        }
    } else {
        //
        // Transparent background.
        //
        // Loop on each glyph row.
        for (h=0; h<font->height; h++) {
            // Loop on every pixel in the row (left to right).
            for (w=0; w<width; w++) {
                if ((w & 15) == 0)
                    bitmask = *bits++;
                else
                    bitmask <<= 1;

                if (bitmask & 0x8000)
                    lcd_pixel(color, x + w, y + h);
            }
        }
    }
}

//
// Draw a character from a specified font.
//
void lcd_char(const struct lcd_font_t *font,
    int color, int background, int x, int y, int sym)
{
    unsigned cindex, width;
    const unsigned short *bits;

    if (x >= 0)
        _col = x;
    if (y >= 0)
        _row = y;
    switch (sym) {
    case '\n':      // goto next line
        _row += font->height;
        _col = 0;
        if (_row > lcd_height - font->height)
            _row = 0;
        return;
    case '\r':      // carriage return - go to begin of line
        _col = 0;
        return;
    case '\t':      // tab replaced by space
        sym = ' ';
        break;
    }

    if (sym < font->firstchar || sym >= font->firstchar + font->size)
        sym = font->defaultchar;
    cindex = sym - font->firstchar;

    // Get font bitmap depending on fixed pitch or not.
    if (font->width) {
        // Proportional font.
        width = font->width[cindex];
    } else {
        // Fixed width font.
        width = font->maxwidth;
    }
    if (font->offset) {
        bits = font->bits + font->offset[cindex];
    } else {
        bits = font->bits + cindex * font->height;
    }

    // Draw a character.
    lcd_glyph(font, color, background, _col, _row, width, bits);
    _col += width;
}

//
// Draw a string of characters.
// TODO: Decode UTF-8.
//
void lcd_text(const struct lcd_font_t *font,
    int color, int background, int x, int y, const char *text)
{
    int sym;

    if (x >= 0)
        _col = x;
    if (y >= 0)
        _row = y;
    for (;;) {
        sym = *text++;
        if (! sym)
            break;

        lcd_char(font, color, background, -1, -1, sym);
    }
}

//
// Swap values of two integer variables.
//
#define swapi(x,y) { int _t = x; x = y; y = _t; }

//
// Fill a triangle.
// Code ported from AdaFruit TFT LCD library.
//
void lcd_fill_triangle(int color, int x0, int y0, int x1, int y1, int x2, int y2)
{
    int a, b, y, last;

    // Sort coordinates by Y order (y2 >= y1 >= y0)
    if (y0 > y1) {
        swapi(y0, y1);
        swapi(x0, x1);
    }
    if (y1 > y2) {
        swapi(y2, y1);
        swapi(x2, x1);
    }
    if (y0 > y1) {
        swapi(y0, y1);
        swapi(x0, x1);
    }

    if (y0 == y2) {
        // Handle awkward all-on-same-line case as its own thing
        a = b = x0;

        if (x1 < a)
            a = x1;
        else if (x1 > b)
            b = x1;

        if (x2 < a)
            a = x2;
        else if (x2 > b)
            b = x2;

        lcd_fill(color, a, y0, b, y0);
        return;
    }

    int dx01 = x1 - x0;
    int dy01 = y1 - y0;
    int dx02 = x2 - x0;
    int dy02 = y2 - y0;
    int dx12 = x2 - x1;
    int dy12 = y2 - y1;
    int sa = 0;
    int sb = 0;

    // For upper part of triangle, find scanline crossings for segments
    // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
    // is included here (and second loop will be skipped, avoiding a /0
    // error there), otherwise scanline y1 is skipped here and handled
    // in the second loop...which also avoids a /0 error here if y0=y1
    // (flat-topped triangle).
    if (y1 == y2)
        last = y1;      // Include y1 scanline
    else
        last = y1-1;    // Skip it

    for (y=y0; y<=last; y++) {
        a   = x0 + sa / dy01;
        b   = x0 + sb / dy02;
        sa += dx01;
        sb += dx02;

        /* longhand:
         * a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
         * b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
         */
        if (a > b)
            swapi(a, b);

        lcd_fill(color, a, y, b, y);
    }

    // For lower part of triangle, find scanline crossings for segments
    // 0-2 and 1-2.  This loop is skipped if y1=y2.
    sa = dx12 * (y - y1);
    sb = dx02 * (y - y0);
    for (; y<=y2; y++) {
        a   = x1 + sa / dy12;
        b   = x0 + sb / dy02;
        sa += dx12;
        sb += dx02;

        /* longhand:
         * a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
         * b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
         */
        if (a > b)
            swapi(a, b);

        lcd_fill(color, a, y, b, y);
    }
}

//
// Get status of joystick Press button.
//
int lcd_get_key_press()
{
    return gpio_read(PIN_KEY_PRESS);
}
