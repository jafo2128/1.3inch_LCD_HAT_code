/*
 * Draw random circles.
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "lcd.h"

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
    int xsize, ysize;

    // Handle Ctrl+C.
    signal(SIGINT, finish);

    // Initialize the display
    printf("Draw random circles.\n");
    lcd_init(0, 0, &xsize, &ysize);
    printf("Screen size %u x %u.\n", xsize, ysize);

    srand(time(0));
    printf("Press ^C to stop.\n");

    for (;;) {
        int x = rand() % xsize;
        int y = rand() % ysize;
        int r = rand() % ysize;
        int color = rand() << 1;

        // Draw random circles.
        lcd_circle(color, x, y, r);
    }
    return 0;
}
