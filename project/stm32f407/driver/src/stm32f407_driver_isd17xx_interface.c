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
 * @file      stm32f407_driver_isd17xx_interface.c
 * @brief     stm32f407 driver isd17xx interface source file
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
#include "delay.h"
#include "spi.h"
#include "uart.h"
#include "wire.h"
#include <stdarg.h>

/**
 * @brief  interface spi bus init
 * @return status code
 *         - 0 success
 *         - 1 spi init failed
 * @note   none
 */
uint8_t isd17xx_interface_spi_init(void)
{
    return spi_init(SPI_MODE_3);
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
    return spi_deinit();
}

/**
 * @brief      interface spi bus read
 * @param[in]  reg register address
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len length of data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t isd17xx_interface_spi_read(uint8_t reg, uint8_t *buf, uint16_t len)
{
    return spi_read(reg, buf, len);
}

/**
 * @brief     interface spi bus write
 * @param[in] reg register address
 * @param[in] *buf pointer to a data buffer
 * @param[in] len length of data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t isd17xx_interface_spi_write(uint8_t reg, uint8_t *buf, uint16_t len)
{
    return spi_write(reg, buf, len);
}

/**
 * @brief      interface spi bus transmit
 * @param[in]  *tx pointer to a tx data buffer
 * @param[out] *rx pointer to a rx data buffer
 * @param[in]  len length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 transmit failed
 * @note       none
 */
uint8_t isd17xx_interface_spi_transmit(uint8_t *tx, uint8_t *rx, uint16_t len)
{
    return spi_transmit(tx, rx, len);
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
 * @param[in] value written value
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
 * @param[in] ms time
 * @note      none
 */
void isd17xx_interface_delay_ms(uint32_t ms)
{
    delay_ms(ms);
}

/**
 * @brief     interface print format data
 * @param[in] fmt format data
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
    (void)uart_write((uint8_t *)str, len);
}

/**
 * @brief     interface receive callback
 * @param[in] type interrupt type
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
