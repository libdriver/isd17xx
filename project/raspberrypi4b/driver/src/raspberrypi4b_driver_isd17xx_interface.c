/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      raspberrypi4b_driver_isd17xx_interface.c
 * @brief     raspberrypi4b driver isd17xx interface source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2022-10-31
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2022/10/31  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_isd17xx_interface.h"
#include "spi.h"
#include "wire.h"
#include <stdarg.h>

/**
 * @brief spi device name definition
 */
#define SPI_DEVICE_NAME "/dev/spidev0.0"    /**< spi device name */

/**
 * @brief spi device handle definition
 */
static int gs_fd;                           /**< spi handle */

/**
 * @brief msb to lsb table definition
 */
static const uint8_t gs_bit_reverse_table[256] = 
{
    0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0,
    0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0,
    0x08, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8,
    0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8,
    0x04, 0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4,
    0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4,
    0x0c, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec,
    0x1c, 0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc,
    0x02, 0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2,
    0x12, 0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2,
    0x0a, 0x8a, 0x4a, 0xca, 0x2a, 0xaa, 0x6a, 0xea,
    0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa,
    0x06, 0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6,
    0x16, 0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6,
    0x0e, 0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee,
    0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe,
    0x01, 0x81, 0x41, 0xc1, 0x21, 0xa1, 0x61, 0xe1,
    0x11, 0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1,
    0x09, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9,
    0x19, 0x99, 0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9,
    0x05, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5,
    0x15, 0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5,
    0x0d, 0x8d, 0x4d, 0xcd, 0x2d, 0xad, 0x6d, 0xed,
    0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd,
    0x03, 0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3,
    0x13, 0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3,
    0x0b, 0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb,
    0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb,
    0x07, 0x87, 0x47, 0xc7, 0x27, 0xa7, 0x67, 0xe7,
    0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7,
    0x0f, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef,
    0x1f, 0x9f, 0x5f, 0xdf, 0x3f, 0xbf, 0x7f, 0xff
};

/**
 * @brief  interface spi bus init
 * @return status code
 *         - 0 success
 *         - 1 spi init failed
 * @note   none
 */
uint8_t isd17xx_interface_spi_init(void)
{
    return spi_init(SPI_DEVICE_NAME, &gs_fd, SPI_MODE_TYPE_3, 1000 * 100);
}

/**
 * @brief  interface spi bus deinit
 * @return status code
 *         - 0 success
 *         - 1 spi deinit failed
 * @note   none
 */
uint8_t isd17xx_interface_spi_deinit(void)
{
    return spi_deinit(gs_fd);
}

/**
 * @brief      interface spi bus read
 * @param[in]  reg is the register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the length of data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t isd17xx_interface_spi_read(uint8_t reg, uint8_t *buf, uint16_t len)
{
    uint8_t res;
    uint16_t i;

    /* spi read */
    res = spi_read(gs_fd, gs_bit_reverse_table[reg], buf, len);

    /* msb to lsb */
    for (i = 0; i < len; i++)
    {
        buf[i] = gs_bit_reverse_table[buf[i]]; 
    }

    return res;
}

/**
 * @brief     interface spi bus write
 * @param[in] reg is the register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the length of data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t isd17xx_interface_spi_write(uint8_t reg, uint8_t *buf, uint16_t len)
{
    uint16_t i;
    
    /* msb to lsb */
    for (i = 0; i < len; i++)
    {
        buf[i] = gs_bit_reverse_table[buf[i]]; 
    }

    return spi_write(gs_fd, gs_bit_reverse_table[reg], buf, len);
}

/**
 * @brief      interface spi bus transmit
 * @param[in]  *tx points to a tx data buffer
 * @param[out] *rx points to a rx data buffer
 * @param[in]  len is the length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 transmit failed
 * @note       none
 */
uint8_t isd17xx_interface_spi_transmit(uint8_t *tx, uint8_t *rx, uint16_t len)
{
    uint8_t res;
    uint16_t i;

    /* msb to lsb */
    for (i = 0; i < len; i++)
    {
        tx[i] = gs_bit_reverse_table[tx[i]]; 
    }

    res = spi_transmit(gs_fd, tx, rx, len);

    /* msb to lsb */
    for (i = 0; i < len; i++)
    {
        rx[i] = gs_bit_reverse_table[rx[i]]; 
    }

    return res;
}

/**
 * @brief  interface gpio reset init
 * @return status code
 *         - 0 success
 *         - 1 gpio reset init failed
 * @note   none
 */
uint8_t isd17xx_interface_gpio_reset_init(void)
{
    return wire_init();
}

/**
 * @brief  interface gpio reset deinit
 * @return status code
 *         - 0 success
 *         - 1 gpio reset deinit failed
 * @note   none
 */
uint8_t isd17xx_interface_gpio_reset_deinit(void)
{
    return wire_deinit();
}

/**
 * @brief     interface gpio reset write
 * @param[in] value is the write value
 * @return    status code
 *            - 0 success
 *            - 1 gpio reset write failed
 * @note      none
 */
uint8_t isd17xx_interface_gpio_reset_write(uint8_t value)
{
    return wire_write(value);
}

/**
 * @brief     interface delay ms
 * @param[in] ms
 * @note      none
 */
void isd17xx_interface_delay_ms(uint32_t ms)
{
    usleep(1000 * ms);
}

/**
 * @brief     interface print format data
 * @param[in] fmt is the format data
 * @note      none
 */
void isd17xx_interface_debug_print(const char *const fmt, ...)
{
    char str[256];
    uint16_t len;
    va_list args;
    
    memset((char *)str, 0, sizeof(char) * 256); 
    va_start(args, fmt);
    vsnprintf((char *)str, 255, (char const *)fmt, args);
    va_end(args);
    
    len = strlen((char *)str);
    (void)printf((uint8_t *)str, len);
}

/**
 * @brief     interface receive callback
 * @param[in] type is the interrupt type
 * @note      none
 */
void isd17xx_interface_receive_callback(uint16_t type)
{
    switch (type)
    {
        case ISD17XX_STATUS1_EOM :
        {
            isd17xx_interface_debug_print("isd17xx: irq eom.\n");
            
            break;
        }
        case ISD17XX_STATUS1_INT :
        {
            isd17xx_interface_debug_print("isd17xx: irq interrupt.\n");
            
            break;
        }
        default :
        {
            isd17xx_interface_debug_print("isd17xx: unknown code.\n");
            
            break;
        }
    }
}
