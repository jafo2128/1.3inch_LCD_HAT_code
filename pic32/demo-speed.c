/*
 * Test the LCD functions for speed.
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "lcd.h"

#define NPIXELS     1000
#define NLINES      30
#define NRECT       20
#define NCIRCLES    20
#define NCHARS      200

extern const struct lcd_font_t font_lucidasans15;

int xsize, ysize;

/*
 * Get current time in milliseconds.
 */
unsigned current_msec()
{
    struct timeval t;

    gettimeofday(&t, 0);
    return t.tv_sec * 1000 + t.tv_usec / 1000;
}

/*
 * Compute elapsed time in milliseconds.
 */
unsigned elapsed_msec(unsigned t0)
{
    struct timeval t1;
    unsigned msec;

    gettimeofday (&t1, 0);
    msec = t1.tv_sec * 1000 + t1.tv_usec / 1000;
    msec -= t0;
    if (msec < 1)
        msec = 1;
    return msec;
}

void finish(int sig)
{
    if (sig)
        printf("Killed by signal #%d.\n", sig);

    // Close the display.
    lcd_close();
    exit(0);
}

int main()
{
    unsigned t0, msec, i;

    // Handle Ctrl+C.
    signal(SIGINT, finish);

    // Initialize the display
    lcd_init(0, 0, &xsize, &ysize);
    printf("Screen size %u x %u.\n", xsize, ysize);

    /* Use fixed seed, for repeatability. */
    srand(1);
    printf("Graphics speed:\n");
    sync();
    usleep(500000);

    /*
     * Pixels.
     */
    t0 = current_msec();
    for (i=0; i<NPIXELS; i++) {
        int x = rand() % xsize;
        int y = rand() % ysize;
        int color = rand() << 1;
        lcd_pixel(color, x, y);
    }
    msec = elapsed_msec(t0);
    printf (" %u pixels/second\n", NPIXELS*1000U / msec);

    /*
     * Lines.
     */
    t0 = current_msec();
    for (i=0; i<NLINES; i++) {
        int x = rand() % xsize;
        int y = rand() % ysize;
        int x1 = rand() % xsize;
        int y1 = rand() % ysize;
        int color = rand() << 1;
        lcd_line(color, x, y, x1, y1);
    }
    msec = elapsed_msec(t0);
    printf (" %u lines/second\n", NLINES*1000U / msec);

    /*
     * Filled rectangles.
     */
    t0 = current_msec();
    for (i=0; i<NRECT; i++) {
        int x = rand() % xsize;
        int y = rand() % ysize;
        int x1 = rand() % xsize;
        int y1 = rand() % ysize;
        int color = rand() << 1;
        lcd_fill(color, x, y, x1, y1);
    }
    msec = elapsed_msec(t0);
    printf (" %u rectangles/second\n", NRECT*1000U / msec);

    /*
     * Circles.
     */
    t0 = current_msec();
    for (i=0; i<NCIRCLES; i++) {
        int x = rand() % xsize;
        int y = rand() % ysize;
        int r = rand() % ysize;
        int color = rand() << 1;
        lcd_circle(color, x, y, r);
    }
    msec = elapsed_msec(t0);
    printf (" %u circles/second\n", NCIRCLES*1000U / msec);

    /*
     * Characters.
     */
    t0 = current_msec();
    for (i=0; i<NCHARS; i++) {
        int x = rand() % xsize;
        int y = rand() % ysize;
        int sym = '!' + rand() % ('~' - ' ');
        int color = rand() << 1;
        lcd_char(&font_lucidasans15, color, -1, x, y, sym);
    }
    msec = elapsed_msec(t0);
    printf (" %u characters/second\n", NCHARS*1000U / msec);

    return 0;
}
