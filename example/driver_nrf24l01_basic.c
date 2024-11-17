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
 * @file      driver_nrf24l01_basic.c
 * @brief     driver nrf24l01 basic source file
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

#include "driver_nrf24l01_basic.h"

static nrf24l01_handle_t gs_handle;        /**< nrf24l01 handle */

/**
 * @brief  nrf24l01 irq
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t nrf24l01_interrupt_irq_handler(void)
{
    if (nrf24l01_irq_handler(&gs_handle) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     basic example init
 * @param[in] type is the chip working mode
 * @param[in] *callback points to a callback function
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t nrf24l01_basic_init(nrf24l01_type_t type, void (*callback)(uint8_t type, uint8_t num, uint8_t *buf, uint8_t len))
{
    uint8_t res;
    uint8_t reg;
    uint8_t addr0[5] = NRF24L01_BASIC_DEFAULT_RX_ADDR_0;
    uint8_t addr1[5] = NRF24L01_BASIC_DEFAULT_RX_ADDR_1;
    uint8_t addr2[5] = NRF24L01_BASIC_DEFAULT_RX_ADDR_2;
    uint8_t addr3[5] = NRF24L01_BASIC_DEFAULT_RX_ADDR_3;
    uint8_t addr4[5] = NRF24L01_BASIC_DEFAULT_RX_ADDR_4;
    uint8_t addr5[5] = NRF24L01_BASIC_DEFAULT_RX_ADDR_5;
    
    /* link function */
    DRIVER_NRF24L01_LINK_INIT(&gs_handle, nrf24l01_handle_t);
    DRIVER_NRF24L01_LINK_SPI_INIT(&gs_handle, nrf24l01_interface_spi_init);
    DRIVER_NRF24L01_LINK_SPI_DEINIT(&gs_handle, nrf24l01_interface_spi_deinit);
    DRIVER_NRF24L01_LINK_SPI_READ(&gs_handle, nrf24l01_interface_spi_read);
    DRIVER_NRF24L01_LINK_SPI_WRITE(&gs_handle, nrf24l01_interface_spi_write);
    DRIVER_NRF24L01_LINK_GPIO_INIT(&gs_handle, nrf24l01_interface_gpio_init);
    DRIVER_NRF24L01_LINK_GPIO_DEINIT(&gs_handle, nrf24l01_interface_gpio_deinit);
    DRIVER_NRF24L01_LINK_GPIO_WRITE(&gs_handle, nrf24l01_interface_gpio_write);
    DRIVER_NRF24L01_LINK_DELAY_MS(&gs_handle, nrf24l01_interface_delay_ms);
    DRIVER_NRF24L01_LINK_DEBUG_PRINT(&gs_handle, nrf24l01_interface_debug_print);
    DRIVER_NRF24L01_LINK_RECEIVE_CALLBACK(&gs_handle, callback);

    /* init */
    res = nrf24l01_init(&gs_handle);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: init failed.\n");
        
        return 1;
    }
    
    /* set active false */
    res = nrf24l01_set_active(&gs_handle, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set active failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable power up */
    res = nrf24l01_set_config(&gs_handle, NRF24L01_CONFIG_PWR_UP, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set config failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set crco */
    res = nrf24l01_set_config(&gs_handle, NRF24L01_CONFIG_CRCO, NRF24L01_BASIC_DEFAULT_CRCO);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set config failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set crc */
    res = nrf24l01_set_config(&gs_handle, NRF24L01_CONFIG_EN_CRC, NRF24L01_BASIC_DEFAULT_ENABLE_CRC);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set config failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable max rt */
    res = nrf24l01_set_config(&gs_handle, NRF24L01_CONFIG_MASK_MAX_RT, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set config failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable tx ds */
    res = nrf24l01_set_config(&gs_handle, NRF24L01_CONFIG_MASK_TX_DS, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set config failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable rx dr */
    res = nrf24l01_set_config(&gs_handle, NRF24L01_CONFIG_MASK_RX_DR, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set config failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    if (type == NRF24L01_TYPE_TX)
    {
        /* set tx mode */
        res = nrf24l01_set_mode(&gs_handle, NRF24L01_MODE_TX);
        if (res != 0)
        {
            nrf24l01_interface_debug_print("nrf24l01: set mode failed.\n");
            (void)nrf24l01_deinit(&gs_handle);
            
            return 1;
        }
    }
    else
    {
        /* set rx mode */
        res = nrf24l01_set_mode(&gs_handle, NRF24L01_MODE_RX);
        if (res != 0)
        {
            nrf24l01_interface_debug_print("nrf24l01: set mode failed.\n");
            (void)nrf24l01_deinit(&gs_handle);
            
            return 1;
        }
    }
    
    /* set pipe 0 auto acknowledgment */
    res = nrf24l01_set_auto_acknowledgment(&gs_handle, NRF24L01_PIPE_0, NRF24L01_BASIC_DEFAULT_PIPE_0_AUTO_ACKNOWLEDGMENT);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set auto acknowledgment failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set pipe 1 auto acknowledgment */
    res = nrf24l01_set_auto_acknowledgment(&gs_handle, NRF24L01_PIPE_1, NRF24L01_BASIC_DEFAULT_PIPE_1_AUTO_ACKNOWLEDGMENT);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set auto acknowledgment failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set pipe 2 auto acknowledgment */
    res = nrf24l01_set_auto_acknowledgment(&gs_handle, NRF24L01_PIPE_2, NRF24L01_BASIC_DEFAULT_PIPE_2_AUTO_ACKNOWLEDGMENT);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set auto acknowledgment failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set pipe 3 auto acknowledgment */
    res = nrf24l01_set_auto_acknowledgment(&gs_handle, NRF24L01_PIPE_3, NRF24L01_BASIC_DEFAULT_PIPE_3_AUTO_ACKNOWLEDGMENT);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set auto acknowledgment failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set pipe 4 auto acknowledgment */
    res = nrf24l01_set_auto_acknowledgment(&gs_handle, NRF24L01_PIPE_4, NRF24L01_BASIC_DEFAULT_PIPE_4_AUTO_ACKNOWLEDGMENT);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set auto acknowledgment failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set pipe 5 auto acknowledgment */
    res = nrf24l01_set_auto_acknowledgment(&gs_handle, NRF24L01_PIPE_5, NRF24L01_BASIC_DEFAULT_PIPE_5_AUTO_ACKNOWLEDGMENT);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set auto acknowledgment failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set pipe 0 rx */
    res = nrf24l01_set_rx_pipe(&gs_handle, NRF24L01_PIPE_0, NRF24L01_BASIC_DEFAULT_RX_PIPE_0);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set pipe 1 rx */
    res = nrf24l01_set_rx_pipe(&gs_handle, NRF24L01_PIPE_1, NRF24L01_BASIC_DEFAULT_RX_PIPE_1);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set pipe 2 rx */
    res = nrf24l01_set_rx_pipe(&gs_handle, NRF24L01_PIPE_2, NRF24L01_BASIC_DEFAULT_RX_PIPE_2);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set pipe 3 rx */
    res = nrf24l01_set_rx_pipe(&gs_handle, NRF24L01_PIPE_3, NRF24L01_BASIC_DEFAULT_RX_PIPE_3);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set pipe 4 rx */
    res = nrf24l01_set_rx_pipe(&gs_handle, NRF24L01_PIPE_4, NRF24L01_BASIC_DEFAULT_RX_PIPE_4);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set pipe 5 rx */
    res = nrf24l01_set_rx_pipe(&gs_handle, NRF24L01_PIPE_5, NRF24L01_BASIC_DEFAULT_RX_PIPE_5);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set address width */
    res = nrf24l01_set_address_width(&gs_handle, NRF24L01_BASIC_DEFAULT_ADDRESS_WIDTH);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set address width failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* convert auto retransmit delay */
    res = nrf24l01_auto_retransmit_delay_convert_to_register(&gs_handle, NRF24L01_BASIC_DEFAULT_RETRANSMIT_DELAY, (uint8_t *)&reg);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: auto retransmit delay failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set auto retransmit delay */
    res = nrf24l01_set_auto_retransmit_delay(&gs_handle, reg);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: auto retransmit delay failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set the auto retransmit count */
    res = nrf24l01_set_auto_retransmit_count(&gs_handle, NRF24L01_BASIC_DEFAULT_RETRANSMIT_COUNT);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: auto retransmit count failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set the channel frequency */
    res = nrf24l01_set_channel_frequency(&gs_handle, NRF24L01_BASIC_DEFAULT_CHANNEL_FREQUENCY);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set channel frequency failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable continuous carrier transmit */
    res = nrf24l01_set_continuous_carrier_transmit(&gs_handle, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set continuous carrier transmit failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable force pll lock signal */
    res = nrf24l01_set_force_pll_lock_signal(&gs_handle, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set force pll lock signal failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set data rate */
    res = nrf24l01_set_data_rate(&gs_handle, NRF24L01_BASIC_DEFAULT_DATA_RATE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set data rate 2M failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set output power */
    res = nrf24l01_set_output_power(&gs_handle, NRF24L01_BASIC_DEFAULT_OUTPUT_POWER);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set output power failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* clear interrupt rx_dr */
    res = nrf24l01_clear_interrupt(&gs_handle, NRF24L01_INTERRUPT_RX_DR);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: clear interrupt failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* clear interrupt tx_ds */
    res = nrf24l01_clear_interrupt(&gs_handle, NRF24L01_INTERRUPT_TX_DS);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: clear interrupt failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* clear interrupt max_rt */
    res = nrf24l01_clear_interrupt(&gs_handle, NRF24L01_INTERRUPT_MAX_RT);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: clear interrupt failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* clear interrupt tx_full */
    res = nrf24l01_clear_interrupt(&gs_handle, NRF24L01_INTERRUPT_TX_FULL);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: clear interrupt failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set pipe 0 payload number */
    res = nrf24l01_set_pipe_0_payload_number(&gs_handle, NRF24L01_BASIC_DEFAULT_PIPE_0_PAYLOAD);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe 0 payload number failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set pipe 1 payload number */
    res = nrf24l01_set_pipe_1_payload_number(&gs_handle, NRF24L01_BASIC_DEFAULT_PIPE_1_PAYLOAD);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe 1 payload number failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set pipe 2 payload number */
    res = nrf24l01_set_pipe_2_payload_number(&gs_handle, NRF24L01_BASIC_DEFAULT_PIPE_2_PAYLOAD);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe 2 payload number failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set pipe 3 payload number */
    res = nrf24l01_set_pipe_3_payload_number(&gs_handle, NRF24L01_BASIC_DEFAULT_PIPE_3_PAYLOAD);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe 3 payload number failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set pipe 4 payload number */
    res = nrf24l01_set_pipe_4_payload_number(&gs_handle, NRF24L01_BASIC_DEFAULT_PIPE_4_PAYLOAD);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe 4 payload number failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set pipe 5 payload number */
    res = nrf24l01_set_pipe_5_payload_number(&gs_handle, NRF24L01_BASIC_DEFAULT_PIPE_5_PAYLOAD);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe 5 payload number failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set pipe 0 dynamic payload */
    res = nrf24l01_set_pipe_dynamic_payload(&gs_handle, NRF24L01_PIPE_0, NRF24L01_BASIC_DEFAULT_PIPE_0_DYNAMIC_PAYLOAD);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set pipe 1 dynamic payload */
    res = nrf24l01_set_pipe_dynamic_payload(&gs_handle, NRF24L01_PIPE_1, NRF24L01_BASIC_DEFAULT_PIPE_1_DYNAMIC_PAYLOAD);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set pipe 2 dynamic payload */
    res = nrf24l01_set_pipe_dynamic_payload(&gs_handle, NRF24L01_PIPE_2, NRF24L01_BASIC_DEFAULT_PIPE_2_DYNAMIC_PAYLOAD);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set pipe 3 dynamic payload */
    res = nrf24l01_set_pipe_dynamic_payload(&gs_handle, NRF24L01_PIPE_3, NRF24L01_BASIC_DEFAULT_PIPE_3_DYNAMIC_PAYLOAD);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set pipe 4 dynamic payload */
    res = nrf24l01_set_pipe_dynamic_payload(&gs_handle, NRF24L01_PIPE_4, NRF24L01_BASIC_DEFAULT_PIPE_4_DYNAMIC_PAYLOAD);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set pipe 5 dynamic payload */
    res = nrf24l01_set_pipe_dynamic_payload(&gs_handle, NRF24L01_PIPE_5, NRF24L01_BASIC_DEFAULT_PIPE_5_DYNAMIC_PAYLOAD);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set dynamic payload */
    res = nrf24l01_set_dynamic_payload(&gs_handle, NRF24L01_BASIC_DEFAULT_DYNAMIC_PAYLOAD);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set payload with ack */
    res = nrf24l01_set_payload_with_ack(&gs_handle, NRF24L01_BASIC_DEFAULT_PAYLOAD_WITH_ACK);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set payload with ack failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set tx payload with no ack */
    res = nrf24l01_set_tx_payload_with_no_ack(&gs_handle, NRF24L01_BASIC_DEFAULT_TX_PAYLOAD_WITH_NO_ACK);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set tx payload with no ack failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set rx pipe 0 address */
    res = nrf24l01_set_rx_pipe_0_address(&gs_handle, (uint8_t *)addr0, NRF24L01_BASIC_DEFAULT_ADDRESS_WIDTH + 2);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe 0 address failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set rx pipe 1 address */
    res = nrf24l01_set_rx_pipe_1_address(&gs_handle, (uint8_t *)addr1, NRF24L01_BASIC_DEFAULT_ADDRESS_WIDTH + 2);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe 1 address failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set rx pipe 2 address */
    res = nrf24l01_set_rx_pipe_2_address(&gs_handle, addr2[4]);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe 2 address failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set rx pipe 3 address */
    res = nrf24l01_set_rx_pipe_3_address(&gs_handle, addr3[4]);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe 3 address failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set rx pipe 4 address */
    res = nrf24l01_set_rx_pipe_4_address(&gs_handle, addr4[4]);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe 4 address failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set rx pipe 5 address */
    res = nrf24l01_set_rx_pipe_5_address(&gs_handle, addr5[4]);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe 5 address failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* flush tx */
    res = nrf24l01_flush_tx(&gs_handle);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: flush tx failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* flush rx */
    res = nrf24l01_flush_rx(&gs_handle);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: flush rx failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set active true */
    res = nrf24l01_set_active(&gs_handle, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set active failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    return 0;
}

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t nrf24l01_basic_deinit(void)
{
    if (nrf24l01_deinit(&gs_handle) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

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
uint8_t nrf24l01_basic_send(uint8_t *addr, uint8_t *buf, uint8_t len)
{
    uint8_t res;
    
    /* set active false */
    res = nrf24l01_set_active(&gs_handle, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        return 1;
    }
    
    /* set tx address */
    res = nrf24l01_set_tx_address(&gs_handle, (uint8_t *)addr, NRF24L01_BASIC_DEFAULT_ADDRESS_WIDTH + 2);
    if (res != 0)
    {
        return 1;
    }
    
    /* set rx pipe 0 address */
    res = nrf24l01_set_rx_pipe_0_address(&gs_handle, (uint8_t *)addr, NRF24L01_BASIC_DEFAULT_ADDRESS_WIDTH + 2);
    if (res != 0)
    {
        return 1;
    }
    
    /* send data */
    res = nrf24l01_send(&gs_handle, (uint8_t *)buf, len);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}
