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
 * @file      driver_isd17xx_basic.h
 * @brief     driver isd17xx basic header file
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

#ifndef DRIVER_ISD17XX_BASIC_H
#define DRIVER_ISD17XX_BASIC_H

#include "driver_isd17xx_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup isd17xx_example_driver isd17xx example driver function
 * @brief    isd17xx example driver modules
 * @ingroup  isd17xx_driver
 * @{
 */

/**
 * @brief isd17xx basic example default definition
 */
#define ISD17XX_BASIC_DEFAULT_VOL_CONTROL                   ISD17XX_VOL_CONTROL_REG          /**< control reg */
#define ISD17XX_BASIC_DEFAULT_VOL                           0x00                             /**< max vol */
#define ISD17XX_BASIC_DEFAULT_MONITOR_INPUT                 ISD17XX_BOOL_FALSE               /**< disable monitor input */
#define ISD17XX_BASIC_DEFAULT_MIX_INPUT                     ISD17XX_BOOL_FALSE               /**< disable mix input */
#define ISD17XX_BASIC_DEFAULT_SOUND_EFFECT_EDITING          ISD17XX_BOOL_TRUE                /**< enable sound effect editing */
#define ISD17XX_BASIC_DEFAULT_SPI_FT                        ISD17XX_BOOL_FALSE               /**< disable spi ft */
#define ISD17XX_BASIC_DEFAULT_ANALOG_OUTPUT                 ISD17XX_ANALOG_OUTPUT_AUD        /**< analog output aud */
#define ISD17XX_BASIC_DEFAULT_PWM_SPEAKER                   ISD17XX_BOOL_TRUE                /**< enable pwm speaker */
#define ISD17XX_BASIC_DEFAULT_POWER_UP_ANALOG_OUTPUT        ISD17XX_BOOL_TRUE                /**< enable power up analog output */
#define ISD17XX_BASIC_DEFAULT_V_ALERT                       ISD17XX_BOOL_FALSE               /**< disable v alert */
#define ISD17XX_BASIC_DEFAULT_EOM                           ISD17XX_BOOL_FALSE               /**< disable eom */

/**
 * @brief     basic example init
 * @param[in] type is the chip type
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t isd17xx_basic_init(isd17xx_type_t type);

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t isd17xx_basic_deinit(void);

/**
 * @brief  basic example global erase
 * @return status code
 *         - 0 success
 *         - 1 erase failed
 * @note   none
 */
uint8_t isd17xx_basic_global_erase(void);

/**
 * @brief     basic example erase
 * @param[in] start_addr is the start address
 * @param[in] end_addr is the end address
 * @return    status code
 *            - 0 success
 *            - 1 erase failed
 * @note      none
 */
uint8_t isd17xx_basic_erase(uint16_t start_addr, uint16_t end_addr);

/**
 * @brief     basic example record
 * @param[in] start_addr is the start address
 * @param[in] end_addr is the end address
 * @return    status code
 *            - 0 success
 *            - 1 record failed
 * @note      none
 */
uint8_t isd17xx_basic_record(uint16_t start_addr, uint16_t end_addr);

/**
 * @brief     basic example play
 * @param[in] start_addr is the start address
 * @param[in] end_addr is the end address
 * @return    status code
 *            - 0 success
 *            - 1 play failed
 * @note      none
 */
uint8_t isd17xx_basic_play(uint16_t start_addr, uint16_t end_addr);

/**
 * @brief     basic example poll
 * @param[in] status is the poll status
 * @return    status code
 *            - 0 success
 *            - 1 poll failed
 * @note      none
 */
uint8_t isd17xx_basic_poll(isd17xx_status2_t status);

/**
 * @brief  basic example next
 * @return status code
 *         - 0 success
 *         - 1 next failed
 * @note   none
 */
uint8_t isd17xx_basic_next(void);

/**
 * @brief  basic example stop
 * @return status code
 *         - 0 success
 *         - 1 stop failed
 * @note   none
 */
uint8_t isd17xx_basic_stop(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif