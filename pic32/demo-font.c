/*
 * Draw samples of various fonts.
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "lcd.h"

/*
 * Data from external font files.
 */
extern const struct lcd_font_t font_lucidasans28;
extern const struct lcd_font_t font_lucidasans15;
extern const struct lcd_font_t font_lucidasans11;
extern const struct lcd_font_t font_digits32;
extern const struct lcd_font_t font_digits20;

/*
 * Color constants.
 */
#define COLOR_RGB(r,g,b)    ((r)<<11 | (g)<<5 | (b))
#define COLOR_BLACK         0
#define COLOR_WHITE         COLOR_RGB(31, 63, 31)
#define COLOR_YELLOW        COLOR_RGB(31, 63, 0)
#define COLOR_MAGENTA       COLOR_RGB(31, 0,  31)
#define COLOR_CYAN          COLOR_RGB(0,  63, 31)
#define COLOR_RED           COLOR_RGB(31, 0,  0)
#define COLOR_GREEN         COLOR_RGB(0,  63, 0)
#define COLOR_BLUE          COLOR_RGB(0,  0,  31)

/*
 * Screen size.
 */
int xsize, ysize;

void show(const struct lcd_font_t *font, const char *title, int digits_only)
{
    char line[100];
    int x = 0, y = 0, i, color;
    const char *phrase = digits_only ? "0123456789" :
                         "The quick brown fox jumps over the lazy dog.";
    static const int colortab[] = {
        COLOR_YELLOW, COLOR_CYAN,  COLOR_MAGENTA,
        COLOR_RED,    COLOR_GREEN, COLOR_BLUE,
        0,
    };

    lcd_clear(COLOR_BLACK);
    lcd_text(&font_lucidasans15, COLOR_WHITE, COLOR_BLACK, x, y, title);
    y += font_lucidasans15.height * 2;

    for (i=0; y<ysize; i++) {
        color = colortab[i];
        if (color == 0)
            color = colortab[i = 0];

        lcd_text(font, color, COLOR_BLACK, x, y, phrase);
        y += font->height;
    }

    printf("Font %s: press Enter...", title);
    fflush(stdout);
    fgets(line, sizeof(line), stdin);
}

void finish(int sig)
{
    if (sig)
        printf("\nKilled by signal #%d.\n", sig);

    // Close the display.
    lcd_close();
    exit(0);
}

int main()
{
    // Handle Ctrl+C.
    signal(SIGINT, finish);

    // Initialize the display
    printf("Draw fonts.\n");
    lcd_init(0, 0, &xsize, &ysize);
    printf("Screen size %u x %u.\n", xsize, ysize);

    srand(time(0));
    printf("Press ^C to stop.\n");

    for (;;) {
        show(&font_lucidasans28, "Lucida Sans 28", 0);
        show(&font_lucidasans15, "Lucida Sans 15", 0);
        show(&font_lucidasans11, "Lucida Sans 11", 0);
        show(&font_digits32, "Digits 32", 1);
        show(&font_digits20, "Digits 20", 1);
    }
    return 0;
}
