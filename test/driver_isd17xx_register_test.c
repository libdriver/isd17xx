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
 * @file      driver_isd17xx_register_test.c
 * @brief     driver isd17xx register test source file
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

#include "driver_isd17xx_register_test.h"
#include <stdlib.h>

static isd17xx_handle_t gs_handle;        /**< isd17xx handle */

/**
 * @brief     register test
 * @param[in] type is the chip type
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t isd17xx_register_test(isd17xx_type_t type)
{
    uint8_t res;
    uint8_t id;
    uint8_t vol;
    uint8_t vol_check;
    uint8_t status2;
    uint16_t status1;
    uint16_t point;
    isd17xx_bool_t enable;
    isd17xx_info_t info;
    isd17xx_type_t type_check;
    isd17xx_vol_control_t control;
    isd17xx_analog_output_t output;
    
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
    
    /* start register test */
    isd17xx_interface_debug_print("isd17xx: start register test.\n");
    
    /* isd17xx_set_type/isd17xx_get_type test */
    isd17xx_interface_debug_print("isd17xx: isd17xx_set_type/isd17xx_get_type test.\n");
    
    /* set type isd1730 */
    res = isd17xx_set_type(&gs_handle, ISD1730);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set type failed.\n");
        
        return 1;
    }
    
    /* output */
    isd17xx_interface_debug_print("isd17xx: set type isd1730.\n");
    
    /* get type */
    res = isd17xx_get_type(&gs_handle, &type_check);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: get type failed.\n");
        
        return 1;
    }
    isd17xx_interface_debug_print("isd17xx: check type %s.\n", (ISD1730 == type_check) ? "ok" : "error");
    
    /* set type isd1740 */
    res = isd17xx_set_type(&gs_handle, ISD1740);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set type failed.\n");
        
        return 1;
    }
    
    /* output */
    isd17xx_interface_debug_print("isd17xx: set type isd1740.\n");
    
    /* get type */
    res = isd17xx_get_type(&gs_handle, &type_check);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: get type failed.\n");
        
        return 1;
    }
    isd17xx_interface_debug_print("isd17xx: check type %s.\n", (ISD1740 == type_check) ? "ok" : "error");
    
    /* set type isd1750 */
    res = isd17xx_set_type(&gs_handle, ISD1750);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set type failed.\n");
        
        return 1;
    }
    
    /* output */
    isd17xx_interface_debug_print("isd17xx: set type isd1750.\n");
    
    /* get type */
    res = isd17xx_get_type(&gs_handle, &type_check);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: get type failed.\n");
        
        return 1;
    }
    isd17xx_interface_debug_print("isd17xx: check type %s.\n", (ISD1750 == type_check) ? "ok" : "error");
    
    /* set type isd1760 */
    res = isd17xx_set_type(&gs_handle, ISD1760);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set type failed.\n");
        
        return 1;
    }
    
    /* output */
    isd17xx_interface_debug_print("isd17xx: set type isd1760.\n");
    
    /* get type */
    res = isd17xx_get_type(&gs_handle, &type_check);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: get type failed.\n");
        
        return 1;
    }
    isd17xx_interface_debug_print("isd17xx: check type %s.\n", (ISD1760 == type_check) ? "ok" : "error");
    
    /* set type isd1790 */
    res = isd17xx_set_type(&gs_handle, ISD1790);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set type failed.\n");
        
        return 1;
    }
    
    /* output */
    isd17xx_interface_debug_print("isd17xx: set type isd1790.\n");
    
    /* get type */
    res = isd17xx_get_type(&gs_handle, &type_check);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: get type failed.\n");
        
        return 1;
    }
    isd17xx_interface_debug_print("isd17xx: check type %s.\n", (ISD1790 == type_check) ? "ok" : "error");
    
    /* set type isd17120 */
    res = isd17xx_set_type(&gs_handle, ISD17120);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set type failed.\n");
        
        return 1;
    }
    
    /* output */
    isd17xx_interface_debug_print("isd17xx: set type isd17120.\n");
    
    /* get type */
    res = isd17xx_get_type(&gs_handle, &type_check);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: get type failed.\n");
        
        return 1;
    }
    isd17xx_interface_debug_print("isd17xx: check type %s.\n", (ISD17120 == type_check) ? "ok" : "error");
    
    /* set type isd17150 */
    res = isd17xx_set_type(&gs_handle, ISD17150);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set type failed.\n");
        
        return 1;
    }
    
    /* output */
    isd17xx_interface_debug_print("isd17xx: set type isd17150.\n");
    
    /* get type */
    res = isd17xx_get_type(&gs_handle, &type_check);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: get type failed.\n");
        
        return 1;
    }
    isd17xx_interface_debug_print("isd17xx: check type %s.\n", (ISD17150 == type_check) ? "ok" : "error");
    
    /* set type isd17180 */
    res = isd17xx_set_type(&gs_handle, ISD17180);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set type failed.\n");
        
        return 1;
    }
    
    /* output */
    isd17xx_interface_debug_print("isd17xx: set type isd17180.\n");
    
    /* get type */
    res = isd17xx_get_type(&gs_handle, &type_check);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: get type failed.\n");
        
        return 1;
    }
    isd17xx_interface_debug_print("isd17xx: check type %s.\n", (ISD17180 == type_check) ? "ok" : "error");
    
    /* set type isd17210 */
    res = isd17xx_set_type(&gs_handle, ISD17210);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set type failed.\n");
        
        return 1;
    }
    
    /* output */
    isd17xx_interface_debug_print("isd17xx: set type isd17210.\n");
    
    /* get type */
    res = isd17xx_get_type(&gs_handle, &type_check);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: get type failed.\n");
        
        return 1;
    }
    isd17xx_interface_debug_print("isd17xx: check type %s.\n", (ISD17210 == type_check) ? "ok" : "error");
    
    /* set type isd17240 */
    res = isd17xx_set_type(&gs_handle, ISD17240);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set type failed.\n");
        
        return 1;
    }
    
    /* output */
    isd17xx_interface_debug_print("isd17xx: set type isd17240.\n");
    
    /* get type */
    res = isd17xx_get_type(&gs_handle, &type_check);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: get type failed.\n");
        
        return 1;
    }
    isd17xx_interface_debug_print("isd17xx: check type %s.\n", (ISD17240 == type_check) ? "ok" : "error");
    
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
    
    /* isd17xx_set_volume_control/isd17xx_get_vol_control test */
    isd17xx_interface_debug_print("isd17xx: isd17xx_set_volume_control/isd17xx_get_vol_control test.\n");
    
    /* set button volume control */
    res = isd17xx_set_volume_control(&gs_handle, ISD17XX_VOL_CONTROL_BUTTON);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set volume control failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    isd17xx_interface_debug_print("isd17xx: set button volume control.\n");
    
    /* get volume control */
    res = isd17xx_get_volume_control(&gs_handle, &control);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: get volume control failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    isd17xx_interface_debug_print("isd17xx: check volume control %s.\n", (ISD17XX_VOL_CONTROL_BUTTON == control) ? "ok" : "error");
    
    /* set register volume control */
    res = isd17xx_set_volume_control(&gs_handle, ISD17XX_VOL_CONTROL_REG);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set volume control failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    isd17xx_interface_debug_print("isd17xx: set register volume control.\n");
    
    /* get volume control */
    res = isd17xx_get_volume_control(&gs_handle, &control);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: get volume control failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    isd17xx_interface_debug_print("isd17xx: check volume control %s.\n", (ISD17XX_VOL_CONTROL_REG == control) ? "ok" : "error");
    
    /* isd17xx_set_volume/isd17xx_get_volume test */
    isd17xx_interface_debug_print("isd17xx: isd17xx_set_volume/isd17xx_get_volume test.\n");
    
    /* set volume */
    vol = rand() % 8;
    res = isd17xx_set_volume(&gs_handle, vol);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set volume failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set volume */
    isd17xx_interface_debug_print("isd17xx: set volume 0x%0x.\n", vol);
    
    /* get volume */
    res = isd17xx_get_volume(&gs_handle, &vol_check);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: get volume failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    isd17xx_interface_debug_print("isd17xx: check volume %s.\n", (vol == vol_check) ? "ok" : "error");
    
    /* isd17xx_set_monitor_input/isd17xx_get_monitor_input test */
    isd17xx_interface_debug_print("isd17xx: isd17xx_set_monitor_input/isd17xx_get_monitor_input test.\n");
    
    /* set monitor input */
    res = isd17xx_set_monitor_input(&gs_handle, ISD17XX_BOOL_TRUE);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set monitor input failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable monitor input */
    isd17xx_interface_debug_print("isd17xx: enable monitor input.\n");
    
    /* get monitor input */
    res = isd17xx_get_monitor_input(&gs_handle, &enable);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: get monitor input failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    isd17xx_interface_debug_print("isd17xx: check monitor input %s.\n", (enable == ISD17XX_BOOL_TRUE) ? "ok" : "error");
    
    /* set monitor input */
    res = isd17xx_set_monitor_input(&gs_handle, ISD17XX_BOOL_FALSE);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set monitor input failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable monitor input */
    isd17xx_interface_debug_print("isd17xx: disable monitor input.\n");
    
    /* get monitor input */
    res = isd17xx_get_monitor_input(&gs_handle, &enable);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: get monitor input failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    isd17xx_interface_debug_print("isd17xx: check monitor input %s.\n", (enable == ISD17XX_BOOL_FALSE) ? "ok" : "error");
    
    /* isd17xx_set_mix_input/isd17xx_get_mix_input test */
    isd17xx_interface_debug_print("isd17xx: isd17xx_set_mix_input/isd17xx_get_mix_input test.\n");
    
    /* set mix input */
    res = isd17xx_set_mix_input(&gs_handle, ISD17XX_BOOL_TRUE);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set mix input failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable mix input */
    isd17xx_interface_debug_print("isd17xx: enable mix input.\n");
    
    /* get mix input */
    res = isd17xx_get_mix_input(&gs_handle, &enable);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: get mix input failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    isd17xx_interface_debug_print("isd17xx: check mix input %s.\n", (enable == ISD17XX_BOOL_TRUE) ? "ok" : "error");
    
    /* set mix input */
    res = isd17xx_set_mix_input(&gs_handle, ISD17XX_BOOL_FALSE);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set mix input failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable mix input */
    isd17xx_interface_debug_print("isd17xx: disable mix input.\n");
    
    /* get mix input */
    res = isd17xx_get_mix_input(&gs_handle, &enable);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: get mix input failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    isd17xx_interface_debug_print("isd17xx: check mix input %s.\n", (enable == ISD17XX_BOOL_FALSE) ? "ok" : "error");
    
    /* isd17xx_set_sound_effect_editing/isd17xx_get_sound_effect_editing test */
    isd17xx_interface_debug_print("isd17xx: isd17xx_set_sound_effect_editing/isd17xx_get_sound_effect_editing test.\n");
    
    /* set sound effect editing */
    res = isd17xx_set_sound_effect_editing(&gs_handle, ISD17XX_BOOL_TRUE);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set sound effect editing failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable sound effect editing */
    isd17xx_interface_debug_print("isd17xx: enable sound effect editing.\n");
    
    /* get sound effect editing */
    res = isd17xx_get_sound_effect_editing(&gs_handle, &enable);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: get sound effect editing failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    isd17xx_interface_debug_print("isd17xx: check sound effect editing %s.\n", (enable == ISD17XX_BOOL_TRUE) ? "ok" : "error");
    
    /* set sound effect editing */
    res = isd17xx_set_sound_effect_editing(&gs_handle, ISD17XX_BOOL_FALSE);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set sound effect editing failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable sound effect editing */
    isd17xx_interface_debug_print("isd17xx: disable sound effect editing.\n");
    
    /* get sound effect editing */
    res = isd17xx_get_sound_effect_editing(&gs_handle, &enable);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: get sound effect editing failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    isd17xx_interface_debug_print("isd17xx: check sound effect editing %s.\n", (enable == ISD17XX_BOOL_FALSE) ? "ok" : "error");
    
    /* isd17xx_set_spi_ft/isd17xx_get_spi_ft test */
    isd17xx_interface_debug_print("isd17xx: isd17xx_set_spi_ft/isd17xx_get_spi_ft test.\n");
    
    /* set spi ft */
    res = isd17xx_set_spi_ft(&gs_handle, ISD17XX_BOOL_TRUE);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set spi ft failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable spi ft */
    isd17xx_interface_debug_print("isd17xx: enable spi ft.\n");
    
    /* get spi ft */
    res = isd17xx_get_spi_ft(&gs_handle, &enable);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: get spi ft failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    isd17xx_interface_debug_print("isd17xx: check spi ft %s.\n", (enable == ISD17XX_BOOL_TRUE) ? "ok" : "error");
    
    /* set spi ft */
    res = isd17xx_set_spi_ft(&gs_handle, ISD17XX_BOOL_FALSE);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set spi ft failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable spi ft */
    isd17xx_interface_debug_print("isd17xx: disable spi ft.\n");
    
    /* get spi ft */
    res = isd17xx_get_spi_ft(&gs_handle, &enable);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: get spi ft failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    isd17xx_interface_debug_print("isd17xx: check spi ft %s.\n", (enable == ISD17XX_BOOL_FALSE) ? "ok" : "error");
    
    /* isd17xx_set_analog_output/isd17xx_get_analog_output test */
    isd17xx_interface_debug_print("isd17xx: isd17xx_set_analog_output/isd17xx_get_analog_output test.\n");
    
    /* set analog output */
    res = isd17xx_set_analog_output(&gs_handle, ISD17XX_ANALOG_OUTPUT_AUD);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set analog output failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set analog output aud */
    isd17xx_interface_debug_print("isd17xx: set analog output aud.\n");
    
    /* get analog output */
    res = isd17xx_get_analog_output(&gs_handle, &output);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: get analog output failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    isd17xx_interface_debug_print("isd17xx: check analog output %s.\n", (output == ISD17XX_ANALOG_OUTPUT_AUD) ? "ok" : "error");
    
    /* set analog output */
    res = isd17xx_set_analog_output(&gs_handle, ISD17XX_ANALOG_OUTPUT_AUX);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set analog output failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set analog output aux */
    isd17xx_interface_debug_print("isd17xx: set analog output aux.\n");
    
    /* get analog output */
    res = isd17xx_get_analog_output(&gs_handle, &output);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: get analog output failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    isd17xx_interface_debug_print("isd17xx: check analog output %s.\n", (output == ISD17XX_ANALOG_OUTPUT_AUX) ? "ok" : "error");
    
    /* isd17xx_set_pwm_speaker/isd17xx_get_pwm_speaker test */
    isd17xx_interface_debug_print("isd17xx: isd17xx_set_pwm_speaker/isd17xx_get_pwm_speaker test.\n");
    
    /* set pwm speaker */
    res = isd17xx_set_pwm_speaker(&gs_handle, ISD17XX_BOOL_TRUE);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set pwm speaker failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable pwm speaker */
    isd17xx_interface_debug_print("isd17xx: enable pwm speaker.\n");
    
    /* get pwm speaker */
    res = isd17xx_get_pwm_speaker(&gs_handle, &enable);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: get pwm speaker failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    isd17xx_interface_debug_print("isd17xx: check pwm speaker %s.\n", (enable == ISD17XX_BOOL_TRUE) ? "ok" : "error");
    
    /* set pwm speaker */
    res = isd17xx_set_pwm_speaker(&gs_handle, ISD17XX_BOOL_FALSE);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set pwm speaker failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable pwm speaker */
    isd17xx_interface_debug_print("isd17xx: disable pwm speaker.\n");
    
    /* get pwm speaker */
    res = isd17xx_get_pwm_speaker(&gs_handle, &enable);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: get pwm speaker failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    isd17xx_interface_debug_print("isd17xx: check pwm speaker %s.\n", (enable == ISD17XX_BOOL_FALSE) ? "ok" : "error");
    
    /* isd17xx_set_power_up_analog_output/isd17xx_get_power_up_analog_output test */
    isd17xx_interface_debug_print("isd17xx: isd17xx_set_power_up_analog_output/isd17xx_get_power_up_analog_output test.\n");
    
    /* set power up analog output */
    res = isd17xx_set_power_up_analog_output(&gs_handle, ISD17XX_BOOL_TRUE);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set power up analog output failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable power up analog output */
    isd17xx_interface_debug_print("isd17xx: enable power up analog output.\n");
    
    /* get power up analog output */
    res = isd17xx_get_power_up_analog_output(&gs_handle, &enable);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: get power up analog output failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    isd17xx_interface_debug_print("isd17xx: check power up analog output %s.\n", (enable == ISD17XX_BOOL_TRUE) ? "ok" : "error");
    
    /* set power up analog output */
    res = isd17xx_set_power_up_analog_output(&gs_handle, ISD17XX_BOOL_FALSE);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set power up analog output failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable power up analog output */
    isd17xx_interface_debug_print("isd17xx: disable power up analog output.\n");
    
    /* get power up analog output */
    res = isd17xx_get_power_up_analog_output(&gs_handle, &enable);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: get power up analog output failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    isd17xx_interface_debug_print("isd17xx: check power up analog output %s.\n", (enable == ISD17XX_BOOL_FALSE) ? "ok" : "error");
    
    /* isd17xx_set_v_alert/isd17xx_get_v_alert test */
    isd17xx_interface_debug_print("isd17xx: isd17xx_set_v_alert/isd17xx_get_v_alert test.\n");
    
    /* set v alert */
    res = isd17xx_set_v_alert(&gs_handle, ISD17XX_BOOL_TRUE);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set v alert failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable v alert */
    isd17xx_interface_debug_print("isd17xx: enable v alert.\n");
    
    /* get v alert */
    res = isd17xx_get_v_alert(&gs_handle, &enable);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: get v alert failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    isd17xx_interface_debug_print("isd17xx: check v alert %s.\n", (enable == ISD17XX_BOOL_TRUE) ? "ok" : "error");
    
    /* set v alert */
    res = isd17xx_set_v_alert(&gs_handle, ISD17XX_BOOL_FALSE);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set v alert failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable v alert */
    isd17xx_interface_debug_print("isd17xx: disable v alert.\n");
    
    /* get v alert */
    res = isd17xx_get_v_alert(&gs_handle, &enable);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: get v alert failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    isd17xx_interface_debug_print("isd17xx: check v alert %s.\n", (enable == ISD17XX_BOOL_FALSE) ? "ok" : "error");
    
    /* isd17xx_set_eom/isd17xx_get_eom test */
    isd17xx_interface_debug_print("isd17xx: isd17xx_set_eom/isd17xx_get_eom test.\n");
    
    /* set eom */
    res = isd17xx_set_eom(&gs_handle, ISD17XX_BOOL_TRUE);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set eom failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable eom */
    isd17xx_interface_debug_print("isd17xx: enable eom.\n");
    
    /* get eom */
    res = isd17xx_get_eom(&gs_handle, &enable);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: get eom failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    isd17xx_interface_debug_print("isd17xx: check eom %s.\n", (enable == ISD17XX_BOOL_TRUE) ? "ok" : "error");
    
    /* set eom */
    res = isd17xx_set_eom(&gs_handle, ISD17XX_BOOL_FALSE);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: set eom failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable eom */
    isd17xx_interface_debug_print("isd17xx: disable eom.\n");
    
    /* get eom */
    res = isd17xx_get_eom(&gs_handle, &enable);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: get eom failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    isd17xx_interface_debug_print("isd17xx: check eom %s.\n", (enable == ISD17XX_BOOL_FALSE) ? "ok" : "error");
    
    /* isd17xx_load_from_nv test */
    isd17xx_interface_debug_print("isd17xx: isd17xx_load_from_nv test.\n");
    
    /* load from nv */
    res = isd17xx_load_from_nv(&gs_handle);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: load from nv failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* load from nv */
    isd17xx_interface_debug_print("isd17xx: load from nv.\n");
    isd17xx_interface_debug_print("isd17xx: check load from nv %s.\n", (res == 0) ? "ok" : "error");

    /* isd17xx_get_device_id test */
    isd17xx_interface_debug_print("isd17xx: isd17xx_get_device_id test.\n");
    
    /* get_device_id */
    res = isd17xx_get_device_id(&gs_handle, &id);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: get device id failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    isd17xx_interface_debug_print("isd17xx: device id is 0x%02X.\n", id);
    
    /* isd17xx_read_play_point/isd17xx_read_record_point test */
    isd17xx_interface_debug_print("isd17xx: isd17xx_read_play_point/isd17xx_read_record_point test.\n");
    
    /* read play point test */
    res = isd17xx_read_play_point(&gs_handle, &point);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: read play point failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    isd17xx_interface_debug_print("isd17xx: play point is 0x%04X.\n", point);
    
    /* read record point test */
    res = isd17xx_read_record_point(&gs_handle, &point);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: read record point failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    isd17xx_interface_debug_print("isd17xx: record point is 0x%04X.\n", point);
    
    /* isd17xx_get_status test */
    isd17xx_interface_debug_print("isd17xx: isd17xx_get_status test.\n");
    
    /* get status */
    res = isd17xx_get_status(&gs_handle, &status1, &status2);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: get status failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }

    /* output */
    isd17xx_interface_debug_print("isd17xx: status1 is 0x%04X.\n", status1);
    
    /* output */
    isd17xx_interface_debug_print("isd17xx: status2 is 0x%02X.\n", status2);
    
    /* isd17xx_clear_interrupt test */
    isd17xx_interface_debug_print("isd17xx: isd17xx_clear_interrupt test.\n");
    
    /* clear interrupt test */
    res = isd17xx_clear_interrupt(&gs_handle);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: clear interrupt failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    isd17xx_interface_debug_print("isd17xx: check clear interrupt %s.\n", (res == 0) ? "ok" : "error");
    
    /* isd17xx_toggle_extern_clock test */
    isd17xx_interface_debug_print("isd17xx: isd17xx_toggle_extern_clock test.\n");
    
    /* toggle extern clock */
    res = isd17xx_toggle_extern_clock(&gs_handle);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: toggle extern clock failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* check toggle extern clock */
    isd17xx_interface_debug_print("isd17xx: check toggle extern clock %s.\n", (res == 0) ? "ok" : "error");
    
    /* delay 100ms */
    isd17xx_interface_delay_ms(100);
    
    /* toggle extern clock */
    res = isd17xx_toggle_extern_clock(&gs_handle);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: toggle extern clock failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* check toggle extern clock */
    isd17xx_interface_debug_print("isd17xx: check toggle extern clock %s.\n", (res == 0) ? "ok" : "error");
    
    /* delay 100ms */
    isd17xx_interface_delay_ms(100);
    
    /* isd17xx_reset test */
    isd17xx_interface_debug_print("isd17xx: isd17xx_reset test.\n");
    
    /* reset */
    res = isd17xx_reset(&gs_handle);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: reset failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    /* check reset */
    isd17xx_interface_debug_print("isd17xx: check reset %s.\n", (res == 0) ? "ok" : "error");
    
    /* power down */
    res = isd17xx_power_down(&gs_handle);
    if (res != 0)
    {
        isd17xx_interface_debug_print("isd17xx: power down failed.\n");
        (void)isd17xx_deinit(&gs_handle);
        
        return 1;
    }
    
    /* finish register test */
    isd17xx_interface_debug_print("isd17xx: finish register test.\n");
    (void)isd17xx_deinit(&gs_handle);
    
    return 0;
}
