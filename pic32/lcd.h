//
// Initialize the pins and SPI protocol.
//
void lcd_init(void);

//
// Close the display.
//
void lcd_close(void);

//
// Initialize the display.
//
void lcd_start(int horiz_flag);

//
// Clear screen
//
void lcd_clear(unsigned width, unsigned height, unsigned color);
