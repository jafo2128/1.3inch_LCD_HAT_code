#include <stdio.h>
#include <stdlib.h>
#include "lcd.h"

int main()
{
    // Initialize the display
    lcd_init();
    printf("SPI demo: clear LCD display.\n");

    // Horizontal orientation.
    lcd_start(1);
    lcd_clear(240, 240, 0xffff);

    // Close the display.
    lcd_close();
    return 0;
}
