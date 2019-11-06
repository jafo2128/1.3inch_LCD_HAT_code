/*****************************************************************************
* | File      	:   LCD_APP.c
* | Author      :   Waveshare team
* | Function    :   Hardware underlying interface
* | Info        :
*                Used to shield the underlying layers of each master
*                and enhance portability
*----------------
* |	This version:   V1.0
* | Date        :   2018-01-11
* | Info        :   Basic version
*
******************************************************************************/
#ifndef __LCD_1IN3_H
#define __LCD_1IN3_H

#include <stdint.h>

#define LCD_HEIGHT  240
#define LCD_WIDTH   240

#define HORIZONTAL  0
#define VERTICAL    1

typedef struct {
    uint16_t WIDTH;
    uint16_t HEIGHT;
    uint8_t SCAN_DIR;
} LCD_ATTRIBUTES;

extern LCD_ATTRIBUTES LCD;

void LCD_Start(uint8_t orientation);
void LCD_Clear(uint16_t Color);
void LCD_Display(uint16_t *Image);
void LCD_DisplayWindows(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t *Image);
void LCD_DisplayPoint(uint16_t X, uint16_t Y, uint16_t Color);
void LCD_Terminate(int signo);
void LCD_Init(void);
void LCD_Exit(void);
#endif
