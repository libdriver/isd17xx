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
 * @file      driver_isd17xx.c
 * @brief     driver isd17xx source file
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

#include "driver_isd17xx.h"

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "Nuvoton ISD17XX"        /**< chip name */
#define MANUFACTURER_NAME         "Nuvoton"                /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        2.4f                     /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        5.5f                     /**< chip max supply voltage */
#define MAX_CURRENT               20.0f                    /**< chip max current */
#define TEMPERATURE_MIN           -40.0f                   /**< chip min operating temperature */
#define TEMPERATURE_MAX           85.0f                    /**< chip max operating temperature */
#define DRIVER_VERSION            1000                     /**< driver version */

/**
 * @brief chip command definition
 */
#define ISD17XX_COMMAND_PU              0x01        /**< power up command */
#define ISD17XX_COMMAND_STOP            0x02        /**< stop command */
#define ISD17XX_COMMAND_RESET           0x03        /**< reset command */
#define ISD17XX_COMMAND_CLR_INT         0x04        /**< clear interrupt command */
#define ISD17XX_COMMAND_RD_STATUS       0x05        /**< read status command */
#define ISD17XX_COMMAND_RD_PLAY_PTR     0x06        /**< read play point command */
#define ISD17XX_COMMAND_PD              0x07        /**< power down command */
#define ISD17XX_COMMAND_RD_REC_PTR      0x08        /**< read record point command */
#define ISD17XX_COMMAND_DEVID           0x09        /**< device id command */
#define ISD17XX_COMMAND_PLAY            0x40        /**< play command */
#define ISD17XX_COMMAND_REC             0x41        /**< record command */
#define ISD17XX_COMMAND_ERASE           0x42        /**< erase command */
#define ISD17XX_COMMAND_G_ERASE         0x43        /**< erase all command */
#define ISD17XX_COMMAND_RD_APC          0x44        /**< read apc command */
#define ISD17XX_COMMAND_WR_APC1         0x45        /**< write apc1 command */
#define ISD17XX_COMMAND_WR_APC2         0x65        /**< write apc2 command */
#define ISD17XX_COMMAND_WR_NVCFG        0x46        /**< write nvcfg command */
#define ISD17XX_COMMAND_LD_NVCFG        0x47        /**< load nvcfg command */
#define ISD17XX_COMMAND_FWD             0x48        /**< forward command */
#define ISD17XX_COMMAND_CHK_MEM         0x49        /**< check memory command */
#define ISD17XX_COMMAND_EXTCLK          0x4A        /**< extern clock command */
#define ISD17XX_COMMAND_SET_PLAY        0x80        /**< set play command */
#define ISD17XX_COMMAND_SET_REC         0x81        /**< set record command */
#define ISD17XX_COMMAND_SET_ERASE       0x82        /**< set erase command */

/**
 * @brief      write and read bytes
 * @param[in]  *handle points to an isd17xx handle structure
 * @param[out] *tx points to a tx data buffer
 * @param[out] *rx points to a rx data buffer
 * @param[in]  len is the length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 spi write and read failed
 * @note       none
 */
static uint8_t a_isd17xx_spi_transmit(isd17xx_handle_t *handle, uint8_t *tx, uint8_t *rx, uint16_t len)
{
    if (handle->spi_transmit(tx, rx, len) != 0)       /* spi transmit */
    {
        return 1;                                     /* return error */
    }
    else
    {
        return 0;                                     /* success return 0 */
    }
}

/**
 * @brief      read bytes
 * @param[in]  *handle points to an isd17xx handle structure
 * @param[in]  reg is the register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 spi read failed
 * @note       none
 */
static uint8_t a_isd17xx_spi_read(isd17xx_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle->spi_read(reg, buf, len) != 0)        /* spi read */
    {
        return 1;                                    /* return error */
    }
    else
    {
        return 0;                                    /* success return 0 */
    }
}

/**
 * @brief     write bytes
 * @param[in] *handle points to an isd17xx handle structure
 * @param[in] reg is the register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data length
 * @return    status code
 *            - 0 success
 *            - 1 spi write failed
 * @note      none
 */
static uint8_t a_isd17xx_spi_write(isd17xx_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle->spi_write(reg, buf, len) != 0)        /* spi write */
    {
        return 1;                                     /* return error */
    }
    else
    {
        return 0;                                     /* success return 0 */
    }
}

/**
 * @brief     check the status
 * @param[in] *handle points to an isd17xx handle structure
 * @param[in] prev_ms is the prev delay in ms
 * @param[in] check_status is the check status bit
 * @param[in] status is the check status
 * @param[in] timeout_ms is the timeout in ms
 * @return    status code
 *            - 0 success
 *            - 1 check status failed
 * @note      none
 */
static uint8_t a_isd17xx_check_status(isd17xx_handle_t *handle, uint32_t prev_ms, uint8_t check_status,
                                      uint16_t status, uint32_t timeout_ms)
{
    uint8_t flag;
    uint8_t status2;
    uint16_t status1;
    uint32_t timeout;
    uint8_t buf[3];
    uint8_t status_buf[3];

    handle->delay_ms(10);                                                                     /* delay 10ms */
    buf[0] = ISD17XX_COMMAND_RD_STATUS;                                                       /* set read status command */
    buf[1] = 0x00;                                                                            /* set 0x00 */
    buf[2] = 0x00;                                                                            /* set 0x00 */
    if (a_isd17xx_spi_transmit(handle, buf, status_buf, 3) != 0)                              /* read the status */
    {
        handle->debug_print("isd17xx: get status failed.\n");                                 /* get status failed */

        return 1;                                                                             /* return error */
    }
    else
    {
        status1 = ((uint16_t)status_buf[0]) << 8 | status_buf[1];                             /* set the status1 */
    }

    if ((status1 & ISD17XX_STATUS1_CMD_ERR) != 0)                                             /* check the command error */
    {
        handle->debug_print("isd17xx: command error.\n");                                     /* command error */

        return 1;                                                                             /* return error */
    }

    if (prev_ms != 0)                                                                         /* if need prev delay */
    {
        handle->delay_ms(prev_ms);                                                            /* delay ms */
    }
    if (check_status != 0)                                                                    /* if check the status */
    {
        timeout = timeout_ms / 10;                                                            /* set the timeout */
        flag = 0;                                                                             /* init 0 */
        while (timeout != 0)                                                                  /* check the timeout */
        {
            buf[0] = ISD17XX_COMMAND_RD_STATUS;                                               /* set read status command */
            buf[1] = 0x00;                                                                    /* set 0x00 */
            buf[2] = 0x00;                                                                    /* set 0x00 */
            if (a_isd17xx_spi_transmit(handle, buf, status_buf, 3) != 0)                      /* read the status */
            {
                handle->debug_print("isd17xx: get status failed.\n");                         /* get status failed */

                return 1;                                                                     /* return error */
            }
            else
            {
                status1 = ((uint16_t)status_buf[0]) << 8 | status_buf[1];                     /* set the status1 */
                status2 = status_buf[2];                                                      /* set the status2 */
            }

            if ((status1 & (ISD17XX_STATUS1_EOM | ISD17XX_STATUS1_INT)) != 0)                 /* check the eom and int */
            {
                buf[0] = 0x00;                                                                /* set 0x00 */
                if (a_isd17xx_spi_write(handle, ISD17XX_COMMAND_CLR_INT, buf, 1) != 0)        /* set clear interrupt */
                {
                    handle->debug_print("isd17xx: set clear interrupt failed.\n");            /* set clear interrupt failed */

                    return 1;                                                                 /* return error */
                }
                flag = 1;                                                                     /* set flag */
            }
            if ((status2 & status) != 0)                                                      /* check the status */
            {
                if (flag != 0)
                {                                                                             /* check the flag */
                    handle->delay_ms(10);                                                     /* delay 10ms */
                }

                break;                                                                        /* break */
            }
            else
            {
                handle->delay_ms(10);                                                         /* delay 10ms */
                timeout--;                                                                    /* timeout-- */
                if (timeout == 0)                                                             /* check the timeout */
                {
                    handle->debug_print("isd17xx: timeout.\n");                               /* timeout */

                    return 1;                                                                 /* return error */
                }
            }
        }
    }

    return 0;                                                                                 /* success return 0 */
}

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
uint8_t isd17xx_power_up(isd17xx_handle_t *handle)
{
    uint8_t buf[1];

    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->inited != 1)                                                 /* check handle initialization */
    {
        return 3;                                                            /* return error */
    }

    buf[0] = 0x00;                                                           /* set 0x00 */
    if (a_isd17xx_spi_write(handle, ISD17XX_COMMAND_PU, buf, 1) != 0)        /* set power up */
    {
        handle->debug_print("isd17xx: set power up failed.\n");              /* set power up failed */

        return 1;                                                            /* return error */
    }
    else
    {
        return a_isd17xx_check_status(handle, 100, 1,
                                      ISD17XX_STATUS2_RDY, 1000);            /* check the status */
    }
}

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
uint8_t isd17xx_stop(isd17xx_handle_t *handle)
{
    uint8_t buf[1];

    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }

    buf[0] = 0x00;                                                             /* set 0x00 */
    if (a_isd17xx_spi_write(handle, ISD17XX_COMMAND_STOP, buf, 1) != 0)        /* set stop */
    {
        handle->debug_print("isd17xx: set stop failed.\n");                    /* set stop failed */

        return 1;                                                              /* return error */
    }
    else
    {
        return a_isd17xx_check_status(handle, 0, 1,
                                      ISD17XX_STATUS2_RDY, 1000);              /* check the status */
    }
}

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
uint8_t isd17xx_reset(isd17xx_handle_t *handle)
{
    uint8_t buf[1];

    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }

    buf[0] = 0x00;                                                              /* set 0x00 */
    if (a_isd17xx_spi_write(handle, ISD17XX_COMMAND_RESET, buf, 1) != 0)        /* set reset */
    {
        handle->debug_print("isd17xx: set reset failed.\n");                    /* set reset failed */

        return 1;                                                               /* return error */
    }
    else
    {
        return a_isd17xx_check_status(handle, 100, 0, 0, 0);                    /* check the status */
    }
}

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
uint8_t isd17xx_clear_interrupt(isd17xx_handle_t *handle)
{
    uint8_t buf[1];

    if (handle == NULL)                                                           /* check handle */
    {
        return 2;                                                                 /* return error */
    }
    if (handle->inited != 1)                                                      /* check handle initialization */
    {
        return 3;                                                                 /* return error */
    }

    buf[0] = 0x00;                                                                /* set 0x00 */
    if (a_isd17xx_spi_write(handle, ISD17XX_COMMAND_CLR_INT, buf, 1) != 0)        /* set clear interrupt */
    {
        handle->debug_print("isd17xx: set clear interrupt failed.\n");            /* set clear interrupt failed */

        return 1;                                                                 /* return error */
    }
    else
    {
        return a_isd17xx_check_status(handle, 0, 0, 0, 0);                        /* check the status */
    }
}

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
uint8_t isd17xx_get_status(isd17xx_handle_t *handle, uint16_t *status1, uint8_t *status2)
{
    uint8_t buf[3];
    uint8_t status[3];

    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }

    buf[0] = ISD17XX_COMMAND_RD_STATUS;                                                   /* set read status command */
    buf[1] = 0x00;                                                                        /* set 0x00 */
    buf[2] = 0x00;                                                                        /* set 0x00 */
    if (a_isd17xx_spi_transmit(handle, buf, status, 3) != 0)                              /* read the status */
    {
        handle->debug_print("isd17xx: get status failed.\n");                             /* get status failed */

        return 1;                                                                         /* return error */
    }
    else
    {
        *status1 = ((uint16_t)status[0]) << 8 | status[1];                                /* set the status1 */
        *status2 = status[2];                                                             /* set the status2 */

        if ((*status1 & (ISD17XX_STATUS1_EOM | ISD17XX_STATUS1_INT)) != 0)                /* check the eom and int */
        {
            buf[0] = 0x00;                                                                /* set 0x00 */
            if (a_isd17xx_spi_write(handle, ISD17XX_COMMAND_CLR_INT, buf, 1) != 0)        /* set clear interrupt */
            {
                handle->debug_print("isd17xx: set clear interrupt failed.\n");            /* set clear interrupt failed */

                return 1;                                                                 /* return error */
            }
        }

        return 0;                                                                         /* success return 0 */
    }
}

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
uint8_t isd17xx_power_down(isd17xx_handle_t *handle)
{
    uint8_t buf[1];

    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->inited != 1)                                                 /* check handle initialization */
    {
        return 3;                                                            /* return error */
    }

    buf[0] = 0x00;                                                           /* set 0x00 */
    if (a_isd17xx_spi_write(handle, ISD17XX_COMMAND_PD, buf, 1) != 0)        /* set power down */
    {
        handle->debug_print("isd17xx: set power down failed.\n");            /* set power down failed */

        return 1;                                                            /* return error */
    }
    else
    {
        return 0;                                                            /* success return 0 */
    }
}

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
uint8_t isd17xx_get_device_id(isd17xx_handle_t *handle, uint8_t *id)
{
    uint8_t buf[3];
    uint8_t status[3];

    if (handle == NULL)                                                 /* check handle */
    {
        return 2;                                                       /* return error */
    }
    if (handle->inited != 1)                                            /* check handle initialization */
    {
        return 3;                                                       /* return error */
    }

    buf[0] = ISD17XX_COMMAND_DEVID;                                     /* set device id command */
    buf[1] = 0x00;                                                      /* set 0x00 */
    buf[2] = 0x00;                                                      /* set 0x00 */
    if (a_isd17xx_spi_transmit(handle, buf, status, 3) != 0)            /* read the device id */
    {
        handle->debug_print("isd17xx: get device id failed.\n");        /* get device id failed */

        return 1;                                                       /* return error */
    }
    else
    {
        *id = (status[2] >> 3) & 0xFF;                                  /* set the device id */

        return a_isd17xx_check_status(handle, 0, 0, 0, 0);              /* check the status */
    }
}

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
uint8_t isd17xx_play(isd17xx_handle_t *handle)
{
    uint8_t buf[1];

    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }

    buf[0] = 0x00;                                                             /* set 0x00 */
    if (a_isd17xx_spi_write(handle, ISD17XX_COMMAND_PLAY, buf, 1) != 0)        /* set play */
    {
        handle->debug_print("isd17xx: set play failed.\n");                    /* set play failed */

        return 1;                                                              /* return error */
    }
    else
    {
        handle->delay_ms(500);                                                 /* delay 500ms */

        return 0;                                                              /* success return 0 */
    }
}

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
uint8_t isd17xx_record(isd17xx_handle_t *handle)
{
    uint8_t buf[1];

    if (handle == NULL)                                                       /* check handle */
    {
        return 2;                                                             /* return error */
    }
    if (handle->inited != 1)                                                  /* check handle initialization */
    {
        return 3;                                                             /* return error */
    }

    buf[0] = 0x00;                                                            /* set 0x00 */
    if (a_isd17xx_spi_write(handle, ISD17XX_COMMAND_REC, buf, 1) != 0)        /* set record */
    {
        handle->debug_print("isd17xx: set record failed.\n");                 /* set record failed */

        return 1;                                                             /* return error */
    }
    else
    {
        handle->delay_ms(500);                                                /* delay 500ms */

        return 0;                                                             /* success return 0 */
    }
}

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
uint8_t isd17xx_erase(isd17xx_handle_t *handle)
{
    uint8_t buf[1];

    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }

    buf[0] = 0x00;                                                              /* set 0x00 */
    if (a_isd17xx_spi_write(handle, ISD17XX_COMMAND_ERASE, buf, 1) != 0)        /* set erase */
    {
        handle->debug_print("isd17xx: set erase failed.\n");                    /* set erase failed */

        return 1;                                                               /* return error */
    }
    else
    {
        handle->delay_ms(500);                                                  /* delay 500ms */

        return 0;                                                               /* success return 0 */
    }
}

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
uint8_t isd17xx_global_erase(isd17xx_handle_t *handle)
{
    uint8_t buf[1];

    if (handle == NULL)                                                           /* check handle */
    {
        return 2;                                                                 /* return error */
    }
    if (handle->inited != 1)                                                      /* check handle initialization */
    {
        return 3;                                                                 /* return error */
    }

    buf[0] = 0x00;                                                                /* set 0x00 */
    if (a_isd17xx_spi_write(handle, ISD17XX_COMMAND_G_ERASE, buf, 1) != 0)        /* set global erase */
    {
        handle->debug_print("isd17xx: set global erase failed.\n");               /* set global erase failed */

        return 1;                                                                 /* return error */
    }
    else
    {
        handle->delay_ms(500);                                                    /* delay 500ms */

        return 0;                                                                 /* success return 0 */
    }
}

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
uint8_t isd17xx_next(isd17xx_handle_t *handle)
{
    uint8_t buf[1];

    if (handle == NULL)                                                       /* check handle */
    {
        return 2;                                                             /* return error */
    }
    if (handle->inited != 1)                                                  /* check handle initialization */
    {
        return 3;                                                             /* return error */
    }

    buf[0] = 0x00;                                                            /* set 0x00 */
    if (a_isd17xx_spi_write(handle, ISD17XX_COMMAND_FWD, buf, 1) != 0)        /* set next */
    {
        handle->debug_print("isd17xx: set next failed.\n");                   /* set next failed */

        return 1;                                                             /* return error */
    }
    else
    {
        handle->delay_ms(500);                                                /* delay 500ms */

        return 0;                                                             /* success return 0 */
    }
}

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
uint8_t isd17xx_check_memory(isd17xx_handle_t *handle)
{
    uint8_t buf[1];

    if (handle == NULL)                                                           /* check handle */
    {
        return 2;                                                                 /* return error */
    }
    if (handle->inited != 1)                                                      /* check handle initialization */
    {
        return 3;                                                                 /* return error */
    }

    buf[0] = 0x00;                                                                /* set 0x00 */
    if (a_isd17xx_spi_write(handle, ISD17XX_COMMAND_CHK_MEM, buf, 1) != 0)        /* set check memory */
    {
        handle->debug_print("isd17xx: set check memory failed.\n");               /* set check memory failed */

        return 1;                                                                 /* return error */
    }
    else
    {
        return a_isd17xx_check_status(handle, 0, 0, 0, 0);                        /* check the status */
    }
}

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
uint8_t isd17xx_read_play_point(isd17xx_handle_t *handle, uint16_t *point)
{
    uint8_t buf[4];
    uint8_t data[4];

    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }

    buf[0] = ISD17XX_COMMAND_RD_PLAY_PTR;                                 /* read play point command */
    buf[1] = 0x00;                                                        /* set 0x00 */
    buf[2] = 0x00;                                                        /* set 0x00 */
    buf[3] = 0x00;                                                        /* set 0x00 */
    if (a_isd17xx_spi_transmit(handle, buf, data, 4) != 0)                /* read the data */
    {
        handle->debug_print("isd17xx: read play point failed.\n");        /* read play point failed */

        return 1;                                                         /* return error */
    }
    else
    {
        *point = (uint16_t)(data[3] & 0x7) << 8 | data[2];                /* set the point */

        return a_isd17xx_check_status(handle, 0, 0, 0, 0);                /* check the status */
    }
}

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
uint8_t isd17xx_read_record_point(isd17xx_handle_t *handle, uint16_t *point)
{
    uint8_t buf[4];
    uint8_t data[4];

    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }

    buf[0] = ISD17XX_COMMAND_RD_REC_PTR;                                  /* read record point command */
    buf[1] = 0x00;                                                        /* set 0x00 */
    buf[2] = 0x00;                                                        /* set 0x00 */
    buf[3] = 0x00;                                                        /* set 0x00 */
    if (a_isd17xx_spi_transmit(handle, buf, data, 4) != 0)                /* read the data */
    {
        handle->debug_print("isd17xx: read record point failed.\n");      /* read record point failed */

        return 1;                                                         /* return error */
    }
    else
    {
        *point = (uint16_t)(data[3] & 0x7) << 8 | data[2];                /* set the point */

        return a_isd17xx_check_status(handle, 0, 0, 0, 0);                /* check the status */
    }
}

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
uint8_t isd17xx_set_volume(isd17xx_handle_t *handle, uint8_t vol)
{
    uint8_t buf[4];
    uint8_t data[4];
    uint8_t config;

    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }
    if (vol > 0x7)                                                        /* check the vol */
    {
        handle->debug_print("isd17xx: vol is over 7.\n");                 /* vol is over 7 */

        return 4;                                                         /* return error */
    }

    buf[0] = ISD17XX_COMMAND_RD_APC;                                      /* read apc command */
    buf[1] = 0x00;                                                        /* set 0x00 */
    buf[2] = 0x00;                                                        /* set 0x00 */
    buf[3] = 0x00;                                                        /* set 0x00 */
    if (a_isd17xx_spi_transmit(handle, buf, data, 4) != 0)                /* read the data */
    {
        handle->debug_print("isd17xx: read config failed.\n");            /* read config failed */

        return 1;                                                         /* return error */
    }
    if (a_isd17xx_check_status(handle, 0, 0, 0, 0) != 0)                  /* check the status */
    {
        return 1;                                                         /* return error */
    }

    config = (uint16_t)(data[3] & 0xF) << 8 | data[2];                    /* get the config */
    config &= ~0x7;                                                       /* clear config */
    config |= vol;                                                        /* set the vol */
    if (handle->vol_control == ISD17XX_VOL_CONTROL_BUTTON)                /* button control */
    {
        buf[0] = ISD17XX_COMMAND_WR_APC1;                                 /* write apc command */
    }
    else                                                                  /* register control */
    {
        buf[0] = ISD17XX_COMMAND_WR_APC2;                                 /* write apc command */
    }
    buf[1] = config & 0xFF;                                               /* set low */
    buf[2] = ((uint16_t)config >> 8) & 0xFF;                              /* set high */
    if (a_isd17xx_spi_transmit(handle, buf, data, 3) != 0)                /* write the data */
    {
        handle->debug_print("isd17xx: write config failed.\n");           /* write config failed */

        return 1;                                                         /* return error */
    }

    return a_isd17xx_check_status(handle, 0, 0, 0, 0);                    /* check the status */
}

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
uint8_t isd17xx_get_volume(isd17xx_handle_t *handle, uint8_t *vol)
{
    uint8_t buf[4];
    uint8_t data[4];
    uint16_t config;

    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }

    buf[0] = ISD17XX_COMMAND_RD_APC;                                      /* read apc command */
    buf[1] = 0x00;                                                        /* set 0x00 */
    buf[2] = 0x00;                                                        /* set 0x00 */
    buf[3] = 0x00;                                                        /* set 0x00 */
    if (a_isd17xx_spi_transmit(handle, buf, data, 4) != 0)                /* read the data */
    {
        handle->debug_print("isd17xx: read config failed.\n");            /* read config failed */

        return 1;                                                         /* return error */
    }
    config = (uint16_t)(data[3] & 0xF) << 8 | data[2];                    /* get the config */
    *vol = config & 0x7;                                                  /* get the volume */

    return a_isd17xx_check_status(handle, 0, 0, 0, 0);                    /* check the status */
}

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
uint8_t isd17xx_set_monitor_input(isd17xx_handle_t *handle, isd17xx_bool_t enable)
{
    uint8_t buf[4];
    uint8_t data[4];
    uint16_t config;

    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }

    buf[0] = ISD17XX_COMMAND_RD_APC;                                      /* read apc command */
    buf[1] = 0x00;                                                        /* set 0x00 */
    buf[2] = 0x00;                                                        /* set 0x00 */
    buf[3] = 0x00;                                                        /* set 0x00 */
    if (a_isd17xx_spi_transmit(handle, buf, data, 4) != 0)                /* read the data */
    {
        handle->debug_print("isd17xx: read config failed.\n");            /* read config failed */

        return 1;                                                         /* return error */
    }
    if (a_isd17xx_check_status(handle, 0, 0, 0, 0) != 0)                  /* check the status */
    {
        return 1;                                                         /* return error */
    }

    config = (uint16_t)(data[3] & 0xF) << 8 | data[2];                    /* get the config */
    config &= ~(1 << 3);                                                  /* clear config */
    config |= enable << 3;                                                /* set the config */
    if (handle->vol_control == ISD17XX_VOL_CONTROL_BUTTON)                /* button control */
    {
        buf[0] = ISD17XX_COMMAND_WR_APC1;                                 /* write apc command */
    }
    else                                                                  /* register control */
    {
        buf[0] = ISD17XX_COMMAND_WR_APC2;                                 /* write apc command */
    }
    buf[1] = config & 0xFF;                                               /* set low */
    buf[2] = (config >> 8) & 0xFF;                                        /* set high */
    if (a_isd17xx_spi_transmit(handle, buf, data, 3) != 0)                /* write the data */
    {
        handle->debug_print("isd17xx: write config failed.\n");           /* write config failed */

        return 1;                                                         /* return error */
    }

    return a_isd17xx_check_status(handle, 0, 0, 0, 0);                    /* check the status */
}

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
uint8_t isd17xx_get_monitor_input(isd17xx_handle_t *handle, isd17xx_bool_t *enable)
{
    uint8_t buf[4];
    uint8_t data[4];
    uint16_t config;

    if (handle == NULL)                                               /* check handle */
    {
        return 2;                                                     /* return error */
    }
    if (handle->inited != 1)                                          /* check handle initialization */
    {
        return 3;                                                     /* return error */
    }

    buf[0] = ISD17XX_COMMAND_RD_APC;                                  /* read apc command */
    buf[1] = 0x00;                                                    /* set 0x00 */
    buf[2] = 0x00;                                                    /* set 0x00 */
    buf[3] = 0x00;                                                    /* set 0x00 */
    if (a_isd17xx_spi_transmit(handle, buf, data, 4) != 0)            /* read the data */
    {
        handle->debug_print("isd17xx: read config failed.\n");        /* read config failed */

        return 1;                                                     /* return error */
    }
    config = (uint16_t)(data[3] & 0xF) << 8 | data[2];                /* get the config */
    *enable = (isd17xx_bool_t)((config >> 3) & 0x01);                 /* get the bool */

    return a_isd17xx_check_status(handle, 0, 0, 0, 0);                /* check the status */
}

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
uint8_t isd17xx_set_mix_input(isd17xx_handle_t *handle, isd17xx_bool_t enable)
{
    uint8_t buf[4];
    uint8_t data[4];
    uint16_t config;

    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }

    buf[0] = ISD17XX_COMMAND_RD_APC;                                      /* read apc command */
    buf[1] = 0x00;                                                        /* set 0x00 */
    buf[2] = 0x00;                                                        /* set 0x00 */
    buf[3] = 0x00;                                                        /* set 0x00 */
    if (a_isd17xx_spi_transmit(handle, buf, data, 4) != 0)                /* read the data */
    {
        handle->debug_print("isd17xx: read config failed.\n");            /* read config failed */

        return 1;                                                         /* return error */
    }
    if (a_isd17xx_check_status(handle, 0, 0, 0, 0) != 0)                  /* check the status */
    {
        return 1;                                                         /* return error */
    }

    config = (uint16_t)(data[3] & 0xF) << 8 | data[2];                    /* get the config */
    config &= ~(1 << 4);                                                  /* clear config */
    config |= enable << 4;                                                /* set the config */
    if (handle->vol_control == ISD17XX_VOL_CONTROL_BUTTON)                /* button control */
    {
        buf[0] = ISD17XX_COMMAND_WR_APC1;                                 /* write apc command */
    }
    else                                                                  /* register control */
    {
        buf[0] = ISD17XX_COMMAND_WR_APC2;                                 /* write apc command */
    }
    buf[1] = config & 0xFF;                                               /* set low */
    buf[2] = (config >> 8) & 0xFF;                                        /* set high */
    if (a_isd17xx_spi_transmit(handle, buf, data, 3) != 0)                /* write the data */
    {
        handle->debug_print("isd17xx: write config failed.\n");           /* write config failed */

        return 1;                                                         /* return error */
    }

    return a_isd17xx_check_status(handle, 0, 0, 0, 0);                    /* check the status */
}

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
uint8_t isd17xx_get_mix_input(isd17xx_handle_t *handle, isd17xx_bool_t *enable)
{
    uint8_t buf[4];
    uint8_t data[4];
    uint16_t config;

    if (handle == NULL)                                               /* check handle */
    {
        return 2;                                                     /* return error */
    }
    if (handle->inited != 1)                                          /* check handle initialization */
    {
        return 3;                                                     /* return error */
    }

    buf[0] = ISD17XX_COMMAND_RD_APC;                                  /* read apc command */
    buf[1] = 0x00;                                                    /* set 0x00 */
    buf[2] = 0x00;                                                    /* set 0x00 */
    buf[3] = 0x00;                                                    /* set 0x00 */
    if (a_isd17xx_spi_transmit(handle, buf, data, 4) != 0)            /* read the data */
    {
        handle->debug_print("isd17xx: read config failed.\n");        /* read config failed */

        return 1;                                                     /* return error */
    }
    config = (uint16_t)(data[3] & 0xF) << 8 | data[2];                /* get the config */
    *enable = (isd17xx_bool_t)((config >> 4) & 0x01);                 /* get the bool */

    return a_isd17xx_check_status(handle, 0, 0, 0, 0);                /* check the status */
}

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
uint8_t isd17xx_set_sound_effect_editing(isd17xx_handle_t *handle, isd17xx_bool_t enable)
{
    uint8_t buf[4];
    uint8_t data[4];
    uint16_t config;

    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }

    buf[0] = ISD17XX_COMMAND_RD_APC;                                      /* read apc command */
    buf[1] = 0x00;                                                        /* set 0x00 */
    buf[2] = 0x00;                                                        /* set 0x00 */
    buf[3] = 0x00;                                                        /* set 0x00 */
    if (a_isd17xx_spi_transmit(handle, buf, data, 4) != 0)                /* read the data */
    {
        handle->debug_print("isd17xx: read config failed.\n");            /* read config failed */

        return 1;                                                         /* return error */
    }
    if (a_isd17xx_check_status(handle, 0, 0, 0, 0) != 0)                  /* check the status */
    {
        return 1;                                                         /* return error */
    }

    config = (uint16_t)(data[3] & 0xF) << 8 | data[2];                    /* get the config */
    config &= ~(1 << 5);                                                  /* clear config */
    config |= (!enable) << 5;                                             /* set the config */
    if (handle->vol_control == ISD17XX_VOL_CONTROL_BUTTON)                /* button control */
    {
        buf[0] = ISD17XX_COMMAND_WR_APC1;                                 /* write apc command */
    }
    else                                                                  /* register control */
    {
        buf[0] = ISD17XX_COMMAND_WR_APC2;                                 /* write apc command */
    }
    buf[1] = config & 0xFF;                                               /* set low */
    buf[2] = (config >> 8) & 0xFF;                                        /* set high */
    if (a_isd17xx_spi_transmit(handle, buf, data, 3) != 0)                /* write the data */
    {
        handle->debug_print("isd17xx: write config failed.\n");           /* write config failed */

        return 1;                                                         /* return error */
    }

    return a_isd17xx_check_status(handle, 0, 0, 0, 0);                    /* check the status */
}

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
uint8_t isd17xx_get_sound_effect_editing(isd17xx_handle_t *handle, isd17xx_bool_t *enable)
{
    uint8_t buf[4];
    uint8_t data[4];
    uint16_t config;

    if (handle == NULL)                                               /* check handle */
    {
        return 2;                                                     /* return error */
    }
    if (handle->inited != 1)                                          /* check handle initialization */
    {
        return 3;                                                     /* return error */
    }

    buf[0] = ISD17XX_COMMAND_RD_APC;                                  /* read apc command */
    buf[1] = 0x00;                                                    /* set 0x00 */
    buf[2] = 0x00;                                                    /* set 0x00 */
    buf[3] = 0x00;                                                    /* set 0x00 */
    if (a_isd17xx_spi_transmit(handle, buf, data, 4) != 0)            /* read the data */
    {
        handle->debug_print("isd17xx: read config failed.\n");        /* read config failed */

        return 1;                                                     /* return error */
    }
    config = (uint16_t)(data[3] & 0xF) << 8 | data[2];                /* get the config */
    *enable = (isd17xx_bool_t)(!((config >> 5) & 0x01));              /* get the bool */

    return a_isd17xx_check_status(handle, 0, 0, 0, 0);                /* check the status */
}

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
uint8_t isd17xx_set_spi_ft(isd17xx_handle_t *handle, isd17xx_bool_t enable)
{
    uint8_t buf[4];
    uint8_t data[4];
    uint16_t config;

    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }

    buf[0] = ISD17XX_COMMAND_RD_APC;                                      /* read apc command */
    buf[1] = 0x00;                                                        /* set 0x00 */
    buf[2] = 0x00;                                                        /* set 0x00 */
    buf[3] = 0x00;                                                        /* set 0x00 */
    if (a_isd17xx_spi_transmit(handle, buf, data, 4) != 0)                /* read the data */
    {
        handle->debug_print("isd17xx: read config failed.\n");            /* read config failed */

        return 1;                                                         /* return error */
    }
    if (a_isd17xx_check_status(handle, 0, 0, 0, 0) != 0)                  /* check the status */
    {
        return 1;                                                         /* return error */
    }

    config = (uint16_t)(data[3] & 0xF) << 8 | data[2];                    /* get the config */
    config &= ~(1 << 6);                                                  /* clear config */
    config |= (!enable) << 6;                                             /* set the config */
    if (handle->vol_control == ISD17XX_VOL_CONTROL_BUTTON)                /* button control */
    {
        buf[0] = ISD17XX_COMMAND_WR_APC1;                                 /* write apc command */
    }
    else                                                                  /* register control */
    {
        buf[0] = ISD17XX_COMMAND_WR_APC2;                                 /* write apc command */
    }
    buf[1] = config & 0xFF;                                               /* set low */
    buf[2] = (config >> 8) & 0xFF;                                        /* set high */
    if (a_isd17xx_spi_transmit(handle, buf, data, 3) != 0)                /* write the data */
    {
        handle->debug_print("isd17xx: write config failed.\n");           /* write config failed */

        return 1;                                                         /* return error */
    }

    return a_isd17xx_check_status(handle, 0, 0, 0, 0);                    /* check the status */
}

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
uint8_t isd17xx_get_spi_ft(isd17xx_handle_t *handle, isd17xx_bool_t *enable)
{
    uint8_t buf[4];
    uint8_t data[4];
    uint16_t config;

    if (handle == NULL)                                               /* check handle */
    {
        return 2;                                                     /* return error */
    }
    if (handle->inited != 1)                                          /* check handle initialization */
    {
        return 3;                                                     /* return error */
    }

    buf[0] = ISD17XX_COMMAND_RD_APC;                                  /* read apc command */
    buf[1] = 0x00;                                                    /* set 0x00 */
    buf[2] = 0x00;                                                    /* set 0x00 */
    buf[3] = 0x00;                                                    /* set 0x00 */
    if (a_isd17xx_spi_transmit(handle, buf, data, 4) != 0)            /* read the data */
    {
        handle->debug_print("isd17xx: read config failed.\n");        /* read config failed */

        return 1;                                                     /* return error */
    }
    config = (uint16_t)(data[3] & 0xF) << 8 | data[2];                /* get the config */
    *enable = (isd17xx_bool_t)(!((config >> 6) & 0x01));              /* get the bool */

    return a_isd17xx_check_status(handle, 0, 0, 0, 0);                /* check the status */
}

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
uint8_t isd17xx_set_analog_output(isd17xx_handle_t *handle, isd17xx_analog_output_t output)
{
    uint8_t buf[4];
    uint8_t data[4];
    uint16_t config;

    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }

    buf[0] = ISD17XX_COMMAND_RD_APC;                                      /* read apc command */
    buf[1] = 0x00;                                                        /* set 0x00 */
    buf[2] = 0x00;                                                        /* set 0x00 */
    buf[3] = 0x00;                                                        /* set 0x00 */
    if (a_isd17xx_spi_transmit(handle, buf, data, 4) != 0)                /* read the data */
    {
        handle->debug_print("isd17xx: read config failed.\n");            /* read config failed */

        return 1;                                                         /* return error */
    }
    if (a_isd17xx_check_status(handle, 0, 0, 0, 0) != 0)                  /* check the status */
    {
        return 1;                                                         /* return error */
    }

    config = (uint16_t)(data[3] & 0xF) << 8 | data[2];                    /* get the config */
    config &= ~(1 << 7);                                                  /* clear config */
    config |= output << 7;                                                /* set the output */
    if (handle->vol_control == ISD17XX_VOL_CONTROL_BUTTON)                /* button control */
    {
        buf[0] = ISD17XX_COMMAND_WR_APC1;                                 /* write apc command */
    }
    else                                                                  /* register control */
    {
        buf[0] = ISD17XX_COMMAND_WR_APC2;                                 /* write apc command */
    }
    buf[1] = config & 0xFF;                                               /* set low */
    buf[2] = (config >> 8) & 0xFF;                                        /* set high */
    if (a_isd17xx_spi_transmit(handle, buf, data, 3) != 0)                /* write the data */
    {
        handle->debug_print("isd17xx: write config failed.\n");           /* write config failed */

        return 1;                                                         /* return error */
    }

    return a_isd17xx_check_status(handle, 0, 0, 0, 0);                    /* check the status */
}

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
uint8_t isd17xx_get_analog_output(isd17xx_handle_t *handle, isd17xx_analog_output_t *output)
{
    uint8_t buf[4];
    uint8_t data[4];
    uint16_t config;

    if (handle == NULL)                                               /* check handle */
    {
        return 2;                                                     /* return error */
    }
    if (handle->inited != 1)                                          /* check handle initialization */
    {
        return 3;                                                     /* return error */
    }

    buf[0] = ISD17XX_COMMAND_RD_APC;                                  /* read apc command */
    buf[1] = 0x00;                                                    /* set 0x00 */
    buf[2] = 0x00;                                                    /* set 0x00 */
    buf[3] = 0x00;                                                    /* set 0x00 */
    if (a_isd17xx_spi_transmit(handle, buf, data, 4) != 0)            /* read the data */
    {
        handle->debug_print("isd17xx: read config failed.\n");        /* read config failed */

        return 1;                                                     /* return error */
    }
    config = (uint16_t)(data[3] & 0xF) << 8 | data[2];                /* get the config */
    *output = (isd17xx_analog_output_t)((config >> 7) & 0x01);        /* get the bool */

    return a_isd17xx_check_status(handle, 0, 0, 0, 0);                /* check the status */
}

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
uint8_t isd17xx_set_pwm_speaker(isd17xx_handle_t *handle, isd17xx_bool_t enable)
{
    uint8_t buf[4];
    uint8_t data[4];
    uint16_t config;

    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }

    buf[0] = ISD17XX_COMMAND_RD_APC;                                      /* read apc command */
    buf[1] = 0x00;                                                        /* set 0x00 */
    buf[2] = 0x00;                                                        /* set 0x00 */
    buf[3] = 0x00;                                                        /* set 0x00 */
    if (a_isd17xx_spi_transmit(handle, buf, data, 4) != 0)                /* read the data */
    {
        handle->debug_print("isd17xx: read config failed.\n");            /* read config failed */

        return 1;                                                         /* return error */
    }
    if (a_isd17xx_check_status(handle, 0, 0, 0, 0) != 0)                  /* check the status */
    {
        return 1;                                                         /* return error */
    }

    config = (uint16_t)(data[3] & 0xF) << 8 | data[2];                    /* get the config */
    config &= ~((uint16_t)1 << 8);                                        /* clear config */
    config |= (!enable) << 8;                                             /* set the config */
    if (handle->vol_control == ISD17XX_VOL_CONTROL_BUTTON)                /* button control */
    {
        buf[0] = ISD17XX_COMMAND_WR_APC1;                                 /* write apc command */
    }
    else                                                                  /* register control */
    {
        buf[0] = ISD17XX_COMMAND_WR_APC2;                                 /* write apc command */
    }
    buf[1] = config & 0xFF;                                               /* set low */
    buf[2] = (config >> 8) & 0xFF;                                        /* set high */
    if (a_isd17xx_spi_transmit(handle, buf, data, 3) != 0)                /* write the data */
    {
        handle->debug_print("isd17xx: write config failed.\n");           /* write config failed */

        return 1;                                                         /* return error */
    }

    return a_isd17xx_check_status(handle, 0, 0, 0, 0);                    /* check the status */
}

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
uint8_t isd17xx_get_pwm_speaker(isd17xx_handle_t *handle, isd17xx_bool_t *enable)
{
    uint8_t buf[4];
    uint8_t data[4];
    uint16_t config;

    if (handle == NULL)                                               /* check handle */
    {
        return 2;                                                     /* return error */
    }
    if (handle->inited != 1)                                          /* check handle initialization */
    {
        return 3;                                                     /* return error */
    }

    buf[0] = ISD17XX_COMMAND_RD_APC;                                  /* read apc command */
    buf[1] = 0x00;                                                    /* set 0x00 */
    buf[2] = 0x00;                                                    /* set 0x00 */
    buf[3] = 0x00;                                                    /* set 0x00 */
    if (a_isd17xx_spi_transmit(handle, buf, data, 4) != 0)            /* read the data */
    {
        handle->debug_print("isd17xx: read config failed.\n");        /* read config failed */

        return 1;                                                     /* return error */
    }
    config = (uint16_t)(data[3] & 0xF) << 8 | data[2];                /* get the config */
    *enable = (isd17xx_bool_t)(!((config >> 8) & 0x01));              /* get the bool */

    return a_isd17xx_check_status(handle, 0, 0, 0, 0);                /* check the status */
}

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
uint8_t isd17xx_set_power_up_analog_output(isd17xx_handle_t *handle, isd17xx_bool_t enable)
{
    uint8_t buf[4];
    uint8_t data[4];
    uint16_t config;

    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }

    buf[0] = ISD17XX_COMMAND_RD_APC;                                      /* read apc command */
    buf[1] = 0x00;                                                        /* set 0x00 */
    buf[2] = 0x00;                                                        /* set 0x00 */
    buf[3] = 0x00;                                                        /* set 0x00 */
    if (a_isd17xx_spi_transmit(handle, buf, data, 4) != 0)                /* read the data */
    {
        handle->debug_print("isd17xx: read config failed.\n");            /* read config failed */

        return 1;                                                         /* return error */
    }
    if (a_isd17xx_check_status(handle, 0, 0, 0, 0) != 0)                  /* check the status */
    {
        return 1;                                                         /* return error */
    }

    config = (uint16_t)(data[3] & 0xF) << 8 | data[2];                    /* get the config */
    config &= ~((uint16_t)1 << 9);                                        /* clear config */
    config |= (!enable) << 9;                                             /* set the config */
    if (handle->vol_control == ISD17XX_VOL_CONTROL_BUTTON)                /* button control */
    {
        buf[0] = ISD17XX_COMMAND_WR_APC1;                                 /* write apc command */
    }
    else                                                                  /* register control */
    {
        buf[0] = ISD17XX_COMMAND_WR_APC2;                                 /* write apc command */
    }
    buf[1] = config & 0xFF;                                               /* set low */
    buf[2] = (config >> 8) & 0xFF;                                        /* set high */
    if (a_isd17xx_spi_transmit(handle, buf, data, 3) != 0)                /* write the data */
    {
        handle->debug_print("isd17xx: write config failed.\n");           /* write config failed */

        return 1;                                                         /* return error */
    }

    return a_isd17xx_check_status(handle, 0, 0, 0, 0);                    /* check the status */
}

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
uint8_t isd17xx_get_power_up_analog_output(isd17xx_handle_t *handle, isd17xx_bool_t *enable)
{
    uint8_t buf[4];
    uint8_t data[4];
    uint16_t config;

    if (handle == NULL)                                               /* check handle */
    {
        return 2;                                                     /* return error */
    }
    if (handle->inited != 1)                                          /* check handle initialization */
    {
        return 3;                                                     /* return error */
    }

    buf[0] = ISD17XX_COMMAND_RD_APC;                                  /* read apc command */
    buf[1] = 0x00;                                                    /* set 0x00 */
    buf[2] = 0x00;                                                    /* set 0x00 */
    buf[3] = 0x00;                                                    /* set 0x00 */
    if (a_isd17xx_spi_transmit(handle, buf, data, 4) != 0)            /* read the data */
    {
        handle->debug_print("isd17xx: read config failed.\n");        /* read config failed */

        return 1;                                                     /* return error */
    }
    config = (uint16_t)(data[3] & 0xF) << 8 | data[2];                /* get the config */
    *enable = (isd17xx_bool_t)(!((config >> 9) & 0x01));              /* get the bool */

    return a_isd17xx_check_status(handle, 0, 0, 0, 0);                /* check the status */
}

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
uint8_t isd17xx_set_v_alert(isd17xx_handle_t *handle, isd17xx_bool_t enable)
{
    uint8_t buf[4];
    uint8_t data[4];
    uint16_t config;

    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }

    buf[0] = ISD17XX_COMMAND_RD_APC;                                      /* read apc command */
    buf[1] = 0x00;                                                        /* set 0x00 */
    buf[2] = 0x00;                                                        /* set 0x00 */
    buf[3] = 0x00;                                                        /* set 0x00 */
    if (a_isd17xx_spi_transmit(handle, buf, data, 4) != 0)                /* read the data */
    {
        handle->debug_print("isd17xx: read config failed.\n");            /* read config failed */

        return 1;                                                         /* return error */
    }
    if (a_isd17xx_check_status(handle, 0, 0, 0, 0) != 0)                  /* check the status */
    {
        return 1;                                                         /* return error */
    }

    config = (uint16_t)(data[3] & 0xF) << 8 | data[2];                    /* get the config */
    config &= ~((uint16_t)1 << 10);                                       /* clear config */
    config |= (!enable) << 10;                                            /* set the config */
    if (handle->vol_control == ISD17XX_VOL_CONTROL_BUTTON)                /* button control */
    {
        buf[0] = ISD17XX_COMMAND_WR_APC1;                                 /* write apc command */
    }
    else                                                                  /* register control */
    {
        buf[0] = ISD17XX_COMMAND_WR_APC2;                                 /* write apc command */
    }
    buf[1] = config & 0xFF;                                               /* set low */
    buf[2] = (config >> 8) & 0xFF;                                        /* set high */
    if (a_isd17xx_spi_transmit(handle, buf, data, 3) != 0)                /* write the data */
    {
        handle->debug_print("isd17xx: write config failed.\n");           /* write config failed */

        return 1;                                                         /* return error */
    }

    return a_isd17xx_check_status(handle, 0, 0, 0, 0);                    /* check the status */
}

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
uint8_t isd17xx_get_v_alert(isd17xx_handle_t *handle, isd17xx_bool_t *enable)
{
    uint8_t buf[4];
    uint8_t data[4];
    uint16_t config;

    if (handle == NULL)                                               /* check handle */
    {
        return 2;                                                     /* return error */
    }
    if (handle->inited != 1)                                          /* check handle initialization */
    {
        return 3;                                                     /* return error */
    }

    buf[0] = ISD17XX_COMMAND_RD_APC;                                  /* read apc command */
    buf[1] = 0x00;                                                    /* set 0x00 */
    buf[2] = 0x00;                                                    /* set 0x00 */
    buf[3] = 0x00;                                                    /* set 0x00 */
    if (a_isd17xx_spi_transmit(handle, buf, data, 4) != 0)            /* read the data */
    {
        handle->debug_print("isd17xx: read config failed.\n");        /* read config failed */

        return 1;                                                     /* return error */
    }
    config = (uint16_t)(data[3] & 0xF) << 8 | data[2];                /* get the config */
    *enable = (isd17xx_bool_t)(!((config >> 10) & 0x01));             /* get the bool */

    return a_isd17xx_check_status(handle, 0, 0, 0, 0);                /* check the status */
}

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
uint8_t isd17xx_set_eom(isd17xx_handle_t *handle, isd17xx_bool_t enable)
{
    uint8_t buf[4];
    uint8_t data[4];
    uint16_t config;

    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }

    buf[0] = ISD17XX_COMMAND_RD_APC;                                      /* read apc command */
    buf[1] = 0x00;                                                        /* set 0x00 */
    buf[2] = 0x00;                                                        /* set 0x00 */
    buf[3] = 0x00;                                                        /* set 0x00 */
    if (a_isd17xx_spi_transmit(handle, buf, data, 4) != 0)                /* read the data */
    {
        handle->debug_print("isd17xx: read config failed.\n");            /* read config failed */

        return 1;                                                         /* return error */
    }
    if (a_isd17xx_check_status(handle, 0, 0, 0, 0) != 0)                  /* check the status */
    {
        return 1;                                                         /* return error */
    }

    config = (uint16_t)(data[3] & 0xF) << 8 | data[2];                    /* get the config */
    config &= ~((uint16_t)1 << 11);                                       /* clear config */
    config |= enable << 11;                                               /* set the config */
    if (handle->vol_control == ISD17XX_VOL_CONTROL_BUTTON)                /* button control */
    {
        buf[0] = ISD17XX_COMMAND_WR_APC1;                                 /* write apc command */
    }
    else                                                                  /* register control */
    {
        buf[0] = ISD17XX_COMMAND_WR_APC2;                                 /* write apc command */
    }
    buf[1] = config & 0xFF;                                               /* set low */
    buf[2] = (config >> 8) & 0xFF;                                        /* set high */
    if (a_isd17xx_spi_transmit(handle, buf, data, 3) != 0)                /* write the data */
    {
        handle->debug_print("isd17xx: write config failed.\n");           /* write config failed */

        return 1;                                                         /* return error */
    }

    return a_isd17xx_check_status(handle, 0, 0, 0, 0);                    /* check the status */
}

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
uint8_t isd17xx_get_eom(isd17xx_handle_t *handle, isd17xx_bool_t *enable)
{
    uint8_t buf[4];
    uint8_t data[4];
    uint16_t config;

    if (handle == NULL)                                               /* check handle */
    {
        return 2;                                                     /* return error */
    }
    if (handle->inited != 1)                                          /* check handle initialization */
    {
        return 3;                                                     /* return error */
    }

    buf[0] = ISD17XX_COMMAND_RD_APC;                                  /* read apc command */
    buf[1] = 0x00;                                                    /* set 0x00 */
    buf[2] = 0x00;                                                    /* set 0x00 */
    buf[3] = 0x00;                                                    /* set 0x00 */
    if (a_isd17xx_spi_transmit(handle, buf, data, 4) != 0)            /* read the data */
    {
        handle->debug_print("isd17xx: read config failed.\n");        /* read config failed */

        return 1;                                                     /* return error */
    }
    config = (uint16_t)(data[3] & 0xF) << 8 | data[2];                /* get the config */
    *enable = (isd17xx_bool_t)((config >> 11) & 0x01);                /* get the bool */

    return a_isd17xx_check_status(handle, 0, 0, 0, 0);                /* check the status */
}

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
uint8_t isd17xx_set_volume_control(isd17xx_handle_t *handle, isd17xx_vol_control_t control)
{
    uint8_t buf[4];
    uint8_t data[4];
    uint16_t config;

    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }

    buf[0] = ISD17XX_COMMAND_RD_APC;                                      /* read apc command */
    buf[1] = 0x00;                                                        /* set 0x00 */
    buf[2] = 0x00;                                                        /* set 0x00 */
    buf[3] = 0x00;                                                        /* set 0x00 */
    if (a_isd17xx_spi_transmit(handle, buf, data, 4) != 0)                /* read the data */
    {
        handle->debug_print("isd17xx: read config failed.\n");            /* read config failed */

        return 1;                                                         /* return error */
    }
    if (a_isd17xx_check_status(handle, 0, 0, 0, 0) != 0)                  /* check the status */
    {
        return 1;                                                         /* return error */
    }

    config = (uint16_t)(data[3] & 0xF) << 8 | data[2];                    /* get the config */
    handle->vol_control = control;                                        /* set vol control */
    if (handle->vol_control == ISD17XX_VOL_CONTROL_BUTTON)                /* button control */
    {
        buf[0] = ISD17XX_COMMAND_WR_APC1;                                 /* write apc command */
    }
    else                                                                  /* register control */
    {
        buf[0] = ISD17XX_COMMAND_WR_APC2;                                 /* write apc command */
    }
    buf[1] = config & 0xFF;                                               /* set low */
    buf[2] = (config >> 8) & 0xFF;                                        /* set high */
    if (a_isd17xx_spi_transmit(handle, buf, data, 3) != 0)                /* write the data */
    {
        handle->debug_print("isd17xx: write config failed.\n");           /* write config failed */

        return 1;                                                         /* return error */
    }

    return a_isd17xx_check_status(handle, 0, 0, 0, 0);                    /* check the status */
}

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
uint8_t isd17xx_get_volume_control(isd17xx_handle_t *handle, isd17xx_vol_control_t *control)
{
    if (handle == NULL)                                             /* check handle */
    {
        return 2;                                                   /* return error */
    }
    if (handle->inited != 1)                                        /* check handle initialization */
    {
        return 3;                                                   /* return error */
    }

    *control = (isd17xx_vol_control_t)(handle->vol_control);        /* get the vol control */

    return a_isd17xx_check_status(handle, 0, 0, 0, 0);              /* check the status */
}

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
uint8_t isd17xx_write_to_nv(isd17xx_handle_t *handle)
{
    uint8_t buf[1];

    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }

    buf[0] = 0x00;                                                                 /* set 0x00 */
    if (a_isd17xx_spi_write(handle, ISD17XX_COMMAND_WR_NVCFG, buf, 1) != 0)        /* set write to nvc */
    {
        handle->debug_print("isd17xx: set write to nvc failed.\n");                /* set write to nvc failed */

        return 1;                                                                  /* return error */
    }

    return a_isd17xx_check_status(handle, 0, 0, 0, 0);                             /* check the status */
}

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
uint8_t isd17xx_load_from_nv(isd17xx_handle_t *handle)
{
    uint8_t buf[1];

    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }

    buf[0] = 0x00;                                                                 /* set 0x00 */
    if (a_isd17xx_spi_write(handle, ISD17XX_COMMAND_LD_NVCFG, buf, 1) != 0)        /* set load from nvc */
    {
        handle->debug_print("isd17xx: set load from nvc failed.\n");               /* set load from nvc failed */

        return 1;                                                                  /* return error */
    }

    return a_isd17xx_check_status(handle, 0, 0, 0, 0);                             /* check the status */
}

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
uint8_t isd17xx_set_play(isd17xx_handle_t *handle, uint16_t start_addr, uint16_t end_addr)
{
    uint8_t buf[6];

    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    if (start_addr >= end_addr)                                                    /* check the address */
    {
        handle->debug_print("isd17xx: start_addr >= end_addr.\n");                 /* start_addr >= end_addr */

        return 4;                                                                  /* return error */
    }
    if (end_addr > handle->end_address)                                            /* check the address */
    {
        handle->debug_print("isd17xx: address is invalid.\n");                     /* address is invalid */

        return 5;                                                                  /* return error */
    }

    buf[0] = 0x00;                                                                 /* set 0x00 */
    buf[1] = start_addr & 0xFF;                                                    /* set start address */
    buf[2] = (start_addr >> 8) & 0x07;                                             /* set start address*/
    buf[3] = end_addr & 0xFF;                                                      /* set end address */
    buf[4] = (end_addr >> 8) & 0x07;                                               /* set end address */
    buf[5] = 0x00;                                                                 /* set 0x00 */
    if (a_isd17xx_spi_write(handle, ISD17XX_COMMAND_SET_PLAY, buf, 6) != 0)        /* set play */
    {
        handle->debug_print("isd17xx: set play failed.\n");                        /* set play failed */

        return 1;                                                                  /* return error */
    }
    handle->delay_ms(500);                                                         /* delay 500ms */

    return 0;                                                                      /* success return 0 */
}

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
uint8_t isd17xx_set_record(isd17xx_handle_t *handle, uint16_t start_addr, uint16_t end_addr)
{
    uint8_t buf[6];

    if (handle == NULL)                                                           /* check handle */
    {
        return 2;                                                                 /* return error */
    }
    if (handle->inited != 1)                                                      /* check handle initialization */
    {
        return 3;                                                                 /* return error */
    }
    if (start_addr >= end_addr)                                                   /* check the address */
    {
        handle->debug_print("isd17xx: start_addr >= end_addr.\n");                /* start_addr >= end_addr */

        return 4;                                                                 /* return error */
    }
    if (end_addr > handle->end_address)                                            /* check the address */
    {
        handle->debug_print("isd17xx: address is invalid.\n");                     /* address is invalid */

        return 5;                                                                  /* return error */
    }

    buf[0] = 0x00;                                                                /* set 0x00 */
    buf[1] = start_addr & 0xFF;                                                   /* set start address */
    buf[2] = (start_addr >> 8) & 0x07;                                            /* set start address */
    buf[3] = end_addr & 0xFF;                                                     /* set end address */
    buf[4] = (end_addr >> 8) & 0x07;                                              /* set end address */
    buf[5] = 0x00;                                                                /* set 0x00 */
    if (a_isd17xx_spi_write(handle, ISD17XX_COMMAND_SET_REC, buf, 6) != 0)        /* set record */
    {
        handle->debug_print("isd17xx: set record failed.\n");                     /* set record failed */

        return 1;                                                                 /* return error */
    }
    handle->delay_ms(500);                                                        /* delay 500ms */

    return 0;                                                                     /* success return 0 */
}

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
uint8_t isd17xx_set_erase(isd17xx_handle_t *handle, uint16_t start_addr, uint16_t end_addr)
{
    uint8_t buf[6];

    if (handle == NULL)                                                           /* check handle */
    {
        return 2;                                                                 /* return error */
    }
    if (handle->inited != 1)                                                      /* check handle initialization */
    {
        return 3;                                                                 /* return error */
    }
    if (start_addr >= end_addr)                                                   /* check the address */
    {
        handle->debug_print("isd17xx: start_addr >= end_addr.\n");                /* start_addr >= end_addr */

        return 4;                                                                 /* return error */
    }
    if (end_addr > handle->end_address)                                            /* check the address */
    {
        handle->debug_print("isd17xx: address is invalid.\n");                     /* address is invalid */

        return 5;                                                                  /* return error */
    }

    buf[0] = 0x00;                                                                /* set 0x00 */
    buf[1] = start_addr & 0xFF;                                                   /* set start address */
    buf[2] = (start_addr >> 8) & 0x07;                                            /* set start address */
    buf[3] = end_addr & 0xFF;                                                     /* set end address */
    buf[4] = (end_addr >> 8) & 0x07;                                              /* set end address */
    buf[5] = 0x00;                                                                /* set 0x00 */
    if (a_isd17xx_spi_write(handle, ISD17XX_COMMAND_SET_ERASE, buf, 6) != 0)      /* set erase */
    {
        handle->debug_print("isd17xx: set erase failed.\n");                      /* set erase failed */

        return 1;                                                                 /* return error */
    }
    handle->delay_ms(500);                                                        /* delay 500ms */

    return 0;                                                                     /* success return 0 */
}

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
uint8_t isd17xx_toggle_extern_clock(isd17xx_handle_t *handle)
{
    uint8_t buf[1];

    if (handle == NULL)                                                          /* check handle */
    {
        return 2;                                                                /* return error */
    }
    if (handle->inited != 1)                                                     /* check handle initialization */
    {
        return 3;                                                                /* return error */
    }

    buf[0] = 0x00;                                                               /* set 0x00 */
    if (a_isd17xx_spi_write(handle, ISD17XX_COMMAND_EXTCLK, buf, 1) != 0)        /* set toggle */
    {
        handle->debug_print("isd17xx: set toggle failed.\n");                    /* set toggle failed */

        return 1;                                                                /* return error */
    }

    return a_isd17xx_check_status(handle, 0, 0, 0, 0);                           /* check the status */
}

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
uint8_t isd17xx_init(isd17xx_handle_t *handle)
{
    uint8_t id;
    uint8_t type_flag;
    uint8_t buf[3];
    uint8_t status[3];

    if (handle == NULL)                                                 /* check handle */
    {
        return 2;                                                       /* return error */
    }
    if (handle->debug_print == NULL)                                    /* check debug_print */
    {
        return 3;                                                       /* return error */
    }
    if (handle->spi_init == NULL)                                       /* check spi_init */
    {
        handle->debug_print("isd17xx: spi_init is null.\n");            /* spi_init is null */

        return 3;                                                       /* return error */
    }
    if (handle->spi_deinit == NULL)                                     /* check spi_deinit */
    {
        handle->debug_print("isd17xx: spi_deinit is null.\n");          /* spi_deinit is null */

        return 3;                                                       /* return error */
    }
    if (handle->spi_read == NULL)                                       /* check spi_read */
    {
        handle->debug_print("isd17xx: spi_read is null.\n");            /* spi_read is null */

        return 3;                                                       /* return error */
    }
    if (handle->spi_write == NULL)                                      /* check spi_write */
    {
        handle->debug_print("isd17xx: spi_write is null.\n");           /* spi_write is null */

        return 3;                                                       /* return error */
    }
    if (handle->spi_transmit == NULL)                                   /* check spi_transmit */
    {
        handle->debug_print("isd17xx: spi_transmit is null.\n");        /* spi_transmit is null */

        return 3;                                                       /* return error */
    }
    if (handle->gpio_reset_init == NULL)                                /* check gpio_reset_init */
    {
        handle->debug_print("isd17xx: gpio_reset_init is null.\n");     /* gpio_reset_init is null */

        return 3;                                                       /* return error */
    }
    if (handle->gpio_reset_deinit == NULL)                              /* check gpio_reset_deinit */
    {
        handle->debug_print("isd17xx: gpio_reset_deinit is null.\n");   /* gpio_reset_deinit is null */

        return 3;                                                       /* return error */
    }
    if (handle->gpio_reset_write == NULL)                               /* check gpio_reset_write */
    {
        handle->debug_print("isd17xx: gpio_reset_write is null.\n");    /* gpio_reset_write is null */

        return 3;                                                       /* return error */
    }
    if (handle->delay_ms == NULL)                                       /* check delay_ms */
    {
        handle->debug_print("isd17xx: delay_ms is null.\n");            /* delay_ms is null */

        return 3;                                                       /* return error */
    }

    if (handle->spi_init() != 0)                                        /* spi init */
    {
        handle->debug_print("isd17xx: spi init failed.\n");             /* spi init failed */

        return 1;                                                       /* return error */
    }
    if (handle->gpio_reset_init() != 0)                                 /* gpio init */
    {
        handle->debug_print("isd17xx: gpio init failed.\n");            /* gpio init failed */
        (void)handle->spi_deinit();                                     /* spi deinit */

        return 1;                                                       /* return error */
    }
    
    handle->gpio_reset_write(1);                                        /* set high */
    handle->delay_ms(1);                                                /* delay 1ms */
    handle->gpio_reset_write(0);                                        /* set low */
    handle->delay_ms(1);                                                /* delay 1ms */
    
    buf[0] = 0x00;                                                      /* set 0x00 */
    if (a_isd17xx_spi_write(handle, ISD17XX_COMMAND_PU,
                            buf, 1) != 0)                               /* set power up */
    {
        handle->debug_print("isd17xx: set power up failed.\n");         /* set power up failed */
        (void)handle->spi_deinit();                                     /* spi deinit */
        (void)handle->gpio_reset_deinit();                              /* gpio deinit */
        
        return 7;                                                       /* return error */
    }
    if (a_isd17xx_check_status(handle, 100, 1,
                               ISD17XX_STATUS2_RDY, 1000) != 0)         /* check the status */
    {
        (void)handle->spi_deinit();                                     /* spi deinit */
        (void)handle->gpio_reset_deinit();                              /* gpio deinit */
        
        return 1;                                                       /* return error */
    }

    buf[0] = ISD17XX_COMMAND_DEVID;                                     /* set device id command */
    buf[1] = 0x00;                                                      /* set 0x00 */
    buf[2] = 0x00;                                                      /* set 0x00 */
    if (a_isd17xx_spi_transmit(handle, buf, status, 3) != 0)            /* read the device id */
    {
        handle->debug_print("isd17xx: get device id failed.\n");        /* get device id failed */
        (void)handle->spi_deinit();                                     /* spi deinit */
        (void)handle->gpio_reset_deinit();                              /* gpio deinit */
        
        return 4;                                                       /* return error */
    }
    id = (status[2] >> 3) & 0xFF;                                       /* set the device id */
    if (id != handle->type)                                             /* check the type */
    {
        handle->debug_print("isd17xx: chip type is invalid.\n");        /* chip type is invalid */
        (void)handle->spi_deinit();                                     /* spi deinit */
        (void)handle->gpio_reset_deinit();                              /* gpio deinit */
        
        return 5;                                                       /* return error */
    }
    type_flag = 0;                                                      /* init 0 */
    switch (handle->type)                                               /* choose the type */
    {
        case ISD1730 :                                                  /* isd1730 */
        {
            handle->end_address = 0x0FF;                                /* set the end address */

            break;                                                      /* break */
        }
        case ISD1740 :                                                  /* isd1740 */
        {
            handle->end_address = 0x14F;                                /* set the end address */

            break;                                                      /* break */
        }
        case ISD1750 :                                                  /* isd1750 */
        {
            handle->end_address = 0x19F;                                /* set the end address */

            break;                                                      /* break */
        }
        case ISD1760 :                                                  /* isd1760 */
        {
            handle->end_address = 0x1EF;                                /* set the end address */

            break;                                                      /* break */
        }
        case ISD1790 :                                                  /* isd1790 */
        {
            handle->end_address = 0x2DF;                                /* set the end address */

            break;                                                      /* break */
        }
        case ISD17120 :                                                 /* isd17120 */
        {
            handle->end_address = 0x3CF;                                /* set the end address */

            break;                                                      /* break */
        }
        case ISD17150 :                                                 /* isd17150 */
        {
            handle->end_address = 0x4BF;                                /* set the end address */

            break;                                                      /* break */
        }
        case ISD17180 :                                                 /* isd17180 */
        {
            handle->end_address = 0x5AF;                                /* set the end address */

            break;                                                      /* break */
        }
        case ISD17210 :                                                 /* isd17210 */
        {
            handle->end_address = 0x69F;                                /* set the end address */

            break;                                                      /* break */
        }
        case ISD17240 :                                                 /* isd17240 */
        {
            handle->end_address = 0x78F;                                /* set the end address */

            break;                                                      /* break */
        }
        default :                                                       /* default */
        {
            type_flag = 1;                                              /* flag error */
            
            break;                                                      /* break */
        }
    }
    if (type_flag == 1)                                                 /* check the type */
    {
        handle->debug_print("isd17xx: unknown type.\n");                /* unknown type */

        return 6;                                                       /* return error */
    }
    
    buf[0] = 0x00;                                                      /* set 0x00 */
    if (a_isd17xx_spi_write(handle, ISD17XX_COMMAND_RESET,
                            buf, 1) != 0)                               /* set reset */
    {
        handle->debug_print("isd17xx: set reset failed.\n");            /* set reset failed */
        (void)handle->spi_deinit();                                     /* spi deinit */
        (void)handle->gpio_reset_deinit();                              /* gpio deinit */
        
        return 8;                                                       /* return error */
    }
    if (a_isd17xx_check_status(handle, 100, 0, 0, 0) != 0)              /* check the status */
    {
        (void)handle->spi_deinit();                                     /* spi deinit */
        (void)handle->gpio_reset_deinit();                              /* gpio deinit */
        
        return 1;                                                       /* return error */
    }
    handle->vol_control = ISD17XX_VOL_CONTROL_REG;                      /* register control */
    handle->inited = 1;                                                 /* flag finish initialization */

    return 0;                                                           /* success return 0 */
}

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
uint8_t isd17xx_deinit(isd17xx_handle_t *handle)
{
    uint8_t buf[1];

    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->inited != 1)                                                 /* check handle initialization */
    {
        return 3;                                                            /* return error */
    }

    buf[0] = 0x00;                                                           /* set 0x00 */
    if (a_isd17xx_spi_write(handle, ISD17XX_COMMAND_PD, buf, 1) != 0)        /* set power down */
    {
        handle->debug_print("isd17xx: set power down failed.\n");            /* set power down failed */

        return 4;                                                            /* return error */
    }
    if (handle->spi_deinit() != 0)                                           /* spi deinit */
    {
        handle->debug_print("isd17xx: spi deinit failed.\n");                /* spi deinit failed */

        return 1;                                                            /* return error */
    }
    if (handle->gpio_reset_deinit() != 0)                                    /* gpio deinit */
    {
        handle->debug_print("isd17xx: gpio deinit failed.\n");               /* gpio deinit failed */

        return 1;                                                            /* return error */
    }
    handle->inited = 0;                                                      /* flag close */

    return 0;                                                                /* success return 0 */
}

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
uint8_t isd17xx_hardware_reset(isd17xx_handle_t *handle)
{
    if (handle == NULL)                 /* check handle */
    {
        return 2;                       /* return error */
    }
    if (handle->inited != 1)            /* check handle initialization */
    {
        return 3;                       /* return error */
    }
    
    handle->gpio_reset_write(1);        /* set high */
    handle->delay_ms(1);                /* delay 1ms */
    handle->gpio_reset_write(0);        /* set low */
    handle->delay_ms(1);                /* delay 1ms */
    
    return 0;                           /* success return 0 */
}

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
uint8_t isd17xx_irq_handler(isd17xx_handle_t *handle)
{
    uint8_t res;
    uint8_t status2;
    uint16_t status1;
    uint8_t buf[1];

    if (handle == NULL)                                                           /* check handle */
    {
        return 2;                                                                 /* return error */
    }
    if (handle->inited != 1)                                                      /* check handle initialization */
    {
        return 3;                                                                 /* return error */
    }

    res = isd17xx_get_status(handle, &status1, &status2);                         /* get status */
    if (res != 0)                                                                 /* check the result */
    {
        handle->debug_print("isd17xx: get status failed.\n");                     /* get status failed */

        return 1;                                                                 /* return error */
    }
    buf[0] = 0x00;                                                                /* set 0x00 */
    if (a_isd17xx_spi_write(handle, ISD17XX_COMMAND_CLR_INT, buf, 1) != 0)        /* set clear interrupt */
    {
        handle->debug_print("isd17xx: set clear interrupt failed.\n");            /* set clear interrupt failed */

        return 1;                                                                 /* return error */
    }
    if ((status1 & ISD17XX_STATUS1_EOM) != 0)                                     /* check the eom */
    {
        if (handle->receive_callback != NULL)                                     /* if receive_callback not null */
        {
            handle->receive_callback(ISD17XX_STATUS1_EOM);                        /* run the callback */
        }
    }
    if ((status1 & ISD17XX_STATUS1_INT) != 0)                                     /* check the int */
    {
        handle->done = 1;                                                         /* flag done */
        if (handle->receive_callback != NULL)                                     /* if receive_callback not null */
        {
            handle->receive_callback(ISD17XX_STATUS1_INT);                        /* run the callback */
        }
    }

    return 0;                                                                     /* success return 0 */
}

/**
 * @brief     set the chip type
 * @param[in] *handle points to an isd17xx handle structure
 * @param[in] type is the chip type
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t isd17xx_set_type(isd17xx_handle_t *handle, isd17xx_type_t type)
{
    if (handle == NULL)         /* check handle */
    {
        return 2;               /* return error */
    }

    handle->type = type;        /* set the type */

    return 0;                   /* success return 0 */
}

/**
 * @brief      get the chip type
 * @param[in]  *handle points to an isd17xx handle structure
 * @param[out] *type points to a chip type buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t isd17xx_get_type(isd17xx_handle_t *handle, isd17xx_type_t *type)
{
    if (handle == NULL)                            /* check handle */
    {
        return 2;                                  /* return error */
    }

    *type = (isd17xx_type_t)(handle->type);        /* set the type */

    return 0;                                      /* success return 0 */
}

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
uint8_t isd17xx_set_reg(isd17xx_handle_t *handle, uint8_t cmd, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                       /* check handle */
    {
        return 2;                                             /* return error */
    }
    if (handle->inited != 1)                                  /* check handle initialization */
    {
        return 3;                                             /* return error */
    }

    return a_isd17xx_spi_write(handle, cmd, buf, len);        /* write data */
}

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
uint8_t isd17xx_get_reg(isd17xx_handle_t *handle, uint8_t cmd, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                      /* check handle */
    {
        return 2;                                            /* return error */
    }
    if (handle->inited != 1)                                 /* check handle initialization */
    {
        return 3;                                            /* return error */
    }

    return a_isd17xx_spi_read(handle, cmd, buf, len);        /* read data */
}

/**
 * @brief      get chip's information
 * @param[out] *info points to an isd17xx info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t isd17xx_info(isd17xx_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }

    memset(info, 0, sizeof(isd17xx_info_t));                        /* initialize isd17xx info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "SPI", 8);                             /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */

    return 0;                                                       /* success return 0 */
}
