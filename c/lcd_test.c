#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "lcd.h"
#include "paint.h"

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
    printf("Image size = %d bytes\r\n", Imagesize);

    uint16_t *image = (uint16_t*) malloc(Imagesize);
    if (!image) {
        printf("Failed to allocate image!\r\n");
        exit(0);
    }

    // Create a new image and fill it white
    Paint_NewImage(image, LCD_WIDTH, LCD_HEIGHT, 0, WHITE, 16);
    Paint_Clear(WHITE);

    // Drawing on the image
    printf("drawing...\r\n");
    Paint_DrawPoint(5, 10, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);//240 240
    Paint_DrawPoint(5, 25, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(5, 40, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawPoint(5, 55, BLACK, DOT_PIXEL_4X4, DOT_STYLE_DFT);

    Paint_DrawLine(20, 10, 70, 60, RED, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 10, 20, 60, RED, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(170, 15, 170, 55, RED, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(150, 35, 190, 35, RED, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);

    Paint_DrawRectangle(20, 10, 70, 60, BLUE, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(85, 10, 130, 60, BLUE, DOT_PIXEL_1X1, DRAW_FILL_FULL);

    Paint_DrawCircle(170, 35, 20, GREEN, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(170, 85, 20, GREEN, DOT_PIXEL_1X1, DRAW_FILL_FULL);

    Paint_DrawString_EN(5, 70, "hello world", &Font16, WHITE, BLACK);
    Paint_DrawString_EN(5, 90, "waveshare", &Font20, RED, IMAGE_BACKGROUND);

    // Paint_DrawNum(20, 200, 123456789, &Font20, BLUE, IMAGE_BACKGROUND);

    // Send the picture from RAM to LCD
    LCD_Display(image);

    // Exit
    free(image);
    image = NULL;
    LCD_Exit();
    return 0;
}
