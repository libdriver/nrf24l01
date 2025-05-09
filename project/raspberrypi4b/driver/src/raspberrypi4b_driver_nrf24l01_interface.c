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
 * @file      raspberrypi4b_driver_nrf24l01_interface.c
 * @brief     raspberrypi4b driver nrf24l01 interface source file
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

#include "driver_nrf24l01_interface.h"
#include "spi.h"
#include "gpio.h"
#include "wire.h"
#include <stdarg.h>

/**
 * @brief spi device name definition
 */
#define SPI_DEVICE_NAME "/dev/spidev0.0"    /**< spi device name */

/**
 * @brief spi device handle definition
 */
static int gs_spi_fd;                       /**< spi handle */

/**
 * @brief  interface spi bus init
 * @return status code
 *         - 0 success
 *         - 1 spi init failed
 * @note   none
 */
uint8_t nrf24l01_interface_spi_init(void)
{
    return spi_init(SPI_DEVICE_NAME, &gs_spi_fd, SPI_MODE_TYPE_0, 1000 * 1000);
}

/**
 * @brief  interface spi bus deinit
 * @return status code
 *         - 0 success
 *         - 1 spi deinit failed
 * @note   none
 */
uint8_t nrf24l01_interface_spi_deinit(void)
{   
    return spi_deinit(gs_spi_fd);
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
uint8_t nrf24l01_interface_spi_read(uint8_t reg, uint8_t *buf, uint16_t len)
{
    return spi_read(gs_spi_fd, reg, buf, len);
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
uint8_t nrf24l01_interface_spi_write(uint8_t reg, uint8_t *buf, uint16_t len)
{
    return spi_write(gs_spi_fd, reg, buf, len);
}

/**
 * @brief  interface gpio init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
uint8_t nrf24l01_interface_gpio_init(void)
{
    return wire_clock_init();
}

/**
 * @brief  interface gpio deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t nrf24l01_interface_gpio_deinit(void)
{
    return wire_clock_deinit();
}

/**
 * @brief     interface gpio write
 * @param[in] data written data
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t nrf24l01_interface_gpio_write(uint8_t data)
{
    return wire_clock_write(data);
}

/**
 * @brief     interface delay ms
 * @param[in] ms time
 * @note      none
 */
void nrf24l01_interface_delay_ms(uint32_t ms)
{
    usleep(1000 * ms);
}

/**
 * @brief     interface print format data
 * @param[in] fmt format data
 * @note      none
 */
void nrf24l01_interface_debug_print(const char *const fmt, ...)
{
    char str[256];
    uint16_t len;
    va_list args;
    
    memset((char *)str, 0, sizeof(char) * 256); 
    va_start(args, fmt);
    vsnprintf((char *)str, 255, (char const *)fmt, args);
    va_end(args);
    
    len = strlen((char *)str);
    (void)printf((uint8_t *)str, len);
}

/**
 * @brief     interface receive callback
 * @param[in] type receive callback type
 * @param[in] num pipe number
 * @param[in] *buf pointer to a data buffer
 * @param[in] len buffer length
 * @note      none
 */
void nrf24l01_interface_receive_callback(uint8_t type, uint8_t num, uint8_t *buf, uint8_t len)
{
    switch (type)
    {
        case NRF24L01_INTERRUPT_RX_DR :
        {
            uint8_t i;
            
            nrf24l01_interface_debug_print("nrf24l01: irq receive with pipe %d with %d.\n", num, len);
            for (i = 0; i < len; i++)
            {
                nrf24l01_interface_debug_print("0x%02X ", buf[i]);
            }
            nrf24l01_interface_debug_print(".\n");
            
            break;
        }
        case NRF24L01_INTERRUPT_TX_DS :
        {
            nrf24l01_interface_debug_print("nrf24l01: irq send ok.\n");
            
            break;
        }
        case NRF24L01_INTERRUPT_MAX_RT :
        {
            nrf24l01_interface_debug_print("nrf24l01: irq reach max retry times.\n");
            
            break;
        }
        case NRF24L01_INTERRUPT_TX_FULL :
        {
            nrf24l01_interface_debug_print("nrf24l01: irq tx full.\n");
            
            break;
        }
        default :
        {
            nrf24l01_interface_debug_print("nrf24l01: unknown code.\n");
            
            break;
        }
    }
}
