#include "dev.h"
#include "lcd.h"
#include "paint.h"
#include "bmp.h"
#include <stdio.h>	//printf()
#include <stdlib.h>	//exit()
#include <signal.h>     //signal()

int main()
{
    // Exception handling:ctrl + c
    signal(SIGINT, Handler_1in3_LCD);

    /* Module Init */
    if(DEV_ModuleInit() != 0){
        DEV_ModuleExit();
        exit(0);
    }

    /* LCD Init */
    printf("1.3inch LCD demo...\r\n");
    LCD_1in3_Init(HORIZONTAL);
    LCD_1in3_Clear(WHITE);

    UWORD *BlackImage;
    UDOUBLE Imagesize = LCD_HEIGHT*LCD_WIDTH*2;
    printf("Imagesize = %d\r\n", Imagesize);
    BlackImage = (UWORD*) malloc(Imagesize);
    if (!BlackImage) {
        printf("Failed to apply for black memory...\r\n");
        exit(0);
    }
    // printf("size = %d\r\n",sizeof(BlackImage) / sizeof(UWORD));
    // /*1.Create a new image cache named IMAGE_RGB and fill it with white*/
    Paint_NewImage(BlackImage, LCD_WIDTH, LCD_HEIGHT, 0, WHITE, 16);

    // show bmp
    printf("show bmp\r\n");
    GUI_ReadBmp("./pic.bmp");

    LCD_1in3_Display(BlackImage);

    /* Module Exit */
    free(BlackImage);
    BlackImage = NULL;
    DEV_ModuleExit();
    return 0;
}
