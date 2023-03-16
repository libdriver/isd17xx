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
 * @file      driver_isd17xx_basic.c
 * @brief     driver isd17xx basic source file
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

#include "driver_isd17xx_basic.h"

static isd17xx_handle_t gs_handle;        /**< isd17xx handle */

/**
 * @brief     basic example init
 * @param[in] type is the chip type
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t isd17xx_basic_init(isd17xx_type_t type)
{
    uint8_t res;
    
    /* link function */
    DRIVER_ISD17XX_LINK_INIT(&gs_handle, isd17xx_handle_t);
    DRIVER_ISD17XX_LINK_SPI_INIT(&gs_handle, isd17xx_interface_spi_init);
    DRIVER_ISD17XX_LINK_SPI_DEINIT(&gs_handle, isd17xx_interface_spi_deinit);
    DRIVER_ISD17XX_LINK_SPI_READ(&gs_handle, isd17xx_interface_spi_read);
    DRIVER_ISD17XX_LINK_SPI_WRITE(&gs_handle, isd17xx_interface_spi_write);
    DRIVER_ISD17XX_LINK_SPI_TRANSMIT(&gs_handle, isd17xx_interface_spi_transmit);
    DRIVER_ISD17XX_LINK_DELAY_MS(&gs_handle, isd17xx_interface_delay_ms);
    DRIVER_ISD17XX_LINK_DEBUG_PRINT(&gs_handle, isd17xx_interface_debug_print);
    DRIVER_ISD17XX_LINK_GPIO_RESET_INIT(&gs_handle, isd17xx_interface_gpio_reset_init);
    DRIVER_ISD17XX_LINK_GPIO_RESET_DEINIT(&gs_handle, isd17xx_interface_gpio_reset_deinit);
    DRIVER_ISD17XX_LINK_GPIO_RESET_WRITE(&gs_handle, isd17xx_interface_gpio_reset_write);
    
    /* set the type */
    res = isd17xx_set_type(&gs_handle, type);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set type failed.\n");
        
        return 1;
    }
    
    /* init */
    res = isd17xx_init(&gs_handle);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: init failed.\n");
        
        return 1;
    }
    
    /* power up */
    res = isd17xx_power_up(&gs_handle);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: power up failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default control volume */
    res = isd17xx_set_volume_control(&gs_handle, ISD17XX_BASIC_DEFAULT_VOL_CONTROL);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set volume control failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default volume */
    res = isd17xx_set_volume(&gs_handle, ISD17XX_BASIC_DEFAULT_VOL);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set volume failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default monitor input */
    res = isd17xx_set_monitor_input(&gs_handle, ISD17XX_BASIC_DEFAULT_MONITOR_INPUT);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set monitor input failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default mix input */
    res = isd17xx_set_mix_input(&gs_handle, ISD17XX_BASIC_DEFAULT_MIX_INPUT);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set mix input failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default sound effect editing */
    res = isd17xx_set_sound_effect_editing(&gs_handle, ISD17XX_BASIC_DEFAULT_SOUND_EFFECT_EDITING);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set sound effect editing failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default spi ft */
    res = isd17xx_set_spi_ft(&gs_handle, ISD17XX_BASIC_DEFAULT_SPI_FT);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set spi ft failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default analog output */
    res = isd17xx_set_analog_output(&gs_handle, ISD17XX_BASIC_DEFAULT_ANALOG_OUTPUT);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set analog output failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default pwm speaker */
    res = isd17xx_set_pwm_speaker(&gs_handle, ISD17XX_BASIC_DEFAULT_PWM_SPEAKER);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set pwm speaker failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default power up analog output */
    res = isd17xx_set_power_up_analog_output(&gs_handle, ISD17XX_BASIC_DEFAULT_POWER_UP_ANALOG_OUTPUT);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set power up analog output failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default v alert */
    res = isd17xx_set_v_alert(&gs_handle, ISD17XX_BASIC_DEFAULT_V_ALERT);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set v alert failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default eom */
    res = isd17xx_set_eom(&gs_handle, ISD17XX_BASIC_DEFAULT_EOM);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set eom failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* 100ms */
    isd17xx_interface_delay_ms(100);
    
    /* check the memory */
    res = isd17xx_check_memory(&gs_handle);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: check memory failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    return 0;
}

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t isd17xx_basic_deinit(void)
{
    if (isd17xx_deinit(&gs_handle) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief  basic example global erase
 * @return status code
 *         - 0 success
 *         - 1 erase failed
 * @note   none
 */
uint8_t isd17xx_basic_global_erase(void)
{
    uint8_t res;
    uint8_t status2;
    uint16_t status1;
    
    /* global erase */
    res = isd17xx_global_erase(&gs_handle);
    if (res != 0)
    {
        return 1;
    }
    
    /* wait */
    while (1)
    {
        /* get status */
        res = isd17xx_get_status(&gs_handle, &status1, &status2);
        if (res != 0)
        {
            return 1;
        }
        
        /* check erase */
        if ((status2 & ISD17XX_STATUS2_ERASE) != ISD17XX_STATUS2_ERASE)
        {
            break;
        }
        
        /* delay 200ms */
        isd17xx_interface_delay_ms(200);
    }
    
    return 0;
}

/**
 * @brief     basic example erase
 * @param[in] start_addr is the start address
 * @param[in] end_addr is the end address
 * @return    status code
 *            - 0 success
 *            - 1 erase failed
 * @note      none
 */
uint8_t isd17xx_basic_erase(uint16_t start_addr, uint16_t end_addr)
{
    uint8_t res;
    uint8_t status2;
    uint16_t status1;
    
    /* set erase address */
    res = isd17xx_set_erase(&gs_handle, start_addr, end_addr);
    if (res != 0)
    {
        return 1;
    }
    
    /* erase */
    res = isd17xx_erase(&gs_handle);
    if (res != 0)
    {
        return 1;
    }
    
    /* wait */
    while (1)
    {
        /* get status */
        res = isd17xx_get_status(&gs_handle, &status1, &status2);
        if (res != 0)
        {
            return 1;
        }
        
        /* check erase */
        if ((status2 & ISD17XX_STATUS2_ERASE) != ISD17XX_STATUS2_ERASE)
        {
            break;
        }
        
        /* delay 200ms */
        isd17xx_interface_delay_ms(200);
    }
    
    return 0;
}

/**
 * @brief     basic example record
 * @param[in] start_addr is the start address
 * @param[in] end_addr is the end address
 * @return    status code
 *            - 0 success
 *            - 1 record failed
 * @note      none
 */
uint8_t isd17xx_basic_record(uint16_t start_addr, uint16_t end_addr)
{
    uint8_t res;
    
    /* set record address */
    res = isd17xx_set_record(&gs_handle, start_addr, end_addr);
    if (res != 0)
    {
        return 1;
    }
    
    /* record */
    res = isd17xx_record(&gs_handle);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief     basic example play
 * @param[in] start_addr is the start address
 * @param[in] end_addr is the end address
 * @return    status code
 *            - 0 success
 *            - 1 play failed
 * @note      none
 */
uint8_t isd17xx_basic_play(uint16_t start_addr, uint16_t end_addr)
{
    uint8_t res;
    
    /* set play address */
    res = isd17xx_set_play(&gs_handle, start_addr, end_addr);
    if (res != 0)
    {
        return 1;
    }
    
    /* play */
    res = isd17xx_play(&gs_handle);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief     basic example poll
 * @param[in] status is the poll status
 * @return    status code
 *            - 0 success
 *            - 1 poll failed
 * @note      none
 */
uint8_t isd17xx_basic_poll(isd17xx_status2_t status)
{
    uint8_t res;
    uint8_t status2;
    uint16_t status1;
    
    /* wait */
    while (1)
    {
        /* get status */
        res = isd17xx_get_status(&gs_handle, &status1, &status2);
        if (res != 0)
        {
            return 1;
        }
        
        /* check the status */
        if ((status2 & status) != status)
        {
            break;
        }
        
        /* delay 200ms */
        isd17xx_interface_delay_ms(200);
    }
    
    return 0;
}

/**
 * @brief  basic example next
 * @return status code
 *         - 0 success
 *         - 1 next failed
 * @note   none
 */
uint8_t isd17xx_basic_next(void)
{
    uint8_t res;
    
    /* goto the next */
    res = isd17xx_next(&gs_handle);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  basic example stop
 * @return status code
 *         - 0 success
 *         - 1 stop failed
 * @note   none
 */
uint8_t isd17xx_basic_stop(void)
{
    uint8_t res;
    
    /* stop */
    res = isd17xx_stop(&gs_handle);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}
