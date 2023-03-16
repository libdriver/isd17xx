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
 * @file      driver_isd17xx_interface_template.c
 * @brief     driver isd17xx interface template source file
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

/**
 * @brief  interface spi bus init
 * @return status code
 *         - 0 success
 *         - 1 spi init failed
 * @note   none
 */
uint8_t isd17xx_interface_spi_init(void)
{
    return 0;
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
    return 0;
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
    return 0;
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
    return 0;
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
    return 0;
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
    return 0;
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
    return 0;
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
    return 0;
}

/**
 * @brief     interface delay ms
 * @param[in] ms
 * @note      none
 */
void isd17xx_interface_delay_ms(uint32_t ms)
{

}

/**
 * @brief     interface print format data
 * @param[in] fmt is the format data
 * @note      none
 */
void isd17xx_interface_debug_print(const char *const fmt, ...)
{
    
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
