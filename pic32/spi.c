#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include "spi.h"

static int      hw_fd;
static uint32_t hw_speed;
static uint8_t  hw_bits;
static uint16_t hw_mode;

#define MODE_CPHA        0x01
#define MODE_CPOL        0x02
#define MODE_CS_HIGH     0x04    // Chip select high
#define MODE_LSB_FIRST   0x08    // LSB
#define MODE_3WIRE       0x10    // 3-wire mode SI and SO same line
#define MODE_LOOP        0x20    // Loopback mode
#define MODE_NO_CS       0x40    // A single device occupies one SPI bus, so there is no chip select
#define MODE_READY       0x80    // Slave pull low to stop data transmission

static struct spi_ioc_transfer ioc;

//
// Initialize SPI port.
//
int spi_init(char *devname, unsigned bits_per_sec)
{
    hw_fd = open(devname, O_RDWR);
    if (hw_fd < 0)  {
        perror(devname);
        return -1;
    }

    // LSB first, ignore CS.
    //hw_mode = MODE_LSB_FIRST | MODE_NO_CS;
    hw_mode = 0;

    //
    // Set transfer size 8 bits.
    //
    hw_bits = 8;
    if (ioctl(hw_fd, SPI_IOC_WR_BITS_PER_WORD, &hw_bits) < 0) {
        return -1;
    }
    if (ioctl(hw_fd, SPI_IOC_RD_BITS_PER_WORD, &hw_bits) < 0) {
        return -1;
    }
    ioc.bits_per_word = hw_bits;
    ioc.delay_usecs = 0;

    //
    // Set SPI speed.
    //
    hw_speed = bits_per_sec;
    if (ioctl(hw_fd, SPI_IOC_WR_MAX_SPEED_HZ, &hw_speed) < 0) {
        return -1;
    }
    if (ioctl(hw_fd, SPI_IOC_RD_MAX_SPEED_HZ, &hw_speed) < 0) {
        return -1;
    }
    ioc.speed_hz = hw_speed;

    spi_set_mode(0);
    return 0;
}

//
// Close SPI port.
//
void spi_close()
{
    close(hw_fd);
    hw_mode = 0;
}

//
// Set SPI mode.
//
int spi_set_mode(int mode)
{
    hw_mode &= ~(MODE_CPHA | MODE_CPOL);
    hw_mode |= mode & (MODE_CPHA | MODE_CPOL);

    if (ioctl(hw_fd, SPI_IOC_WR_MODE, &hw_mode) < 0) {
        return -1;
    }
    return 0;
}

//
// Send/receive one byte.
// Return the received byte, or -1 on error.
//
int spi_transfer(uint8_t send)
{
    uint8_t receive[1];

    ioc.len = 1;
    ioc.tx_buf = (unsigned long)&send;
    ioc.rx_buf = (unsigned long)receive;

    if (ioctl(hw_fd, SPI_IOC_MESSAGE(1), &ioc) < 0) {
        return -1;
    }
    return receive[0];
}

//
// Send/receive an array of bytes.
//
int spi_bulk_rw(uint8_t *buf, uint32_t len)
{
    ioc.len = len;
    ioc.tx_buf = (unsigned long)buf;
    ioc.rx_buf = (unsigned long)buf;

    if (ioctl(hw_fd, SPI_IOC_MESSAGE(1), &ioc) < 0) {
        return -1;
    }
    return 0;
}
