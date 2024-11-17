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
 * @file      driver_nrf24l01.h
 * @brief     driver nrf24l01 header file
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

#ifndef DRIVER_NRF24L01_H
#define DRIVER_NRF24L01_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup nrf24l01_driver nrf24l01 driver function
 * @brief    nrf24l01 driver modules
 * @{
 */

/**
 * @addtogroup nrf24l01_basic_driver
 * @{
 */

/**
 * @brief nrf24l01 bool enumeration definition
 */
typedef enum
{
    NRF24L01_BOOL_FALSE = 0x00,        /**< disable function */
    NRF24L01_BOOL_TRUE  = 0x01,        /**< enable function */
} nrf24l01_bool_t;

/**
 * @brief nrf24l01 config enumeration definition
 */
typedef enum
{
    NRF24L01_CONFIG_MASK_RX_DR  = 6,        /**< mask interrupt caused by RX_DR */
    NRF24L01_CONFIG_MASK_TX_DS  = 5,        /**< mask interrupt caused by TX_DS */
    NRF24L01_CONFIG_MASK_MAX_RT = 4,        /**< mask interrupt caused by MAX_RT */
    NRF24L01_CONFIG_EN_CRC      = 3,        /**< enable CRC */
    NRF24L01_CONFIG_CRCO        = 2,        /**< enable CRC encoding 2 bytes or 1 byte */
    NRF24L01_CONFIG_PWR_UP      = 1,        /**< power up */
} nrf24l01_config_t;

/**
 * @brief nrf24l01 mode enumeration definition
 */
typedef enum
{
    NRF24L01_MODE_TX = 0x00,        /**< tx mode */
    NRF24L01_MODE_RX = 0x01,        /**< rx mode */
} nrf24l01_mode_t;

/**
 * @brief nrf24l01 pipe enumeration definition
 */
typedef enum
{
    NRF24L01_PIPE_0 = 0,        /**< pipe 0 */
    NRF24L01_PIPE_1 = 1,        /**< pipe 1 */
    NRF24L01_PIPE_2 = 2,        /**< pipe 2 */
    NRF24L01_PIPE_3 = 3,        /**< pipe 3 */
    NRF24L01_PIPE_4 = 4,        /**< pipe 4 */
    NRF24L01_PIPE_5 = 5,        /**< pipe 5 */
} nrf24l01_pipe_t;

/**
 * @brief nrf24l01 address width enumeration definition
 */
typedef enum
{
    NRF24L01_ADDRESS_WIDTH_ILLEGAL = 0,       /**< illegal */
    NRF24L01_ADDRESS_WIDTH_3_BYTES = 1,       /**< 3 bytes */
    NRF24L01_ADDRESS_WIDTH_4_BYTES = 2,       /**< 4 bytes */
    NRF24L01_ADDRESS_WIDTH_5_BYTES = 3,       /**< 5 bytes */
} nrf24l01_address_width_t;

/**
 * @brief nrf24l01 data rate enumeration definition
 */
typedef enum
{
    NRF24L01_DATA_RATE_1M   = 0,        /**< 1 Mbps */
    NRF24L01_DATA_RATE_2M   = 1,        /**< 2 Mbps */
    NRF24L01_DATA_RATE_250K = 2,        /**< 250 Kbps */
} nrf24l01_data_rate_t;

/**
 * @brief nrf24l01 output power enumeration definition
 */
typedef enum
{
    NRF24L01_OUTPUT_POWER_NEGATIVE_18_DBM = 0,        /**< -18 dBm */
    NRF24L01_OUTPUT_POWER_NEGATIVE_12_DBM = 1,        /**< -12 dBm */
    NRF24L01_OUTPUT_POWER_NEGATIVE_6_DBM  = 2,        /**< -6 dBm */
    NRF24L01_OUTPUT_POWER_0_DBM          = 3,         /**< 0 dBm */
} nrf24l01_output_power_t;

/**
 * @brief nrf24l01 interrupt enumeration definition
 */
typedef enum
{
    NRF24L01_INTERRUPT_RX_DR   = 6,        /**< data ready rx fifo interrupt */
    NRF24L01_INTERRUPT_TX_DS   = 5,        /**< data send rx fifo interrupt */
    NRF24L01_INTERRUPT_MAX_RT  = 4,        /**< maximum number of tx retransmits interrupt */
    NRF24L01_INTERRUPT_TX_FULL = 0,        /**< tx fifo full flag */
} nrf24l01_interrupt_t;

/**
 * @brief nrf24l01 fifo status enumeration definition
 */
typedef enum
{
    NRF24L01_FIFO_STATUS_TX_REUSE = 6,        /**< tx reuse */
    NRF24L01_FIFO_STATUS_TX_FULL  = 5,        /**< tx full */
    NRF24L01_FIFO_STATUS_TX_EMPTY = 4,        /**< tx empty */
    NRF24L01_FIFO_STATUS_RX_FULL  = 1,        /**< rx full */
    NRF24L01_FIFO_STATUS_RX_EMPTY = 0,        /**< rx empty */
} nrf24l01_fifo_status_t;

/**
 * @brief nrf24l01 handle structure definition
 */
typedef struct nrf24l01_handle_s
{
    uint8_t (*gpio_init)(void);                                                            /**< point to a gpio_init function address */
    uint8_t (*gpio_deinit)(void);                                                          /**< point to a gpio_deinit function address */
    uint8_t (*gpio_write)(uint8_t value);                                                  /**< point to a gpio_write function address */
    uint8_t (*spi_init)(void);                                                             /**< point to a spi_init function address */
    uint8_t (*spi_deinit)(void);                                                           /**< point to a spi_deinit function address */
    uint8_t (*spi_read)(uint8_t reg, uint8_t *buf, uint16_t len);                          /**< point to a spi_read function address */
    uint8_t (*spi_write)(uint8_t reg, uint8_t *buf, uint16_t len);                         /**< point to a spi_write function address */
    void (*delay_ms)(uint32_t ms);                                                         /**< point to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);                                       /**< point to a debug_print function address */
    void (*receive_callback)(uint8_t type, uint8_t num, uint8_t *buf, uint8_t len);        /**< point to a receive_callback function address */
    uint8_t inited;                                                                        /**< inited flag */
    uint8_t finished;                                                                      /**< finished flag */
} nrf24l01_handle_t;

/**
 * @brief nrf24l01 information structure definition
 */
typedef struct nrf24l01_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} nrf24l01_info_t;

/**
 * @}
 */

/**
 * @defgroup nrf24l01_link_driver nrf24l01 link driver function
 * @brief    nrf24l01 link driver modules
 * @ingroup  nrf24l01_driver
 * @{
 */

/**
 * @brief     initialize nrf24l01_handle_t structure
 * @param[in] HANDLE points to an nrf24l01 handle structure
 * @param[in] STRUCTURE is nrf24l01_handle_t
 * @note      none
 */
#define DRIVER_NRF24L01_LINK_INIT(HANDLE, STRUCTURE)         memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link spi_init function
 * @param[in] HANDLE points to an nrf24l01 handle structure
 * @param[in] FUC points to a spi_init function address
 * @note      none
 */
#define DRIVER_NRF24L01_LINK_SPI_INIT(HANDLE, FUC)          (HANDLE)->spi_init = FUC

/**
 * @brief     link spi_deinit function
 * @param[in] HANDLE points to an nrf24l01 handle structure
 * @param[in] FUC points to a spi_deinit function address
 * @note      none
 */
#define DRIVER_NRF24L01_LINK_SPI_DEINIT(HANDLE, FUC)        (HANDLE)->spi_deinit = FUC

/**
 * @brief     link spi_read function
 * @param[in] HANDLE points to an nrf24l01 handle structure
 * @param[in] FUC points to a spi_read function address
 * @note      none
 */
#define DRIVER_NRF24L01_LINK_SPI_READ(HANDLE, FUC)          (HANDLE)->spi_read = FUC

/**
 * @brief     link gpio_write function
 * @param[in] HANDLE points to an nrf24l01 handle structure
 * @param[in] FUC points to a gpio_write function address
 * @note      none
 */
#define DRIVER_NRF24L01_LINK_SPI_WRITE(HANDLE, FUC)         (HANDLE)->spi_write = FUC

/**
 * @brief     link gpio_init function
 * @param[in] HANDLE points to an nrf24l01 handle structure
 * @param[in] FUC points to a gpio_init function address
 * @note      none
 */
#define DRIVER_NRF24L01_LINK_GPIO_INIT(HANDLE, FUC)         (HANDLE)->gpio_init = FUC

/**
 * @brief     link gpio_deinit function
 * @param[in] HANDLE points to an nrf24l01 handle structure
 * @param[in] FUC points to a gpio_deinit function address
 * @note      none
 */
#define DRIVER_NRF24L01_LINK_GPIO_DEINIT(HANDLE, FUC)       (HANDLE)->gpio_deinit = FUC

/**
 * @brief     link gpio_write function
 * @param[in] HANDLE points to an nrf24l01 handle structure
 * @param[in] FUC points to a gpio_write function address
 * @note      none
 */
#define DRIVER_NRF24L01_LINK_GPIO_WRITE(HANDLE, FUC)        (HANDLE)->gpio_write = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE points to an nrf24l01 handle structure
 * @param[in] FUC points to a delay_ms function address
 * @note      none
 */
#define DRIVER_NRF24L01_LINK_DELAY_MS(HANDLE, FUC)          (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE points to an nrf24l01 handle structure
 * @param[in] FUC points to a debug_print function address
 * @note      none
 */
#define DRIVER_NRF24L01_LINK_DEBUG_PRINT(HANDLE, FUC)       (HANDLE)->debug_print = FUC

/**
 * @brief     link receive_callback function
 * @param[in] HANDLE points to an nrf24l01 handle structure
 * @param[in] FUC points to a receive_callback function address
 * @note      none
 */
#define DRIVER_NRF24L01_LINK_RECEIVE_CALLBACK(HANDLE, FUC)  (HANDLE)->receive_callback = FUC

/**
 * @}
 */

/**
 * @defgroup nrf24l01_basic_driver nrf24l01 basic driver function
 * @brief    nrf24l01 basic driver modules
 * @ingroup  nrf24l01_driver
 * @{
 */

/**
 * @brief      get chip's information
 * @param[out] *info points to an nrf24l01 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t nrf24l01_info(nrf24l01_info_t *info);

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
uint8_t nrf24l01_irq_handler(nrf24l01_handle_t *handle);

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
uint8_t nrf24l01_init(nrf24l01_handle_t *handle);

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
uint8_t nrf24l01_deinit(nrf24l01_handle_t *handle);

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
uint8_t nrf24l01_send(nrf24l01_handle_t *handle, uint8_t *buf, uint8_t len);

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
uint8_t nrf24l01_set_active(nrf24l01_handle_t *handle, nrf24l01_bool_t enable);

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
uint8_t nrf24l01_set_config(nrf24l01_handle_t *handle, nrf24l01_config_t config, nrf24l01_bool_t enable);

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
uint8_t nrf24l01_get_config(nrf24l01_handle_t *handle, nrf24l01_config_t config, nrf24l01_bool_t *enable);

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
uint8_t nrf24l01_set_mode(nrf24l01_handle_t *handle, nrf24l01_mode_t mode);

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
uint8_t nrf24l01_get_mode(nrf24l01_handle_t *handle, nrf24l01_mode_t *mode);

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
uint8_t nrf24l01_set_auto_acknowledgment(nrf24l01_handle_t *handle, nrf24l01_pipe_t pipe, nrf24l01_bool_t enable);

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
uint8_t nrf24l01_get_auto_acknowledgment(nrf24l01_handle_t *handle, nrf24l01_pipe_t pipe, nrf24l01_bool_t *enable);

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
uint8_t nrf24l01_set_rx_pipe(nrf24l01_handle_t *handle, nrf24l01_pipe_t pipe, nrf24l01_bool_t enable);

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
uint8_t nrf24l01_get_rx_pipe(nrf24l01_handle_t *handle, nrf24l01_pipe_t pipe, nrf24l01_bool_t *enable);

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
uint8_t nrf24l01_set_address_width(nrf24l01_handle_t *handle, nrf24l01_address_width_t width);

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
uint8_t nrf24l01_get_address_width(nrf24l01_handle_t *handle, nrf24l01_address_width_t *width);

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
uint8_t nrf24l01_set_auto_retransmit_delay(nrf24l01_handle_t *handle, uint8_t delay);

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
uint8_t nrf24l01_get_auto_retransmit_delay(nrf24l01_handle_t *handle, uint8_t *delay);

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
uint8_t nrf24l01_auto_retransmit_delay_convert_to_register(nrf24l01_handle_t *handle, uint32_t us, uint8_t *reg);

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
uint8_t nrf24l01_auto_retransmit_delay_convert_to_data(nrf24l01_handle_t *handle, uint8_t reg, uint32_t *us);

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
uint8_t nrf24l01_set_auto_retransmit_count(nrf24l01_handle_t *handle, uint8_t count);

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
uint8_t nrf24l01_get_auto_retransmit_count(nrf24l01_handle_t *handle, uint8_t *count);

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
uint8_t nrf24l01_set_channel_frequency(nrf24l01_handle_t *handle, uint8_t freq);

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
uint8_t nrf24l01_get_channel_frequency(nrf24l01_handle_t *handle, uint8_t *freq);

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
uint8_t nrf24l01_set_continuous_carrier_transmit(nrf24l01_handle_t *handle, nrf24l01_bool_t enable);

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
uint8_t nrf24l01_get_continuous_carrier_transmit(nrf24l01_handle_t *handle, nrf24l01_bool_t *enable);

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
uint8_t nrf24l01_set_force_pll_lock_signal(nrf24l01_handle_t *handle, nrf24l01_bool_t enable);

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
uint8_t nrf24l01_get_force_pll_lock_signal(nrf24l01_handle_t *handle, nrf24l01_bool_t *enable);

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
uint8_t nrf24l01_set_data_rate(nrf24l01_handle_t *handle, nrf24l01_data_rate_t rate);

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
uint8_t nrf24l01_get_data_rate(nrf24l01_handle_t *handle, nrf24l01_data_rate_t *rate);

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
uint8_t nrf24l01_set_output_power(nrf24l01_handle_t *handle, nrf24l01_output_power_t power);

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
uint8_t nrf24l01_get_output_power(nrf24l01_handle_t *handle, nrf24l01_output_power_t *power);

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
uint8_t nrf24l01_get_interrupt(nrf24l01_handle_t *handle, nrf24l01_interrupt_t type, nrf24l01_bool_t *enable);

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
uint8_t nrf24l01_clear_interrupt(nrf24l01_handle_t *handle, nrf24l01_interrupt_t type);

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
uint8_t nrf24l01_get_data_pipe_number(nrf24l01_handle_t *handle, uint8_t *number);

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
uint8_t nrf24l01_get_lost_packet_count(nrf24l01_handle_t *handle, uint8_t *count);

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
uint8_t nrf24l01_get_retransmitted_packet_count(nrf24l01_handle_t *handle, uint8_t *count);

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
uint8_t nrf24l01_get_received_power_detector(nrf24l01_handle_t *handle, nrf24l01_bool_t *enable);

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
uint8_t nrf24l01_set_rx_pipe_0_address(nrf24l01_handle_t *handle, uint8_t *addr, uint8_t len);

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
uint8_t nrf24l01_get_rx_pipe_0_address(nrf24l01_handle_t *handle, uint8_t *addr, uint8_t *len);

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
uint8_t nrf24l01_set_rx_pipe_1_address(nrf24l01_handle_t *handle, uint8_t *addr, uint8_t len);

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
uint8_t nrf24l01_get_rx_pipe_1_address(nrf24l01_handle_t *handle, uint8_t *addr, uint8_t *len);

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
uint8_t nrf24l01_set_rx_pipe_2_address(nrf24l01_handle_t *handle, uint8_t addr);

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
uint8_t nrf24l01_get_rx_pipe_2_address(nrf24l01_handle_t *handle, uint8_t *addr);

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
uint8_t nrf24l01_set_rx_pipe_3_address(nrf24l01_handle_t *handle, uint8_t addr);

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
uint8_t nrf24l01_get_rx_pipe_3_address(nrf24l01_handle_t *handle, uint8_t *addr);

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
uint8_t nrf24l01_set_rx_pipe_4_address(nrf24l01_handle_t *handle, uint8_t addr);

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
uint8_t nrf24l01_get_rx_pipe_4_address(nrf24l01_handle_t *handle, uint8_t *addr);

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
uint8_t nrf24l01_set_rx_pipe_5_address(nrf24l01_handle_t *handle, uint8_t addr);

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
uint8_t nrf24l01_get_rx_pipe_5_address(nrf24l01_handle_t *handle, uint8_t *addr);

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
uint8_t nrf24l01_set_tx_address(nrf24l01_handle_t *handle, uint8_t *addr, uint8_t len);

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
uint8_t nrf24l01_get_tx_address(nrf24l01_handle_t *handle, uint8_t *addr, uint8_t *len);

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
uint8_t nrf24l01_set_pipe_0_payload_number(nrf24l01_handle_t *handle, uint8_t num);

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
uint8_t nrf24l01_get_pipe_0_payload_number(nrf24l01_handle_t *handle, uint8_t *num);

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
uint8_t nrf24l01_set_pipe_1_payload_number(nrf24l01_handle_t *handle, uint8_t num);

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
uint8_t nrf24l01_get_pipe_1_payload_number(nrf24l01_handle_t *handle, uint8_t *num);

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
uint8_t nrf24l01_set_pipe_2_payload_number(nrf24l01_handle_t *handle, uint8_t num);

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
uint8_t nrf24l01_get_pipe_2_payload_number(nrf24l01_handle_t *handle, uint8_t *num);

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
uint8_t nrf24l01_set_pipe_3_payload_number(nrf24l01_handle_t *handle, uint8_t num);

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
uint8_t nrf24l01_get_pipe_3_payload_number(nrf24l01_handle_t *handle, uint8_t *num);

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
uint8_t nrf24l01_set_pipe_4_payload_number(nrf24l01_handle_t *handle, uint8_t num);

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
uint8_t nrf24l01_get_pipe_4_payload_number(nrf24l01_handle_t *handle, uint8_t *num);

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
uint8_t nrf24l01_set_pipe_5_payload_number(nrf24l01_handle_t *handle, uint8_t num);

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
uint8_t nrf24l01_get_pipe_5_payload_number(nrf24l01_handle_t *handle, uint8_t *num);

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
uint8_t nrf24l01_get_fifo_status(nrf24l01_handle_t *handle, uint8_t *status);

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
uint8_t nrf24l01_set_pipe_dynamic_payload(nrf24l01_handle_t *handle, nrf24l01_pipe_t pipe, nrf24l01_bool_t enable);

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
uint8_t nrf24l01_get_pipe_dynamic_payload(nrf24l01_handle_t *handle, nrf24l01_pipe_t pipe, nrf24l01_bool_t *enable);

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
uint8_t nrf24l01_set_dynamic_payload(nrf24l01_handle_t *handle, nrf24l01_bool_t enable);

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
uint8_t nrf24l01_get_dynamic_payload(nrf24l01_handle_t *handle, nrf24l01_bool_t *enable);

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
uint8_t nrf24l01_set_payload_with_ack(nrf24l01_handle_t *handle, nrf24l01_bool_t enable);

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
uint8_t nrf24l01_get_payload_with_ack(nrf24l01_handle_t *handle, nrf24l01_bool_t *enable);

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
uint8_t nrf24l01_set_tx_payload_with_no_ack(nrf24l01_handle_t *handle, nrf24l01_bool_t enable);

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
uint8_t nrf24l01_get_tx_payload_with_no_ack(nrf24l01_handle_t *handle, nrf24l01_bool_t *enable);

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
uint8_t nrf24l01_read_rx_payload(nrf24l01_handle_t *handle, uint8_t *buf, uint8_t len);

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
uint8_t nrf24l01_write_tx_payload(nrf24l01_handle_t *handle, uint8_t *buf, uint8_t len);

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
uint8_t nrf24l01_flush_tx(nrf24l01_handle_t *handle);

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
uint8_t nrf24l01_flush_rx(nrf24l01_handle_t *handle);

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
uint8_t nrf24l01_reuse_tx_payload(nrf24l01_handle_t *handle);

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
uint8_t nrf24l01_get_rx_payload_width(nrf24l01_handle_t *handle, uint8_t *width);

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
uint8_t nrf24l01_write_payload_with_ack(nrf24l01_handle_t *handle, nrf24l01_pipe_t pipe, uint8_t *buf, uint8_t len);

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
uint8_t nrf24l01_write_payload_with_no_ack(nrf24l01_handle_t *handle, uint8_t *buf, uint8_t len);

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
uint8_t nrf24l01_nop(nrf24l01_handle_t *handle);

/**
 * @}
 */

/**
 * @defgroup nrf24l01_extern_driver nrf24l01 extern driver function
 * @brief    nrf24l01 extern driver modules
 * @ingroup  nrf24l01_driver
 * @{
 */

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
uint8_t nrf24l01_set_reg(nrf24l01_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

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
uint8_t nrf24l01_get_reg(nrf24l01_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
