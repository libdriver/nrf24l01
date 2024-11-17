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
 * @file      driver_nrf24l01_basic.h
 * @brief     driver nrf24l01 basic header file
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

#ifndef DRIVER_NRF24L01_BASIC_H
#define DRIVER_NRF24L01_BASIC_H

#include "driver_nrf24l01_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup nrf24l01_example_driver nrf24l01 example driver function
 * @brief    nrf24l01 example driver modules
 * @ingroup  nrf24l01_driver
 * @{
 */

/**
 * @brief nrf24l01 basic example default definition
 */
#define NRF24L01_BASIC_DEFAULT_CRCO                              NRF24L01_BOOL_TRUE                    /**< enable 2 bytes crc */
#define NRF24L01_BASIC_DEFAULT_ENABLE_CRC                        NRF24L01_BOOL_TRUE                    /**< enable crc */
#define NRF24L01_BASIC_DEFAULT_PIPE_0_AUTO_ACKNOWLEDGMENT        NRF24L01_BOOL_TRUE                    /**< enable pipe 0 auto acknowledgment */
#define NRF24L01_BASIC_DEFAULT_PIPE_1_AUTO_ACKNOWLEDGMENT        NRF24L01_BOOL_TRUE                    /**< enable pipe 1 auto acknowledgment */
#define NRF24L01_BASIC_DEFAULT_PIPE_2_AUTO_ACKNOWLEDGMENT        NRF24L01_BOOL_TRUE                    /**< enable pipe 2 auto acknowledgment */
#define NRF24L01_BASIC_DEFAULT_PIPE_3_AUTO_ACKNOWLEDGMENT        NRF24L01_BOOL_TRUE                    /**< enable pipe 3 auto acknowledgment */
#define NRF24L01_BASIC_DEFAULT_PIPE_4_AUTO_ACKNOWLEDGMENT        NRF24L01_BOOL_TRUE                    /**< enable pipe 4 auto acknowledgment */
#define NRF24L01_BASIC_DEFAULT_PIPE_5_AUTO_ACKNOWLEDGMENT        NRF24L01_BOOL_TRUE                    /**< enable pipe 5 auto acknowledgment */
#define NRF24L01_BASIC_DEFAULT_RX_PIPE_0                         NRF24L01_BOOL_TRUE                    /**< enable rx pipe 0 */
#define NRF24L01_BASIC_DEFAULT_RX_PIPE_1                         NRF24L01_BOOL_TRUE                    /**< enable rx pipe 1 */
#define NRF24L01_BASIC_DEFAULT_RX_PIPE_2                         NRF24L01_BOOL_TRUE                    /**< enable rx pipe 2 */
#define NRF24L01_BASIC_DEFAULT_RX_PIPE_3                         NRF24L01_BOOL_TRUE                    /**< enable rx pipe 3 */
#define NRF24L01_BASIC_DEFAULT_RX_PIPE_4                         NRF24L01_BOOL_TRUE                    /**< enable rx pipe 4 */
#define NRF24L01_BASIC_DEFAULT_RX_PIPE_5                         NRF24L01_BOOL_TRUE                    /**< enable rx pipe 5 */
#define NRF24L01_BASIC_DEFAULT_ADDRESS_WIDTH                     NRF24L01_ADDRESS_WIDTH_5_BYTES        /**< set 5 bytes width */
#define NRF24L01_BASIC_DEFAULT_RETRANSMIT_DELAY                  750                                   /**< 750 us */
#define NRF24L01_BASIC_DEFAULT_RETRANSMIT_COUNT                  10                                    /**< 10 times */
#define NRF24L01_BASIC_DEFAULT_CHANNEL_FREQUENCY                 20                                    /**< 20 MHz */
#define NRF24L01_BASIC_DEFAULT_DATA_RATE                         NRF24L01_DATA_RATE_2M                 /**< 2M */
#define NRF24L01_BASIC_DEFAULT_OUTPUT_POWER                      NRF24L01_OUTPUT_POWER_0_DBM           /**< 0dBm */
#define NRF24L01_BASIC_DEFAULT_PIPE_0_PAYLOAD                    32                                    /**< 32 bytes */
#define NRF24L01_BASIC_DEFAULT_PIPE_1_PAYLOAD                    32                                    /**< 32 bytes */
#define NRF24L01_BASIC_DEFAULT_PIPE_2_PAYLOAD                    32                                    /**< 32 bytes */
#define NRF24L01_BASIC_DEFAULT_PIPE_3_PAYLOAD                    32                                    /**< 32 bytes */
#define NRF24L01_BASIC_DEFAULT_PIPE_4_PAYLOAD                    32                                    /**< 32 bytes */
#define NRF24L01_BASIC_DEFAULT_PIPE_5_PAYLOAD                    32                                    /**< 32 bytes */
#define NRF24L01_BASIC_DEFAULT_PIPE_0_DYNAMIC_PAYLOAD            NRF24L01_BOOL_TRUE                    /**< enable pipe 0 dynamic payload */
#define NRF24L01_BASIC_DEFAULT_PIPE_1_DYNAMIC_PAYLOAD            NRF24L01_BOOL_TRUE                    /**< enable pipe 1 dynamic payload */
#define NRF24L01_BASIC_DEFAULT_PIPE_2_DYNAMIC_PAYLOAD            NRF24L01_BOOL_TRUE                    /**< enable pipe 2 dynamic payload */
#define NRF24L01_BASIC_DEFAULT_PIPE_3_DYNAMIC_PAYLOAD            NRF24L01_BOOL_TRUE                    /**< enable pipe 3 dynamic payload */
#define NRF24L01_BASIC_DEFAULT_PIPE_4_DYNAMIC_PAYLOAD            NRF24L01_BOOL_TRUE                    /**< enable pipe 4 dynamic payload */
#define NRF24L01_BASIC_DEFAULT_PIPE_5_DYNAMIC_PAYLOAD            NRF24L01_BOOL_TRUE                    /**< enable pipe 5 dynamic payload */
#define NRF24L01_BASIC_DEFAULT_DYNAMIC_PAYLOAD                   NRF24L01_BOOL_TRUE                    /**< enable dynamic payload */
#define NRF24L01_BASIC_DEFAULT_PAYLOAD_WITH_ACK                  NRF24L01_BOOL_FALSE                   /**< disable payload with ack */
#define NRF24L01_BASIC_DEFAULT_TX_PAYLOAD_WITH_NO_ACK            NRF24L01_BOOL_FALSE                   /**< disable tx payload with no ack */
#define NRF24L01_BASIC_DEFAULT_RX_ADDR_0                         {0x1A, 0x01, 0x02, 0x03, 0x00}        /**< rx address 0 */
#define NRF24L01_BASIC_DEFAULT_RX_ADDR_1                         {0x1B, 0x01, 0x02, 0x03, 0x01}        /**< rx address 1 */
#define NRF24L01_BASIC_DEFAULT_RX_ADDR_2                         {0x1B, 0x01, 0x02, 0x03, 0x02}        /**< rx address 2 */
#define NRF24L01_BASIC_DEFAULT_RX_ADDR_3                         {0x1B, 0x01, 0x02, 0x03, 0x03}        /**< rx address 3 */
#define NRF24L01_BASIC_DEFAULT_RX_ADDR_4                         {0x1B, 0x01, 0x02, 0x03, 0x04}        /**< rx address 4 */
#define NRF24L01_BASIC_DEFAULT_RX_ADDR_5                         {0x1B, 0x01, 0x02, 0x03, 0x05}        /**< rx address 5 */

/**
 * @brief nrf24l01 type enumeration definition
 */
typedef enum
{
    NRF24L01_TYPE_TX = 0x00,        /**< tx mode */
    NRF24L01_TYPE_RX = 0x01,        /**< rx mode */
} nrf24l01_type_t;

/**
 * @brief  nrf24l01 irq
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t nrf24l01_interrupt_irq_handler(void);

/**
 * @brief     basic example init
 * @param[in] type is the chip working mode
 * @param[in] *callback points to a callback function
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t nrf24l01_basic_init(nrf24l01_type_t type, void (*callback)(uint8_t type, uint8_t num, uint8_t *buf, uint8_t len));

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t nrf24l01_basic_deinit(void);

/**
 * @brief     basic example send
 * @param[in] *addr points to a address buffer
 * @param[in] *buf points to a data buffer
 * @param[in] len is the buffer length
 * @return    status code
 *            - 0 success
 *            - 1 send failed
 * @note      none
 */
uint8_t nrf24l01_basic_send(uint8_t *addr, uint8_t *buf, uint8_t len);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
