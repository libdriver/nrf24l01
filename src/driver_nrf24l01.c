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
 * @file      driver_nrf24l01.c
 * @brief     driver nrf24l01 source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2021-11-28
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2021/11/28  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_nrf24l01.h"

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "Nordic nRF24L01"        /**< chip name */
#define MANUFACTURER_NAME         "Nordic"                 /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        1.9f                     /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        3.6f                     /**< chip max supply voltage */
#define MAX_CURRENT               13.50f                   /**< chip max current */
#define TEMPERATURE_MIN           -40.0f                   /**< chip min operating temperature */
#define TEMPERATURE_MAX           85.0f                    /**< chip max operating temperature */
#define DRIVER_VERSION            1000                     /**< driver version */

/**
 * @brief chip command definition
 */
#define NRF24L01_COMMAND_R_REGISTER                 0x00        /**< read command */
#define NRF24L01_COMMAND_W_REGISTER                 0x20        /**< write command */
#define NRF24L01_COMMAND_R_RX_PAYLOAD               0x61        /**< read payload command */
#define NRF24L01_COMMAND_W_TX_PAYLOAD               0xA0        /**< write payload command */
#define NRF24L01_COMMAND_FLUSH_TX                   0xE1        /**< flush tx command */
#define NRF24L01_COMMAND_FLUSH_RX                   0xE2        /**< flush rx command */
#define NRF24L01_COMMAND_REUSE_TX_PL                0xE3        /**< reuse tx payload command */
#define NRF24L01_COMMAND_R_RX_PL_WID                0x60        /**< read rx payload width for the top command */
#define NRF24L01_COMMAND_W_ACK_PAYLOAD              0xA8        /**< write ack payload command */
#define NRF24L01_COMMAND_W_TX_PAYLOAD_NO_ACK        0xB0        /**< write ack payload with ack command */
#define NRF24L01_COMMAND_NOP                        0xFF        /**< nop command */

/**
 * @brief chip register definition
 */
#define NRF24L01_REG_CONFIG             0x00        /**< config register */
#define NRF24L01_REG_EN_AA              0x01        /**< enable auto acknowledgment register */
#define NRF24L01_REG_EN_RXADDR          0x02        /**< enabled rx addresses register */
#define NRF24L01_REG_SETUP_AW           0x03        /**< setup of address widths register */
#define NRF24L01_REG_SETUP_RETR         0x04        /**< setup of automatic retransmission register */
#define NRF24L01_REG_RF_CH              0x05        /**< rf channel register */
#define NRF24L01_REG_RF_SETUP           0x06        /**< rf setup register register */
#define NRF24L01_REG_STATUS             0x07        /**< status register */
#define NRF24L01_REG_OBSERVE_TX         0x08        /**< transmit observe register */
#define NRF24L01_REG_RPD                0x09        /**< received power detector register */
#define NRF24L01_REG_RX_ADDR_P0         0x0A        /**< receive address data pipe 0 register */
#define NRF24L01_REG_RX_ADDR_P1         0x0B        /**< receive address data pipe 1 register */
#define NRF24L01_REG_RX_ADDR_P2         0x0C        /**< receive address data pipe 2 register */
#define NRF24L01_REG_RX_ADDR_P3         0x0D        /**< receive address data pipe 3 register */
#define NRF24L01_REG_RX_ADDR_P4         0x0E        /**< receive address data pipe 4 register */
#define NRF24L01_REG_RX_ADDR_P5         0x0F        /**< receive address data pipe 5 register */
#define NRF24L01_REG_TX_ADDR            0x10        /**< transmit address register */
#define NRF24L01_REG_RX_PW_P0           0x11        /**< number of bytes in rx payload in data pipe 0 register */
#define NRF24L01_REG_RX_PW_P1           0x12        /**< number of bytes in rx payload in data pipe 1 register */
#define NRF24L01_REG_RX_PW_P2           0x13        /**< number of bytes in rx payload in data pipe 2 register */
#define NRF24L01_REG_RX_PW_P3           0x14        /**< number of bytes in rx payload in data pipe 3 register */
#define NRF24L01_REG_RX_PW_P4           0x15        /**< number of bytes in rx payload in data pipe 4 register */
#define NRF24L01_REG_RX_PW_P5           0x16        /**< number of bytes in rx payload in data pipe 5 register */
#define NRF24L01_REG_FIFO_STATUS        0x17        /**< fifo status register */
#define NRF24L01_REG_DYNPD              0x1C        /**< enable dynamic payload length register */
#define NRF24L01_REG_FEATURE            0x1D        /**< feature register */

/**
 * @brief      read bytes
 * @param[in]  *handle points to an nrf24l01 handle structure
 * @param[in]  reg is the register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 spi read failed
 * @note       none
 */
static uint8_t a_nrf24l01_spi_read(nrf24l01_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle->spi_read(NRF24L01_COMMAND_R_REGISTER | reg, buf, len) != 0)   /* spi read */
    {
        return 1;                                                             /* return error */
    }
    else
    {
        return 0;                                                             /* success return 0 */
    }
}

/**
 * @brief     write bytes
 * @param[in] *handle points to an nrf24l01 handle structure
 * @param[in] reg is the register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data length
 * @return    status code
 *            - 0 success
 *            - 1 spi write failed
 * @note      none
 */
static uint8_t a_nrf24l01_spi_write(nrf24l01_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle->spi_write(NRF24L01_COMMAND_W_REGISTER | reg, buf, len) != 0)   /* spi write */
    {
        return 1;                                                              /* return error */
    }
    else
    {
        return 0;                                                              /* success return 0 */
    }
}

/**
 * @brief     initialize the chip
 * @param[in] *handle points to an nrf24l01 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 spi initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 *            - 4 gpio init failed
 * @note      none
 */
uint8_t nrf24l01_init(nrf24l01_handle_t *handle)
{
    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->debug_print == NULL)                                         /* check debug_print */
    {
        return 3;                                                            /* return error */
    }
    if (handle->spi_init == NULL)                                            /* check spi_init */
    {
        handle->debug_print("nrf24l01: spi_init is null.\n");                /* spi_init is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->spi_deinit == NULL)                                          /* check spi_deinit */
    {
        handle->debug_print("nrf24l01: spi_deinit is null.\n");              /* spi_deinit is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->spi_read == NULL)                                            /* check spi_read */
    {
        handle->debug_print("nrf24l01: spi_read is null.\n");                /* spi_read is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->spi_write == NULL)                                           /* check spi_write */
    {
        handle->debug_print("nrf24l01: spi_write is null.\n");               /* spi_write is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->gpio_init == NULL)                                           /* check gpio_init */
    {
        handle->debug_print("nrf24l01: gpio_init is null.\n");               /* gpio_init is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->gpio_deinit == NULL)                                         /* check gpio_deinit */
    {
        handle->debug_print("nrf24l01: gpio_deinit is null.\n");             /* gpio_deinit is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->gpio_write == NULL)                                          /* check gpio_write */
    {
        handle->debug_print("nrf24l01: gpio_write is null.\n");              /* gpio_write is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->delay_ms == NULL)                                            /* check delay_ms */
    {
        handle->debug_print("nrf24l01: delay_ms is null.\n");                /* delay_ms is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->receive_callback == NULL)                                    /* check receive_callback */
    {
        handle->debug_print("nrf24l01: receive_callback is null.\n");        /* receive_callback is null */
       
        return 3;                                                            /* return error */
    }
    
    if (handle->gpio_init() != 0)                                            /* gpio init */
    {
        handle->debug_print("nrf24l01: gpio init failed.\n");                /* gpio init failed */
       
        return 4;                                                            /* return error */
    }
    if (handle->spi_init() != 0)                                             /* spi init */
    {
        handle->debug_print("nrf24l01: spi init failed.\n");                 /* spi init failed */
        (void)handle->gpio_deinit();                                         /* gpio deinit */
        
        return 1;                                                            /* return error */
    }
    
    handle->inited = 1;                                                      /* flag finish initialization */
    
    return 0;                                                                /* success return 0 */
}

/**
 * @brief     close the chip
 * @param[in] *handle points to an nrf24l01 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 nop failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 gpio deinit failed
 *            - 5 spi deinit failed
 * @note      none
 */
uint8_t nrf24l01_deinit(nrf24l01_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = handle->spi_write(NRF24L01_COMMAND_NOP, NULL, 0);                             /* nop */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("nrf24l01: nop failed.\n");                                 /* nop failed */
       
        return 1;                                                                       /* return error */
    }
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_CONFIG, (uint8_t *)&prev, 1);        /* get config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("nrf24l01: get config failed.\n");                          /* get config failed */
       
        return 1;                                                                       /* return error */
    }
    prev &= ~(1 << 1);                                                                  /* clear config */
    res = a_nrf24l01_spi_write(handle, NRF24L01_REG_CONFIG, (uint8_t *)&prev, 1);       /* set config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("nrf24l01: set config failed.\n");                          /* set config failed */
       
        return 1;                                                                       /* return error */
    }
    res = handle->gpio_deinit();                                                        /* gpio deinit */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("nrf24l01: gpio deinit failed.\n");                         /* gpio deinit failed */
       
        return 4;                                                                       /* return error */
    }
    res = handle->spi_deinit();                                                         /* spi deinit */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("nrf24l01: spi deinit failed.\n");                          /* spi deinit failed */
       
        return 5;                                                                       /* return error */
    }
    handle->inited = 0;                                                                 /* flag close */
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief     enable or disable the chip
 * @param[in] *handle points to an nrf24l01 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set active failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf24l01_set_active(nrf24l01_handle_t *handle, nrf24l01_bool_t enable)
{
    if (handle == NULL)                                               /* check handle */
    {
        return 2;                                                     /* return error */
    }
    if (handle->inited != 1)                                          /* check handle initialization */
    {
        return 3;                                                     /* return error */
    }
    
    if (handle->gpio_write(enable) != 0)                              /* gpio write */
    {
        handle->debug_print("nrf24l01: gpio write failed.\n");        /* gpio write failed */
       
        return 1;                                                     /* return error */
    }
    
    return 0;                                                         /* success return 0 */
}

/**
 * @brief     send data
 * @param[in] *handle points to an nrf24l01 handle structure
 * @param[in] *buf points to a data buffer
 * @param[in] len is the buffer length
 * @return    status code
 *            - 0 success
 *            - 1 send failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 len is over 32
 *            - 5 send timeout
 * @note      none
 */
uint8_t nrf24l01_send(nrf24l01_handle_t *handle, uint8_t *buf, uint8_t len)
{
    uint8_t res;
    uint8_t i;
    uint8_t k;
    uint8_t tmp;
    uint8_t buffer[32];
    uint32_t timeout;
    
    if (handle == NULL)                                                                    /* check handle */
    {
        return 2;                                                                          /* return error */
    }
    if (handle->inited != 1)                                                               /* check handle initialization */
    {
        return 3;                                                                          /* return error */
    }
    if (len > 32)                                                                          /* check the result */
    {
        handle->debug_print("nrf24l01: len is over 32.\n");                                /* len is over 32 */
       
        return 4;                                                                          /* return error */
    }

    memcpy((uint8_t *)buffer, buf, len);                                                   /* copy the data */
    k = len / 2;                                                                           /* get the half */
    for (i = 0; i < k; i++)                                                                /* run k times */
    {
        tmp = buffer[i];                                                                   /* copy to tmp */
        buffer[i] = buffer[len - 1 - i];                                                   /* buffer[i] = buffer[n - 1 - i] */
        buffer[len - 1 - i] = tmp;                                                         /* set buffer[n - 1 - i]*/
    }
    handle->finished = 0;                                                                  /* clear finished */
    if (handle->gpio_write(0) != 0)                                                        /* gpio write */
    {
        handle->debug_print("nrf24l01: gpio write failed.\n");                             /* gpio write failed */
       
        return 1;                                                                          /* return error */
    }
    res = handle->spi_write(NRF24L01_COMMAND_W_TX_PAYLOAD, (uint8_t *)buffer, len);        /* set tx payload */
    if (res != 0)                                                                          /* check result */
    {
        handle->debug_print("nrf24l01: set tx payload failed.\n");                         /* set tx payload failed */
       
        return 1;                                                                          /* return error */
    }
    if (handle->gpio_write(1) != 0)                                                        /* gpio write */
    {
        handle->debug_print("nrf24l01: gpio write failed.\n");                             /* gpio write failed */
       
        return 1;                                                                          /* return error */
    }
    timeout = 5000;                                                                        /* set timeout */
    while ((timeout != 0) && (handle->finished == 0))                                      /* wait time */
    {
        handle->delay_ms(1);                                                               /* delay 1 ms */
        timeout--;                                                                         /* timeout-- */
    }
    if (timeout == 0)                                                                      /* check timeout */
    {
        handle->debug_print("nrf24l01: send timeout.\n");                                  /* send timeout failed */
       
        return 5;                                                                          /* return error */
    }
    if (handle->finished == 1)                                                             /* check finished */
    {
        return 0;                                                                          /* success return 0 */
    }
    else
    {
        handle->debug_print("nrf24l01: send failed.\n");                                   /* send failed */
       
        return 1;                                                                          /* return error */
    }
}

/**
 * @brief     irq handler
 * @param[in] *handle points to an nrf24l01 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf24l01_irq_handler(nrf24l01_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                                     /* check handle */
    {
        return 2;                                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                                /* check handle initialization */
    {
        return 3;                                                                                           /* return error */
    }
    
    res = handle->gpio_write(0);                                                                            /* set gpio */
    if (res != 0)                                                                                           /* check result */
    {
        handle->debug_print("nrf24l01: gpio write failed.\n");                                              /* gpio write failed */
       
        return 1;                                                                                           /* return error */
    }
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_STATUS, (uint8_t *)&prev, 1);                            /* get status register */
    if (res != 0)                                                                                           /* check result */
    {
        handle->debug_print("nrf24l01: get status register failed.\n");                                     /* get status register failed */
        (void)handle->gpio_write(1);                                                                        /* set gpio */
       
        return 1;                                                                                           /* return error */
    }
    res = a_nrf24l01_spi_write(handle, NRF24L01_REG_STATUS, (uint8_t *)&prev, 1);                           /* clear status register */
    if (res != 0)                                                                                           /* check result */
    {
        handle->debug_print("nrf24l01: set status register failed.\n");                                     /* set status register failed */
        (void)handle->gpio_write(1);                                                                        /* set gpio */
        
        return 1;                                                                                           /* return error */
    }
    
    if (((prev >> 0) & 0x01) != 0)                                                                          /* tx full */
    {
        if (handle->receive_callback != NULL)                                                               /* if receive callback */
        {
            handle->receive_callback(NRF24L01_INTERRUPT_TX_FULL, 0, NULL, 0);                               /* run receive callback */
        }
    }
    if (((prev >> 4) & 0x01) != 0)                                                                          /* max rt */
    {
        res = handle->spi_write(NRF24L01_COMMAND_FLUSH_TX, NULL, 0);                                        /* flush tx */
        if (res != 0)                                                                                       /* check result */
        {
            handle->debug_print("nrf24l01: flush tx failed.\n");                                            /* flush tx failed */
            (void)handle->gpio_write(1);                                                                    /* set gpio */
            
            return 1;                                                                                       /* return error */
        }
        handle->finished = 2;                                                                               /* set timeout */
        if (handle->receive_callback != NULL)                                                               /* if receive callback */
        {
            handle->receive_callback(NRF24L01_INTERRUPT_MAX_RT, 0, NULL, 0);                                /* run receive callback */
        }
    }
    if (((prev >> 5) & 0x01) != 0)                                                                          /* send ok */
    {
        handle->finished = 1;                                                                               /* set finished */
        if (handle->receive_callback != NULL)                                                               /* if receive callback */
        {
            handle->receive_callback(NRF24L01_INTERRUPT_TX_DS, 0, NULL, 0);                                 /* run receive callback */
        }
    }
    if (((prev >> 6) & 0x01) != 0)                                                                          /* receive */
    {
        uint8_t num;
        uint8_t width;
        uint8_t i;
        uint8_t k;
        uint8_t tmp;
        uint8_t buffer[33];
        
        res = handle->spi_read(NRF24L01_COMMAND_R_RX_PL_WID, (uint8_t *)&width, 1);                         /* get payload width */
        if (res != 0)                                                                                       /* check result */
        {
            handle->debug_print("nrf24l01: get payload width failed.\n");                                   /* get payload width failed */
            (void)handle->gpio_write(1);                                                                    /* set gpio */
            
            return 1;                                                                                       /* return error */
        }
        if (width > 32)                                                                                     /* check width */
        {
            res = handle->spi_write(NRF24L01_COMMAND_FLUSH_RX, NULL, 0);                                    /* flush rx */
            if (res != 0)                                                                                   /* check result */
            {
                handle->debug_print("nrf24l01: flush rx failed.\n");                                        /* flush rx failed */
                (void)handle->gpio_write(1);                                                                /* set gpio */
                
                return 1;                                                                                   /* return error */
            }
        }
        res = handle->spi_read(NRF24L01_COMMAND_R_RX_PAYLOAD, (uint8_t *)buffer, width);                    /* get rx payload */
        if (res != 0)                                                                                       /* check result */
        {
            handle->debug_print("nrf24l01: get rx payload failed.\n");                                      /* get rx payload failed */
            (void)handle->gpio_write(1);                                                                    /* set gpio */
            
            return 1;                                                                                       /* return error */
        }
        k = width / 2;                                                                                      /* get the half */
        for (i = 0; i < k; i++)                                                                             /* run k times */
        {
            tmp = buffer[i];                                                                                /* copy to tmp */
            buffer[i] = buffer[width - 1 - i];                                                              /* buffer[i] = buffer[n - 1 - i] */
            buffer[width - 1 - i] = tmp;                                                                    /* set buffer[n - 1 - i]*/
        }
        num = (prev >> 1) & 0x7;                                                                            /* get number */
        if (handle->receive_callback != NULL)                                                               /* if receive callback */
        {
            handle->receive_callback(NRF24L01_INTERRUPT_RX_DR, num, (uint8_t *)buffer, width);              /* run receive callback */
        }
    }
    res = handle->gpio_write(1);                                                                            /* set gpio write */
    if (res != 0)                                                                                           /* check result */
    {
        handle->debug_print("nrf24l01: gpio write failed.\n");                                              /* gpio write failed */
       
        return 1;                                                                                           /* return error */
    }
    
    return 0;                                                                                               /* success return 0 */
}

/**
 * @brief     enable or disable configure
 * @param[in] *handle points to an nrf24l01 handle structure
 * @param[in] config is the config type
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set config failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf24l01_set_config(nrf24l01_handle_t *handle, nrf24l01_config_t config, nrf24l01_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_CONFIG, (uint8_t *)&prev, 1);        /* get config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("nrf24l01: get config failed.\n");                          /* get config failed */
       
        return 1;                                                                       /* return error */
    }
    prev &= ~(1 << config);                                                             /* clear config */
    prev |= enable << config;                                                           /* set config */
    res = a_nrf24l01_spi_write(handle, NRF24L01_REG_CONFIG, (uint8_t *)&prev, 1);       /* set config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("nrf24l01: set config failed.\n");                          /* set config failed */
       
        return 1;                                                                       /* return error */
    }
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief      get the configure
 * @param[in]  *handle points to an nrf24l01 handle structure
 * @param[in]  config is the config type
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get config failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf24l01_get_config(nrf24l01_handle_t *handle, nrf24l01_config_t config, nrf24l01_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_CONFIG, (uint8_t *)&prev, 1);        /* get config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("nrf24l01: get config failed.\n");                          /* get config failed */
       
        return 1;                                                                       /* return error */
    }
    *enable = (nrf24l01_bool_t)((prev >> config) & 0x01);                               /* get config */
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief     set the chip mode
 * @param[in] *handle points to an nrf24l01 handle structure
 * @param[in] mode is the chip mode
 * @return    status code
 *            - 0 success
 *            - 1 set mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf24l01_set_mode(nrf24l01_handle_t *handle, nrf24l01_mode_t mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_CONFIG, (uint8_t *)&prev, 1);        /* get config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("nrf24l01: get config failed.\n");                          /* get config failed */
       
        return 1;                                                                       /* return error */
    }
    prev &= ~(1 << 0);                                                                  /* clear config */
    prev |= mode << 0;                                                                  /* set config */
    res = a_nrf24l01_spi_write(handle, NRF24L01_REG_CONFIG, (uint8_t *)&prev, 1);       /* set config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("nrf24l01: set config failed.\n");                          /* set config failed */
       
        return 1;                                                                       /* return error */
    }
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief      get the chip mode
 * @param[in]  *handle points to an nrf24l01 handle structure
 * @param[out] *mode points to a chip mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf24l01_get_mode(nrf24l01_handle_t *handle, nrf24l01_mode_t *mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_CONFIG, (uint8_t *)&prev, 1);       /* get config */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("nrf24l01: get config failed.\n");                         /* get config failed */
       
        return 1;                                                                      /* return error */
    }
    *mode = (nrf24l01_mode_t)(prev & 0x01);                                            /* get mode */
    
    return 0;                                                                          /* success return 0 */
}

/**
 * @brief     enable or disable auto acknowledgment
 * @param[in] *handle points to an nrf24l01 handle structure
 * @param[in] pipe is the rf pipe
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set auto acknowledgment failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf24l01_set_auto_acknowledgment(nrf24l01_handle_t *handle, nrf24l01_pipe_t pipe, nrf24l01_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_EN_AA, (uint8_t *)&prev, 1);        /* get auto acknowledgment */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("nrf24l01: get auto acknowledgment failed.\n");            /* get auto acknowledgment failed */
       
        return 1;                                                                      /* return error */
    }
    prev &= ~(1 << pipe);                                                              /* clear config */
    prev |= enable << pipe;                                                            /* set pipe */
    res = a_nrf24l01_spi_write(handle, NRF24L01_REG_EN_AA, (uint8_t *)&prev, 1);       /* set auto acknowledgment */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("nrf24l01: set auto acknowledgment failed.\n");            /* set auto acknowledgment failed */
       
        return 1;                                                                      /* return error */
    }
    
    return 0;                                                                          /* success return 0 */
}

/**
 * @brief      get the auto acknowledgment status
 * @param[in]  *handle points to an nrf24l01 handle structure
 * @param[in]  pipe is the rf pipe
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get auto acknowledgment failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf24l01_get_auto_acknowledgment(nrf24l01_handle_t *handle, nrf24l01_pipe_t pipe, nrf24l01_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_EN_AA, (uint8_t *)&prev, 1);        /* get auto acknowledgment */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("nrf24l01: get auto acknowledgment failed.\n");            /* get auto acknowledgment failed */
       
        return 1;                                                                      /* return error */
    }
    *enable = (nrf24l01_bool_t)((prev >> pipe) & 0x01);                                /* get pipe */
    
    return 0;                                                                          /* success return 0 */
}

/**
 * @brief     enable or disable rx pipe
 * @param[in] *handle points to an nrf24l01 handle structure
 * @param[in] pipe is the rf pipe
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set rx pipe failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf24l01_set_rx_pipe(nrf24l01_handle_t *handle, nrf24l01_pipe_t pipe, nrf24l01_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_EN_RXADDR, (uint8_t *)&prev, 1);       /* get rx address */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("nrf24l01: get rx address failed.\n");                        /* get rx address failed */
       
        return 1;                                                                         /* return error */
    }
    prev &= ~(1 << pipe);                                                                 /* clear config */
    prev |= enable << pipe;                                                               /* set pipe */
    res = a_nrf24l01_spi_write(handle, NRF24L01_REG_EN_RXADDR, (uint8_t *)&prev, 1);      /* set rx address */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("nrf24l01: set rx address failed.\n");                        /* set rx address failed */
       
        return 1;                                                                         /* return error */
    }
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief      get the rx pipe status
 * @param[in]  *handle points to an nrf24l01 handle structure
 * @param[in]  pipe is the rf pipe
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get rx pipe failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf24l01_get_rx_pipe(nrf24l01_handle_t *handle, nrf24l01_pipe_t pipe, nrf24l01_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_EN_RXADDR, (uint8_t *)&prev, 1);       /* get rx address */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("nrf24l01: get rx address failed.\n");                        /* get rx address failed */
       
        return 1;                                                                         /* return error */
    }
    *enable = (nrf24l01_bool_t)((prev >> pipe) & 0x01);                                   /* get pipe */
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief     set the address width
 * @param[in] *handle points to an nrf24l01 handle structure
 * @param[in] width is the address width
 * @return    status code
 *            - 0 success
 *            - 1 set address width failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf24l01_set_address_width(nrf24l01_handle_t *handle, nrf24l01_address_width_t width)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_SETUP_AW, (uint8_t *)&prev, 1);       /* get setup of address widths */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("nrf24l01: get setup of address widths failed.\n");          /* get setup of address widths failed */
       
        return 1;                                                                        /* return error */
    }
    prev &= ~(3 << 0);                                                                   /* clear config */
    prev |= width << 0;                                                                  /* set width */
    res = a_nrf24l01_spi_write(handle, NRF24L01_REG_SETUP_AW, (uint8_t *)&prev, 1);      /* set setup of address widths */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("nrf24l01: set setup of address widths failed.\n");          /* set setup of address widths failed */
       
        return 1;                                                                        /* return error */
    }
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief      get the address width
 * @param[in]  *handle points to an nrf24l01 handle structure
 * @param[out] *width points to an address width buffer
 * @return     status code
 *             - 0 success
 *             - 1 get address width failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf24l01_get_address_width(nrf24l01_handle_t *handle, nrf24l01_address_width_t *width)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_SETUP_AW, (uint8_t *)&prev, 1);       /* get setup of address widths */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("nrf24l01: get setup of address widths failed.\n");          /* get setup of address widths failed */
       
        return 1;                                                                        /* return error */
    }
    *width = (nrf24l01_address_width_t)((prev >> 0) & 0x3);                              /* get config */
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief     set the auto retransmit delay
 * @param[in] *handle points to an nrf24l01 handle structure
 * @param[in] delay is the auto retransmit delay
 * @return    status code
 *            - 0 success
 *            - 1 set auto retransmit delay failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 delay is over 0xF
 * @note      delay <= 0xF
 */
uint8_t nrf24l01_set_auto_retransmit_delay(nrf24l01_handle_t *handle, uint8_t delay)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    if (delay > 0xF)                                                                             /* check delay */
    {
        handle->debug_print("nrf24l01: delay is over 0xF.\n");                                   /* delay is over 0xF */
       
        return 4;                                                                                /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_SETUP_RETR, (uint8_t *)&prev, 1);             /* get setup of automatic retransmission */
    if (res != 0)                                                                                /* check result */
    {
        handle->debug_print("nrf24l01: get setup of automatic retransmission failed.\n");        /* get setup of automatic retransmission failed */
       
        return 1;                                                                                /* return error */
    }
    prev &= ~(0xF << 4);                                                                         /* clear config */
    prev |= delay << 4;                                                                          /* set delay */
    res = a_nrf24l01_spi_write(handle, NRF24L01_REG_SETUP_RETR, (uint8_t *)&prev, 1);            /* set setup of automatic retransmission */
    if (res != 0)                                                                                /* check result */
    {
        handle->debug_print("nrf24l01: set setup of automatic retransmission failed.\n");        /* set setup of automatic retransmission failed */
       
        return 1;                                                                                /* return error */
    }
    
    return 0;                                                                                    /* success return 0 */
}

/**
 * @brief      get the auto retransmit delay
 * @param[in]  *handle points to an nrf24l01 handle structure
 * @param[out] *delay points to an auto retransmit delay buffer
 * @return     status code
 *             - 0 success
 *             - 1 get auto retransmit delay failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf24l01_get_auto_retransmit_delay(nrf24l01_handle_t *handle, uint8_t *delay)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_SETUP_RETR, (uint8_t *)&prev, 1);             /* get setup of automatic retransmission */
    if (res != 0)                                                                                /* check result */
    {
        handle->debug_print("nrf24l01: get setup of automatic retransmission failed.\n");        /* get setup of automatic retransmission failed */
       
        return 1;                                                                                /* return error */
    }
    *delay = (prev >> 4) & 0xF;                                                                  /* get delay */
    
    return 0;                                                                                    /* success return 0 */
}

/**
 * @brief      convert the delay to the register raw data
 * @param[in]  *handle points to an nrf24l01 handle structure
 * @param[in]  us is the delay
 * @param[out] *reg points to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf24l01_auto_retransmit_delay_convert_to_register(nrf24l01_handle_t *handle, uint32_t us, uint8_t *reg)
{
    if (handle == NULL)                /* check handle */
    { 
        return 2;                      /* return error */
    }
    if (handle->inited != 1)           /* check handle initialization */
    {
        return 3;                      /* return error */
    }
    
    *reg = (uint8_t)(us / 250);        /* convert real data to register data */
    
    return 0;                          /* success return 0 */
}

/**
 * @brief      convert the register raw data to the delay
 * @param[in]  *handle points to an nrf24l01 handle structure
 * @param[in]  reg is the register raw data
 * @param[out] *us points to a delay buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf24l01_auto_retransmit_delay_convert_to_data(nrf24l01_handle_t *handle, uint8_t reg, uint32_t *us)
{
    if (handle == NULL)                 /* check handle */
    {
        return 2;                       /* return error */
    }
    if (handle->inited != 1)            /* check handle initialization */
    {
        return 3;                       /* return error */
    }
    
    *us = (uint32_t)(reg) * 250;        /* convert raw data to real data */
    
    return 0;                           /* success return 0 */
}

/**
 * @brief     set the auto retransmit count
 * @param[in] *handle points to an nrf24l01 handle structure
 * @param[in] count is the auto retransmit count
 * @return    status code
 *            - 0 success
 *            - 1 set auto retransmit count failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 count is over 0xF
 * @note      count <= 0xF
 */
uint8_t nrf24l01_set_auto_retransmit_count(nrf24l01_handle_t *handle, uint8_t count)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    if (count > 0xF)                                                                             /* check count */
    {
        handle->debug_print("nrf24l01: count is over 0xF.\n");                                   /* count is over 0xF */
       
        return 4;                                                                                /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_SETUP_RETR, (uint8_t *)&prev, 1);             /* get setup of automatic retransmission */
    if (res != 0)                                                                                /* check result */
    {
        handle->debug_print("nrf24l01: get setup of automatic retransmission failed.\n");        /* get setup of automatic retransmission failed */
       
        return 1;                                                                                /* return error */
    }
    prev &= ~(0xF << 0);                                                                         /* clear config */
    prev |= count << 0;                                                                          /* set count */
    res = a_nrf24l01_spi_write(handle, NRF24L01_REG_SETUP_RETR, (uint8_t *)&prev, 1);            /* set setup of automatic retransmission */
    if (res != 0)                                                                                /* check result */
    {
        handle->debug_print("nrf24l01: set setup of automatic retransmission failed.\n");        /* set setup of automatic retransmission failed */
       
        return 1;                                                                                /* return error */
    }
    
    return 0;                                                                                    /* success return 0 */
}

/**
 * @brief      get the auto retransmit count
 * @param[in]  *handle points to an nrf24l01 handle structure
 * @param[out] *count points to an auto retransmit count buffer
 * @return     status code
 *             - 0 success
 *             - 1 get auto retransmit count failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf24l01_get_auto_retransmit_count(nrf24l01_handle_t *handle, uint8_t *count)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_SETUP_RETR, (uint8_t *)&prev, 1);             /* get setup of automatic retransmission */
    if (res != 0)                                                                                /* check result */
    {
        handle->debug_print("nrf24l01: get setup of automatic retransmission failed.\n");        /* get setup of automatic retransmission failed */
       
        return 1;                                                                                /* return error */
    }
    *count = (prev >> 0) & 0xF;                                                                  /* get count */
    
    return 0;                                                                                    /* success return 0 */
}

/**
 * @brief     set the channel frequency
 * @param[in] *handle points to an nrf24l01 handle structure
 * @param[in] freq is the channel freq
 * @return    status code
 *            - 0 success
 *            - 1 set channel frequency failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 freq is over 0x7F
 * @note      freq <= 0x7F
 */
uint8_t nrf24l01_set_channel_frequency(nrf24l01_handle_t *handle, uint8_t freq)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }
    if (freq > 0x7F)                                                                   /* check freq */
    {
        handle->debug_print("nrf24l01: freq is over 0x7F.\n");                         /* freq is over 0x7F */
       
        return 4;                                                                      /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_RF_CH, (uint8_t *)&prev, 1);        /* get rf channel */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("nrf24l01: get rf channel failed.\n");                     /* get rf channel failed */
       
        return 1;                                                                      /* return error */
    }
    prev &= ~(0x7F << 0);                                                              /* clear config */
    prev |= freq << 0;                                                                 /* set freq */
    res = a_nrf24l01_spi_write(handle, NRF24L01_REG_RF_CH, (uint8_t *)&prev, 1);       /* set rf channel */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("nrf24l01: set rf channel failed.\n");                     /* set rf channel failed */
       
        return 1;                                                                      /* return error */
    }
    
    return 0;                                                                          /* success return 0 */
}

/**
 * @brief      get the channel frequency
 * @param[in]  *handle points to an nrf24l01 handle structure
 * @param[out] *freq points to a channel freq buffer
 * @return     status code
 *             - 0 success
 *             - 1 get channel frequency failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf24l01_get_channel_frequency(nrf24l01_handle_t *handle, uint8_t *freq)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                               /* check handle */
    {
        return 2;                                                                     /* return error */
    }
    if (handle->inited != 1)                                                          /* check handle initialization */
    {
        return 3;                                                                     /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_RF_CH, (uint8_t *)&prev, 1);       /* get rf channel */
    if (res != 0)                                                                     /* check result */
    {
        handle->debug_print("nrf24l01: get rf channel failed.\n");                    /* get rf channel failed */
       
        return 1;                                                                     /* return error */
    }
    *freq = prev & 0x7F;                                                              /* get freq */
    
    return 0;                                                                         /* success return 0 */
}

/**
 * @brief     enable or disable continuous carrier transmit
 * @param[in] *handle points to an nrf24l01 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set continuous carrier transmit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf24l01_set_continuous_carrier_transmit(nrf24l01_handle_t *handle, nrf24l01_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_RF_SETUP, (uint8_t *)&prev, 1);        /* get rf setup register */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("nrf24l01: get rf setup register failed.\n");                 /* get rf setup register failed */
       
        return 1;                                                                         /* return error */
    }
    prev &= ~(1 << 7);                                                                    /* clear config */
    prev |= enable << 7;                                                                  /* set bool */
    res = a_nrf24l01_spi_write(handle, NRF24L01_REG_RF_SETUP, (uint8_t *)&prev, 1);       /* set rf setup register */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("nrf24l01: set rf setup register failed.\n");                 /* set rf setup register failed */
       
        return 1;                                                                         /* return error */
    }
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief      get the continuous carrier transmit status
 * @param[in]  *handle points to an nrf24l01 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get continuous carrier transmit failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf24l01_get_continuous_carrier_transmit(nrf24l01_handle_t *handle, nrf24l01_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_RF_SETUP, (uint8_t *)&prev, 1);        /* get rf setup register */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("nrf24l01: get rf setup register failed.\n");                 /* get rf setup register failed */
       
        return 1;                                                                         /* return error */
    }
    *enable = (nrf24l01_bool_t)((prev >> 7) & 0x01);                                      /* get bool */
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief     enable or disable force pll lock signal
 * @param[in] *handle points to an nrf24l01 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set force pll lock signal failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf24l01_set_force_pll_lock_signal(nrf24l01_handle_t *handle, nrf24l01_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_RF_SETUP, (uint8_t *)&prev, 1);        /* get rf setup register */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("nrf24l01: get rf setup register failed.\n");                 /* get rf setup register failed */
       
        return 1;                                                                         /* return error */
    }
    prev &= ~(1 << 4);                                                                    /* clear config */
    prev |= enable << 4;                                                                  /* set bool */
    res = a_nrf24l01_spi_write(handle, NRF24L01_REG_RF_SETUP, (uint8_t *)&prev, 1);       /* set rf setup register */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("nrf24l01: set rf setup register failed.\n");                 /* set rf setup register failed */
       
        return 1;                                                                         /* return error */
    }
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief      get the force pll lock signal status
 * @param[in]  *handle points to an nrf24l01 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get force pll lock signal failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf24l01_get_force_pll_lock_signal(nrf24l01_handle_t *handle, nrf24l01_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_RF_SETUP, (uint8_t *)&prev, 1);        /* get rf setup register */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("nrf24l01: get rf setup register failed.\n");                 /* get rf setup register failed */
       
        return 1;                                                                         /* return error */
    }
    *enable = (nrf24l01_bool_t)((prev >> 4) & 0x01);                                      /* get bool */
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief     set the data rate
 * @param[in] *handle points to an nrf24l01 handle structure
 * @param[in] rate is the data rate
 * @return    status code
 *            - 0 success
 *            - 1 set data rate failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf24l01_set_data_rate(nrf24l01_handle_t *handle, nrf24l01_data_rate_t rate)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_RF_SETUP, (uint8_t *)&prev, 1);        /* get rf setup register */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("nrf24l01: get rf setup register failed.\n");                 /* get rf setup register failed */
       
        return 1;                                                                         /* return error */
    }
    prev &= ~(1 << 5);                                                                    /* clear config */
    prev &= ~(1 << 3);                                                                    /* clear config */
    prev |= ((rate >> 0) & 0x1) << 3;                                                     /* set rate */
    prev |= ((rate >> 1) & 0x1) << 5;                                                     /* set rate */
    res = a_nrf24l01_spi_write(handle, NRF24L01_REG_RF_SETUP, (uint8_t *)&prev, 1);       /* set rf setup register */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("nrf24l01: set rf setup register failed.\n");                 /* set rf setup register failed */
       
        return 1;                                                                         /* return error */
    }
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief      get the data rate
 * @param[in]  *handle points to an nrf24l01 handle structure
 * @param[out] *rate points to a data rate buffer
 * @return     status code
 *             - 0 success
 *             - 1 get data rate failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf24l01_get_data_rate(nrf24l01_handle_t *handle, nrf24l01_data_rate_t *rate)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_RF_SETUP, (uint8_t *)&prev, 1);           /* get rf setup register */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("nrf24l01: get rf setup register failed.\n");                    /* get rf setup register failed */
       
        return 1;                                                                            /* return error */
    }
    *rate = (nrf24l01_data_rate_t)(((prev >> 3) & 0x01) | ((prev >> 5) & 0x01) << 1);        /* get rate */
    
    return 0;                                                                                /* success return 0 */
}

/**
 * @brief     set the output power
 * @param[in] *handle points to an nrf24l01 handle structure
 * @param[in] power is the output power
 * @return    status code
 *            - 0 success
 *            - 1 set output power failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf24l01_set_output_power(nrf24l01_handle_t *handle, nrf24l01_output_power_t power)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_RF_SETUP, (uint8_t *)&prev, 1);        /* get rf setup register */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("nrf24l01: get rf setup register failed.\n");                 /* get rf setup register failed */
       
        return 1;                                                                         /* return error */
    }
    prev &= ~(3 << 1);                                                                    /* clear config */
    prev |= power << 1;                                                                   /* set power */
    res = a_nrf24l01_spi_write(handle, NRF24L01_REG_RF_SETUP, (uint8_t *)&prev, 1);       /* set rf setup register */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("nrf24l01: set rf setup register failed.\n");                 /* set rf setup register failed */
       
        return 1;                                                                         /* return error */
    }
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief      get the output power
 * @param[in]  *handle points to an nrf24l01 handle structure
 * @param[out] *power points to an output power buffer
 * @return     status code
 *             - 0 success
 *             - 1 get output power failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf24l01_get_output_power(nrf24l01_handle_t *handle, nrf24l01_output_power_t *power)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_RF_SETUP, (uint8_t *)&prev, 1);       /* get rf setup register */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("nrf24l01: get rf setup register failed.\n");                /* get rf setup register failed */
       
        return 1;                                                                        /* return error */
    }
    *power = (nrf24l01_output_power_t)((prev >> 1) & 0x03);                              /* get output power */
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief      get the interrupt status
 * @param[in]  *handle points to an nrf24l01 handle structure
 * @param[in]  type is the interrupt type
 * @param[out] *enable points to a bool buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf24l01_get_interrupt(nrf24l01_handle_t *handle, nrf24l01_interrupt_t type, nrf24l01_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_STATUS, (uint8_t *)&prev, 1);       /* get status register */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("nrf24l01: get status register failed.\n");                /* get status register failed */
       
        return 1;                                                                      /* return error */
    }
    *enable = (nrf24l01_bool_t)((prev >> type) & 0x01);                                /* get bool */
    
    return 0;                                                                          /* success return 0 */
}

/**
 * @brief     clear the interrupt status
 * @param[in] *handle points to an nrf24l01 handle structure
 * @param[in] type is the interrupt type
 * @return    status code
 *            - 0 success
 *            - 1 clear interrupt failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf24l01_clear_interrupt(nrf24l01_handle_t *handle, nrf24l01_interrupt_t type)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_STATUS, (uint8_t *)&prev, 1);        /* get status register */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("nrf24l01: get status register failed.\n");                 /* get status register failed */
       
        return 1;                                                                       /* return error */
    }
    prev &= ~(1 << type);                                                               /* clear config */
    prev |= 1 << type;                                                                  /* set interrupt */
    res = a_nrf24l01_spi_write(handle, NRF24L01_REG_STATUS, (uint8_t *)&prev, 1);       /* set status register */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("nrf24l01: set status register failed.\n");                 /* set status register failed */
       
        return 1;                                                                       /* return error */
    }
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief      get the data pipe number
 * @param[in]  *handle points to an nrf24l01 handle structure
 * @param[out] *number points to a number buffer
 * @return     status code
 *             - 0 success
 *             - 1 get data pipe number failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf24l01_get_data_pipe_number(nrf24l01_handle_t *handle, uint8_t *number)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_STATUS, (uint8_t *)&prev, 1);       /* get status register */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("nrf24l01: get status register failed.\n");                /* get status register failed */
       
        return 1;                                                                      /* return error */
    }
    *number = (prev >> 1) & 0x7;                                                       /* get number */
    
    return 0;                                                                          /* success return 0 */
}

/**
 * @brief      get the lost packet count
 * @param[in]  *handle points to an nrf24l01 handle structure
 * @param[out] *count points to a count buffer
 * @return     status code
 *             - 0 success
 *             - 1 get lost packet count failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf24l01_get_lost_packet_count(nrf24l01_handle_t *handle, uint8_t *count)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                    /* check handle */
    {
        return 2;                                                                          /* return error */
    }
    if (handle->inited != 1)                                                               /* check handle initialization */
    {
        return 3;                                                                          /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_OBSERVE_TX, (uint8_t *)&prev, 1);       /* get transmit observe register */
    if (res != 0)                                                                          /* check result */
    {
        handle->debug_print("nrf24l01: get transmit observe register failed.\n");          /* get transmit observe failed */
       
        return 1;                                                                          /* return error */
    }
    *count = (prev >> 4) & 0xF;                                                            /* get count */
    
    return 0;                                                                              /* success return 0 */
}

/**
 * @brief      get the retransmitted packet count
 * @param[in]  *handle points to an nrf24l01 handle structure
 * @param[out] *count points to a count buffer
 * @return     status code
 *             - 0 success
 *             - 1 get retransmitted packet count failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf24l01_get_retransmitted_packet_count(nrf24l01_handle_t *handle, uint8_t *count)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                    /* check handle */
    {
        return 2;                                                                          /* return error */
    }
    if (handle->inited != 1)                                                               /* check handle initialization */
    {
        return 3;                                                                          /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_OBSERVE_TX, (uint8_t *)&prev, 1);       /* get transmit observe register */
    if (res != 0)                                                                          /* check result */
    {
        handle->debug_print("nrf24l01: get transmit observe register failed.\n");          /* get transmit observe failed */
       
        return 1;                                                                          /* return error */
    }
    *count = (prev >> 0) & 0xF;                                                            /* get count */
    
    return 0;                                                                              /* success return 0 */
}

/**
 * @brief      get the received power detector
 * @param[in]  *handle points to an nrf24l01 handle structure
 * @param[out] *enable points to a bool buffer
 * @return     status code
 *             - 0 success
 *             - 1 get received power detector failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf24l01_get_received_power_detector(nrf24l01_handle_t *handle, nrf24l01_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->inited != 1)                                                        /* check handle initialization */
    {
        return 3;                                                                   /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_RPD, (uint8_t *)&prev, 1);       /* get rpd register */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("nrf24l01: get rpd failed.\n");                         /* get rpd failed */
       
        return 1;                                                                   /* return error */
    }
    *enable = (nrf24l01_bool_t)((prev >> 0) & 0x01);                                /* get bool */
    
    return 0;                                                                       /* success return 0 */
}

/**
 * @brief     set the rx pipe 0 address
 * @param[in] *handle points to an nrf24l01 handle structure
 * @param[in] *addr points to an address buffer
 * @param[in] len is the address length
 * @return    status code
 *            - 0 success
 *            - 1 set rx pipe 0 address failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 len is too long
 * @note      none
 */
uint8_t nrf24l01_set_rx_pipe_0_address(nrf24l01_handle_t *handle, uint8_t *addr, uint8_t len)
{
    uint8_t res;
    uint8_t prev;
    uint8_t width;
    uint8_t i;
    uint8_t k;
    uint8_t tmp;
    uint8_t buffer[8];
    
    if (handle == NULL)                                                                              /* check handle */
    {
        return 2;                                                                                    /* return error */
    }
    if (handle->inited != 1)                                                                         /* check handle initialization */
    {
        return 3;                                                                                    /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_SETUP_AW, (uint8_t *)&prev, 1);                   /* get setup of address widths */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("nrf24l01: get setup of address widths failed.\n");                      /* get setup of address widths failed */
       
        return 1;                                                                                    /* return error */
    }
    width = (nrf24l01_address_width_t)((prev >> 0) & 0x3);                                           /* get width */
    if (width == 0)                                                                                  /* check width */
    {
        handle->debug_print("nrf24l01: len is too long with %d.\n", 0);                              /* len is too long */
       
        return 4;                                                                                    /* return error */
    }
    else
    {
        width += 2;                                                                                  /* width + 2 */
    }
    if (len > width)                                                                                 /* check width */
    {
        handle->debug_print("nrf24l01: len is too long with %d.\n", width);                          /* len is too long */
       
        return 4;                                                                                    /* return error */
    }
    
    k = len / 2;                                                                                     /* set k */
    memcpy((uint8_t *)buffer, addr, len);                                                            /* copy data */
    for (i = 0; i < k; i++)                                                                          /* run k times */
    {
        tmp = buffer[i];                                                                             /* copy to tmp */
        buffer[i] = buffer[len - 1 - i];                                                             /* buffer[i] = buffer[n - 1 - i] */
        buffer[len - 1 - i] = tmp;                                                                   /* set buffer[n - 1 - i]*/
    }
    res = a_nrf24l01_spi_write(handle, NRF24L01_REG_RX_ADDR_P0, (uint8_t *)buffer, len);             /* set receive address data pipe p0 register */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("nrf24l01: set receive address data pipe p0 register failed.\n");        /* set receive address data pipe p0 register failed */
       
        return 1;                                                                                    /* return error */
    }
    
    return 0;                                                                                        /* success return 0 */
}

/**
 * @brief         get the rx pipe 0 address
 * @param[in]     *handle points to an nrf24l01 handle structure
 * @param[in]     *addr points to an address buffer
 * @param[in,out] *len points to an address length buffer
 * @return        status code
 *                - 0 success
 *                - 1 get rx pipe 0 address failed
 *                - 2 handle is NULL
 *                - 3 handle is not initialized
 *                - 4 len is too short
 * @note          none
 */
uint8_t nrf24l01_get_rx_pipe_0_address(nrf24l01_handle_t *handle, uint8_t *addr, uint8_t *len)
{
    uint8_t res;
    uint8_t i;
    uint8_t k;
    uint8_t tmp;
    uint8_t prev;
    uint8_t width;
    
    if (handle == NULL)                                                                              /* check handle */
    {
        return 2;                                                                                    /* return error */
    }
    if (handle->inited != 1)                                                                         /* check handle initialization */
    {
        return 3;                                                                                    /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_SETUP_AW, (uint8_t *)&prev, 1);                   /* get setup of address widths */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("nrf24l01: get setup of address widths failed.\n");                      /* get setup of address widths failed */
       
        return 1;                                                                                    /* return error */
    }
    width = (nrf24l01_address_width_t)((prev >> 0) & 0x3);                                           /* get width */
    if (width == 0)                                                                                  /* check width */
    {
        handle->debug_print("nrf24l01: len is too long with %d.\n", 0);                              /* len is too long */
       
        return 4;                                                                                    /* return error */
    }
    else
    {
        width += 2;                                                                                  /* width + 2 */
    }
    if (*len < width)                                                                                /* check width */
    {
        handle->debug_print("nrf24l01: len is too short with %d.\n", width);                         /* len is too short */
       
        return 4;                                                                                    /* return error */
    }
    *len = width;                                                                                    /* set width */
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_RX_ADDR_P0, (uint8_t *)addr, width);              /* get receive address data pipe p0 register */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("nrf24l01: get receive address data pipe p0 register failed.\n");        /* get receive address data pipe p0 register failed */
       
        return 1;                                                                                    /* return error */
    }
    k = (*len) / 2;                                                                                  /* set k */
    for (i = 0; i < k; i++)                                                                          /* run k times */
    {
        tmp = addr[i];                                                                               /* copy to tmp */
        addr[i] = addr[(*len) - 1 - i];                                                              /* addr[i] = addr[n - 1 - i] */
        addr[(*len) - 1 - i] = tmp;                                                                  /* set addr[n - 1 - i]*/
    }
    
    return 0;                                                                                        /* success return 0 */
}

/**
 * @brief     set the rx pipe 1 address
 * @param[in] *handle points to an nrf24l01 handle structure
 * @param[in] *addr points to an address buffer
 * @param[in] len is the address length
 * @return    status code
 *            - 0 success
 *            - 1 set rx pipe 1 address failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 len is too long
 * @note      none
 */
uint8_t nrf24l01_set_rx_pipe_1_address(nrf24l01_handle_t *handle, uint8_t *addr, uint8_t len)
{
    uint8_t res;
    uint8_t prev;
    uint8_t width;
    uint8_t i;
    uint8_t k;
    uint8_t tmp;
    uint8_t buffer[8];
    
    if (handle == NULL)                                                                              /* check handle */
    {
        return 2;                                                                                    /* return error */
    }
    if (handle->inited != 1)                                                                         /* check handle initialization */
    {
        return 3;                                                                                    /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_SETUP_AW, (uint8_t *)&prev, 1);                   /* get setup of address widths */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("nrf24l01: get setup of address widths failed.\n");                      /* get setup of address widths failed */
       
        return 1;                                                                                    /* return error */
    }
    width = (nrf24l01_address_width_t)((prev >> 0) & 0x3);                                           /* get width */
    if (width == 0)                                                                                  /* check width */
    {
        handle->debug_print("nrf24l01: len is too long with %d.\n", 0);                              /* len is too long */
       
        return 4;                                                                                    /* return error */
    }
    else
    {
        width += 2;                                                                                  /* width + 2 */
    }
    if (len > width)                                                                                 /* check width */
    {
        handle->debug_print("nrf24l01: len is too long with %d.\n", width);                          /* len is too long */
       
        return 4;                                                                                    /* return error */
    }
    
    k = len / 2;                                                                                     /* set k */
    memcpy((uint8_t *)buffer, addr, len);                                                            /* copy data */
    for (i = 0; i < k; i++)                                                                          /* run k times */
    {
        tmp = buffer[i];                                                                             /* copy to tmp */
        buffer[i] = buffer[len - 1 - i];                                                             /* buffer[i] = buffer[n - 1 - i] */
        buffer[len - 1 - i] = tmp;                                                                   /* set buffer[n - 1 - i]*/
    }
    res = a_nrf24l01_spi_write(handle, NRF24L01_REG_RX_ADDR_P1, (uint8_t *)buffer, len);             /* set receive address data pipe p1 register */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("nrf24l01: set receive address data pipe p1 register failed.\n");        /* set receive address data pipe p1 register failed */
       
        return 1;                                                                                    /* return error */
    }
    
    return 0;                                                                                        /* success return 0 */
}

/**
 * @brief         get the rx pipe 1 address
 * @param[in]     *handle points to an nrf24l01 handle structure
 * @param[in]     *addr points to an address buffer
 * @param[in,out] *len points to an address length buffer
 * @return        status code
 *                - 0 success
 *                - 1 get rx pipe 1 address failed
 *                - 2 handle is NULL
 *                - 3 handle is not initialized
 *                - 4 len is too short
 * @note          none
 */
uint8_t nrf24l01_get_rx_pipe_1_address(nrf24l01_handle_t *handle, uint8_t *addr, uint8_t *len)
{
    uint8_t res;
    uint8_t i;
    uint8_t k;
    uint8_t tmp;
    uint8_t prev;
    uint8_t width;
    
    if (handle == NULL)                                                                              /* check handle */
    {
        return 2;                                                                                    /* return error */
    }
    if (handle->inited != 1)                                                                         /* check handle initialization */
    {
        return 3;                                                                                    /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_SETUP_AW, (uint8_t *)&prev, 1);                   /* get setup of address widths */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("nrf24l01: get setup of address widths failed.\n");                      /* get setup of address widths failed */
       
        return 1;                                                                                    /* return error */
    }
    width = (nrf24l01_address_width_t)((prev >> 0) & 0x3);                                           /* get width */
    if (width == 0)                                                                                  /* check width */
    {
        handle->debug_print("nrf24l01: len is too long with %d.\n", 0);                              /* len is too long */
       
        return 4;                                                                                    /* return error */
    }
    else
    {
        width += 2;                                                                                  /* width + 2 */
    }
    if (*len < width)                                                                                /* check width */
    {
        handle->debug_print("nrf24l01: len is too short with %d.\n", width);                         /* len is too short */
       
        return 4;                                                                                    /* return error */
    }
    *len = width;                                                                                    /* set width */
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_RX_ADDR_P1, (uint8_t *)addr, width);              /* get receive address data pipe p1 register */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("nrf24l01: get receive address data pipe p1 register failed.\n");        /* get receive address data pipe p1 register failed */
       
        return 1;                                                                                    /* return error */
    }
    k = (*len) / 2;                                                                                  /* set k */
    for (i = 0; i < k; i++)                                                                          /* run k times */
    {
        tmp = addr[i];                                                                               /* copy to tmp */
        addr[i] = addr[(*len) - 1 - i];                                                              /* addr[i] = addr[n - 1 - i] */
        addr[(*len) - 1 - i] = tmp;                                                                  /* set addr[n - 1 - i]*/
    }
    
    return 0;                                                                                        /* success return 0 */
}

/**
 * @brief     set the rx pipe 2 address
 * @param[in] *handle points to an nrf24l01 handle structure
 * @param[in] addr is the rx pipe 2 address
 * @return    status code
 *            - 0 success
 *            - 1 set rx pipe 2 address failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf24l01_set_rx_pipe_2_address(nrf24l01_handle_t *handle, uint8_t addr)
{
    uint8_t res;
    
    if (handle == NULL)                                                                              /* check handle */
    {
        return 2;                                                                                    /* return error */
    }
    if (handle->inited != 1)                                                                         /* check handle initialization */
    {
        return 3;                                                                                    /* return error */
    }
    
    res = a_nrf24l01_spi_write(handle, NRF24L01_REG_RX_ADDR_P2, (uint8_t *)&addr, 1);                /* set receive address data pipe p2 register */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("nrf24l01: set receive address data pipe p2 register failed.\n");        /* set receive address data pipe p2 register failed */
       
        return 1;                                                                                    /* return error */
    }
    
    return 0;                                                                                        /* success return 0 */
}

/**
 * @brief      get the rx pipe 2 address
 * @param[in]  *handle points to an nrf24l01 handle structure
 * @param[out] *addr points to an rx pipe 2 address buffer
 * @return     status code
 *             - 0 success
 *             - 1 get rx pipe 2 address failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf24l01_get_rx_pipe_2_address(nrf24l01_handle_t *handle, uint8_t *addr)
{
    uint8_t res;
    
    if (handle == NULL)                                                                              /* check handle */
    {
        return 2;                                                                                    /* return error */
    }
    if (handle->inited != 1)                                                                         /* check handle initialization */
    {
        return 3;                                                                                    /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_RX_ADDR_P2, (uint8_t *)addr, 1);                  /* get receive address data pipe p2 register */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("nrf24l01: get receive address data pipe p2 register failed.\n");        /* get receive address data pipe p2 register failed */
       
        return 1;                                                                                    /* return error */
    }
    
    return 0;                                                                                        /* success return 0 */
}

/**
 * @brief     set the rx pipe 3 address
 * @param[in] *handle points to an nrf24l01 handle structure
 * @param[in] addr is the rx pipe 3 address
 * @return    status code
 *            - 0 success
 *            - 1 set rx pipe 3 address failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf24l01_set_rx_pipe_3_address(nrf24l01_handle_t *handle, uint8_t addr)
{
    uint8_t res;
    
    if (handle == NULL)                                                                              /* check handle */
    {
        return 2;                                                                                    /* return error */
    }
    if (handle->inited != 1)                                                                         /* check handle initialization */
    {
        return 3;                                                                                    /* return error */
    }
    
    res = a_nrf24l01_spi_write(handle, NRF24L01_REG_RX_ADDR_P3, (uint8_t *)&addr, 1);                /* set receive address data pipe p3 register */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("nrf24l01: set receive address data pipe p3 register failed.\n");        /* set receive address data pipe p3 register failed */
       
        return 1;                                                                                    /* return error */
    }
    
    return 0;                                                                                        /* success return 0 */
}

/**
 * @brief      get the rx pipe 3 address
 * @param[in]  *handle points to an nrf24l01 handle structure
 * @param[out] *addr points to an rx pipe 3 address buffer
 * @return     status code
 *             - 0 success
 *             - 1 get rx pipe 3 address failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf24l01_get_rx_pipe_3_address(nrf24l01_handle_t *handle, uint8_t *addr)
{
    uint8_t res;
    
    if (handle == NULL)                                                                              /* check handle */
    {
        return 2;                                                                                    /* return error */
    }
    if (handle->inited != 1)                                                                         /* check handle initialization */
    {
        return 3;                                                                                    /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_RX_ADDR_P3, (uint8_t *)addr, 1);                  /* get receive address data pipe p3 register */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("nrf24l01: get receive address data pipe p3 register failed.\n");        /* get receive address data pipe p3 register failed */
       
        return 1;                                                                                    /* return error */
    }
    
    return 0;                                                                                        /* success return 0 */
}

/**
 * @brief     set the rx pipe 4 address
 * @param[in] *handle points to an nrf24l01 handle structure
 * @param[in] addr is the rx pipe 4 address
 * @return    status code
 *            - 0 success
 *            - 1 set rx pipe 4 address failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf24l01_set_rx_pipe_4_address(nrf24l01_handle_t *handle, uint8_t addr)
{
    uint8_t res;
    
    if (handle == NULL)                                                                              /* check handle */
    {
        return 2;                                                                                    /* return error */
    }
    if (handle->inited != 1)                                                                         /* check handle initialization */
    {
        return 3;                                                                                    /* return error */
    }
    
    res = a_nrf24l01_spi_write(handle, NRF24L01_REG_RX_ADDR_P4, (uint8_t *)&addr, 1);                /* set receive address data pipe p4 register */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("nrf24l01: set receive address data pipe p4 register failed.\n");        /* set receive address data pipe p4 register failed */
       
        return 1;                                                                                    /* return error */
    }
    
    return 0;                                                                                        /* success return 0 */
}

/**
 * @brief      get the rx pipe 4 address
 * @param[in]  *handle points to an nrf24l01 handle structure
 * @param[out] *addr points to an rx pipe 4 address buffer
 * @return     status code
 *             - 0 success
 *             - 1 get rx pipe 4 address failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf24l01_get_rx_pipe_4_address(nrf24l01_handle_t *handle, uint8_t *addr)
{
    uint8_t res;
    
    if (handle == NULL)                                                                              /* check handle */
    {
        return 2;                                                                                    /* return error */
    }
    if (handle->inited != 1)                                                                         /* check handle initialization */
    {
        return 3;                                                                                    /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_RX_ADDR_P4, (uint8_t *)addr, 1);                  /* get receive address data pipe p4 register */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("nrf24l01: get receive address data pipe p4 register failed.\n");        /* get receive address data pipe p4 register failed */
       
        return 1;                                                                                    /* return error */
    }
    
    return 0;                                                                                        /* success return 0 */
}

/**
 * @brief     set the rx pipe 5 address
 * @param[in] *handle points to an nrf24l01 handle structure
 * @param[in] addr is the rx pipe 5 address
 * @return    status code
 *            - 0 success
 *            - 1 set rx pipe 5 address failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf24l01_set_rx_pipe_5_address(nrf24l01_handle_t *handle, uint8_t addr)
{
    uint8_t res;
    
    if (handle == NULL)                                                                              /* check handle */
    {
        return 2;                                                                                    /* return error */
    }
    if (handle->inited != 1)                                                                         /* check handle initialization */
    {
        return 3;                                                                                    /* return error */
    }
    
    res = a_nrf24l01_spi_write(handle, NRF24L01_REG_RX_ADDR_P5, (uint8_t *)&addr, 1);                /* set receive address data pipe p5 register */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("nrf24l01: set receive address data pipe p5 register failed.\n");        /* set receive address data pipe p5 register failed */
       
        return 1;                                                                                    /* return error */
    }
    
    return 0;                                                                                        /* success return 0 */
}

/**
 * @brief      get the rx pipe 5 address
 * @param[in]  *handle points to an nrf24l01 handle structure
 * @param[out] *addr points to an rx pipe 5 address buffer
 * @return     status code
 *             - 0 success
 *             - 1 get rx pipe 5 address failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf24l01_get_rx_pipe_5_address(nrf24l01_handle_t *handle, uint8_t *addr)
{
    uint8_t res;
    
    if (handle == NULL)                                                                              /* check handle */
    {
        return 2;                                                                                    /* return error */
    }
    if (handle->inited != 1)                                                                         /* check handle initialization */
    {
        return 3;                                                                                    /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_RX_ADDR_P5, (uint8_t *)addr, 1);                  /* get receive address data pipe p5 register */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("nrf24l01: get receive address data pipe p5 register failed.\n");        /* get receive address data pipe p5 register failed */
       
        return 1;                                                                                    /* return error */
    }
    
    return 0;                                                                                        /* success return 0 */
}

/**
 * @brief     set the tx address
 * @param[in] *handle points to an nrf24l01 handle structure
 * @param[in] *addr points to an address buffer
 * @param[in] len is the address length
 * @return    status code
 *            - 0 success
 *            - 1 set tx address failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 len is too long
 * @note      none
 */
uint8_t nrf24l01_set_tx_address(nrf24l01_handle_t *handle, uint8_t *addr, uint8_t len)
{
    uint8_t res;
    uint8_t prev;
    uint8_t width;
    uint8_t i;
    uint8_t k;
    uint8_t tmp;
    uint8_t buffer[8];
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_SETUP_AW, (uint8_t *)&prev, 1);        /* get setup of address widths */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("nrf24l01: get setup of address widths failed.\n");           /* get setup of address widths failed */
       
        return 1;                                                                         /* return error */
    }
    width = (nrf24l01_address_width_t)((prev >> 0) & 0x3);                                /* get width */
    if (width == 0)                                                                       /* check width */
    {
        handle->debug_print("nrf24l01: len is too long with %d.\n", 0);                   /* len is too long */
       
        return 4;                                                                         /* return error */
    }
    else
    {
        width += 2;                                                                       /* width + 2 */
    }
    if (len > width)                                                                      /* check width */
    {
        handle->debug_print("nrf24l01: len is too long with %d.\n", width);               /* len is too long */
       
        return 4;                                                                         /* return error */
    }
    
    k = len / 2;                                                                          /* set k */
    memcpy((uint8_t *)buffer, addr, len);                                                 /* copy data */
    for (i = 0; i < k; i++)                                                               /* run k times */
    {
        tmp = buffer[i];                                                                  /* copy to tmp */
        buffer[i] = buffer[len - 1 - i];                                                  /* buffer[i] = buffer[n - 1 - i] */
        buffer[len - 1 - i] = tmp;                                                        /* set buffer[n - 1 - i]*/
    }
    res = a_nrf24l01_spi_write(handle, NRF24L01_REG_TX_ADDR, (uint8_t *)buffer, len);     /* set tx address */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("nrf24l01: set tx address failed.\n");                        /* set tx address failed */
       
        return 1;                                                                         /* return error */
    }
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief         get the tx address
 * @param[in]     *handle points to an nrf24l01 handle structure
 * @param[in]     *addr points to an address buffer
 * @param[in,out] *len points to an address length buffer
 * @return        status code
 *                - 0 success
 *                - 1 get tx address failed
 *                - 2 handle is NULL
 *                - 3 handle is not initialized
 *                - 4 len is too short
 * @note          none
 */
uint8_t nrf24l01_get_tx_address(nrf24l01_handle_t *handle, uint8_t *addr, uint8_t *len)
{
    uint8_t res;
    uint8_t prev;
    uint8_t width;
    uint8_t i;
    uint8_t k;
    uint8_t tmp;
    
    if (handle == NULL)                                                                              /* check handle */
    {
        return 2;                                                                                    /* return error */
    }
    if (handle->inited != 1)                                                                         /* check handle initialization */
    {
        return 3;                                                                                    /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_SETUP_AW, (uint8_t *)&prev, 1);                   /* get setup of address widths */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("nrf24l01: get setup of address widths failed.\n");                      /* get setup of address widths failed */
       
        return 1;                                                                                    /* return error */
    }
    width = (nrf24l01_address_width_t)((prev >> 0) & 0x3);                                           /* get width */
    if (width == 0)                                                                                  /* check width */
    {
        handle->debug_print("nrf24l01: len is too long with %d.\n", 0);                              /* len is too long */
       
        return 4;                                                                                    /* return error */
    }
    else
    {
        width += 2;                                                                                  /* width + 2 */
    }
    if (*len < width)                                                                                /* check width */
    {
        handle->debug_print("nrf24l01: len is too short with %d.\n", width);                         /* len is too short */
       
        return 4;                                                                                    /* return error */
    }
    *len = width;                                                                                    /* set width */
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_TX_ADDR, (uint8_t *)addr, width);                 /* get tx address register */
    if (res != 0)                                                                                    /* check result */
    {
        handle->debug_print("nrf24l01: get tx address failed.\n");                                   /* get tx address failed */
       
        return 1;                                                                                    /* return error */
    }
    k = (*len) / 2;                                                                                  /* set k */
    for (i = 0; i < k; i++)                                                                          /* run k times */
    {
        tmp = addr[i];                                                                               /* copy to tmp */
        addr[i] = addr[(*len) - 1 - i];                                                              /* addr[i] = addr[n - 1 - i] */
        addr[(*len) - 1 - i] = tmp;                                                                  /* set addr[n - 1 - i]*/
    }
    
    return 0;                                                                                        /* success return 0 */
}

/**
 * @brief     set the pipe 0 payload number
 * @param[in] *handle points to an nrf24l01 handle structure
 * @param[in] num is the payload number
 * @return    status code
 *            - 0 success
 *            - 1 set pipe 0 payload number failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 num is over 0x3F
 * @note      none
 */
uint8_t nrf24l01_set_pipe_0_payload_number(nrf24l01_handle_t *handle, uint8_t num)
{
    uint8_t res;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    if (num > 0x3F)                                                                      /* check num */
    {
        handle->debug_print("nrf24l01: num is over 0x3F.\n");                            /* num is over 0x3F */
       
        return 4;                                                                        /* return error */
    }
    
    res = a_nrf24l01_spi_write(handle, NRF24L01_REG_RX_PW_P0, (uint8_t *)&num, 1);       /* set pipe 0 payload number failed */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("nrf24l01: set pipe 0 payload number failed.\n");            /* set pipe 0 payload number failed */
       
        return 1;                                                                        /* return error */
    }
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief      get the pipe 0 payload number
 * @param[in]  *handle points to an nrf24l01 handle structure
 * @param[out] *num points to a payload number buffer
 * @return     status code
 *             - 0 success
 *             - 1 get pipe 0 payload number failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf24l01_get_pipe_0_payload_number(nrf24l01_handle_t *handle, uint8_t *num)
{
    uint8_t res;
    
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_RX_PW_P0, (uint8_t *)num, 1);       /* get pipe 0 payload number failed */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("nrf24l01: get pipe 0 payload number failed.\n");          /* get pipe 0 payload number failed */
       
        return 1;                                                                      /* return error */
    }
    *num = (*num) & 0x3F;                                                              /* get number */
    
    return 0;                                                                          /* success return 0 */
}

/**
 * @brief     set the pipe 1 payload number
 * @param[in] *handle points to an nrf24l01 handle structure
 * @param[in] num is the payload number
 * @return    status code
 *            - 0 success
 *            - 1 set pipe 1 payload number failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 num is over 0x3F
 * @note      none
 */
uint8_t nrf24l01_set_pipe_1_payload_number(nrf24l01_handle_t *handle, uint8_t num)
{
    uint8_t res;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    if (num > 0x3F)                                                                      /* check num */
    {
        handle->debug_print("nrf24l01: num is over 0x3F.\n");                            /* num is over 0x3F */
       
        return 4;                                                                        /* return error */
    }
    
    res = a_nrf24l01_spi_write(handle, NRF24L01_REG_RX_PW_P1, (uint8_t *)&num, 1);       /* set pipe 1 payload number failed */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("nrf24l01: set pipe 1 payload number failed.\n");            /* set pipe 1 payload number failed */
       
        return 1;                                                                        /* return error */
    }
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief      get the pipe 1 payload number
 * @param[in]  *handle points to an nrf24l01 handle structure
 * @param[out] *num points to a payload number buffer
 * @return     status code
 *             - 0 success
 *             - 1 get pipe 1 payload number failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf24l01_get_pipe_1_payload_number(nrf24l01_handle_t *handle, uint8_t *num)
{
    uint8_t res;
    
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_RX_PW_P1, (uint8_t *)num, 1);       /* get pipe 1 payload number failed */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("nrf24l01: get pipe 1 payload number failed.\n");          /* get pipe 1 payload number failed */
       
        return 1;                                                                      /* return error */
    }
    *num = (*num) & 0x3F;                                                              /* get number */
    
    return 0;                                                                          /* success return 0 */
}

/**
 * @brief     set the pipe 2 payload number
 * @param[in] *handle points to an nrf24l01 handle structure
 * @param[in] num is the payload number
 * @return    status code
 *            - 0 success
 *            - 1 set pipe 2 payload number failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 num is over 0x3F
 * @note      none
 */
uint8_t nrf24l01_set_pipe_2_payload_number(nrf24l01_handle_t *handle, uint8_t num)
{
    uint8_t res;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    if (num > 0x3F)                                                                      /* check num */
    {
        handle->debug_print("nrf24l01: num is over 0x3F.\n");                            /* num is over 0x3F */
       
        return 4;                                                                        /* return error */
    }
    
    res = a_nrf24l01_spi_write(handle, NRF24L01_REG_RX_PW_P2, (uint8_t *)&num, 1);       /* set pipe 2 payload number failed */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("nrf24l01: set pipe 2 payload number failed.\n");            /* set pipe 2 payload number failed */
       
        return 1;                                                                        /* return error */
    }
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief      get the pipe 2 payload number
 * @param[in]  *handle points to an nrf24l01 handle structure
 * @param[out] *num points to a payload number buffer
 * @return     status code
 *             - 0 success
 *             - 1 get pipe 2 payload number failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf24l01_get_pipe_2_payload_number(nrf24l01_handle_t *handle, uint8_t *num)
{
    uint8_t res;
    
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_RX_PW_P2, (uint8_t *)num, 1);       /* get pipe 2 payload number failed */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("nrf24l01: get pipe 2 payload number failed.\n");          /* get pipe 2 payload number failed */
       
        return 1;                                                                      /* return error */
    }
    *num = (*num) & 0x3F;                                                              /* get number */
    
    return 0;                                                                          /* success return 0 */
}

/**
 * @brief     set the pipe 3 payload number
 * @param[in] *handle points to an nrf24l01 handle structure
 * @param[in] num is the payload number
 * @return    status code
 *            - 0 success
 *            - 1 set pipe 3 payload number failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 num is over 0x3F
 * @note      none
 */
uint8_t nrf24l01_set_pipe_3_payload_number(nrf24l01_handle_t *handle, uint8_t num)
{
    uint8_t res;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    if (num > 0x3F)                                                                      /* check num */
    {
        handle->debug_print("nrf24l01: num is over 0x3F.\n");                            /* num is over 0x3F */
       
        return 4;                                                                        /* return error */
    }
    
    res = a_nrf24l01_spi_write(handle, NRF24L01_REG_RX_PW_P3, (uint8_t *)&num, 1);       /* set pipe 3 payload number failed */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("nrf24l01: set pipe 3 payload number failed.\n");            /* set pipe 3 payload number failed */
       
        return 1;                                                                        /* return error */
    }
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief      get the pipe 3 payload number
 * @param[in]  *handle points to an nrf24l01 handle structure
 * @param[out] *num points to a payload number buffer
 * @return     status code
 *             - 0 success
 *             - 1 get pipe 3 payload number failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf24l01_get_pipe_3_payload_number(nrf24l01_handle_t *handle, uint8_t *num)
{
    uint8_t res;
    
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_RX_PW_P3, (uint8_t *)num, 1);       /* get pipe 3 payload number failed */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("nrf24l01: get pipe 3 payload number failed.\n");          /* get pipe 3 payload number failed */
       
        return 1;                                                                      /* return error */
    }
    *num = (*num) & 0x3F;                                                              /* get number */
    
    return 0;                                                                          /* success return 0 */
}

/**
 * @brief     set the pipe 4 payload number
 * @param[in] *handle points to an nrf24l01 handle structure
 * @param[in] num is the payload number
 * @return    status code
 *            - 0 success
 *            - 1 set pipe 4 payload number failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 num is over 0x3F
 * @note      none
 */
uint8_t nrf24l01_set_pipe_4_payload_number(nrf24l01_handle_t *handle, uint8_t num)
{
    uint8_t res;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    if (num > 0x3F)                                                                      /* check num */
    {
        handle->debug_print("nrf24l01: num is over 0x3F.\n");                            /* num is over 0x3F */
       
        return 4;                                                                        /* return error */
    }
    
    res = a_nrf24l01_spi_write(handle, NRF24L01_REG_RX_PW_P4, (uint8_t *)&num, 1);       /* set pipe 4 payload number failed */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("nrf24l01: set pipe 4 payload number failed.\n");            /* set pipe 4 payload number failed */
       
        return 1;                                                                        /* return error */
    }
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief      get the pipe 4 payload number
 * @param[in]  *handle points to an nrf24l01 handle structure
 * @param[out] *num points to a payload number buffer
 * @return     status code
 *             - 0 success
 *             - 1 get pipe 4 payload number failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf24l01_get_pipe_4_payload_number(nrf24l01_handle_t *handle, uint8_t *num)
{
    uint8_t res;
    
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_RX_PW_P4, (uint8_t *)num, 1);       /* get pipe 4 payload number failed */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("nrf24l01: get pipe 4 payload number failed.\n");          /* get pipe 4 payload number failed */
       
        return 1;                                                                      /* return error */
    }
    *num = (*num) & 0x3F;                                                              /* get number */
    
    return 0;                                                                          /* success return 0 */
}

/**
 * @brief     set the pipe 5 payload number
 * @param[in] *handle points to an nrf24l01 handle structure
 * @param[in] num is the payload number
 * @return    status code
 *            - 0 success
 *            - 1 set pipe 5 payload number failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 num is over 0x3F
 * @note      none
 */
uint8_t nrf24l01_set_pipe_5_payload_number(nrf24l01_handle_t *handle, uint8_t num)
{
    uint8_t res;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    if (num > 0x3F)                                                                      /* check num */
    {
        handle->debug_print("nrf24l01: num is over 0x3F.\n");                            /* num is over 0x3F */
       
        return 4;                                                                        /* return error */
    }
    
    res = a_nrf24l01_spi_write(handle, NRF24L01_REG_RX_PW_P5, (uint8_t *)&num, 1);       /* set pipe 5 payload number failed */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("nrf24l01: set pipe 5 payload number failed.\n");            /* set pipe 5 payload number failed */
       
        return 1;                                                                        /* return error */
    }
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief      get the pipe 5 payload number
 * @param[in]  *handle points to an nrf24l01 handle structure
 * @param[out] *num points to a payload number buffer
 * @return     status code
 *             - 0 success
 *             - 1 get pipe 5 payload number failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf24l01_get_pipe_5_payload_number(nrf24l01_handle_t *handle, uint8_t *num)
{
    uint8_t res;
    
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_RX_PW_P5, (uint8_t *)num, 1);       /* get pipe 5 payload number failed */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("nrf24l01: get pipe 5 payload number failed.\n");          /* get pipe 5 payload number failed */
       
        return 1;                                                                      /* return error */
    }
    *num = (*num) & 0x3F;                                                              /* get number */
    
    return 0;                                                                          /* success return 0 */
}

/**
 * @brief      get the fifo status
 * @param[in]  *handle points to an nrf24l01 handle structure
 * @param[out] *status points to a fifo status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fifo status failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf24l01_get_fifo_status(nrf24l01_handle_t *handle, uint8_t *status)
{
    uint8_t res;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_FIFO_STATUS, (uint8_t *)status, 1);       /* get fifo status failed */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("nrf24l01: get fifo status failed.\n");                          /* get fifo status failed */
       
        return 1;                                                                            /* return error */
    }
    
    return 0;                                                                                /* success return 0 */
}

/**
 * @brief     enable or disable the pipe dynamic payload
 * @param[in] *handle points to an nrf24l01 handle structure
 * @param[in] pipe is the pipe number
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set pipe dynamic payload failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf24l01_set_pipe_dynamic_payload(nrf24l01_handle_t *handle, nrf24l01_pipe_t pipe, nrf24l01_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                        /* check handle */
    {
        return 2;                                                                              /* return error */
    }
    if (handle->inited != 1)                                                                   /* check handle initialization */
    {
        return 3;                                                                              /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_DYNPD, (uint8_t *)&prev, 1);                /* get dynamic payload length register */
    if (res != 0)                                                                              /* check result */
    {
        handle->debug_print("nrf24l01: get dynamic payload length register failed.\n");        /* get dynamic payload length register failed */
       
        return 1;                                                                              /* return error */
    }
    prev &= ~(1 << pipe);                                                                      /* clear config */
    prev |= enable << pipe;                                                                    /* set bool */
    res = a_nrf24l01_spi_write(handle, NRF24L01_REG_DYNPD, (uint8_t *)&prev, 1);               /* set dynamic payload length register */
    if (res != 0)                                                                              /* check result */
    {
        handle->debug_print("nrf24l01: set dynamic payload length register failed.\n");        /* set dynamic payload length register failed */
       
        return 1;                                                                              /* return error */
    }
    
    return 0;                                                                                  /* success return 0 */
}

/**
 * @brief      get the pipe dynamic payload status
 * @param[in]  *handle points to an nrf24l01 handle structure
 * @param[in]  pipe is the pipe number
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get pipe dynamic payload failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf24l01_get_pipe_dynamic_payload(nrf24l01_handle_t *handle, nrf24l01_pipe_t pipe, nrf24l01_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                        /* check handle */
    {
        return 2;                                                                              /* return error */
    }
    if (handle->inited != 1)                                                                   /* check handle initialization */
    {
        return 3;                                                                              /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_DYNPD, (uint8_t *)&prev, 1);                /* get dynamic payload length register */
    if (res != 0)                                                                              /* check result */
    {
        handle->debug_print("nrf24l01: get dynamic payload length register failed.\n");        /* get dynamic payload length register failed */
       
        return 1;                                                                              /* return error */
    }
    *enable = (nrf24l01_bool_t)((prev >> pipe) & 0x01);                                        /* get bool */
    
    return 0;                                                                                  /* success return 0 */
}

/**
 * @brief     enable or disable the dynamic payload
 * @param[in] *handle points to an nrf24l01 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set dynamic payload failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf24l01_set_dynamic_payload(nrf24l01_handle_t *handle, nrf24l01_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_FEATURE, (uint8_t *)&prev, 1);        /* get feature register */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("nrf24l01: get feature register failed.\n");                 /* get feature register failed */
       
        return 1;                                                                        /* return error */
    }
    prev &= ~(1 << 2);                                                                   /* clear config */
    prev |= enable << 2;                                                                 /* set bool */
    res = a_nrf24l01_spi_write(handle, NRF24L01_REG_FEATURE, (uint8_t *)&prev, 1);       /* set feature register */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("nrf24l01: set feature register failed.\n");                 /* set feature register failed */
       
        return 1;                                                                        /* return error */
    }
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief      get the dynamic payload status
 * @param[in]  *handle points to an nrf24l01 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get dynamic payload failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf24l01_get_dynamic_payload(nrf24l01_handle_t *handle, nrf24l01_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_FEATURE, (uint8_t *)&prev, 1);        /* get feature register */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("nrf24l01: get feature register failed.\n");                 /* get feature register failed */
       
        return 1;                                                                        /* return error */
    }
    *enable = (nrf24l01_bool_t)((prev >> 2) & 0x01);                                     /* get bool */
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief     enable or disable the payload with ack
 * @param[in] *handle points to an nrf24l01 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set payload with ack failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf24l01_set_payload_with_ack(nrf24l01_handle_t *handle, nrf24l01_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_FEATURE, (uint8_t *)&prev, 1);        /* get feature register */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("nrf24l01: get feature register failed.\n");                 /* get feature register failed */
       
        return 1;                                                                        /* return error */
    }
    prev &= ~(1 << 1);                                                                   /* clear config */
    prev |= enable << 1;                                                                 /* set bool */
    res = a_nrf24l01_spi_write(handle, NRF24L01_REG_FEATURE, (uint8_t *)&prev, 1);       /* set feature register */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("nrf24l01: set feature register failed.\n");                 /* set feature register failed */
       
        return 1;                                                                        /* return error */
    }
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief      get the payload with ack status
 * @param[in]  *handle points to an nrf24l01 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get payload with ack failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf24l01_get_payload_with_ack(nrf24l01_handle_t *handle, nrf24l01_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_FEATURE, (uint8_t *)&prev, 1);       /* get feature register */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("nrf24l01: get feature register failed.\n");                /* get feature register failed */
       
        return 1;                                                                       /* return error */
    }
    *enable = (nrf24l01_bool_t)((prev >> 1) & 0x01);                                    /* get bool */
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief     enable or disable the tx payload with no ack
 * @param[in] *handle points to an nrf24l01 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set tx payload with no ack failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf24l01_set_tx_payload_with_no_ack(nrf24l01_handle_t *handle, nrf24l01_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_FEATURE, (uint8_t *)&prev, 1);        /* get feature register */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("nrf24l01: get feature register failed.\n");                 /* get feature register failed */
       
        return 1;                                                                        /* return error */
    }
    prev &= ~(1 << 0);                                                                   /* clear config */
    prev |= enable << 0;                                                                 /* set bool */
    res = a_nrf24l01_spi_write(handle, NRF24L01_REG_FEATURE, (uint8_t *)&prev, 1);       /* set feature register */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("nrf24l01: set feature register failed.\n");                 /* set feature register failed */
       
        return 1;                                                                        /* return error */
    }
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief      get the tx payload with no ack status
 * @param[in]  *handle points to an nrf24l01 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get tx payload with no ack failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf24l01_get_tx_payload_with_no_ack(nrf24l01_handle_t *handle, nrf24l01_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_nrf24l01_spi_read(handle, NRF24L01_REG_FEATURE, (uint8_t *)&prev, 1);        /* get feature register */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("nrf24l01: get feature register failed.\n");                 /* get feature register failed */
       
        return 1;                                                                        /* return error */
    }
    *enable = (nrf24l01_bool_t)((prev >> 0) & 0x01);                                     /* get bool */
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief      read the rx payload
 * @param[in]  *handle points to an nrf24l01 handle structure
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the buffer length
 * @return     status code
 *             - 0 success
 *             - 1 read rx payload failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 len is over 32
 * @note       none
 */
uint8_t nrf24l01_read_rx_payload(nrf24l01_handle_t *handle, uint8_t *buf, uint8_t len)
{
    uint8_t res;
    uint8_t i;
    uint8_t k;
    uint8_t tmp;
    
    if (handle == NULL)                                                     /* check handle */
    {
        return 2;                                                           /* return error */
    }
    if (handle->inited != 1)                                                /* check handle initialization */
    {
        return 3;                                                           /* return error */
    }
    if (len > 32)
    {
        handle->debug_print("nrf24l01: len is over 32.\n");                 /* len is over 32 */
       
        return 4;                                                           /* return error */
    }
    
    res = handle->spi_read(NRF24L01_COMMAND_R_RX_PAYLOAD, buf, len);        /* get rx payload */
    if (res != 0)                                                           /* check result */
    {
        handle->debug_print("nrf24l01: get rx payload failed.\n");          /* get rx payload failed */
       
        return 1;                                                           /* return error */
    }
    k = len / 2;                                                            /* get the half */
    for (i = 0; i < k; i++)                                                 /* run k times */
    {
        tmp = buf[i];                                                       /* copy to tmp */
        buf[i] = buf[len - 1 - i];                                          /* buf[i] = buf[n - 1 - i] */
        buf[len - 1 - i] = tmp;                                             /* set buf[n - 1 - i]*/
    }

    return 0;                                                               /* success return 0 */
}

/**
 * @brief     write the tx payload
 * @param[in] *handle points to an nrf24l01 handle structure
 * @param[in] *buf points to a data buffer
 * @param[in] len is the buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write tx payload failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 len is over 32
 * @note      none
 */
uint8_t nrf24l01_write_tx_payload(nrf24l01_handle_t *handle, uint8_t *buf, uint8_t len)
{
    uint8_t res;
    uint8_t i;
    uint8_t k;
    uint8_t tmp;
    uint8_t buffer[32];
    
    if (handle == NULL)                                                                    /* check handle */
    {
        return 2;                                                                          /* return error */
    }
    if (handle->inited != 1)                                                               /* check handle initialization */
    {
        return 3;                                                                          /* return error */
    }
    if (len > 32)                                                                          /* check len */
    {
        handle->debug_print("nrf24l01: len is over 32.\n");                                /* len is over 32 */
       
        return 4;                                                                          /* return error */
    }
    
    for (i = 0; i < len; i++)                                                              /* copy the data */
    {
        buffer[i] = buf[i];                                                                /* copy */
    }
    k = len / 2;                                                                           /* get the half */
    for (i = 0; i < k; i++)                                                                /* run k times */
    {
        tmp = buffer[i];                                                                   /* copy to tmp */
        buffer[i] = buffer[len - 1 - i];                                                   /* buffer[i] = buffer[n - 1 - i] */
        buffer[len - 1 - i] = tmp;                                                         /* set buffer[n - 1 - i]*/
    }
    res = handle->spi_write(NRF24L01_COMMAND_W_TX_PAYLOAD, (uint8_t *)buffer, len);        /* set tx payload */
    if (res != 0)                                                                          /* check result */
    {
        handle->debug_print("nrf24l01: set tx payload failed.\n");                         /* set tx payload failed */
       
        return 1;                                                                          /* return error */
    }

    return 0;                                                                              /* success return 0 */
}

/**
 * @brief     flush tx
 * @param[in] *handle points to an nrf24l01 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 flush tx failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf24l01_flush_tx(nrf24l01_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                 /* check handle */
    {
        return 2;                                                       /* return error */
    }
    if (handle->inited != 1)                                            /* check handle initialization */
    {
        return 3;                                                       /* return error */
    }
    
    res = handle->spi_write(NRF24L01_COMMAND_FLUSH_TX, NULL, 0);        /* flush tx */
    if (res != 0)                                                       /* check result */
    {
        handle->debug_print("nrf24l01: flush tx failed.\n");            /* flush tx failed */
       
        return 1;                                                       /* return error */
    }

    return 0;                                                           /* success return 0 */
}

/**
 * @brief     flush rx
 * @param[in] *handle points to an nrf24l01 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 flush rx failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf24l01_flush_rx(nrf24l01_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                 /* check handle */
    {
        return 2;                                                       /* return error */
    }
    if (handle->inited != 1)                                            /* check handle initialization */
    {
        return 3;                                                       /* return error */
    }
    
    res = handle->spi_write(NRF24L01_COMMAND_FLUSH_RX, NULL, 0);        /* flush rx */
    if (res != 0)                                                       /* check result */
    {
        handle->debug_print("nrf24l01: flush rx failed.\n");            /* flush rx failed */
       
        return 1;                                                       /* return error */
    }

    return 0;                                                           /* success return 0 */
}

/**
 * @brief     reuse the tx payload
 * @param[in] *handle points to an nrf24l01 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 reuse tx payload failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf24l01_reuse_tx_payload(nrf24l01_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                     /* check handle */
    {
        return 2;                                                           /* return error */
    }
    if (handle->inited != 1)                                                /* check handle initialization */
    {
        return 3;                                                           /* return error */
    }
    
    res = handle->spi_write(NRF24L01_COMMAND_REUSE_TX_PL, NULL, 0);         /* reuse tx payload */
    if (res != 0)                                                           /* check result */
    {
        handle->debug_print("nrf24l01: reuse tx payload failed.\n");        /* reuse tx payload failed */
       
        return 1;                                                           /* return error */
    }

    return 0;                                                               /* success return 0 */
}

/**
 * @brief      get the rx payload width
 * @param[in]  *handle points to an nrf24l01 handle structure
 * @param[out] *width points to a width buffer
 * @return     status code
 *             - 0 success
 *             - 1 get rx payload width failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf24l01_get_rx_payload_width(nrf24l01_handle_t *handle, uint8_t *width)
{
    uint8_t res;
    
    if (handle == NULL)                                                     /* check handle */
    {
        return 2;                                                           /* return error */
    }
    if (handle->inited != 1)                                                /* check handle initialization */
    {
        return 3;                                                           /* return error */
    }
    
    res = handle->spi_read(NRF24L01_COMMAND_R_RX_PL_WID, width, 1);         /* get payload width */
    if (res != 0)                                                           /* check result */
    {
        handle->debug_print("nrf24l01: get payload width failed.\n");       /* get payload width failed */
       
        return 1;                                                           /* return error */
    }

    return 0;                                                               /* success return 0 */
}

/**
 * @brief     write the payload with ack
 * @param[in] *handle points to an nrf24l01 handle structure
 * @param[in] pipe is the pipe number
 * @param[in] *buf points to a data buffer
 * @param[in] len is the buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write payload with ack failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 len is over 32
 * @note      used in rx mode
 */
uint8_t nrf24l01_write_payload_with_ack(nrf24l01_handle_t *handle, nrf24l01_pipe_t pipe, uint8_t *buf, uint8_t len)
{
    uint8_t res;
    uint8_t i;
    uint8_t k;
    uint8_t tmp;
    uint8_t buffer[32];
    
    if (handle == NULL)                                                                            /* check handle */
    {
        return 2;                                                                                  /* return error */
    }
    if (handle->inited != 1)                                                                       /* check handle initialization */
    {
        return 3;                                                                                  /* return error */
    }
    if (len > 32)                                                                                  /* check len */
    {
        handle->debug_print("nrf24l01: len is over 32.\n");                                        /* len is over 32 */
       
        return 4;                                                                                  /* return error */
    }
    
    for (i = 0; i < len; i++)                                                                      /* copy the data */
    {
        buffer[i] = buf[i];                                                                        /* copy */
    }
    k = len / 2;                                                                                   /* get the half */
    for (i = 0; i < k; i++)                                                                        /* run k times */
    {
        tmp = buffer[i];                                                                           /* copy to tmp */
        buffer[i] = buffer[len - 1 - i];                                                           /* buffer[i] = buffer[n - 1 - i] */
        buffer[len - 1 - i] = tmp;                                                                 /* set buffer[n - 1 - i]*/
    }
    res = handle->spi_write((uint8_t)(NRF24L01_COMMAND_W_ACK_PAYLOAD | pipe),
                            (uint8_t *)buffer, len);                                               /* set payload with ack */
    if (res != 0)                                                                                  /* check result */
    {
        handle->debug_print("nrf24l01: set payload with ack failed.\n");                           /* set payload with ack failed */
       
        return 1;                                                                                  /* return error */
    }

    return 0;                                                                                      /* success return 0 */
}

/**
 * @brief     write the payload with no ack
 * @param[in] *handle points to an nrf24l01 handle structure
 * @param[in] *buf points to a data buffer
 * @param[in] len is the buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write payload with no ack failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 len is over 32
 * @note      none
 */
uint8_t nrf24l01_write_payload_with_no_ack(nrf24l01_handle_t *handle, uint8_t *buf, uint8_t len)
{
    uint8_t res;
    uint8_t i;
    uint8_t k;
    uint8_t tmp;
    uint8_t buffer[32];
    
    if (handle == NULL)                                                                            /* check handle */
    {
        return 2;                                                                                  /* return error */
    }
    if (handle->inited != 1)                                                                       /* check handle initialization */
    {
        return 3;                                                                                  /* return error */
    }
    if (len > 32)                                                                                  /* check len */
    {
        handle->debug_print("nrf24l01: len is over 32.\n");                                        /* len is over 32 */
       
        return 4;                                                                                  /* return error */
    }
    
    for (i = 0; i < len; i++)                                                                      /* copy the data */
    {
        buffer[i] = buf[i];                                                                        /* copy */
    }
    k = len / 2;                                                                                   /* get the half */
    for (i = 0; i < k; i++)                                                                        /* run k times */
    {
        tmp = buffer[i];                                                                           /* copy to tmp */
        buffer[i] = buffer[len - 1 - i];                                                           /* buffer[i] = buffer[n - 1 - i] */
        buffer[len - 1 - i] = tmp;                                                                 /* set buffer[n - 1 - i]*/
    }
    
    res = handle->spi_write(NRF24L01_COMMAND_W_TX_PAYLOAD_NO_ACK, (uint8_t *)buffer, len);         /* set payload with no ack */
    if (res != 0)                                                                                  /* check result */
    {
        handle->debug_print("nrf24l01: set payload with no ack failed.\n");                        /* set payload with no ack failed */
       
        return 1;                                                                                  /* return error */
    }

    return 0;                                                                                      /* success return 0 */
}

/**
 * @brief     nop
 * @param[in] *handle points to an nrf24l01 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 nop failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf24l01_nop(nrf24l01_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                            /* check handle */
    {
        return 2;                                                  /* return error */
    }
    if (handle->inited != 1)                                       /* check handle initialization */
    {
        return 3;                                                  /* return error */
    }
    
    res = handle->spi_write(NRF24L01_COMMAND_NOP, NULL, 0);        /* nop */
    if (res != 0)                                                  /* check result */
    {
        handle->debug_print("nrf24l01: nop failed.\n");            /* nop failed */
       
        return 1;                                                  /* return error */
    }

    return 0;                                                      /* success return 0 */
}

/**
 * @brief     set the chip register
 * @param[in] *handle points to an nrf24l01 handle structure
 * @param[in] reg is the register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf24l01_set_reg(nrf24l01_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                       /* check handle */
    {
        return 2;                                             /* return error */
    }
    if (handle->inited != 1)                                  /* check handle initialization */
    {
        return 3;                                             /* return error */
    }
    
    return a_nrf24l01_spi_write(handle, reg, buf, len);       /* write data */
}

/**
 * @brief      get the chip register
 * @param[in]  *handle points to an nrf24l01 handle structure
 * @param[in]  reg is the register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data buffer length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf24l01_get_reg(nrf24l01_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                      /* check handle */
    {
        return 2;                                            /* return error */
    }
    if (handle->inited != 1)                                 /* check handle initialization */
    {
        return 3;                                            /* return error */
    }
    
    return a_nrf24l01_spi_read(handle, reg, buf, len);       /* read data */
}

/**
 * @brief      get chip's information
 * @param[out] *info points to an nrf24l01 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t nrf24l01_info(nrf24l01_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(nrf24l01_info_t));                       /* initialize nrf24l01 info structure */
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
