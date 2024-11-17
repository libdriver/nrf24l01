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

#include "driver_nrf24l01_send_receive_test.h"
#include "driver_nrf24l01_register_test.h"
#include "driver_nrf24l01_basic.h"
#include "gpio.h"
#include <getopt.h>
#include <stdlib.h>

uint8_t (*g_gpio_irq)(void) = NULL;        /**< gpio irq function address */

/**
 * @brief     interface callback
 * @param[in] type is the receive callback type
 * @param[in] num is the pipe number
 * @param[in] *buf points to a data buffer
 * @param[in] len is the buffer length
 * @note      none
 */
static void a_callback(uint8_t type, uint8_t num, uint8_t *buf, uint8_t len)
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

/**
 * @brief     nrf24l01 full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 5 param is invalid
 * @note      none
 */
uint8_t nrf24l01(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"channel", required_argument, NULL, 1},
        {"data", required_argument, NULL, 2},
        {"timeout", required_argument, NULL, 3},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    char data[33] = "LibDriver";
    uint32_t timeout = 1000;
    uint8_t addr0[5] = NRF24L01_BASIC_DEFAULT_RX_ADDR_0;
    uint8_t addr1[5] = NRF24L01_BASIC_DEFAULT_RX_ADDR_1;
    uint8_t addr2[5] = NRF24L01_BASIC_DEFAULT_RX_ADDR_2;
    uint8_t addr3[5] = NRF24L01_BASIC_DEFAULT_RX_ADDR_3;
    uint8_t addr4[5] = NRF24L01_BASIC_DEFAULT_RX_ADDR_4;
    uint8_t addr5[5] = NRF24L01_BASIC_DEFAULT_RX_ADDR_5;
    uint8_t *addr = addr0;
    
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
            
            /* channel */
            case 1 :
            {
                /* copy data */
                
                if (strcmp("0", optarg) == 0)
                {
                    addr = (uint8_t *)addr0;
                }
                else if (strcmp("1", optarg) == 0)
                {
                    addr = (uint8_t *)addr1;
                }
                else if (strcmp("2", optarg) == 0)
                {
                    addr = (uint8_t *)addr2;
                }
                else if (strcmp("3", optarg) == 0)
                {
                    addr = (uint8_t *)addr3;
                }
                else if (strcmp("4", optarg) == 0)
                {
                    addr = (uint8_t *)addr4;
                }
                else if (strcmp("5", optarg) == 0)
                {
                    addr = (uint8_t *)addr5;
                }
                else
                {
                    return 5;
                }
                
                break;
            }
            
            /* data */
            case 2 :
            {
                /* copy data */
                memset(data, 0, sizeof(char) * 33);
                strncpy(data, optarg, 32);
                
                break;
            }
            
            /* timeout */
            case 3 :
            {
                /* set the timeout */
                timeout = atol(optarg);
                
                break;
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
        uint8_t res;
        
        /* run reg test */
        res = nrf24l01_register_test();
        if (res != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("t_send", type) == 0)
    {
        uint8_t res;
        
        /* gpio init */
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }
        
        /* set gpio irq */
        g_gpio_irq = nrf24l01_interrupt_test_irq_handler;
        
        /* run send test */
        res = nrf24l01_send_test();
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }
        
        /* gpio deinit */
        (void)gpio_interrupt_deinit();
        g_gpio_irq = NULL;
        
        return 0;
    }
    else if (strcmp("t_receive", type) == 0)
    {
        uint8_t res;
        
        /* gpio init */
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }
        
        /* set gpio irq */
        g_gpio_irq = nrf24l01_interrupt_test_irq_handler;
        
        /* run receive test */
        res = nrf24l01_receive_test();
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }
        
        /* gpio deinit */
        (void)gpio_interrupt_deinit();
        g_gpio_irq = NULL;
        
        return 0;
    }
    else if (strcmp("e_send", type) == 0)
    {
        uint8_t res;
        
        /* gpio init */
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }
        
        /* set gpio irq */
        g_gpio_irq = nrf24l01_interrupt_irq_handler;
        
        /* basic init */
        res = nrf24l01_basic_init(NRF24L01_TYPE_TX, a_callback);
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }
        
        /* output */
        nrf24l01_interface_debug_print("nrf24l01: send %s.\n", data);
        
        /* basic send */
        if (nrf24l01_basic_send((uint8_t *)addr, (uint8_t *)data, (uint8_t)strlen(data)) != 0)
        {
            (void)nrf24l01_basic_deinit();
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
        }
        
        /* basic deinit */
        if (nrf24l01_basic_deinit() != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
        }
        
        /* deinit */
        (void)gpio_interrupt_deinit();
        g_gpio_irq = NULL;
        
        return 0;
    }
    else if (strcmp("e_receive", type) == 0)
    {
        uint8_t res;
        
        /* output */
        nrf24l01_interface_debug_print("nrf24l01: receiving with timeout %d ms.\n", timeout);
        
        /* gpio init */
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }
        
        /* set gpio irq */
        g_gpio_irq = nrf24l01_interrupt_irq_handler;
        
        /* basic init */
        res = nrf24l01_basic_init(NRF24L01_TYPE_RX, a_callback);
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }
        
        /* delay timeout */
        nrf24l01_interface_delay_ms(timeout);
        
        /* basic deinit */
        if (nrf24l01_basic_deinit() != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
        }
        
        /* gpio deinit */
        (void)gpio_interrupt_deinit();
        g_gpio_irq = NULL;
        
        /* output */
        nrf24l01_interface_debug_print("nrf24l01: finish receiving.\n");
        
        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        nrf24l01_interface_debug_print("Usage:\n");
        nrf24l01_interface_debug_print("  nrf24l01 (-i | --information)\n");
        nrf24l01_interface_debug_print("  nrf24l01 (-h | --help)\n");
        nrf24l01_interface_debug_print("  nrf24l01 (-p | --port)\n");
        nrf24l01_interface_debug_print("  nrf24l01 (-t reg | --test=reg)\n");
        nrf24l01_interface_debug_print("  nrf24l01 (-t send | --test=send)\n");
        nrf24l01_interface_debug_print("  nrf24l01 (-t receive | --test=receive)\n");
        nrf24l01_interface_debug_print("  nrf24l01 (-e send | --example=send) [--channel=<0 | 1 | 2 | 3 | 4 | 5>] [--data=<str>]\n");
        nrf24l01_interface_debug_print("  nrf24l01 (-e receive | --example=receive) [--timeout=<ms>]\n");
        nrf24l01_interface_debug_print("\n");
        nrf24l01_interface_debug_print("Options:\n");
        nrf24l01_interface_debug_print("      --channel=<0 | 1 | 2 | 3 | 4 | 5>\n");
        nrf24l01_interface_debug_print("                        Set the send channel.([default: 0])\n");
        nrf24l01_interface_debug_print("      --data=<str>      Set the send data and it's length must be less 32.([default: LibDriver])\n");
        nrf24l01_interface_debug_print("  -e <send | receive>, --example=<send | receive>\n");
        nrf24l01_interface_debug_print("                        Run the driver example.\n");
        nrf24l01_interface_debug_print("  -h, --help            Show the help.\n");
        nrf24l01_interface_debug_print("  -i, --information     Show the chip information.\n");
        nrf24l01_interface_debug_print("  -p, --port            Display the pin connections of the current board.\n");
        nrf24l01_interface_debug_print("  -t <reg | send | receive>, --test=<reg | send | receive>\n");
        nrf24l01_interface_debug_print("                        Run the driver test.\n");
        nrf24l01_interface_debug_print("      --timeout=<ms>    Set the receive timeout in ms.([default: 5000])\n");

        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        nrf24l01_info_t info;
        
        /* print nrf24l01 info */
        nrf24l01_info(&info);
        nrf24l01_interface_debug_print("nrf24l01: chip is %s.\n", info.chip_name);
        nrf24l01_interface_debug_print("nrf24l01: manufacturer is %s.\n", info.manufacturer_name);
        nrf24l01_interface_debug_print("nrf24l01: interface is %s.\n", info.interface);
        nrf24l01_interface_debug_print("nrf24l01: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        nrf24l01_interface_debug_print("nrf24l01: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        nrf24l01_interface_debug_print("nrf24l01: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        nrf24l01_interface_debug_print("nrf24l01: max current is %0.2fmA.\n", info.max_current_ma);
        nrf24l01_interface_debug_print("nrf24l01: max temperature is %0.1fC.\n", info.temperature_max);
        nrf24l01_interface_debug_print("nrf24l01: min temperature is %0.1fC.\n", info.temperature_min);
        
        return 0;
    }
    else if (strcmp("p", type) == 0)
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
        nrf24l01_interface_debug_print("nrf24l01: unknown status code.\n");
    }

    return 0;
}
