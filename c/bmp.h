/*****************************************************************************
* | File      	:   GUI_BMP.c
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
#ifndef __BMP_H
#define __BMP_H

#include <stdint.h>

#define RGB(r,g,b)          (((r>>3)<<11) | ((g>>2)<<5) | (b>>3))

/*
 * Bitmap file header
 * 14 bytes
 */
typedef struct BMP_FILE_HEADER {
    uint16_t bType;                 // File identifier
    uint32_t bSize;                 // The size of the file
    uint16_t bReserved1;            // Reserved value, must be set to 0
    uint16_t bReserved2;            // Reserved value, must be set to 0
    uint32_t bOffset;               // The offset from the beginning of the file header to the beginning of the image data bit
} __attribute__ ((packed)) BMPFILEHEADER;

/*
 * Bitmap information header
 * 40 bytes
 */
typedef struct BMP_INFO {
    uint32_t bInfoSize;             // The size of the header
    uint32_t bWidth;                // The width of the image
    uint32_t bHeight;               // The height of the image
    uint16_t bPlanes;               // The number of planes in the image
    uint16_t bBitCount;             // The number of bits per pixel
    uint32_t bCompression;          // Compression type
    uint32_t bmpImageSize;          // The size of the image, in bytes
    uint32_t bXPelsPerMeter;        // Horizontal resolution
    uint32_t bYPelsPerMeter;        // Vertical resolution
    uint32_t bClrUsed;              // The number of colors used
    uint32_t bClrImportant;         // The number of important colors
} __attribute__ ((packed)) BMPINF;

/*
 * Color table: palette
 */
typedef struct RGB_QUAD {
    uint8_t rgbBlue;                // Blue intensity
    uint8_t rgbGreen;               // Green strength
    uint8_t rgbRed;                 // Red intensity
    uint8_t rgbReversed;            // Reserved value
} __attribute__ ((packed)) RGBQUAD;

int BMP_LoadFile(const char *path);

#endif
