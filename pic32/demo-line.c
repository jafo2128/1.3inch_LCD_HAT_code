/*
 * Draw random lines.
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
    lcd_init(0, 0, &xsize, &ysize);
    printf("Screen size %u x %u.\n", xsize, ysize);

    srand(time(0));
    printf("Draw random lines.\n");
    printf("Press ^C to stop.\n");

    for (;;) {
        int x0 = rand() % xsize;
        int y0 = rand() % ysize;
        int x1 = rand() % xsize;
        int y1 = rand() % ysize;
        int color = rand() << 1;

        // Draw random lines.
        lcd_line(color, x0, y0, x1, y1);
    }
    return 0;
}
