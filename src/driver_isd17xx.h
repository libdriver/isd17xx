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
 * @file      driver_isd17xx.h
 * @brief     driver isd17xx header file
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

#ifndef DRIVER_ISD17XX_H
#define DRIVER_ISD17XX_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup isd17xx_driver isd17xx driver function
 * @brief    isd17xx driver modules
 * @{
 */

/**
 * @addtogroup isd17xx_base_driver
 * @{
 */

/**
 * @brief isd17xx type enumeration definition
 */
typedef enum
{
    ISD1730  = 0x10,        /**< isd1730 */
    ISD1740  = 0x16,        /**< isd1740 */
    ISD1750  = 0x15,        /**< isd1750 */
    ISD1760  = 0x14,        /**< isd1760 */
    ISD1790  = 0x1A,        /**< isd1790 */
    ISD17120 = 0x19,        /**< isd17120 */
    ISD17150 = 0x18,        /**< isd17150 */
    ISD17180 = 0x1E,        /**< isd17180 */
    ISD17210 = 0x1D,        /**< isd17210 */
    ISD17240 = 0x1C,        /**< isd17240 */
} isd17xx_type_t;

/**
 * @brief isd17xx bool enumeration definition
 */
typedef enum
{
    ISD17XX_BOOL_FALSE = 0x00,        /**< disable */
    ISD17XX_BOOL_TRUE  = 0x01,        /**< enable */
} isd17xx_bool_t;

/**
 * @brief isd17xx analog output enumeration definition
 */
typedef enum
{
    ISD17XX_ANALOG_OUTPUT_AUD = 0x00,        /**< aud */
    ISD17XX_ANALOG_OUTPUT_AUX = 0x01,        /**< aux */
} isd17xx_analog_output_t;

/**
 * @brief isd17xx vol control enumeration definition
 */
typedef enum
{
    ISD17XX_VOL_CONTROL_BUTTON = 0x00,        /**< button control */
    ISD17XX_VOL_CONTROL_REG    = 0x01,        /**< register control */
} isd17xx_vol_control_t;

/**
 * @brief isd17xx status1 enumeration definition
 */
typedef enum
{
    ISD17XX_STATUS1_CMD_ERR = (1 << 0),        /**< command error */
    ISD17XX_STATUS1_FULL    = (1 << 1),        /**< full */
    ISD17XX_STATUS1_PU      = (1 << 2),        /**< power up */
    ISD17XX_STATUS1_EOM     = (1 << 3),        /**< eom */
    ISD17XX_STATUS1_INT     = (1 << 4),        /**< interrupt */
} isd17xx_status1_t;

/**
 * @brief isd17xx status2 enumeration definition
 */
typedef enum
{
    ISD17XX_STATUS2_RDY   = (1 << 0),        /**< ready */
    ISD17XX_STATUS2_ERASE = (1 << 1),        /**< erase */
    ISD17XX_STATUS2_PLAY  = (1 << 2),        /**< play */
    ISD17XX_STATUS2_REC   = (1 << 3),        /**< record */
    ISD17XX_STATUS2_SE4   = (1 << 4),        /**< se4 */
    ISD17XX_STATUS2_SE3   = (1 << 5),        /**< se3 */
    ISD17XX_STATUS2_SE2   = (1 << 6),        /**< se2 */
    ISD17XX_STATUS2_SE1   = (1 << 7),        /**< se1 */
} isd17xx_status2_t;

/**
 * @brief isd17xx handle structure definition
 */
typedef struct isd17xx_handle_s
{
    uint8_t (*spi_init)(void);                                              /**< point to a spi_init function address */
    uint8_t (*spi_deinit)(void);                                            /**< point to a spi_deinit function address */
    uint8_t (*spi_read)(uint8_t reg, uint8_t *buf, uint16_t len);           /**< point to a spi_read function address */
    uint8_t (*spi_write)(uint8_t reg, uint8_t *buf, uint16_t len);          /**< point to a spi_write function address */
    uint8_t (*spi_transmit)(uint8_t *tx, uint8_t *rx, uint16_t len);        /**< point to a spi_transmit function address */
    uint8_t (*gpio_reset_init)(void);                                       /**< point to a gpio_reset_init function address */
    uint8_t (*gpio_reset_deinit)(void);                                     /**< point to a gpio_reset_deinit function address */
    uint8_t (*gpio_reset_write)(uint8_t level);                             /**< point to a gpio_reset_write function address */
    void (*receive_callback)(uint16_t type);                                /**< point to a receive_callback function address */
    void (*delay_ms)(uint32_t ms);                                          /**< point to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);                        /**< point to a debug_print function address */
    uint16_t end_address;                                                   /**< end address */
    uint8_t vol_control;                                                    /**< vol control */
    uint8_t type;                                                           /**< chip type */
    uint8_t inited;                                                         /**< inited flag */
    uint8_t done;                                                           /**< done flag */
} isd17xx_handle_t;

/**
 * @brief isd17xx information structure definition
 */
typedef struct isd17xx_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} isd17xx_info_t;

/**
 * @}
 */

/**
 * @defgroup isd17xx_link_driver isd17xx link driver function
 * @brief    isd17xx link driver modules
 * @ingroup  isd17xx_driver
 * @{
 */

/**
 * @brief     initialize isd17xx_handle_t structure
 * @param[in] HANDLE points to an isd17xx handle structure
 * @param[in] STRUCTURE is isd17xx_handle_t
 * @note      none
 */
#define DRIVER_ISD17XX_LINK_INIT(HANDLE, STRUCTURE)               memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link spi_init function
 * @param[in] HANDLE points to an isd17xx handle structure
 * @param[in] FUC points to a spi_init function address
 * @note      none
 */
#define DRIVER_ISD17XX_LINK_SPI_INIT(HANDLE, FUC)                 (HANDLE)->spi_init = FUC

/**
 * @brief     link spi_deinit function
 * @param[in] HANDLE points to an isd17xx handle structure
 * @param[in] FUC points to a spi_deinit function address
 * @note      none
 */
#define DRIVER_ISD17XX_LINK_SPI_DEINIT(HANDLE, FUC)               (HANDLE)->spi_deinit = FUC

/**
 * @brief     link spi_read function
 * @param[in] HANDLE points to an isd17xx handle structure
 * @param[in] FUC points to a spi_read function address
 * @note      none
 */
#define DRIVER_ISD17XX_LINK_SPI_READ(HANDLE, FUC)                 (HANDLE)->spi_read = FUC

/**
 * @brief     link spi_write function
 * @param[in] HANDLE points to an isd17xx handle structure
 * @param[in] FUC points to a spi_write function address
 * @note      none
 */
#define DRIVER_ISD17XX_LINK_SPI_WRITE(HANDLE, FUC)                (HANDLE)->spi_write = FUC

/**
 * @brief     link spi_transmit function
 * @param[in] HANDLE points to an isd17xx handle structure
 * @param[in] FUC points to a spi_transmit function address
 * @note      none
 */
#define DRIVER_ISD17XX_LINK_SPI_TRANSMIT(HANDLE, FUC)             (HANDLE)->spi_transmit = FUC

/**
 * @brief     link gpio_reset_init function
 * @param[in] HANDLE points to an isd17xx handle structure
 * @param[in] FUC points to a gpio_reset_init function address
 * @note      none
 */
#define DRIVER_ISD17XX_LINK_GPIO_RESET_INIT(HANDLE, FUC)          (HANDLE)->gpio_reset_init = FUC

/**
 * @brief     link gpio_reset_deinit function
 * @param[in] HANDLE points to an isd17xx handle structure
 * @param[in] FUC points to a gpio_reset_deinit function address
 * @note      none
 */
#define DRIVER_ISD17XX_LINK_GPIO_RESET_DEINIT(HANDLE, FUC)        (HANDLE)->gpio_reset_deinit = FUC

/**
 * @brief     link gpio_reset_write function
 * @param[in] HANDLE points to an isd17xx handle structure
 * @param[in] FUC points to a gpio_reset_write function address
 * @note      none
 */
#define DRIVER_ISD17XX_LINK_GPIO_RESET_WRITE(HANDLE, FUC)         (HANDLE)->gpio_reset_write = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE points to an isd17xx handle structure
 * @param[in] FUC points to a delay_ms function address
 * @note      none
 */
#define DRIVER_ISD17XX_LINK_DELAY_MS(HANDLE, FUC)                 (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE points to an isd17xx handle structure
 * @param[in] FUC points to a debug_print function address
 * @note      none
 */
#define DRIVER_ISD17XX_LINK_DEBUG_PRINT(HANDLE, FUC)              (HANDLE)->debug_print = FUC

/**
 * @brief     link receive_callback function
 * @param[in] HANDLE points to an isd17xx handle structure
 * @param[in] FUC points to a receive_callback function address
 * @note      none
 */
#define DRIVER_ISD17XX_LINK_RECEIVE_CALLBACK(HANDLE, FUC)         (HANDLE)->receive_callback = FUC

/**
 * @}
 */

/**
 * @defgroup isd17xx_base_driver isd17xx base driver function
 * @brief    isd17xx base driver modules
 * @ingroup  isd17xx_driver
 * @{
 */

/**
 * @brief      get chip's information
 * @param[out] *info points to an isd17xx info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t isd17xx_info(isd17xx_info_t *info);

/**
 * @brief     set the chip type
 * @param[in] *handle points to an isd17xx handle structure
 * @param[in] type is the chip type
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t isd17xx_set_type(isd17xx_handle_t *handle, isd17xx_type_t type);

/**
 * @brief      get the chip type
 * @param[in]  *handle points to an isd17xx handle structure
 * @param[out] *type points to a chip type buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t isd17xx_get_type(isd17xx_handle_t *handle, isd17xx_type_t *type);

/**
 * @brief     irq handler
 * @param[in] *handle points to an isd17xx handle structure
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t isd17xx_irq_handler(isd17xx_handle_t *handle);

/**
 * @brief     initialize the chip
 * @param[in] *handle points to an isd17xx handle structure
 * @return    status code
 *            - 0 success
 *            - 1 spi or gpio initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 *            - 4 get device id failed.
 *            - 5 chip type is invalid
 *            - 6 unknown type
 *            - 7 set power up failed
 *            - 8 set reset failed
 * @note      spi need lsb fist
 */
uint8_t isd17xx_init(isd17xx_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle points to an isd17xx handle structure
 * @return    status code
 *            - 0 success
 *            - 1 deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 set power down failed
 * @note      none
 */
uint8_t isd17xx_deinit(isd17xx_handle_t *handle);

/**
 * @brief     power up
 * @param[in] *handle points to an isd17xx handle structure
 * @return    status code
 *            - 0 success
 *            - 1 power up failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t isd17xx_power_up(isd17xx_handle_t *handle);

/**
 * @brief     power down
 * @param[in] *handle points to an isd17xx handle structure
 * @return    status code
 *            - 0 success
 *            - 1 power down failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t isd17xx_power_down(isd17xx_handle_t *handle);

/**
 * @brief     play
 * @param[in] *handle points to an isd17xx handle structure
 * @return    status code
 *            - 0 success
 *            - 1 play failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t isd17xx_play(isd17xx_handle_t *handle);

/**
 * @brief     record
 * @param[in] *handle points to an isd17xx handle structure
 * @return    status code
 *            - 0 success
 *            - 1 record failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t isd17xx_record(isd17xx_handle_t *handle);

/**
 * @brief     stop
 * @param[in] *handle points to an isd17xx handle structure
 * @return    status code
 *            - 0 success
 *            - 1 stop failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t isd17xx_stop(isd17xx_handle_t *handle);

/**
 * @brief     erase
 * @param[in] *handle points to an isd17xx handle structure
 * @return    status code
 *            - 0 success
 *            - 1 erase failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t isd17xx_erase(isd17xx_handle_t *handle);

/**
 * @brief     global erase
 * @param[in] *handle points to an isd17xx handle structure
 * @return    status code
 *            - 0 success
 *            - 1 global erase failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t isd17xx_global_erase(isd17xx_handle_t *handle);

/**
 * @brief     next
 * @param[in] *handle points to an isd17xx handle structure
 * @return    status code
 *            - 0 success
 *            - 1 next failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t isd17xx_next(isd17xx_handle_t *handle);

/**
 * @brief     check memory
 * @param[in] *handle points to an isd17xx handle structure
 * @return    status code
 *            - 0 success
 *            - 1 check memory failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t isd17xx_check_memory(isd17xx_handle_t *handle);

/**
 * @brief     reset
 * @param[in] *handle points to an isd17xx handle structure
 * @return    status code
 *            - 0 success
 *            - 1 reset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t isd17xx_reset(isd17xx_handle_t *handle);

/**
 * @brief     hardware reset
 * @param[in] *handle points to an isd17xx handle structure
 * @return    status code
 *            - 0 success
 *            - 1 reset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t isd17xx_hardware_reset(isd17xx_handle_t *handle);

/**
 * @brief     clear interrupt
 * @param[in] *handle points to an isd17xx handle structure
 * @return    status code
 *            - 0 success
 *            - 1 clear interrupt failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t isd17xx_clear_interrupt(isd17xx_handle_t *handle);

/**
 * @brief      get the status
 * @param[in]  *handle points to an isd17xx handle structure
 * @param[in]  *status1 points to a status1 buffer
 * @param[out] *status2 points to a status2 buffer
 * @return     status code
 *             - 0 success
 *             - 1 get status failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t isd17xx_get_status(isd17xx_handle_t *handle, uint16_t *status1, uint8_t *status2);

/**
 * @brief      get the device id
 * @param[in]  *handle points to an isd17xx handle structure
 * @param[in]  *id points to an id buffer
 * @return     status code
 *             - 0 success
 *             - 1 get device id failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t isd17xx_get_device_id(isd17xx_handle_t *handle, uint8_t *id);

/**
 * @brief      read the play point
 * @param[in]  *handle points to an isd17xx handle structure
 * @param[out] *point points to a play point buffer
 * @return     status code
 *             - 0 success
 *             - 1 read play point failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t isd17xx_read_play_point(isd17xx_handle_t *handle, uint16_t *point);

/**
 * @brief      read the record point
 * @param[in]  *handle points to an isd17xx handle structure
 * @param[out] *point points to a record point buffer
 * @return     status code
 *             - 0 success
 *             - 1 read record point failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t isd17xx_read_record_point(isd17xx_handle_t *handle, uint16_t *point);

/**
 * @brief     set the volume control type
 * @param[in] *handle points to an isd17xx handle structure
 * @param[in] control is the volume control type
 * @return    status code
 *            - 0 success
 *            - 1 set volume control failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t isd17xx_set_volume_control(isd17xx_handle_t *handle, isd17xx_vol_control_t control);

/**
 * @brief      get the volume control type
 * @param[in]  *handle points to an isd17xx handle structure
 * @param[out] *control points to a volume control type buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t isd17xx_get_volume_control(isd17xx_handle_t *handle, isd17xx_vol_control_t *control);

/**
 * @brief     set the volume
 * @param[in] *handle points to an isd17xx handle structure
 * @param[in] vol is the set volume
 * @return    status code
 *            - 0 success
 *            - 1 set volume failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 vol is over 7
 * @note      -4dB per step volume adjustment
 *            0 is max and 7 is min
 */
uint8_t isd17xx_set_volume(isd17xx_handle_t *handle, uint8_t vol);

/**
 * @brief      get the volume
 * @param[in]  *handle points to an isd17xx handle structure
 * @param[out] *vol points to a volume buffer
 * @return     status code
 *             - 0 success
 *             - 1 get volume failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t isd17xx_get_volume(isd17xx_handle_t *handle, uint8_t *vol);

/**
 * @brief     enable or disable the monitor input
 * @param[in] *handle points to an isd17xx handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set monitor input failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t isd17xx_set_monitor_input(isd17xx_handle_t *handle, isd17xx_bool_t enable);

/**
 * @brief      get the monitor input status
 * @param[in]  *handle points to an isd17xx handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get monitor input failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t isd17xx_get_monitor_input(isd17xx_handle_t *handle, isd17xx_bool_t *enable);

/**
 * @brief     enable or disable the mix input
 * @param[in] *handle points to an isd17xx handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set mix input failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t isd17xx_set_mix_input(isd17xx_handle_t *handle, isd17xx_bool_t enable);

/**
 * @brief      get the mix input status
 * @param[in]  *handle points to an isd17xx handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get mix input failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t isd17xx_get_mix_input(isd17xx_handle_t *handle, isd17xx_bool_t *enable);

/**
 * @brief     enable or disable sound effect editing
 * @param[in] *handle points to an isd17xx handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set sound effect editing failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t isd17xx_set_sound_effect_editing(isd17xx_handle_t *handle, isd17xx_bool_t enable);

/**
 * @brief      get the sound effect editing status
 * @param[in]  *handle points to an isd17xx handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get sound effect editing failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t isd17xx_get_sound_effect_editing(isd17xx_handle_t *handle, isd17xx_bool_t *enable);

/**
 * @brief     enable or disable spi ft
 * @param[in] *handle points to an isd17xx handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set spi ft failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t isd17xx_set_spi_ft(isd17xx_handle_t *handle, isd17xx_bool_t enable);

/**
 * @brief      get the spi ft status
 * @param[in]  *handle points to an isd17xx handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get spi ft failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t isd17xx_get_spi_ft(isd17xx_handle_t *handle, isd17xx_bool_t *enable);

/**
 * @brief     set the analog output type
 * @param[in] *handle points to an isd17xx handle structure
 * @param[in] output is the analog output type
 * @return    status code
 *            - 0 success
 *            - 1 set analog output failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t isd17xx_set_analog_output(isd17xx_handle_t *handle, isd17xx_analog_output_t output);

/**
 * @brief      get the analog output type
 * @param[in]  *handle points to an isd17xx handle structure
 * @param[out] *output points to an analog output type buffer
 * @return     status code
 *             - 0 success
 *             - 1 get analog output failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t isd17xx_get_analog_output(isd17xx_handle_t *handle, isd17xx_analog_output_t *output);

/**
 * @brief     enable or disable pwm speaker
 * @param[in] *handle points to an isd17xx handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set pwm speaker failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t isd17xx_set_pwm_speaker(isd17xx_handle_t *handle, isd17xx_bool_t enable);

/**
 * @brief      get the pwm speaker status
 * @param[in]  *handle points to an isd17xx handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get pwm speaker failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t isd17xx_get_pwm_speaker(isd17xx_handle_t *handle, isd17xx_bool_t *enable);

/**
 * @brief     enable or disable power up analog output
 * @param[in] *handle points to an isd17xx handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set power up analog output failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t isd17xx_set_power_up_analog_output(isd17xx_handle_t *handle, isd17xx_bool_t enable);

/**
 * @brief      get the power up analog output status
 * @param[in]  *handle points to an isd17xx handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get power up analog output failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t isd17xx_get_power_up_analog_output(isd17xx_handle_t *handle, isd17xx_bool_t *enable);

/**
 * @brief     enable or disable v alert
 * @param[in] *handle points to an isd17xx handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set v alert failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t isd17xx_set_v_alert(isd17xx_handle_t *handle, isd17xx_bool_t enable);

/**
 * @brief      get the v alert status
 * @param[in]  *handle points to an isd17xx handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get v alert failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t isd17xx_get_v_alert(isd17xx_handle_t *handle, isd17xx_bool_t *enable);

/**
 * @brief     enable or disable eom stop playing
 * @param[in] *handle points to an isd17xx handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set eom failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t isd17xx_set_eom(isd17xx_handle_t *handle, isd17xx_bool_t enable);

/**
 * @brief      get the eom status
 * @param[in]  *handle points to an isd17xx handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get eom failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t isd17xx_get_eom(isd17xx_handle_t *handle, isd17xx_bool_t *enable);

/**
 * @brief     write config to non volatile memory
 * @param[in] *handle points to an isd17xx handle structure
 * @return    status code
 *            - 0 success
 *            - 1 write to nvc failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t isd17xx_write_to_nv(isd17xx_handle_t *handle);

/**
 * @brief     load config from non volatile memory
 * @param[in] *handle points to an isd17xx handle structure
 * @return    status code
 *            - 0 success
 *            - 1 load from nvc failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t isd17xx_load_from_nv(isd17xx_handle_t *handle);

/**
 * @brief     set the play point
 * @param[in] *handle points to an isd17xx handle structure
 * @param[in] start_addr is the start address
 * @param[in] end_addr is the end address
 * @return    status code
 *            - 0 success
 *            - 1 set play failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 start_addr >= end_addr
 *            - 5 address is invalid
 * @note      none
 */
uint8_t isd17xx_set_play(isd17xx_handle_t *handle, uint16_t start_addr, uint16_t end_addr);

/**
 * @brief     set the record point
 * @param[in] *handle points to an isd17xx handle structure
 * @param[in] start_addr is the start address
 * @param[in] end_addr is the end address
 * @return    status code
 *            - 0 success
 *            - 1 set record failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 start_addr >= end_addr
 *            - 5 address is invalid
 * @note      none
 */
uint8_t isd17xx_set_record(isd17xx_handle_t *handle, uint16_t start_addr, uint16_t end_addr);

/**
 * @brief     set the erase point
 * @param[in] *handle points to an isd17xx handle structure
 * @param[in] start_addr is the start address
 * @param[in] end_addr is the end address
 * @return    status code
 *            - 0 success
 *            - 1 set erase failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 start_addr >= end_addr
 *            - 5 address is invalid
 * @note      none
 */
uint8_t isd17xx_set_erase(isd17xx_handle_t *handle, uint16_t start_addr, uint16_t end_addr);

/**
 * @brief     toggle the enable and disable of the external clock
 * @param[in] *handle points to an isd17xx handle structure
 * @return    status code
 *            - 0 success
 *            - 1 toggle extern clock failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t isd17xx_toggle_extern_clock(isd17xx_handle_t *handle);

/**
 * @}
 */

/**
 * @defgroup isd17xx_extend_driver isd17xx extend driver function
 * @brief    isd17xx extend driver modules
 * @ingroup  isd17xx_driver
 * @{
 */

/**
 * @brief     set the chip register
 * @param[in] *handle points to an isd17xx handle structure
 * @param[in] cmd is the command
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t isd17xx_set_reg(isd17xx_handle_t *handle, uint8_t cmd, uint8_t *buf, uint16_t len);

/**
 * @brief      get the chip register
 * @param[in]  *handle points to an isd17xx handle structure
 * @param[in]  cmd is the command
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data buffer length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t isd17xx_get_reg(isd17xx_handle_t *handle, uint8_t cmd, uint8_t *buf, uint16_t len);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
