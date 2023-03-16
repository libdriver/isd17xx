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
 * @file      main.c
 * @brief     main source file
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
#include "driver_isd17xx_play_record_test.h"
#include "driver_isd17xx_basic.h"
#include "shell.h"
#include "clock.h"
#include "delay.h"
#include "uart.h"
#include "getopt.h"
#include <math.h>
#include <stdlib.h>

/**
 * @brief global var definition
 */
uint8_t g_buf[256];             /**< uart buffer */
volatile uint16_t g_len;        /**< uart buffer length */

/**
 * @brief     isd17xx full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *             - 0 success
 *             - 1 run failed
 *             - 5 param is invalid
 * @note      none
 */
uint8_t isd17xx(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    char short_options[] = "hipe:t:";
    struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"end", required_argument, NULL, 1},
        {"start", required_argument, NULL, 2},
        {"type", required_argument, NULL, 3},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    uint16_t start = 0x0000;
    uint16_t end =0x00FF;
    isd17xx_type_t chip_type = ISD1760;
    
    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }

    /* init 0 */
    optind = 0;

    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);

        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");

                break;
            }

            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");

                break;
            }

            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");

                break;
            }

            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);

                break;
            }

            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);

                break;
            }

            /* end */
            case 1 :
            {
                char *p;
                uint16_t l;
                uint16_t i;
                uint64_t hex_data;

                /* set the data */
                l = strlen(optarg);

                /* check the header */
                if (l >= 2)
                {
                    if (strncmp(optarg, "0x", 2) == 0)
                    {
                        p = optarg + 2;
                        l -= 2;
                    }
                    else if (strncmp(optarg, "0X", 2) == 0)
                    {
                        p = optarg + 2;
                        l -= 2;
                    }
                    else
                    {
                        p = optarg;
                    }
                }
                else
                {
                    p = optarg;
                }
                
                /* init 0 */
                hex_data = 0;

                /* loop */
                for (i = 0; i < l; i++)
                {
                    if ((p[i] <= '9') && (p[i] >= '0'))
                    {
                        hex_data += (p[i] - '0') * (uint32_t)pow(16, l - i - 1);
                    }
                    else if ((p[i] <= 'F') && (p[i] >= 'A'))
                    {
                        hex_data += ((p[i] - 'A') + 10) * (uint32_t)pow(16, l - i - 1);
                    }
                    else if ((p[i] <= 'f') && (p[i] >= 'a'))
                    {
                        hex_data += ((p[i] - 'a') + 10) * (uint32_t)pow(16, l - i - 1);
                    }
                    else
                    {
                        return 5;
                    }
                }
                
                /* set the data */
                end = hex_data & 0xFFFF;
                
                break;
            }

            /* start */
            case 2 :
            {
                char *p;
                uint16_t l;
                uint16_t i;
                uint64_t hex_data;

                /* set the data */
                l = strlen(optarg);

                /* check the header */
                if (l >= 2)
                {
                    if (strncmp(optarg, "0x", 2) == 0)
                    {
                        p = optarg + 2;
                        l -= 2;
                    }
                    else if (strncmp(optarg, "0X", 2) == 0)
                    {
                        p = optarg + 2;
                        l -= 2;
                    }
                    else
                    {
                        p = optarg;
                    }
                }
                else
                {
                    p = optarg;
                }
                
                /* init 0 */
                hex_data = 0;

                /* loop */
                for (i = 0; i < l; i++)
                {
                    if ((p[i] <= '9') && (p[i] >= '0'))
                    {
                        hex_data += (p[i] - '0') * (uint32_t)pow(16, l - i - 1);
                    }
                    else if ((p[i] <= 'F') && (p[i] >= 'A'))
                    {
                        hex_data += ((p[i] - 'A') + 10) * (uint32_t)pow(16, l - i - 1);
                    }
                    else if ((p[i] <= 'f') && (p[i] >= 'a'))
                    {
                        hex_data += ((p[i] - 'a') + 10) * (uint32_t)pow(16, l - i - 1);
                    }
                    else
                    {
                        return 5;
                    }
                }
                
                /* set the data */
                start = hex_data & 0xFFFF;
                
                break;
            }
            
            /* chip type */
            case 3 :
            {
                /* set the chip type */
                if (strcmp("ISD1730", optarg) == 0)
                {
                    chip_type = ISD1730;
                }
                else if (strcmp("ISD1740", optarg) == 0)
                {
                    chip_type = ISD1740;
                }
                else if (strcmp("ISD1750", optarg) == 0)
                {
                    chip_type = ISD1750;
                }
                else if (strcmp("ISD1760", optarg) == 0)
                {
                    chip_type = ISD1760;
                }
                else if (strcmp("ISD1790", optarg) == 0)
                {
                    chip_type = ISD1790;
                }
                else if (strcmp("ISD17120", optarg) == 0)
                {
                    chip_type = ISD17120;
                }
                else if (strcmp("ISD17150", optarg) == 0)
                {
                    chip_type = ISD17150;
                }
                else if (strcmp("ISD17180", optarg) == 0)
                {
                    chip_type = ISD17180;
                }
                else if (strcmp("ISD17210", optarg) == 0)
                {
                    chip_type = ISD17210;
                }
                else if (strcmp("ISD17240", optarg) == 0)
                {
                    chip_type = ISD17240;
                }
                else
                {
                    return 5;
                }
            }
            
            /* the end */
            case -1 :
            {
                break;
            }

            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run the function */
    if (strcmp("t_reg", type) == 0)
    {
        /* run reg test */
        if (isd17xx_register_test(chip_type) != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("t_audio", type) == 0)
    {
        /* run audio test */
        if (isd17xx_play_record_test(chip_type) != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("e_record", type) == 0)
    {
        uint8_t res;
        
        /* basic init */
        res = isd17xx_basic_init(chip_type);
        if (res != 0)
        {
            isd17xx_interface_debug_print("isd17xx: basic init failed.\n");
            
            return 1;
        }
        
        /* erase */
        res = isd17xx_basic_erase(start, end);
        if (res != 0)
        {
            isd17xx_interface_debug_print("isd17xx: basic erase failed.\n");
            (void)isd17xx_basic_deinit();
            
            return 1;
        }
        
        /* wait erase finished */
        res = isd17xx_basic_poll(ISD17XX_STATUS2_ERASE);
        if (res != 0)
        {
            isd17xx_interface_debug_print("isd17xx: basic poll failed.\n");
            (void)isd17xx_basic_deinit();
            
            return 1;
        }
        
        /* output */
        isd17xx_interface_debug_print("isd17xx: recording...\n");
        
        /* record */
        res = isd17xx_basic_record(start, end);
        if (res != 0)
        {
            isd17xx_interface_debug_print("isd17xx: basic record failed.\n");
            (void)isd17xx_basic_deinit();
            
            return 1;
        }
        
        /* wait rec finished */
        res = isd17xx_basic_poll(ISD17XX_STATUS2_REC);
        if (res != 0)
        {
            isd17xx_interface_debug_print("isd17xx: basic poll failed.\n");
            (void)isd17xx_basic_deinit();
            
            return 1;
        }
        
        /* output */
        isd17xx_interface_debug_print("isd17xx: finish.\n");
        
        /* deinit */
        (void)isd17xx_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_play", type) == 0)
    {
        uint8_t res;
        
        /* basic init */
        res = isd17xx_basic_init(chip_type);
        if (res != 0)
        {
            isd17xx_interface_debug_print("isd17xx: basic init failed.\n");
            
            return 1;
        }
        
        /* output */
        isd17xx_interface_debug_print("isd17xx: playing...\n");
        
        /* play */
        res = isd17xx_basic_play(start, end);
        if (res != 0)
        {
            isd17xx_interface_debug_print("isd17xx: basic play failed.\n");
            (void)isd17xx_basic_deinit();
            
            return 1;
        }
        
        /* wait play finished */
        res = isd17xx_basic_poll(ISD17XX_STATUS2_PLAY);
        if (res != 0)
        {
            isd17xx_interface_debug_print("isd17xx: basic poll failed.\n");
            (void)isd17xx_basic_deinit();
            
            return 1;
        }
        
        /* output */
        isd17xx_interface_debug_print("isd17xx: finish.\n");
        
        /* deinit */
        (void)isd17xx_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_global-erase", type) == 0)
    {
        uint8_t res;
        
        /* basic init */
        res = isd17xx_basic_init(chip_type);
        if (res != 0)
        {
            isd17xx_interface_debug_print("isd17xx: basic init failed.\n");
            
            return 1;
        }
        
        /* output */
        isd17xx_interface_debug_print("isd17xx: global erase.\n");
        
        /* global erase */
        res = isd17xx_basic_global_erase();
        if (res != 0)
        {
            isd17xx_interface_debug_print("isd17xx: basic global erase failed.\n");
            (void)isd17xx_basic_deinit();
            
            return 1;
        }
        
        /* wait erase finished */
        res = isd17xx_basic_poll(ISD17XX_STATUS2_ERASE);
        if (res != 0)
        {
            isd17xx_interface_debug_print("isd17xx: basic poll failed.\n");
            (void)isd17xx_basic_deinit();
            
            return 1;
        }
        
        /* output */
        isd17xx_interface_debug_print("isd17xx: finish.\n");
        
        /* deinit */
        (void)isd17xx_basic_deinit();
        
        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        isd17xx_interface_debug_print("Usage:\n");
        isd17xx_interface_debug_print("  isd17xx (-i | --information)\n");
        isd17xx_interface_debug_print("  isd17xx (-h | --help)\n");
        isd17xx_interface_debug_print("  isd17xx (-p | --port)\n");
        isd17xx_interface_debug_print("  isd17xx (-t reg | --test=reg) [--type=<ISD1730 | ISD1740 | ISD1750 | ISD1760 | ISD1790 | ISD17120 | ISD17150 | ISD17180 | ISD17210 | ISD17240>]\n");
        isd17xx_interface_debug_print("  isd17xx (-t audio | --test=audio) [--type=<ISD1730 | ISD1740 | ISD1750 | ISD1760 | ISD1790 | ISD17120 | ISD17150 | ISD17180 | ISD17210 | ISD17240>]\n");
        isd17xx_interface_debug_print("  isd17xx (-e record | --example=record) [--start=<hex>] [--end=<hex>]\n");
        isd17xx_interface_debug_print("          [--type=<ISD1730 | ISD1740 | ISD1750 | ISD1760 | ISD1790 | ISD17120 | ISD17150 | ISD17180 | ISD17210 | ISD17240>]\n");
        isd17xx_interface_debug_print("  isd17xx (-e play | --example=play) [--start=<hex>] [--end=<hex>]\n");
        isd17xx_interface_debug_print("          [--type=<ISD1730 | ISD1740 | ISD1750 | ISD1760 | ISD1790 | ISD17120 | ISD17150 | ISD17180 | ISD17210 | ISD17240>]\n");
        isd17xx_interface_debug_print("  isd17xx (-e global-erase | --example=global-erase)\n");
        isd17xx_interface_debug_print("          [--type=<ISD1730 | ISD1740 | ISD1750 | ISD1760 | ISD1790 | ISD17120 | ISD17150 | ISD17180 | ISD17210 | ISD17240>]\n");
        isd17xx_interface_debug_print("\n");
        isd17xx_interface_debug_print("Options:\n");
        isd17xx_interface_debug_print("  -e <record | play | global-erase>, --example=<record | play | global-erase>\n");
        isd17xx_interface_debug_print("                       Run the driver example.\n");
        isd17xx_interface_debug_print("      --end=<hex>      Set the end address and it is hexadecimal.([default: 0xFF])\n");
        isd17xx_interface_debug_print("  -h, --help           Show the help.\n");
        isd17xx_interface_debug_print("  -i, --information    Show the chip information.\n");
        isd17xx_interface_debug_print("  -p, --port           Display the pin connections of the current board.\n");
        isd17xx_interface_debug_print("      --start=<hex>    Set the start address and it is hexadecimal.([default: 0x00])\n");
        isd17xx_interface_debug_print("  -t <reg | audio>, --test=<reg | audio>\n");
        isd17xx_interface_debug_print("                       Run the driver test.\n");
        isd17xx_interface_debug_print("      --type=<ISD1730 | ISD1740 | ISD1750 | ISD1760 | ISD1790 | ISD17120 | ISD17150 | ISD17180 | ISD17210 | ISD17240>\n");
        isd17xx_interface_debug_print("                       Set the chip type.([default: ISD1760])\n");

        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        isd17xx_info_t info;

        /* print isd17xx info */
        isd17xx_info(&info);
        isd17xx_interface_debug_print("isd17xx: chip is %s.\n", info.chip_name);
        isd17xx_interface_debug_print("isd17xx: manufacturer is %s.\n", info.manufacturer_name);
        isd17xx_interface_debug_print("isd17xx: interface is %s.\n", info.interface);
        isd17xx_interface_debug_print("isd17xx: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        isd17xx_interface_debug_print("isd17xx: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        isd17xx_interface_debug_print("isd17xx: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        isd17xx_interface_debug_print("isd17xx: max current is %0.2fmA.\n", info.max_current_ma);
        isd17xx_interface_debug_print("isd17xx: max temperature is %0.1fC.\n", info.temperature_max);
        isd17xx_interface_debug_print("isd17xx: min temperature is %0.1fC.\n", info.temperature_min);

        return 0;
    }
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        isd17xx_interface_debug_print("isd17xx: SPI interface SCK connected to GPIOA PIN5.\n");
        isd17xx_interface_debug_print("isd17xx: SPI interface MISO connected to GPIOA PIN6.\n");
        isd17xx_interface_debug_print("isd17xx: SPI interface MOSI connected to GPIOA PIN7.\n");
        isd17xx_interface_debug_print("isd17xx: SPI interface CS connected to GPIOA PIN4.\n");
        isd17xx_interface_debug_print("isd17xx: RESET GPIO connected to GPIOA PIN8.\n");

        return 0;
    }
    else
    {
        return 5;
    }
}

/**
 * @brief main function
 * @note  none
 */
int main(void)
{
    uint8_t res;
    
    /* stm32f407 clock init and hal init */
    clock_init();
    
    /* delay init */
    delay_init();
    
    /* uart init */
    uart_init(115200);
    
    /* shell init && register isd17xx function */
    shell_init();
    shell_register("isd17xx", isd17xx);
    uart_print("isd17xx: welcome to libdriver isd17xx.\n");
    
    while (1)
    {
        /* read uart */
        g_len = uart_read(g_buf, 256);
        if (g_len != 0)
        {
            /* run shell */
            res = shell_parse((char *)g_buf, g_len);
            if (res == 0)
            {
                /* run success */
            }
            else if (res == 1)
            {
                uart_print("isd17xx: run failed.\n");
            }
            else if (res == 2)
            {
                uart_print("isd17xx: unknown command.\n");
            }
            else if (res == 3)
            {
                uart_print("isd17xx: length is too long.\n");
            }
            else if (res == 4)
            {
                uart_print("isd17xx: pretreat failed.\n");
            }
            else if (res == 5)
            {
                uart_print("isd17xx: param is invalid.\n");
            }
            else
            {
                uart_print("isd17xx: unknown status code.\n");
            }
            uart_flush();
        }
        delay_ms(100);
    }
}
