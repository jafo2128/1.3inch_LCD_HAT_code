#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "lcd.h"
#include "paint.h"
#include "bmp.h"

int main()
{
    // Handle Ctrl+C
    signal(SIGINT, LCD_Terminate);

    // Initialize the display
    LCD_Init();
    printf("1.3inch LCD demo...\r\n");
    LCD_Start(HORIZONTAL);
    LCD_Clear(WHITE);

    uint32_t Imagesize = LCD_HEIGHT*LCD_WIDTH*2;
    printf("Imagesize = %d bytes\r\n", Imagesize);

    uint16_t *image = (uint16_t*) malloc(Imagesize);
    if (!image) {
        printf("Failed to allocate image!\r\n");
        exit(0);
    }

    // Create a new image and fill it white
    Paint_NewImage(image, LCD_WIDTH, LCD_HEIGHT, 0, WHITE, 16);

    // show bmp
    printf("show bmp\r\n");
    BMP_LoadFile("./pic.bmp");
    LCD_Display(image);

    // Exit
    free(image);
    image = NULL;
    LCD_Exit();
    return 0;
}
