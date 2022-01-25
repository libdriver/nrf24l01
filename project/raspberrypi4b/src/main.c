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
 * @date      2021-02-22
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2021/02/22  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_nrf24l01_sent_receive_test.h"
#include "driver_nrf24l01_register_test.h"
#include "driver_nrf24l01_basic.h"
#include "gpio.h"
#include <stdlib.h>

uint8_t g_flag;                            /**< interrupt flag */
uint8_t (*g_gpio_irq)(void) = NULL;        /**< gpio irq function address */

/**
 * @brief     interface callback
 * @param[in] type is the receive callback type
 * @param[in] num is the pipe number
 * @param[in] *buf points to a data buffer
 * @param[in] len is the buffer length
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 * @note      none
 */
static uint8_t _callback(uint8_t type, uint8_t num, uint8_t *buf, uint8_t len)
{
    switch (type)
    {
        case NRF24L01_INTERRUPT_RX_DR :
        {
            volatile uint8_t i;
            
            nrf24l01_interface_debug_print("nrf24l01: irq receive with pipe %d with %d.\n", num, len);
            for (i = 0; i < len; i++)
            {
                nrf24l01_interface_debug_print("0x%02X ", buf[i]);
            }
            nrf24l01_interface_debug_print(".\n");
            
            return 0;
        }
        case NRF24L01_INTERRUPT_TX_DS :
        {
            nrf24l01_interface_debug_print("nrf24l01: irq sent ok.\n");
            
            return 0;
        }
        case NRF24L01_INTERRUPT_MAX_RT :
        {
            nrf24l01_interface_debug_print("nrf24l01: irq reach max retry times.\n");
            
            return 0;
        }
        case NRF24L01_INTERRUPT_TX_FULL :
        {
            return 0;
        }
        default :
        {
            return 1;
        }
    }
}

/**
 * @brief     nrf24l01 full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *             - 0 success
 *             - 1 run failed
 *             - 5 param is invalid
 * @note      none
 */
uint8_t nrf24l01(uint8_t argc, char **argv)
{
    if (argc == 1)
    {
        goto help;
    }
    else if (argc == 2)
    {
        if (strcmp("-i", argv[1]) == 0)
        {
            nrf24l01_info_t info;
            
            /* print nrf24l01 info */
            nrf24l01_info(&info);
            nrf24l01_interface_debug_print("nrf24l01: chip is %s.\n", info.chip_name);
            nrf24l01_interface_debug_print("nrf24l01: manufacturer is %s.\n", info.manufacturer_name);
            nrf24l01_interface_debug_print("nrf24l01: interface is %s.\n", info.interface);
            nrf24l01_interface_debug_print("nrf24l01: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000)/100);
            nrf24l01_interface_debug_print("nrf24l01: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
            nrf24l01_interface_debug_print("nrf24l01: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
            nrf24l01_interface_debug_print("nrf24l01: max current is %0.2fmA.\n", info.max_current_ma);
            nrf24l01_interface_debug_print("nrf24l01: max temperature is %0.1fC.\n", info.temperature_max);
            nrf24l01_interface_debug_print("nrf24l01: min temperature is %0.1fC.\n", info.temperature_min);
            
            return 0;
        }
        else if (strcmp("-p", argv[1]) == 0)
        {
            /* print pin connection */
            nrf24l01_interface_debug_print("nrf24l01: SCK connected to GPIO11(BCM).\n");
            nrf24l01_interface_debug_print("nrf24l01: MISO connected to GPIO9(BCM).\n");
            nrf24l01_interface_debug_print("nrf24l01: MOSI connected to GPIO10(BCM).\n");
            nrf24l01_interface_debug_print("nrf24l01: CS connected to GPIO8(BCM).\n");
            nrf24l01_interface_debug_print("nrf24l01: CE connected to GPIO27(BCM.\n");
            nrf24l01_interface_debug_print("nrf24l01: INT connected to GPIO17(BCM).\n");
            
            return 0;
        }
        else if (strcmp("-h", argv[1]) == 0)
        {
            /* show nrf24l01 help */
            
            help:
            nrf24l01_interface_debug_print("nrf24l01 -i\n\tshow nrf24l01 chip and driver information.\n");
            nrf24l01_interface_debug_print("nrf24l01 -h\n\tshow nrf24l01 help.\n");
            nrf24l01_interface_debug_print("nrf24l01 -p\n\tshow nrf24l01 pin connections of the current board.\n");
            nrf24l01_interface_debug_print("nrf24l01 -t reg\n\trun nrf24l01 register test.\n");
            nrf24l01_interface_debug_print("nrf24l01 -t sent\n\trun nrf24l01 sent test.\n");
            nrf24l01_interface_debug_print("nrf24l01 -t receive\n\trun nrf24l01 receive test.\n");
            nrf24l01_interface_debug_print("nrf24l01 -c sent <channel> <data>\n\trun nrf24l01 sent function."
                                           "channel is the rf channel and it can be \"0\" - \"5\".data is the send data and it's length must be less 32.\n");
            nrf24l01_interface_debug_print("nrf24l01 -c receive <timeout>\n\trun nrf24l01 receive function.timeout is the timeout time.\n");
            
            return 0;
        }
        else
        {
            return 5;
        }
    }
    else if (argc == 3)
    {
        /* run test */
        if (strcmp("-t", argv[1]) == 0)
        {
            /* reg test */
            if (strcmp("reg", argv[2]) == 0)
            {
                volatile uint8_t res;
                
                res = nrf24l01_register_test();
                if (res)
                {
                    return 1;
                }
                
                return 0;
            }
            
            /* sent test */
            else if (strcmp("sent", argv[2]) == 0)
            {
                volatile uint8_t res;
                
                res = gpio_interrupt_init();
                if (res)
                {
                    return 1;
                }
                g_gpio_irq = nrf24l01_interrupt_test_irq_handler;
                res = nrf24l01_sent_test();
                if (res)
                {
                    gpio_interrupt_deinit();
                    g_gpio_irq = NULL;
                    
                    return 1;
                }
                gpio_interrupt_deinit();
                g_gpio_irq = NULL;
                
                return 0;
            }
            
            /* receive test */
            else if (strcmp("receive", argv[2]) == 0)
            {
                volatile uint8_t res;
                
                res = gpio_interrupt_init();
                if (res)
                {
                    return 1;
                }
                g_gpio_irq = nrf24l01_interrupt_test_irq_handler;
                res = nrf24l01_receive_test();
                if (res)
                {
                    gpio_interrupt_deinit();
                    g_gpio_irq = NULL;
                    
                    return 1;
                }
                gpio_interrupt_deinit();
                g_gpio_irq = NULL;
                
                return 0;
            }
            
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* param is invalid */
        else
        {
            return 5;
        }
    }
    else if (argc == 4)
    {
        /* run the function */
        if (strcmp("-c", argv[1]) == 0)
        {
            if (strcmp("receive", argv[2]) == 0)
            {
                volatile uint8_t res;
                volatile uint32_t timeout;
                
                timeout = atoi(argv[3]);
                nrf24l01_interface_debug_print("nrf24l01: receiving with timeout %d ms.\n", timeout);
                res = gpio_interrupt_init();
                if (res)
                {
                    return 1;
                }
                g_gpio_irq = nrf24l01_interrupt_irq_handler;
                res = nrf24l01_basic_init(NRF24L01_TYPE_RX, _callback);
                if (res)
                {
                    gpio_interrupt_deinit();
                    g_gpio_irq = NULL;
                    
                    return 1;
                }
                nrf24l01_interface_delay_ms(timeout);
                if (nrf24l01_basic_deinit())
                {
                    gpio_interrupt_deinit();
                    g_gpio_irq = NULL;
                }
                
                gpio_interrupt_deinit();
                g_gpio_irq = NULL;
                
                nrf24l01_interface_debug_print("nrf24l01: finish receiving.\n");
                
                return 0;
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* param is invalid */
        else
        {
            return 5;
        }
    }
    else if (argc == 5)
    {
        /* run the function */
        if (strcmp("-c", argv[1]) == 0)
        {
            if (strcmp("sent", argv[2]) == 0)
            {
                volatile uint8_t res;
                volatile uint8_t ch;
                volatile uint8_t *addr;
                const uint8_t addr0[5] = NRF24L01_BASIC_DEFAULT_RX_ADDR_0;
                const uint8_t addr1[5] = NRF24L01_BASIC_DEFAULT_RX_ADDR_1;
                const uint8_t addr2[5] = NRF24L01_BASIC_DEFAULT_RX_ADDR_2;
                const uint8_t addr3[5] = NRF24L01_BASIC_DEFAULT_RX_ADDR_3;
                const uint8_t addr4[5] = NRF24L01_BASIC_DEFAULT_RX_ADDR_4;
                const uint8_t addr5[5] = NRF24L01_BASIC_DEFAULT_RX_ADDR_5;
                
                ch = atoi(argv[3]);
                if (ch == 0)
                {
                    addr = (uint8_t *)addr0;
                }
                else if (ch == 1)
                {
                    addr = (uint8_t *)addr1;
                }
                else if (ch == 2)
                {
                    addr = (uint8_t *)addr2;
                }
                else if (ch == 3)
                {
                    addr = (uint8_t *)addr3;
                }
                else if (ch == 4)
                {
                    addr = (uint8_t *)addr4;
                }
                else if (ch == 5)
                {
                    addr = (uint8_t *)addr5;
                }
                else
                {
                    return 5;
                }
                res = gpio_interrupt_init();
                if (res)
                {
                    return 1;
                }
                g_gpio_irq = nrf24l01_interrupt_irq_handler;
                res = nrf24l01_basic_init(NRF24L01_TYPE_TX, _callback);
                if (res)
                {
                    gpio_interrupt_deinit();
                    g_gpio_irq = NULL;
                    
                    return 1;
                }
                nrf24l01_interface_debug_print("nrf24l01: sent %s.\n", argv[4]);
                if (nrf24l01_basic_sent((uint8_t *)addr, (uint8_t *)argv[4], strlen(argv[4])))
                {
                    nrf24l01_basic_deinit();
                    gpio_interrupt_deinit();
                    g_gpio_irq = NULL;
                }
                if (nrf24l01_basic_deinit())
                {
                    gpio_interrupt_deinit();
                    g_gpio_irq = NULL;
                }
                
                gpio_interrupt_deinit();
                g_gpio_irq = NULL;
                
                return 0;
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* param is invalid */
        else
        {
            return 5;
        }
    }
    /* param is invalid */
    else
    {
        return 5;
    }
}

/**
 * @brief     main function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *             - 0 success
 * @note      none
 */
int main(uint8_t argc, char **argv)
{
    uint8_t res;

    res = nrf24l01(argc, argv);
    if (res == 0)
    {
        /* run success */
    }
    else if (res == 1)
    {
        nrf24l01_interface_debug_print("nrf24l01: run failed.\n");
    }
    else if (res == 5)
    {
        nrf24l01_interface_debug_print("nrf24l01: param is invalid.\n");
    }
    else
    {
        nrf24l01_interface_debug_print("nrf24l01: unknow status code.\n");
    }

    return 0;
}
