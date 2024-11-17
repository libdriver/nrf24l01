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
 * @file      driver_nrf24l01_send_receive_test.c
 * @brief     driver nrf24l01 send receive test source file
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

#include "driver_nrf24l01_send_receive_test.h"

static nrf24l01_handle_t gs_handle;        /**< nrf24l01 handle */

/**
 * @brief  nrf24l01 interrupt test irq
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t nrf24l01_interrupt_test_irq_handler(void)
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
 * @brief  send test
 * @return status code
 *         - 0 success
 *         - 1 test failed
 * @note   none
 */
uint8_t nrf24l01_send_test(void)
{
    uint8_t res;
    uint8_t reg;
    uint8_t i;
    nrf24l01_info_t info;
    uint8_t addr0[5] = {0x1A, 0x01, 0x02, 0x03, 0x00};
    uint8_t addr1[5] = {0x1B, 0x01, 0x02, 0x03, 0x01};
    uint8_t addr2[5] = {0x1B, 0x01, 0x02, 0x03, 0x02};
    uint8_t addr3[5] = {0x1B, 0x01, 0x02, 0x03, 0x03};
    uint8_t addr4[5] = {0x1B, 0x01, 0x02, 0x03, 0x04};
    uint8_t addr5[5] = {0x1B, 0x01, 0x02, 0x03, 0x05};
    uint8_t data[32];
    
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
    DRIVER_NRF24L01_LINK_RECEIVE_CALLBACK(&gs_handle, nrf24l01_interface_receive_callback);
    
    /* get information */
    res = nrf24l01_info(&info);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get info failed.\n");
        
        return 1;
    }
    else
    {
        /* print chip info */
        nrf24l01_interface_debug_print("nrf24l01: chip is %s.\n", info.chip_name);
        nrf24l01_interface_debug_print("nrf24l01: manufacturer is %s.\n", info.manufacturer_name);
        nrf24l01_interface_debug_print("nrf24l01: interface is %s.\n", info.interface);
        nrf24l01_interface_debug_print("nrf24l01: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        nrf24l01_interface_debug_print("nrf24l01: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        nrf24l01_interface_debug_print("nrf24l01: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        nrf24l01_interface_debug_print("nrf24l01: max current is %0.2fmA.\n", info.max_current_ma);
        nrf24l01_interface_debug_print("nrf24l01: max temperature is %0.1fC.\n", info.temperature_max);
        nrf24l01_interface_debug_print("nrf24l01: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start send test */
    nrf24l01_interface_debug_print("nrf24l01: start send test.\n");
    
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
    
    /* enable crco */
    res = nrf24l01_set_config(&gs_handle, NRF24L01_CONFIG_CRCO, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set config failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable crc */
    res = nrf24l01_set_config(&gs_handle, NRF24L01_CONFIG_EN_CRC, NRF24L01_BOOL_TRUE);
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
    
    /* set tx mode */
    res = nrf24l01_set_mode(&gs_handle, NRF24L01_MODE_TX);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set mode failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable pipe 0 auto acknowledgment */
    res = nrf24l01_set_auto_acknowledgment(&gs_handle, NRF24L01_PIPE_0, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set auto acknowledgment failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable pipe 1 auto acknowledgment */
    res = nrf24l01_set_auto_acknowledgment(&gs_handle, NRF24L01_PIPE_1, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set auto acknowledgment failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable pipe 2 auto acknowledgment */
    res = nrf24l01_set_auto_acknowledgment(&gs_handle, NRF24L01_PIPE_2, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set auto acknowledgment failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable pipe 3 auto acknowledgment */
    res = nrf24l01_set_auto_acknowledgment(&gs_handle, NRF24L01_PIPE_3, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set auto acknowledgment failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable pipe 4 auto acknowledgment */
    res = nrf24l01_set_auto_acknowledgment(&gs_handle, NRF24L01_PIPE_4, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set auto acknowledgment failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable pipe 5 auto acknowledgment */
    res = nrf24l01_set_auto_acknowledgment(&gs_handle, NRF24L01_PIPE_5, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set auto acknowledgment failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable pipe 0 rx */
    res = nrf24l01_set_rx_pipe(&gs_handle, NRF24L01_PIPE_0, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable pipe 1 rx */
    res = nrf24l01_set_rx_pipe(&gs_handle, NRF24L01_PIPE_1, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable pipe 2 rx */
    res = nrf24l01_set_rx_pipe(&gs_handle, NRF24L01_PIPE_2, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable pipe 3 rx */
    res = nrf24l01_set_rx_pipe(&gs_handle, NRF24L01_PIPE_3, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable pipe 4 rx */
    res = nrf24l01_set_rx_pipe(&gs_handle, NRF24L01_PIPE_4, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable pipe 5 rx */
    res = nrf24l01_set_rx_pipe(&gs_handle, NRF24L01_PIPE_5, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set address width 5 bytes */
    res = nrf24l01_set_address_width(&gs_handle, NRF24L01_ADDRESS_WIDTH_5_BYTES);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set address width failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* convert auto retransmit delay */
    res = nrf24l01_auto_retransmit_delay_convert_to_register(&gs_handle, 750, (uint8_t *)&reg);
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
    res = nrf24l01_set_auto_retransmit_count(&gs_handle, 10);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: auto retransmit count failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set the channel frequency */
    res = nrf24l01_set_channel_frequency(&gs_handle, 20);
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
    
    /* 2M rate */
    res = nrf24l01_set_data_rate(&gs_handle, NRF24L01_DATA_RATE_2M);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set data rate 2M failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set output power 0 dBm */
    res = nrf24l01_set_output_power(&gs_handle, NRF24L01_OUTPUT_POWER_0_DBM);
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
    
    /* set rx pipe 0 address */
    res = nrf24l01_set_rx_pipe_0_address(&gs_handle, (uint8_t *)addr0, 5);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe 0 address failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set rx pipe 1 address */
    res = nrf24l01_set_rx_pipe_1_address(&gs_handle, (uint8_t *)addr1, 5);
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
    
    /* set pipe 0 payload number */
    res = nrf24l01_set_pipe_0_payload_number(&gs_handle, 32);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe 0 payload number failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set pipe 1 payload number */
    res = nrf24l01_set_pipe_1_payload_number(&gs_handle, 32);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe 1 payload number failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set pipe 2 payload number */
    res = nrf24l01_set_pipe_2_payload_number(&gs_handle, 32);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe 2 payload number failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set pipe 3 payload number */
    res = nrf24l01_set_pipe_3_payload_number(&gs_handle, 32);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe 3 payload number failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set pipe 4 payload number */
    res = nrf24l01_set_pipe_4_payload_number(&gs_handle, 32);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe 4 payload number failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set pipe 5 payload number */
    res = nrf24l01_set_pipe_5_payload_number(&gs_handle, 32);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe 5 payload number failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable pipe 0 dynamic payload */
    res = nrf24l01_set_pipe_dynamic_payload(&gs_handle, NRF24L01_PIPE_0, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable pipe 1 dynamic payload */
    res = nrf24l01_set_pipe_dynamic_payload(&gs_handle, NRF24L01_PIPE_1, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable pipe 2 dynamic payload */
    res = nrf24l01_set_pipe_dynamic_payload(&gs_handle, NRF24L01_PIPE_2, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable pipe 3 dynamic payload */
    res = nrf24l01_set_pipe_dynamic_payload(&gs_handle, NRF24L01_PIPE_3, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable pipe 4 dynamic payload */
    res = nrf24l01_set_pipe_dynamic_payload(&gs_handle, NRF24L01_PIPE_4, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable pipe 5 dynamic payload */
    res = nrf24l01_set_pipe_dynamic_payload(&gs_handle, NRF24L01_PIPE_5, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable dynamic payload */
    res = nrf24l01_set_dynamic_payload(&gs_handle, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable payload with ack */
    res = nrf24l01_set_payload_with_ack(&gs_handle, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set payload with ack failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable tx payload with no ack */
    res = nrf24l01_set_tx_payload_with_no_ack(&gs_handle, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set tx payload with no ack failed.\n");
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
    
    /* set tx address */
    res = nrf24l01_set_tx_address(&gs_handle, (uint8_t *)addr0, 5);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set tx address failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set rx pipe 0 address */
    res = nrf24l01_set_rx_pipe_0_address(&gs_handle, (uint8_t *)addr0, 5);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe 0 address failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set data */
    for (i = 0; i < 32; i++)
    {
        data[i] = i;
    }
    
    /* send data */
    res = nrf24l01_send(&gs_handle, (uint8_t *)data, 32);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: send data failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* delay 100 ms */
    nrf24l01_interface_delay_ms(100);
    
    /* disable active */
    res = nrf24l01_set_active(&gs_handle, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set active failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set tx address */
    res = nrf24l01_set_tx_address(&gs_handle, (uint8_t *)addr1, 5);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set tx address failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set rx pipe 1 address */
    res = nrf24l01_set_rx_pipe_0_address(&gs_handle, (uint8_t *)addr1, 5);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe 0 address failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }

    /* send data */
    res = nrf24l01_send(&gs_handle, (uint8_t *)data, 32);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: send data failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* delay 100 ms */
    nrf24l01_interface_delay_ms(100);
    
    /* disable active */
    res = nrf24l01_set_active(&gs_handle, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set active failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set tx address */
    res = nrf24l01_set_tx_address(&gs_handle, (uint8_t *)&addr2, 5);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set tx address failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set rx pipe 2 address */
    res = nrf24l01_set_rx_pipe_0_address(&gs_handle, (uint8_t *)addr2, 5);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe 0 address failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* send data */
    res = nrf24l01_send(&gs_handle, (uint8_t *)data, 32);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: send data failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* delay 100 ms */
    nrf24l01_interface_delay_ms(100);
    
    /* disable active */
    res = nrf24l01_set_active(&gs_handle, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set active failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set tx address */
    res = nrf24l01_set_tx_address(&gs_handle, (uint8_t *)&addr3, 5);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set tx address failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set rx pipe 3 address */
    res = nrf24l01_set_rx_pipe_0_address(&gs_handle, (uint8_t *)addr3, 5);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe 0 address failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* send data */
    res = nrf24l01_send(&gs_handle, (uint8_t *)data, 32);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: send data failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* delay 100 ms */
    nrf24l01_interface_delay_ms(100);
    
    /* disable active */
    res = nrf24l01_set_active(&gs_handle, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set active failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set tx address */
    res = nrf24l01_set_tx_address(&gs_handle, (uint8_t *)&addr4, 5);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set tx address failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set rx pipe 4 address */
    res = nrf24l01_set_rx_pipe_0_address(&gs_handle, (uint8_t *)addr4, 5);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe 0 address failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* send data */
    res = nrf24l01_send(&gs_handle, (uint8_t *)data, 32);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: send data failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* delay 100 ms */
    nrf24l01_interface_delay_ms(100);
    
    /* disable active */
    res = nrf24l01_set_active(&gs_handle, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set active failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set tx address */
    res = nrf24l01_set_tx_address(&gs_handle, (uint8_t *)&addr5, 5);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set tx address failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set rx pipe 5 address */
    res = nrf24l01_set_rx_pipe_0_address(&gs_handle, (uint8_t *)addr5, 5);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe 0 address failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* send data */
    res = nrf24l01_send(&gs_handle, (uint8_t *)data, 32);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: send data failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* finish send test */
    nrf24l01_interface_debug_print("nrf24l01: finish send test.\n");
    (void)nrf24l01_deinit(&gs_handle);
    
    return 0;
}

/**
 * @brief  receive test
 * @return status code
 *         - 0 success
 *         - 1 test failed
 * @note   none
 */
uint8_t nrf24l01_receive_test(void)
{
    uint8_t res;
    uint8_t reg;
    nrf24l01_info_t info;
    uint8_t addr0[5] = {0x1A, 0x01, 0x02, 0x03, 0x00};
    uint8_t addr1[5] = {0x1B, 0x01, 0x02, 0x03, 0x01};
    uint8_t addr2[5] = {0x1B, 0x01, 0x02, 0x03, 0x02};
    uint8_t addr3[5] = {0x1B, 0x01, 0x02, 0x03, 0x03};
    uint8_t addr4[5] = {0x1B, 0x01, 0x02, 0x03, 0x04};
    uint8_t addr5[5] = {0x1B, 0x01, 0x02, 0x03, 0x05};
    
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
    DRIVER_NRF24L01_LINK_RECEIVE_CALLBACK(&gs_handle, nrf24l01_interface_receive_callback);
    
    /* get information */
    res = nrf24l01_info(&info);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get info failed.\n");
        
        return 1;
    }
    else
    {
        /* print chip info */
        nrf24l01_interface_debug_print("nrf24l01: chip is %s.\n", info.chip_name);
        nrf24l01_interface_debug_print("nrf24l01: manufacturer is %s.\n", info.manufacturer_name);
        nrf24l01_interface_debug_print("nrf24l01: interface is %s.\n", info.interface);
        nrf24l01_interface_debug_print("nrf24l01: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        nrf24l01_interface_debug_print("nrf24l01: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        nrf24l01_interface_debug_print("nrf24l01: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        nrf24l01_interface_debug_print("nrf24l01: max current is %0.2fmA.\n", info.max_current_ma);
        nrf24l01_interface_debug_print("nrf24l01: max temperature is %0.1fC.\n", info.temperature_max);
        nrf24l01_interface_debug_print("nrf24l01: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start receive test */
    nrf24l01_interface_debug_print("nrf24l01: start receive test.\n");
    
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
    
    /* enable crco */
    res = nrf24l01_set_config(&gs_handle, NRF24L01_CONFIG_CRCO, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set config failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable crc */
    res = nrf24l01_set_config(&gs_handle, NRF24L01_CONFIG_EN_CRC, NRF24L01_BOOL_TRUE);
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
    
    /* set rx mode */
    res = nrf24l01_set_mode(&gs_handle, NRF24L01_MODE_RX);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set mode failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable pipe 0 auto acknowledgment */
    res = nrf24l01_set_auto_acknowledgment(&gs_handle, NRF24L01_PIPE_0, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set auto acknowledgment failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable pipe 1 auto acknowledgment */
    res = nrf24l01_set_auto_acknowledgment(&gs_handle, NRF24L01_PIPE_1, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set auto acknowledgment failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable pipe 2 auto acknowledgment */
    res = nrf24l01_set_auto_acknowledgment(&gs_handle, NRF24L01_PIPE_2, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set auto acknowledgment failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable pipe 3 auto acknowledgment */
    res = nrf24l01_set_auto_acknowledgment(&gs_handle, NRF24L01_PIPE_3, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set auto acknowledgment failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable pipe 4 auto acknowledgment */
    res = nrf24l01_set_auto_acknowledgment(&gs_handle, NRF24L01_PIPE_4, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set auto acknowledgment failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable pipe 5 auto acknowledgment */
    res = nrf24l01_set_auto_acknowledgment(&gs_handle, NRF24L01_PIPE_5, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set auto acknowledgment failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable pipe 0 rx */
    res = nrf24l01_set_rx_pipe(&gs_handle, NRF24L01_PIPE_0, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable pipe 1 rx */
    res = nrf24l01_set_rx_pipe(&gs_handle, NRF24L01_PIPE_1, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable pipe 2 rx */
    res = nrf24l01_set_rx_pipe(&gs_handle, NRF24L01_PIPE_2, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable pipe 3 rx */
    res = nrf24l01_set_rx_pipe(&gs_handle, NRF24L01_PIPE_3, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable pipe 4 rx */
    res = nrf24l01_set_rx_pipe(&gs_handle, NRF24L01_PIPE_4, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable pipe 5 rx */
    res = nrf24l01_set_rx_pipe(&gs_handle, NRF24L01_PIPE_5, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set address width 5 bytes */
    res = nrf24l01_set_address_width(&gs_handle, NRF24L01_ADDRESS_WIDTH_5_BYTES);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set address width failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* convert auto retransmit delay */
    res = nrf24l01_auto_retransmit_delay_convert_to_register(&gs_handle, 750, (uint8_t *)&reg);
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
    res = nrf24l01_set_auto_retransmit_count(&gs_handle, 10);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: auto retransmit count failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set the channel frequency */
    res = nrf24l01_set_channel_frequency(&gs_handle, 20);
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
    
    /* 2M rate */
    res = nrf24l01_set_data_rate(&gs_handle, NRF24L01_DATA_RATE_2M);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set data rate 2M failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set output power 0 dBm */
    res = nrf24l01_set_output_power(&gs_handle, NRF24L01_OUTPUT_POWER_0_DBM);
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
    
    /* set rx pipe 0 address */
    res = nrf24l01_set_rx_pipe_0_address(&gs_handle, (uint8_t *)addr0, 5);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe 0 address failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set rx pipe 1 address */
    res = nrf24l01_set_rx_pipe_1_address(&gs_handle, (uint8_t *)addr1, 5);
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
    
    /* set pipe 0 payload number */
    res = nrf24l01_set_pipe_0_payload_number(&gs_handle, 32);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe 0 payload number failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set pipe 1 payload number */
    res = nrf24l01_set_pipe_1_payload_number(&gs_handle, 32);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe 1 payload number failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set pipe 2 payload number */
    res = nrf24l01_set_pipe_2_payload_number(&gs_handle, 32);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe 2 payload number failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set pipe 3 payload number */
    res = nrf24l01_set_pipe_3_payload_number(&gs_handle, 32);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe 3 payload number failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set pipe 4 payload number */
    res = nrf24l01_set_pipe_4_payload_number(&gs_handle, 32);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe 4 payload number failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set pipe 5 payload number */
    res = nrf24l01_set_pipe_5_payload_number(&gs_handle, 32);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe 5 payload number failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable pipe 0 dynamic payload */
    res = nrf24l01_set_pipe_dynamic_payload(&gs_handle, NRF24L01_PIPE_0, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable pipe 1 dynamic payload */
    res = nrf24l01_set_pipe_dynamic_payload(&gs_handle, NRF24L01_PIPE_1, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable pipe 2 dynamic payload */
    res = nrf24l01_set_pipe_dynamic_payload(&gs_handle, NRF24L01_PIPE_2, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable pipe 3 dynamic payload */
    res = nrf24l01_set_pipe_dynamic_payload(&gs_handle, NRF24L01_PIPE_3, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable pipe 4 dynamic payload */
    res = nrf24l01_set_pipe_dynamic_payload(&gs_handle, NRF24L01_PIPE_4, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable pipe 5 dynamic payload */
    res = nrf24l01_set_pipe_dynamic_payload(&gs_handle, NRF24L01_PIPE_5, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable dynamic payload */
    res = nrf24l01_set_dynamic_payload(&gs_handle, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable payload with ack */
    res = nrf24l01_set_payload_with_ack(&gs_handle, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set payload with ack failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable tx payload with no ack */
    res = nrf24l01_set_tx_payload_with_no_ack(&gs_handle, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set tx payload with no ack failed.\n");
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

    /* enable active */
    res = nrf24l01_set_active(&gs_handle, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set active failed.\n");
        (void)nrf24l01_deinit(&gs_handle);
        
        return 1;
    }
    
    /* delay 5s */
    nrf24l01_interface_delay_ms(5000);
    
    /* finish receive test */
    nrf24l01_interface_debug_print("nrf24l01: finish receive test.\n");
    (void)nrf24l01_deinit(&gs_handle);
    
    return 0;
}
