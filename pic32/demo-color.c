/*
 * Display a color palette.
 * Assume RGB 5-6-5 color format.
 * Color is a 16-bit value: rrrrrggggggbbbbb, where rrrrr,
 * gggggg and bbbbb are red, green and blue components.
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "lcd.h"

int xsize, ysize;

void finish(int sig)
{
    if (sig)
        printf("Killed by signal #%d.\n", sig);

    // Close the display.
    lcd_close();
    exit(0);
}

int scale_red(int x, int y)
{
    int r;

    r = 32 * y * (xsize-x-1) / xsize / ysize;
    if (r > 31)
        r = 31;
    return r;
}

int scale_green(int x, int y)
{
    int g;

    g = 64 * x / xsize;
    if (g > 63)
        g = 63;
    return g;
}

int scale_blue(int x, int y)
{
    int b;

    b = 32 * (ysize-y-1) * (xsize-x-1) / xsize / ysize;
    if (b > 31)
        b = 31;
    return b;
}

int main()
{
    // Handle Ctrl+C.
    signal(SIGINT, finish);

    // Initialize the display
    printf("Display color palette.\n");
    lcd_init(0, 0, &xsize, &ysize);
    printf("Screen size %u x %u.\n", xsize, ysize);

    srand(time(0));

    int x, y;
    for (y=0; y<ysize; y++) {
        for (x=0; x<xsize; x++) {
            int r = scale_red(x, y);
            int g = scale_green(x, y);
            int b = scale_blue(x, y);
            int color = r << 11 | g << 5 | b;

            lcd_pixel(color, x, y);
        }
    }
    return 0;
}
