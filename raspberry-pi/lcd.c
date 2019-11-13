/*****************************************************************************
* | File      	:   LCD.c
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
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "lcd.h"
#include "pins.h"
#include "gpio.h"
#include "spi.h"

LCD_ATTRIBUTES LCD;

/******************************************************************************
function :	Hardware reset
parameter:
******************************************************************************/
static void LCD_Reset(void)
{
    GPIO_Write(LCD_RST, 1);
    usleep(100000);
    GPIO_Write(LCD_RST, 0);
    usleep(100000);
    GPIO_Write(LCD_RST, 1);
    usleep(100000);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void LCD_SendCommand(uint8_t Reg)
{
    GPIO_Write(LCD_DC, 0);
    // GPIO_Write(LCD_CS, 0);
    SPI_TransferByte(Reg);
    // GPIO_Write(LCD_CS, 1);
}

/******************************************************************************
function :	send data
parameter:
    Data : Write data
******************************************************************************/
static void LCD_SendByte(uint8_t Data)
{
    GPIO_Write(LCD_DC, 1);
    // GPIO_Write(LCD_CS, 0);
    SPI_TransferByte(Data);
    // GPIO_Write(LCD_CS, 1);
}

/******************************************************************************
function :	send data
parameter:
    Data : Write data
******************************************************************************/
static void LCD_SendWord(uint16_t Data)
{
    GPIO_Write(LCD_DC, 1);
    // GPIO_Write(LCD_CS, 0);
    SPI_TransferByte((Data >> 8) & 0xFF);
    SPI_TransferByte(Data & 0xFF);
    // GPIO_Write(LCD_CS, 1);
}

/******************************************************************************
function :	Initialize the lcd register
parameter:
******************************************************************************/
static void LCD_InitReg(void)
{
    LCD_SendCommand(0x3A);
    LCD_SendByte(0x05);

    LCD_SendCommand(0xB2);
    LCD_SendByte(0x0C);
    LCD_SendByte(0x0C);
    LCD_SendByte(0x00);
    LCD_SendByte(0x33);
    LCD_SendByte(0x33);

    LCD_SendCommand(0xB7);  //Gate Control
    LCD_SendByte(0x35);

    LCD_SendCommand(0xBB);  //VCOM Setting
    LCD_SendByte(0x19);

    LCD_SendCommand(0xC0); //LCM Control
    LCD_SendByte(0x2C);

    LCD_SendCommand(0xC2);  //VDV and VRH Command Enable
    LCD_SendByte(0x01);
    LCD_SendCommand(0xC3);  //VRH Set
    LCD_SendByte(0x12);
    LCD_SendCommand(0xC4);  //VDV Set
    LCD_SendByte(0x20);

    LCD_SendCommand(0xC6);  //Frame Rate Control in Normal Mode
    LCD_SendByte(0x0F);

    LCD_SendCommand(0xD0);  // Power Control 1
    LCD_SendByte(0xA4);
    LCD_SendByte(0xA1);

    LCD_SendCommand(0xE0);  //Positive Voltage Gamma Control
    LCD_SendByte(0xD0);
    LCD_SendByte(0x04);
    LCD_SendByte(0x0D);
    LCD_SendByte(0x11);
    LCD_SendByte(0x13);
    LCD_SendByte(0x2B);
    LCD_SendByte(0x3F);
    LCD_SendByte(0x54);
    LCD_SendByte(0x4C);
    LCD_SendByte(0x18);
    LCD_SendByte(0x0D);
    LCD_SendByte(0x0B);
    LCD_SendByte(0x1F);
    LCD_SendByte(0x23);

    LCD_SendCommand(0xE1);  //Negative Voltage Gamma Control
    LCD_SendByte(0xD0);
    LCD_SendByte(0x04);
    LCD_SendByte(0x0C);
    LCD_SendByte(0x11);
    LCD_SendByte(0x13);
    LCD_SendByte(0x2C);
    LCD_SendByte(0x3F);
    LCD_SendByte(0x44);
    LCD_SendByte(0x51);
    LCD_SendByte(0x2F);
    LCD_SendByte(0x1F);
    LCD_SendByte(0x1F);
    LCD_SendByte(0x20);
    LCD_SendByte(0x23);

    LCD_SendCommand(0x21);  //Display Inversion On

    LCD_SendCommand(0x11);  //Sleep Out

    LCD_SendCommand(0x29);  //Display On
}

/********************************************************************************
function:	Set the resolution and scanning method of the screen
parameter:
		Scan_dir:   Scan direction
********************************************************************************/
static void LCD_SetAttributes(uint8_t Scan_dir)
{
    //Get the screen scan direction
    LCD.SCAN_DIR = Scan_dir;
    uint8_t MemoryAccessReg = 0x00;

    //Get GRAM and LCD width and height
    if(Scan_dir == HORIZONTAL) {
        LCD.HEIGHT	= LCD_HEIGHT;
        LCD.WIDTH   = LCD_WIDTH;
        MemoryAccessReg = 0X70;
    } else {
        LCD.HEIGHT	= LCD_WIDTH;
        LCD.WIDTH   = LCD_HEIGHT;
        MemoryAccessReg = 0X00;
    }

    // Set the read / write scan direction of the frame memory
    LCD_SendCommand(0x36); //MX, MY, RGB mode
    LCD_SendByte(MemoryAccessReg);	//0x08 set RGB
}

/********************************************************************************
function :	Initialize the lcd
parameter:
********************************************************************************/
void LCD_Start(uint8_t orientation)
{
    //Turn on the backlight
    GPIO_Write(LCD_BL, 1);

    //Hardware reset
    LCD_Reset();

    //Set the resolution and scanning method of the screen
    LCD_SetAttributes(orientation);

    //Set the initialization register
    LCD_InitReg();
}

/********************************************************************************
function:	Sets the start position and size of the display area
parameter:
		Xstart 	:   X direction Start coordinates
		Ystart  :   Y direction Start coordinates
		Xend    :   X direction end coordinates
		Yend    :   Y direction end coordinates
********************************************************************************/
void LCD_SetWindows(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend)
{
    //set the X coordinates
    LCD_SendCommand(0x2A);
    LCD_SendByte((Xstart >> 8) & 0xFF);
    LCD_SendByte(Xstart & 0xFF);
    LCD_SendByte(((Xend  - 1) >> 8) & 0xFF);
    LCD_SendByte((Xend  - 1) & 0xFF);

    //set the Y coordinates
    LCD_SendCommand(0x2B);
    LCD_SendByte((Ystart >> 8) & 0xFF);
    LCD_SendByte(Ystart & 0xFF);
    LCD_SendByte(((Yend  - 1) >> 8) & 0xFF);
    LCD_SendByte((Yend  - 1) & 0xFF);

    LCD_SendCommand(0X2C);
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void LCD_Clear(uint16_t Color)
{
    uint16_t j;
    uint16_t Image[LCD_WIDTH*LCD_HEIGHT];

    Color = ((Color<<8)&0xff00)|(Color>>8);

    for (j = 0; j < LCD_HEIGHT*LCD_WIDTH; j++) {
        Image[j] = Color;
    }

    LCD_SetWindows(0, 0, LCD_WIDTH, LCD_HEIGHT);
    GPIO_Write(LCD_DC, 1);
    for(j = 0; j < LCD_HEIGHT; j++){
        SPI_Transfer((uint8_t *)&Image[j*LCD_WIDTH], LCD_WIDTH*2);
    }
}

/******************************************************************************
function :	Sends the image buffer in RAM to displays
parameter:
******************************************************************************/
void LCD_Display(uint16_t *Image)
{
    uint16_t j;
    LCD_SetWindows(0, 0, LCD_WIDTH, LCD_HEIGHT);
    GPIO_Write(LCD_DC, 1);
    for (j = 0; j < LCD_HEIGHT; j++) {
        SPI_Transfer((uint8_t *)&Image[j*LCD_WIDTH], LCD_WIDTH*2);
    }
}

void LCD_DisplayWindows(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t *Image)
{
    // display
    uint32_t Addr = 0;

    uint16_t j;
    LCD_SetWindows(Xstart, Ystart, Xend-1 , Yend-1);
    GPIO_Write(LCD_DC, 1);
    for (j = Ystart; j < Yend - 1; j++) {
        Addr = Xstart + j * LCD_WIDTH ;
        SPI_Transfer((uint8_t *)&Image[Addr], (Xend-Xstart-1)*2);
    }
}

void LCD_DisplayPoint(uint16_t X, uint16_t Y, uint16_t Color)
{
    LCD_SetWindows(X,Y,X,Y);
    LCD_SendWord(Color);
}

void LCD_Terminate(int signo)
{
    // System Exit
    printf("\r\nHandler:Program stop\r\n");
    LCD_Exit();
    exit(0);
}

static void LCD_Pin_Mode(uint16_t Pin, uint16_t Mode)
{
    GPIO_Export(Pin);
    if(Mode == 0 || Mode == GPIO_IN){
        GPIO_Direction(Pin, GPIO_IN);
        // printf("IN Pin = %d\r\n",Pin);
    }else{
        GPIO_Direction(Pin, GPIO_OUT);
        // printf("OUT Pin = %d\r\n",Pin);
    }
}

/*
 * Initialize the pins and SPI protocol.
 */
void LCD_Init(void)
{
    LCD_Pin_Mode(LCD_CS, 1);
    LCD_Pin_Mode(LCD_RST, 1);
    LCD_Pin_Mode(LCD_DC, 1);
    LCD_Pin_Mode(LCD_BL, 1);

    LCD_Pin_Mode(KEY_UP_PIN, 0);
    LCD_Pin_Mode(KEY_DOWN_PIN, 0);
    LCD_Pin_Mode(KEY_LEFT_PIN, 0);
    LCD_Pin_Mode(KEY_RIGHT_PIN, 0);
    LCD_Pin_Mode(KEY_PRESS_PIN, 0);
    LCD_Pin_Mode(KEY1_PIN, 0);
    LCD_Pin_Mode(KEY2_PIN, 0);
    LCD_Pin_Mode(KEY3_PIN, 0);
    GPIO_Write(LCD_CS, 1);

    SPI_begin("/dev/spidev0.0");
}

/*
 * Close SPI connection.
 */
void LCD_Exit(void)
{
    SPI_end();
}
