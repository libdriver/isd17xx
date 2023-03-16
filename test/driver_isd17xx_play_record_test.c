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
 * @file      driver_isd17xx_play_record_test.c
 * @brief     driver isd17xx play record test source file
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

#include "driver_isd17xx_play_record_test.h"

static isd17xx_handle_t gs_handle;        /**< isd17xx handle */

/**
 * @brief     play record test
 * @param[in] type is the chip type
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t isd17xx_play_record_test(isd17xx_type_t type)
{
    uint8_t res;
    uint8_t status2;
    uint16_t status1;
    isd17xx_info_t info;
    
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
    
    /* get information */
    res = isd17xx_info(&info);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: get info failed.\n");
        
        return 1;
    }
    else
    {
        /* print chip info */
        isd17xx_interface_debug_print("isd17xx: chip is %s.\n", info.chip_name);
        isd17xx_interface_debug_print("isd17xx: manufacturer is %s.\n", info.manufacturer_name);
        isd17xx_interface_debug_print("isd17xx: interface is %s.\n", info.interface);
        isd17xx_interface_debug_print("isd17xx: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        isd17xx_interface_debug_print("isd17xx: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        isd17xx_interface_debug_print("isd17xx: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        isd17xx_interface_debug_print("isd17xx: max current is %0.2fmA.\n", info.max_current_ma);
        isd17xx_interface_debug_print("isd17xx: max temperature is %0.1fC.\n", info.temperature_max);
        isd17xx_interface_debug_print("isd17xx: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start play record test */
    isd17xx_interface_debug_print("isd17xx: start play record test.\n");
    
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
    
    /* register control volume */
    res = isd17xx_set_volume_control(&gs_handle, ISD17XX_VOL_CONTROL_REG);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set volume control failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* max volume */
    res = isd17xx_set_volume(&gs_handle, 0x00);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set volume failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable monitor input */
    res = isd17xx_set_monitor_input(&gs_handle, ISD17XX_BOOL_FALSE);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set monitor input failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable mix input */
    res = isd17xx_set_mix_input(&gs_handle, ISD17XX_BOOL_FALSE);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set mix input failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable sound effect editing */
    res = isd17xx_set_sound_effect_editing(&gs_handle, ISD17XX_BOOL_TRUE);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set sound effect editing failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable spi ft */
    res = isd17xx_set_spi_ft(&gs_handle, ISD17XX_BOOL_FALSE);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set spi ft failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set analog output aud */
    res = isd17xx_set_analog_output(&gs_handle, ISD17XX_ANALOG_OUTPUT_AUD);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set analog output failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable pwm speaker */
    res = isd17xx_set_pwm_speaker(&gs_handle, ISD17XX_BOOL_TRUE);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set pwm speaker failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable power up analog output */
    res = isd17xx_set_power_up_analog_output(&gs_handle, ISD17XX_BOOL_TRUE);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set power up analog output failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable v alert */
    res = isd17xx_set_v_alert(&gs_handle, ISD17XX_BOOL_FALSE);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set v alert failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable eom */
    res = isd17xx_set_eom(&gs_handle, ISD17XX_BOOL_FALSE);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set eom failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* check the memory */
    res = isd17xx_check_memory(&gs_handle);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: check memory failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start global erase */
    isd17xx_interface_debug_print("isd17xx: start global erase.\n");
    
    /* start global erase */
    res = isd17xx_global_erase(&gs_handle);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: erase failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* wait */
    while (1)
    {
        /* get status */
        res = isd17xx_get_status(&gs_handle, &status1, &status2);
        if (res != 0)
        {
            isd17xx_interface_debug_print("isd17xx: get status failed.\n");
            (void)isd17xx_deinit(&gs_handle);
            
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
    
    /* start erase */
    isd17xx_interface_debug_print("isd17xx: start erase.\n");
    
    /* set erase */
    res = isd17xx_set_erase(&gs_handle, 0x10, 0xFF);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set erase failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start erase */
    res = isd17xx_erase(&gs_handle);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: erase failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* wait */
    while (1)
    {
        /* get status */
        res = isd17xx_get_status(&gs_handle, &status1, &status2);
        if (res != 0)
        {
            isd17xx_interface_debug_print("isd17xx: get status failed.\n");
            (void)isd17xx_deinit(&gs_handle);
            
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
    
    /* please speak */
    isd17xx_interface_debug_print("isd17xx: please speak...\n");
    
    /* set record */
    res = isd17xx_set_record(&gs_handle, 0x10, 0xFF);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set record failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start record */
    res = isd17xx_record(&gs_handle);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: record failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* wait */
    while (1)
    {
        /* get status */
        res = isd17xx_get_status(&gs_handle, &status1, &status2);
        if (res != 0)
        {
            isd17xx_interface_debug_print("isd17xx: get status failed.\n");
            (void)isd17xx_deinit(&gs_handle);
            
            return 1;
        }
        
        /* check the rec */
        if ((status2 & ISD17XX_STATUS2_REC) != ISD17XX_STATUS2_REC)
        {
            break;
        }
        
        /* delay 200ms */
        isd17xx_interface_delay_ms(200);
    }
    
    
    /* start play */
    isd17xx_interface_debug_print("isd17xx: start play.\n");
    
    /* set play */
    res = isd17xx_set_play(&gs_handle, 0x10, 0xFF);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set play failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start play */
    res = isd17xx_play(&gs_handle);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: play failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    while (1)
    {
        /* get status */
        res = isd17xx_get_status(&gs_handle, &status1, &status2);
        if (res != 0)
        {
            isd17xx_interface_debug_print("isd17xx: get status failed.\n");
            (void)isd17xx_deinit(&gs_handle);
            
            return 1;
        }
        
        /* check the play */
        if ((status2 & ISD17XX_STATUS2_PLAY) != ISD17XX_STATUS2_PLAY)
        {
            break;
        }
        
        /* delay 200ms */
        isd17xx_interface_delay_ms(200);
    }
    
    /* goto next */
    isd17xx_interface_debug_print("isd17xx: goto next.\n");
    
    /* goto the next */
    res = isd17xx_next(&gs_handle);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: next failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* finish play record test */
    isd17xx_interface_debug_print("isd17xx: finish play record test.\n");
    (void)isd17xx_deinit(&gs_handle);
    
    return 0;
}
