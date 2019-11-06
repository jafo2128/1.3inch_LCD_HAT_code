#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "pins.h"
#include "lcd.h"
#include "paint.h"
#include "gpio.h"

void Draw_Init(uint16_t *Image)
{
    /* Press */
    Paint_DrawCircle(90, 120, 25, RED, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawString_EN(82, 112, "P", &Font24, IMAGE_BACKGROUND, BLUE);

    /* Left */
    Paint_DrawRectangle(15, 95, 65, 145, RED, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawString_EN(32, 112, "L", &Font24, IMAGE_BACKGROUND, BLUE);
// Paint_DrawRectangle( uint16_t Color, DOT_PIXEL Line_width, DRAW_FILL Draw_Fill);

    /* Down */
    Paint_DrawRectangle(65, 145, 115, 195, RED, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawString_EN(82, 162, "D", &Font24, IMAGE_BACKGROUND, BLUE);

    /* Right */
    Paint_DrawRectangle(115, 95, 165, 145, RED, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawString_EN(132, 112, "R", &Font24, IMAGE_BACKGROUND, BLUE);

    /* Up */
    Paint_DrawRectangle(65, 45, 115, 95, RED, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawString_EN(82, 62, "U", &Font24, IMAGE_BACKGROUND, BLUE);

    /* Key1 */
    Paint_DrawRectangle(185, 35, 235, 85, RED, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawString_EN(195, 52, "K1", &Font24, IMAGE_BACKGROUND, BLUE);

    /* Key2	*/
    Paint_DrawRectangle(185, 95, 235, 145, RED, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawString_EN(195, 112, "K2", &Font24, IMAGE_BACKGROUND, BLUE);

    /* Key3 */
    Paint_DrawRectangle(185, 155, 235, 205, RED, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawString_EN(195, 172, "K3", &Font24, IMAGE_BACKGROUND, BLUE);

    LCD_Display(Image);
}

void KEY_Listen(uint16_t *Image)
{
    Draw_Init(Image);
    for(;;) {
        if (GPIO_Read(KEY_UP_PIN) == 0){
            while (GPIO_Read(KEY_UP_PIN) == 0){
                Paint_DrawRectangle(65, 45, 115, 95, RED, DOT_PIXEL_1X1, DRAW_FILL_FULL);
                Paint_DrawString_EN(82, 62, "U", &Font24, IMAGE_BACKGROUND, BLUE);
                Paint_Refresh(65, 45, 115, 95);
            }
            Paint_DrawRectangle(65, 45, 115, 95, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);
            Paint_DrawRectangle(65, 45, 115, 95, RED, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
            Paint_DrawString_EN(82, 62, "U", &Font24, IMAGE_BACKGROUND, BLUE);
            Paint_Refresh(65, 45, 115, 95);
        }
        if (GPIO_Read(KEY_DOWN_PIN) == 0){
            while (GPIO_Read(KEY_DOWN_PIN) == 0) {
                Paint_DrawRectangle(65, 145, 115, 195, RED, DOT_PIXEL_1X1, DRAW_FILL_FULL);
                Paint_DrawString_EN(82, 162, "D", &Font24, IMAGE_BACKGROUND, BLUE);
                Paint_Refresh(65, 145, 115, 195);
            }
            Paint_DrawRectangle(65, 145, 115, 195, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);
            Paint_DrawRectangle(65, 145, 115, 195, RED, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
            Paint_DrawString_EN(82, 162, "D", &Font24, IMAGE_BACKGROUND, BLUE);
            Paint_Refresh(65, 145, 115, 195);
        }

        if (GPIO_Read(KEY_LEFT_PIN) == 0) {
            while (GPIO_Read(KEY_LEFT_PIN) == 0) {
                Paint_DrawRectangle(15, 95, 65, 145, RED, DOT_PIXEL_1X1, DRAW_FILL_FULL);
                Paint_DrawString_EN(32, 112, "L", &Font24, IMAGE_BACKGROUND, BLUE);
                Paint_Refresh(15, 95, 65, 145);
            }
            Paint_DrawRectangle(15, 95, 65, 145, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);
            Paint_DrawRectangle(15, 95, 65, 145, RED, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
            Paint_DrawString_EN(32, 112, "L", &Font24, IMAGE_BACKGROUND, BLUE);
            Paint_Refresh(15, 95, 65, 145);
        }
        if (GPIO_Read(KEY_RIGHT_PIN) == 0) {
            while (GPIO_Read(KEY_RIGHT_PIN) == 0) {
                Paint_DrawRectangle(115, 95, 165, 145, RED, DOT_PIXEL_1X1, DRAW_FILL_FULL);
                Paint_DrawString_EN(132, 112, "R", &Font24, IMAGE_BACKGROUND, BLUE);
                Paint_Refresh(115, 95, 165, 145);
            }
            Paint_DrawRectangle(115, 95, 165, 145, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);
            Paint_DrawRectangle(115, 95, 165, 145, RED, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
            Paint_DrawString_EN(132, 112, "R", &Font24, IMAGE_BACKGROUND, BLUE);
            Paint_Refresh(115, 95, 165, 145);
        }
        if (GPIO_Read(KEY_PRESS_PIN) == 0) {
            while (GPIO_Read(KEY_PRESS_PIN) == 0) {
                Paint_DrawRectangle(66, 95, 114, 144, RED, DOT_PIXEL_1X1, DRAW_FILL_FULL);
                Paint_DrawString_EN(82, 112, "P", &Font24, IMAGE_BACKGROUND, BLUE);
                Paint_Refresh(65, 95, 115, 145);
            }
            Paint_DrawRectangle(65, 95, 115, 145, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);
            Paint_DrawCircle(90, 120, 25, RED, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
            Paint_DrawRectangle(65, 95, 115, 145, RED, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
            Paint_DrawString_EN(82, 112, "P", &Font24, IMAGE_BACKGROUND, BLUE);
            Paint_Refresh(65 , 95, 115, 145);
        }
        if (GPIO_Read(KEY1_PIN) == 0) {
            while (GPIO_Read(KEY1_PIN) == 0) {
                Paint_DrawRectangle(185, 35, 235, 85, RED, DOT_PIXEL_1X1, DRAW_FILL_FULL);
                Paint_DrawString_EN(195, 52, "K1", &Font24, IMAGE_BACKGROUND, BLUE);
                Paint_Refresh(185, 35, 235, 85);
            }
            Paint_DrawRectangle(184, 35, 235, 85, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);
            Paint_DrawString_EN(195, 52, "K1", &Font24, IMAGE_BACKGROUND, BLUE);
            Paint_DrawRectangle(185, 35, 235, 85, RED, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
            Paint_Refresh(185, 35, 235, 85);

        }
        if (GPIO_Read(KEY2_PIN) == 0) {
            while (GPIO_Read(KEY2_PIN) == 0) {
                Paint_DrawRectangle(185, 95, 235, 145, RED, DOT_PIXEL_1X1, DRAW_FILL_FULL);
                Paint_DrawString_EN(195, 112, "K2", &Font24, IMAGE_BACKGROUND, BLUE);
                Paint_Refresh(185, 95, 235, 145);
            }
            Paint_DrawRectangle(185, 95, 235, 145, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);
            Paint_DrawRectangle(185, 95, 235, 145, RED, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
            Paint_DrawString_EN(195, 112, "K2", &Font24, IMAGE_BACKGROUND, BLUE);
            Paint_Refresh(185, 95, 235, 145);
        }
        if (GPIO_Read(KEY3_PIN) == 0) {
            while (GPIO_Read(KEY3_PIN) == 0) {
                Paint_DrawRectangle(185, 155, 235, 205, RED, DOT_PIXEL_1X1, DRAW_FILL_FULL);
                Paint_DrawString_EN(195, 172, "K3", &Font24, IMAGE_BACKGROUND, BLUE);
                Paint_Refresh(185, 155, 235, 205);
            }
            Paint_DrawRectangle(185, 155, 235, 205, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);
            Paint_DrawRectangle(185, 155, 235, 205, RED, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
            Paint_DrawString_EN(195, 172, "K3", &Font24, IMAGE_BACKGROUND, BLUE);
            Paint_Refresh(185, 155, 235, 205);
        }
    }
}

int main()
{
    // Handle Ctrl+C
    signal(SIGINT, LCD_Terminate);

    // Initialize the display
    LCD_Init();
    printf("1.3inch LCD KEY demo...\r\n");
    LCD_Start(HORIZONTAL);
    LCD_Clear(WHITE);

    uint16_t *image = (uint16_t*) malloc(LCD_HEIGHT * LCD_WIDTH);
    if (!image) {
        printf("Failed to allocate image!\r\n");
        exit(0);
    }

    // Create a new image and fill it white
    Paint_NewImage(image, LCD_WIDTH, LCD_HEIGHT, 0, WHITE, 16);
    Paint_Clear(WHITE);

    // Scan buttons
    printf("Listening KEY\r\n");
    KEY_Listen(image);

    // Exit
    free(image);
    image = NULL;
    LCD_Exit();
    return 0;
}
