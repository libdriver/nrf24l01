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
 * @file      driver_nrf24l01_register_test.c
 * @brief     driver nrf24l01 register test source file
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

#include "driver_nrf24l01_register_test.h"
#include <stdlib.h>

static nrf24l01_handle_t gs_handle;        /**< nrf24l01 handle */

/**
 * @brief  register test
 * @return status code
 *         - 0 success
 *         - 1 test failed
 * @note   none
 */
uint8_t nrf24l01_register_test(void)
{
    uint8_t res;
    uint8_t len;
    uint8_t value, value_check;
    uint32_t us, us_check;
    uint8_t addr[5];
    uint8_t addr_check[5];
    uint8_t buf[5];
    nrf24l01_info_t info;
    nrf24l01_bool_t enable;
    nrf24l01_mode_t mode;
    nrf24l01_address_width_t width;
    nrf24l01_data_rate_t rate;
    nrf24l01_output_power_t power;

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

    /* start register test */
    nrf24l01_interface_debug_print("nrf24l01: start register test.\n");

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

    /* nrf24l01_set_config/nrf24l01_get_config test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_set_config/nrf24l01_get_config test.\n");

    /* disable power up */
    res = nrf24l01_set_config(&gs_handle, NRF24L01_CONFIG_PWR_UP, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set config failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: disable power up.\n");
    res = nrf24l01_get_config(&gs_handle, NRF24L01_CONFIG_PWR_UP, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get config failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check config %s.\n", enable == NRF24L01_BOOL_FALSE ? "ok" : "error");

    /* enable power up */
    res = nrf24l01_set_config(&gs_handle, NRF24L01_CONFIG_PWR_UP, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set config failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: enable power up.\n");
    res = nrf24l01_get_config(&gs_handle, NRF24L01_CONFIG_PWR_UP, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get config failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check config %s.\n", enable == NRF24L01_BOOL_TRUE ? "ok" : "error");

    /* disable encoding 2 bytes */
    res = nrf24l01_set_config(&gs_handle, NRF24L01_CONFIG_CRCO, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set config failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: disable encoding 2 bytes.\n");
    res = nrf24l01_get_config(&gs_handle, NRF24L01_CONFIG_CRCO, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get config failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check config %s.\n", enable == NRF24L01_BOOL_FALSE ? "ok" : "error");

    /* enable encoding 2 bytes */
    res = nrf24l01_set_config(&gs_handle, NRF24L01_CONFIG_CRCO, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set config failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: enable encoding 2 bytes.\n");
    res = nrf24l01_get_config(&gs_handle, NRF24L01_CONFIG_CRCO, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get config failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check config %s.\n", enable == NRF24L01_BOOL_TRUE ? "ok" : "error");

    /* disable crc */
    res = nrf24l01_set_config(&gs_handle, NRF24L01_CONFIG_EN_CRC, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set config failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: disable crc.\n");
    res = nrf24l01_get_config(&gs_handle, NRF24L01_CONFIG_EN_CRC, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get config failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check config %s.\n", enable == NRF24L01_BOOL_FALSE ? "ok" : "error");

    /* enable crc */
    res = nrf24l01_set_config(&gs_handle, NRF24L01_CONFIG_EN_CRC, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set config failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: enable crc.\n");
    res = nrf24l01_get_config(&gs_handle, NRF24L01_CONFIG_EN_CRC, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get config failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check config %s.\n", enable == NRF24L01_BOOL_TRUE ? "ok" : "error");

    /* disable max rt */
    res = nrf24l01_set_config(&gs_handle, NRF24L01_CONFIG_MASK_MAX_RT, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set config failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: disable max rt.\n");
    res = nrf24l01_get_config(&gs_handle, NRF24L01_CONFIG_MASK_MAX_RT, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get config failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check config %s.\n", enable == NRF24L01_BOOL_FALSE ? "ok" : "error");

    /* enable max rt */
    res = nrf24l01_set_config(&gs_handle, NRF24L01_CONFIG_MASK_MAX_RT, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set config failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: enable max rt.\n");
    res = nrf24l01_get_config(&gs_handle, NRF24L01_CONFIG_MASK_MAX_RT, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get config failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check config %s.\n", enable == NRF24L01_BOOL_TRUE ? "ok" : "error");

    /* disable tx ds */
    res = nrf24l01_set_config(&gs_handle, NRF24L01_CONFIG_MASK_TX_DS, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set config failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: disable tx ds.\n");
    res = nrf24l01_get_config(&gs_handle, NRF24L01_CONFIG_MASK_TX_DS, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get config failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check config %s.\n", enable == NRF24L01_BOOL_FALSE ? "ok" : "error");

    /* enable tx ds */
    res = nrf24l01_set_config(&gs_handle, NRF24L01_CONFIG_MASK_TX_DS, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set config failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: enable tx ds.\n");
    res = nrf24l01_get_config(&gs_handle, NRF24L01_CONFIG_MASK_TX_DS, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get config failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check config %s.\n", enable == NRF24L01_BOOL_TRUE ? "ok" : "error");

    /* disable rx dr */
    res = nrf24l01_set_config(&gs_handle, NRF24L01_CONFIG_MASK_RX_DR, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set config failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: disable rx dr.\n");
    res = nrf24l01_get_config(&gs_handle, NRF24L01_CONFIG_MASK_RX_DR, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get config failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check config %s.\n", enable == NRF24L01_BOOL_FALSE ? "ok" : "error");

    /* enable rx dr */
    res = nrf24l01_set_config(&gs_handle, NRF24L01_CONFIG_MASK_RX_DR, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set config failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: enable rx dr.\n");
    res = nrf24l01_get_config(&gs_handle, NRF24L01_CONFIG_MASK_RX_DR, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get config failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check config %s.\n", enable == NRF24L01_BOOL_TRUE ? "ok" : "error");

    /* nrf24l01_set_mode/nrf24l01_get_mode test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_set_mode/nrf24l01_get_mode test.\n");

    /* set tx mode */
    res = nrf24l01_set_mode(&gs_handle, NRF24L01_MODE_TX);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set mode failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: set tx mode.\n");
    res = nrf24l01_get_mode(&gs_handle, &mode);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get mode failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check mode %s.\n", mode == NRF24L01_MODE_TX ? "ok" : "error");

    /* set rx mode */
    res = nrf24l01_set_mode(&gs_handle, NRF24L01_MODE_RX);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set mode failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: set rx mode.\n");
    res = nrf24l01_get_mode(&gs_handle, &mode);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get mode failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check mode %s.\n", mode == NRF24L01_MODE_RX ? "ok" : "error");

    /* nrf24l01_set_auto_acknowledgment/nrf24l01_get_auto_acknowledgment test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_set_auto_acknowledgment/nrf24l01_get_auto_acknowledgment test.\n");

    /* pipe 0 disable */
    res = nrf24l01_set_auto_acknowledgment(&gs_handle, NRF24L01_PIPE_0, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set auto acknowledgment failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: disable pipe 0 auto acknowledgment.\n");
    res = nrf24l01_get_auto_acknowledgment(&gs_handle, NRF24L01_PIPE_0, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get auto acknowledgment failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check auto acknowledgment %s.\n", enable == NRF24L01_BOOL_FALSE ? "ok" : "error");

    /* pipe 0 enable */
    res = nrf24l01_set_auto_acknowledgment(&gs_handle, NRF24L01_PIPE_0, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set auto acknowledgment failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: enable pipe 0 auto acknowledgment.\n");
    res = nrf24l01_get_auto_acknowledgment(&gs_handle, NRF24L01_PIPE_0, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get auto acknowledgment failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check auto acknowledgment %s.\n", enable == NRF24L01_BOOL_TRUE ? "ok" : "error");

    /* pipe 1 disable */
    res = nrf24l01_set_auto_acknowledgment(&gs_handle, NRF24L01_PIPE_1, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set auto acknowledgment failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: disable pipe 1 auto acknowledgment.\n");
    res = nrf24l01_get_auto_acknowledgment(&gs_handle, NRF24L01_PIPE_1, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get auto acknowledgment failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check auto acknowledgment %s.\n", enable == NRF24L01_BOOL_FALSE ? "ok" : "error");

    /* pipe 1 enable */
    res = nrf24l01_set_auto_acknowledgment(&gs_handle, NRF24L01_PIPE_1, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set auto acknowledgment failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: enable pipe 1 auto acknowledgment.\n");
    res = nrf24l01_get_auto_acknowledgment(&gs_handle, NRF24L01_PIPE_1, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get auto acknowledgment failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check auto acknowledgment %s.\n", enable == NRF24L01_BOOL_TRUE ? "ok" : "error");

    /* pipe 2 disable */
    res = nrf24l01_set_auto_acknowledgment(&gs_handle, NRF24L01_PIPE_2, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set auto acknowledgment failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: disable pipe 2 auto acknowledgment.\n");
    res = nrf24l01_get_auto_acknowledgment(&gs_handle, NRF24L01_PIPE_2, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get auto acknowledgment failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check auto acknowledgment %s.\n", enable == NRF24L01_BOOL_FALSE ? "ok" : "error");

    /* pipe 2 enable */
    res = nrf24l01_set_auto_acknowledgment(&gs_handle, NRF24L01_PIPE_2, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set auto acknowledgment failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: enable pipe 2 auto acknowledgment.\n");
    res = nrf24l01_get_auto_acknowledgment(&gs_handle, NRF24L01_PIPE_2, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get auto acknowledgment failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check auto acknowledgment %s.\n", enable == NRF24L01_BOOL_TRUE ? "ok" : "error");

    /* pipe 3 disable */
    res = nrf24l01_set_auto_acknowledgment(&gs_handle, NRF24L01_PIPE_3, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set auto acknowledgment failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: disable pipe 3 auto acknowledgment.\n");
    res = nrf24l01_get_auto_acknowledgment(&gs_handle, NRF24L01_PIPE_3, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get auto acknowledgment failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check auto acknowledgment %s.\n", enable == NRF24L01_BOOL_FALSE ? "ok" : "error");

    /* pipe 3 enable */
    res = nrf24l01_set_auto_acknowledgment(&gs_handle, NRF24L01_PIPE_3, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set auto acknowledgment failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: enable pipe 3 auto acknowledgment.\n");
    res = nrf24l01_get_auto_acknowledgment(&gs_handle, NRF24L01_PIPE_3, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get auto acknowledgment failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check auto acknowledgment %s.\n", enable == NRF24L01_BOOL_TRUE ? "ok" : "error");

    /* pipe 4 disable */
    res = nrf24l01_set_auto_acknowledgment(&gs_handle, NRF24L01_PIPE_4, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set auto acknowledgment failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: disable pipe 4 auto acknowledgment.\n");
    res = nrf24l01_get_auto_acknowledgment(&gs_handle, NRF24L01_PIPE_4, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get auto acknowledgment failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check auto acknowledgment %s.\n", enable == NRF24L01_BOOL_FALSE ? "ok" : "error");

    /* pipe 4 enable */
    res = nrf24l01_set_auto_acknowledgment(&gs_handle, NRF24L01_PIPE_4, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set auto acknowledgment failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: enable pipe 4 auto acknowledgment.\n");
    res = nrf24l01_get_auto_acknowledgment(&gs_handle, NRF24L01_PIPE_4, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get auto acknowledgment failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check auto acknowledgment %s.\n", enable == NRF24L01_BOOL_TRUE ? "ok" : "error");

    /* pipe 5 disable */
    res = nrf24l01_set_auto_acknowledgment(&gs_handle, NRF24L01_PIPE_5, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set auto acknowledgment failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: disable pipe 5 auto acknowledgment.\n");
    res = nrf24l01_get_auto_acknowledgment(&gs_handle, NRF24L01_PIPE_5, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get auto acknowledgment failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check auto acknowledgment %s.\n", enable == NRF24L01_BOOL_FALSE ? "ok" : "error");

    /* pipe 5 enable */
    res = nrf24l01_set_auto_acknowledgment(&gs_handle, NRF24L01_PIPE_5, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set auto acknowledgment failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: enable pipe 5 auto acknowledgment.\n");
    res = nrf24l01_get_auto_acknowledgment(&gs_handle, NRF24L01_PIPE_5, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get auto acknowledgment failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check auto acknowledgment %s.\n", enable == NRF24L01_BOOL_TRUE ? "ok" : "error");

    /* nrf24l01_set_rx_pipe/nrf24l01_get_rx_pipe test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_set_rx_pipe/nrf24l01_get_rx_pipe test.\n");

    /* disable rx pipe 0 */
    res = nrf24l01_set_rx_pipe(&gs_handle, NRF24L01_PIPE_0, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: disable rx pipe 0.\n");
    res = nrf24l01_get_rx_pipe(&gs_handle, NRF24L01_PIPE_0, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get rx pipe failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check rx pipe %s.\n", enable == NRF24L01_BOOL_FALSE ? "ok" : "error");

    /* enable rx pipe 0 */
    res = nrf24l01_set_rx_pipe(&gs_handle, NRF24L01_PIPE_0, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: enable rx pipe 0.\n");
    res = nrf24l01_get_rx_pipe(&gs_handle, NRF24L01_PIPE_0, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get rx pipe failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check rx pipe %s.\n", enable == NRF24L01_BOOL_TRUE ? "ok" : "error");

    /* disable rx pipe 1 */
    res = nrf24l01_set_rx_pipe(&gs_handle, NRF24L01_PIPE_1, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: disable rx pipe 1.\n");
    res = nrf24l01_get_rx_pipe(&gs_handle, NRF24L01_PIPE_1, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get rx pipe failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check rx pipe %s.\n", enable == NRF24L01_BOOL_FALSE ? "ok" : "error");

    /* enable rx pipe 1 */
    res = nrf24l01_set_rx_pipe(&gs_handle, NRF24L01_PIPE_1, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: enable rx pipe 1.\n");
    res = nrf24l01_get_rx_pipe(&gs_handle, NRF24L01_PIPE_1, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get rx pipe failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check rx pipe %s.\n", enable == NRF24L01_BOOL_TRUE ? "ok" : "error");

    /* disable rx pipe 2 */
    res = nrf24l01_set_rx_pipe(&gs_handle, NRF24L01_PIPE_2, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: disable rx pipe 2.\n");
    res = nrf24l01_get_rx_pipe(&gs_handle, NRF24L01_PIPE_2, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get rx pipe failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check rx pipe %s.\n", enable == NRF24L01_BOOL_FALSE ? "ok" : "error");

    /* enable rx pipe 2 */
    res = nrf24l01_set_rx_pipe(&gs_handle, NRF24L01_PIPE_2, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: enable rx pipe 2.\n");
    res = nrf24l01_get_rx_pipe(&gs_handle, NRF24L01_PIPE_2, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get rx pipe failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check rx pipe %s.\n", enable == NRF24L01_BOOL_TRUE ? "ok" : "error");

    /* disable rx pipe 3 */
    res = nrf24l01_set_rx_pipe(&gs_handle, NRF24L01_PIPE_3, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: disable rx pipe 3.\n");
    res = nrf24l01_get_rx_pipe(&gs_handle, NRF24L01_PIPE_3, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get rx pipe failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check rx pipe %s.\n", enable == NRF24L01_BOOL_FALSE ? "ok" : "error");

    /* enable rx pipe 3 */
    res = nrf24l01_set_rx_pipe(&gs_handle, NRF24L01_PIPE_3, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: enable rx pipe 3.\n");
    res = nrf24l01_get_rx_pipe(&gs_handle, NRF24L01_PIPE_3, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get rx pipe failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check rx pipe %s.\n", enable == NRF24L01_BOOL_TRUE ? "ok" : "error");

    /* disable rx pipe 4 */
    res = nrf24l01_set_rx_pipe(&gs_handle, NRF24L01_PIPE_4, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: disable rx pipe 4.\n");
    res = nrf24l01_get_rx_pipe(&gs_handle, NRF24L01_PIPE_4, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get rx pipe failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check rx pipe %s.\n", enable == NRF24L01_BOOL_FALSE ? "ok" : "error");

    /* enable rx pipe 4 */
    res = nrf24l01_set_rx_pipe(&gs_handle, NRF24L01_PIPE_4, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: enable rx pipe 4.\n");
    res = nrf24l01_get_rx_pipe(&gs_handle, NRF24L01_PIPE_4, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get rx pipe failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check rx pipe %s.\n", enable == NRF24L01_BOOL_TRUE ? "ok" : "error");

    /* disable rx pipe 5 */
    res = nrf24l01_set_rx_pipe(&gs_handle, NRF24L01_PIPE_5, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: disable rx pipe 5.\n");
    res = nrf24l01_get_rx_pipe(&gs_handle, NRF24L01_PIPE_5, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get rx pipe failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check rx pipe %s.\n", enable == NRF24L01_BOOL_FALSE ? "ok" : "error");

    /* enable rx pipe 5 */
    res = nrf24l01_set_rx_pipe(&gs_handle, NRF24L01_PIPE_5, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: enable rx pipe 5.\n");
    res = nrf24l01_get_rx_pipe(&gs_handle, NRF24L01_PIPE_5, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get rx pipe failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check rx pipe %s.\n", enable == NRF24L01_BOOL_TRUE ? "ok" : "error");

    /* nrf24l01_set_address_width/nrf24l01_get_address_width test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_set_address_width/nrf24l01_get_address_width test.\n");

    /* set address width illegal */
    res = nrf24l01_set_address_width(&gs_handle, NRF24L01_ADDRESS_WIDTH_ILLEGAL);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set address width failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: set address width illegal.\n");
    res = nrf24l01_get_address_width(&gs_handle, &width);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get address width failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check address width %s.\n", width == NRF24L01_ADDRESS_WIDTH_ILLEGAL ? "ok" : "error");

    /* set address width 3 bytes */
    res = nrf24l01_set_address_width(&gs_handle, NRF24L01_ADDRESS_WIDTH_3_BYTES);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set address width failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: set address width 3 bytes.\n");
    res = nrf24l01_get_address_width(&gs_handle, &width);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get address width failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check address width %s.\n", width == NRF24L01_ADDRESS_WIDTH_3_BYTES ? "ok" : "error");

    /* set address width 4 bytes */
    res = nrf24l01_set_address_width(&gs_handle, NRF24L01_ADDRESS_WIDTH_4_BYTES);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set address width failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: set address width 4 bytes.\n");
    res = nrf24l01_get_address_width(&gs_handle, &width);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get address width failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check address width %s.\n", width == NRF24L01_ADDRESS_WIDTH_4_BYTES ? "ok" : "error");

    /* set address width 5 bytes */
    res = nrf24l01_set_address_width(&gs_handle, NRF24L01_ADDRESS_WIDTH_5_BYTES);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set address width failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: set address width 5 bytes.\n");
    res = nrf24l01_get_address_width(&gs_handle, &width);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get address width failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check address width %s.\n", width == NRF24L01_ADDRESS_WIDTH_5_BYTES ? "ok" : "error");

    /* nrf24l01_set_address_width/nrf24l01_get_address_width test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_set_address_width/nrf24l01_get_address_width test.\n");

    value = rand() % 0xF;
    res = nrf24l01_set_auto_retransmit_delay(&gs_handle, value);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set auto retransmit delay failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: set auto retransmit delay %d.\n", value);
    res = nrf24l01_get_auto_retransmit_delay(&gs_handle, (uint8_t *)&value_check);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get auto retransmit delay failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check auto retransmit delay %s.\n", value == value_check ? "ok" : "error");

    /* nrf24l01_auto_retransmit_delay_convert_to_register/nrf24l01_auto_retransmit_delay_convert_to_data test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_auto_retransmit_delay_convert_to_register/nrf24l01_auto_retransmit_delay_convert_to_data test.\n");

    us = 250 * (rand() % 0xF);
    res = nrf24l01_auto_retransmit_delay_convert_to_register(&gs_handle, us, (uint8_t *)&value);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: auto retransmit delay convert to register failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    res = nrf24l01_auto_retransmit_delay_convert_to_data(&gs_handle, value, (uint32_t *)&us_check);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: auto retransmit delay convert to data failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check auto retransmit delay convert %d %d %d.\n", us, value, us_check);

    /* nrf24l01_set_auto_retransmit_count/nrf24l01_get_auto_retransmit_count test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_set_auto_retransmit_count/nrf24l01_get_auto_retransmit_count test.\n");

    value = rand() % 0xF;
    res = nrf24l01_set_auto_retransmit_count(&gs_handle, value);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set auto retransmit count failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: set auto retransmit count %d.\n", value);
    res = nrf24l01_get_auto_retransmit_count(&gs_handle, (uint8_t *)&value_check);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get auto retransmit count failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check auto retransmit count %s.\n", value == value_check ? "ok" : "error");

    /* nrf24l01_set_channel_frequency/nrf24l01_get_channel_frequency test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_set_channel_frequency/nrf24l01_get_channel_frequency test.\n");

    value = rand() % 0xF;
    res = nrf24l01_set_channel_frequency(&gs_handle, value);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set channel frequency failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: set channel frequency %d.\n", value);
    res = nrf24l01_get_channel_frequency(&gs_handle, (uint8_t *)&value_check);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get channel frequency failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check channel frequency %s.\n", value == value_check ? "ok" : "error");

    /* nrf24l01_set_continuous_carrier_transmit/nrf24l01_get_continuous_carrier_transmit test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_set_continuous_carrier_transmit/nrf24l01_get_continuous_carrier_transmit test.\n");

    /* disable continuous carrier transmit */
    res = nrf24l01_set_continuous_carrier_transmit(&gs_handle, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set continuous carrier transmit failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: disable continuous carrier transmit.\n");
    res = nrf24l01_get_continuous_carrier_transmit(&gs_handle, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get continuous carrier transmit failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check continuous carrier transmit %s.\n", enable == NRF24L01_BOOL_FALSE ? "ok" : "error");

    /* enable continuous carrier transmit */
    res = nrf24l01_set_continuous_carrier_transmit(&gs_handle, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set continuous carrier transmit failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: enable continuous carrier transmit.\n");
    res = nrf24l01_get_continuous_carrier_transmit(&gs_handle, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get continuous carrier transmit failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check continuous carrier transmit %s.\n", enable == NRF24L01_BOOL_TRUE ? "ok" : "error");

    /* nrf24l01_set_force_pll_lock_signal/nrf24l01_get_force_pll_lock_signal test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_set_force_pll_lock_signal/nrf24l01_get_force_pll_lock_signal test.\n");

    /* enable force pll lock signal */
    res = nrf24l01_set_force_pll_lock_signal(&gs_handle, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set force pll lock signal failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: enable force pll lock signal.\n");
    res = nrf24l01_get_force_pll_lock_signal(&gs_handle, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get force pll lock signal failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check force pll lock signal %s.\n", enable == NRF24L01_BOOL_TRUE ? "ok" : "error");

    /* disable force pll lock signal */
    res = nrf24l01_set_force_pll_lock_signal(&gs_handle, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set force pll lock signal failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: disable force pll lock signal.\n");
    res = nrf24l01_get_force_pll_lock_signal(&gs_handle, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get force pll lock signal failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check force pll lock signal %s.\n", enable == NRF24L01_BOOL_FALSE ? "ok" : "error");

    /* nrf24l01_set_data_rate/nrf24l01_get_data_rate test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_set_data_rate/nrf24l01_get_data_rate test.\n");

    /* data rate 1M */
    res = nrf24l01_set_data_rate(&gs_handle, NRF24L01_DATA_RATE_1M);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set data rate failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: data rate 1M.\n");
    res = nrf24l01_get_data_rate(&gs_handle, &rate);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get data rate failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check data rate %s.\n", rate == NRF24L01_DATA_RATE_1M ? "ok" : "error");

    /* data rate 2M */
    res = nrf24l01_set_data_rate(&gs_handle, NRF24L01_DATA_RATE_2M);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set data rate failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: data rate 2M.\n");
    res = nrf24l01_get_data_rate(&gs_handle, &rate);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get data rate failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check data rate %s.\n", rate == NRF24L01_DATA_RATE_2M ? "ok" : "error");

    /* data rate 250K */
    res = nrf24l01_set_data_rate(&gs_handle, NRF24L01_DATA_RATE_250K);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set data rate failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: data rate 250K.\n");
    res = nrf24l01_get_data_rate(&gs_handle, &rate);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get data rate failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check data rate %s.\n", rate == NRF24L01_DATA_RATE_250K ? "ok" : "error");

    /* nrf24l01_set_output_power/nrf24l01_get_output_power test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_set_output_power/nrf24l01_get_output_power test.\n");

    /* output power -18 dBm */
    res = nrf24l01_set_output_power(&gs_handle, NRF24L01_OUTPUT_POWER_NEGATIVE_18_DBM);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set output power failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: set output power -18 dBm.\n");
    res = nrf24l01_get_output_power(&gs_handle, &power);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get output power failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check output power %s.\n", power == NRF24L01_OUTPUT_POWER_NEGATIVE_18_DBM ? "ok" : "error");

    /* output power -12 dBm */
    res = nrf24l01_set_output_power(&gs_handle, NRF24L01_OUTPUT_POWER_NEGATIVE_12_DBM);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set output power failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: set output power -12 dBm.\n");
    res = nrf24l01_get_output_power(&gs_handle, &power);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get output power failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check output power %s.\n", power == NRF24L01_OUTPUT_POWER_NEGATIVE_12_DBM ? "ok" : "error");

    /* output power -6 dBm */
    res = nrf24l01_set_output_power(&gs_handle, NRF24L01_OUTPUT_POWER_NEGATIVE_6_DBM);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set output power failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: set output power -6 dBm.\n");
    res = nrf24l01_get_output_power(&gs_handle, &power);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get output power failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check output power %s.\n", power == NRF24L01_OUTPUT_POWER_NEGATIVE_6_DBM ? "ok" : "error");

    /* output power 0 dBm */
    res = nrf24l01_set_output_power(&gs_handle, NRF24L01_OUTPUT_POWER_0_DBM);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set output power failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: set output power 0 dBm.\n");
    res = nrf24l01_get_output_power(&gs_handle, &power);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get output power failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check output power %s.\n", power == NRF24L01_OUTPUT_POWER_0_DBM ? "ok" : "error");

    /* nrf24l01_set_rx_pipe_0_address/nrf24l01_get_rx_pipe_0_address test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_set_rx_pipe_0_address/nrf24l01_get_rx_pipe_0_address test.\n");

    /* rx pipe 0 */
    addr[0] = rand() % 256;
    addr[1] = rand() % 256;
    addr[2] = rand() % 256;
    addr[3] = rand() % 256;
    addr[4] = rand() % 256;
    res = nrf24l01_set_rx_pipe_0_address(&gs_handle, (uint8_t *)addr, 5);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe 0 address failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: set rx pipe 0 addr 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X.\n",
                                   addr[0], addr[1], addr[2], addr[3], addr[4]);
    len = 5;
    res = nrf24l01_get_rx_pipe_0_address(&gs_handle, (uint8_t *)addr_check, (uint8_t *)&len);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get rx pipe 0 address failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check address %s with %d.\n", memcmp((uint8_t *)addr, (uint8_t *)addr_check, 5) == 0 ? "ok" : "error", len);

    /* nrf24l01_set_rx_pipe_1_address/nrf24l01_get_rx_pipe_1_address test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_set_rx_pipe_1_address/nrf24l01_get_rx_pipe_1_address test.\n");

    /* rx pipe 1 */
    addr[0] = rand() % 256;
    addr[1] = rand() % 256;
    addr[2] = rand() % 256;
    addr[3] = rand() % 256;
    addr[4] = rand() % 256;
    res = nrf24l01_set_rx_pipe_1_address(&gs_handle, (uint8_t *)addr, 5);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe 1 address failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: set rx pipe 1 addr 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X.\n",
                                   addr[0], addr[1], addr[2], addr[3], addr[4]);
    len = 5;
    res = nrf24l01_get_rx_pipe_1_address(&gs_handle, (uint8_t *)addr_check, (uint8_t *)&len);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get rx pipe 1 address failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check address %s with %d.\n", memcmp((uint8_t *)addr, (uint8_t *)addr_check, 5) == 0 ? "ok" : "error", len);

    /* nrf24l01_set_rx_pipe_2_address/nrf24l01_get_rx_pipe_2_address test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_set_rx_pipe_2_address/nrf24l01_get_rx_pipe_2_address test.\n");

    /* pipe 2 */
    value = rand() % 256;
    res = nrf24l01_set_rx_pipe_2_address(&gs_handle, value);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe 2 address failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: set rx pipe 2 addr 0x%02X.\n", value);
    res = nrf24l01_get_rx_pipe_2_address(&gs_handle, (uint8_t *)&value_check);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get rx pipe 2 address failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check address %s.\n", value_check == value ? "ok" : "error");

    /* nrf24l01_set_rx_pipe_3_address/nrf24l01_get_rx_pipe_3_address test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_set_rx_pipe_3_address/nrf24l01_get_rx_pipe_3_address test.\n");

    /* pipe 3 */
    value = rand() % 256;
    res = nrf24l01_set_rx_pipe_3_address(&gs_handle, value);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe 3 address failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: set rx pipe 3 addr 0x%02X.\n", value);
    res = nrf24l01_get_rx_pipe_3_address(&gs_handle, (uint8_t *)&value_check);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get rx pipe 3 address failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check address %s.\n", value_check == value ? "ok" : "error");

    /* nrf24l01_set_rx_pipe_4_address/nrf24l01_get_rx_pipe_4_address test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_set_rx_pipe_4_address/nrf24l01_get_rx_pipe_4_address test.\n");

    /* pipe 4 */
    value = rand() % 256;
    res = nrf24l01_set_rx_pipe_4_address(&gs_handle, value);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe 4 address failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: set rx pipe 4 addr 0x%02X.\n", value);
    res = nrf24l01_get_rx_pipe_4_address(&gs_handle, (uint8_t *)&value_check);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get rx pipe 4 address failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check address %s.\n", value_check == value ? "ok" : "error");

    /* nrf24l01_set_rx_pipe_5_address/nrf24l01_get_rx_pipe_5_address test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_set_rx_pipe_5_address/nrf24l01_get_rx_pipe_5_address test.\n");

    /* pipe 5 */
    value = rand() % 256;
    res = nrf24l01_set_rx_pipe_5_address(&gs_handle, value);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx pipe 5 address failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: set rx pipe 5 addr 0x%02X.\n", value);
    res = nrf24l01_get_rx_pipe_5_address(&gs_handle, (uint8_t *)&value_check);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get rx pipe 5 address failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check address %s.\n", value_check == value ? "ok" : "error");

    /* nrf24l01_set_tx_address/nrf24l01_get_tx_address test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_set_tx_address/nrf24l01_get_tx_address test.\n");

    /* tx */
    addr[0] = rand() % 256;
    addr[1] = rand() % 256;
    addr[2] = rand() % 256;
    addr[3] = rand() % 256;
    addr[4] = rand() % 256;
    res = nrf24l01_set_tx_address(&gs_handle, (uint8_t *)addr, 5);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set tx address failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: set tx addr 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X.\n",
                                   addr[0], addr[1], addr[2], addr[3], addr[4]);
    len = 5;
    res = nrf24l01_get_tx_address(&gs_handle, (uint8_t *)addr_check, (uint8_t *)&len);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get tx address failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check address %s with %d.\n", memcmp((uint8_t *)addr, (uint8_t *)addr_check, 5) == 0 ? "ok" : "error", len);

    /* nrf24l01_set_pipe_0_payload_number/nrf24l01_get_pipe_0_payload_number test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_set_pipe_0_payload_number/nrf24l01_get_pipe_0_payload_number test.\n");

    value = rand() % 32;
    res = nrf24l01_set_pipe_0_payload_number(&gs_handle, value);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe 0 payload number failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: set pipe 0 payload number %d.\n", value);
    res = nrf24l01_get_pipe_0_payload_number(&gs_handle, (uint8_t *)&value_check);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get pipe 0 payload number failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check payload number %s.\n", value_check == value ? "ok" : "error");

    /* nrf24l01_set_pipe_1_payload_number/nrf24l01_get_pipe_1_payload_number test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_set_pipe_1_payload_number/nrf24l01_get_pipe_1_payload_number test.\n");

    value = rand() % 32;
    res = nrf24l01_set_pipe_1_payload_number(&gs_handle, value);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe 1 payload number failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: set pipe 1 payload number %d.\n", value);
    res = nrf24l01_get_pipe_1_payload_number(&gs_handle, (uint8_t *)&value_check);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get pipe 1 payload number failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check payload number %s.\n", value_check == value ? "ok" : "error");

    /* nrf24l01_set_pipe_2_payload_number/nrf24l01_get_pipe_2_payload_number test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_set_pipe_2_payload_number/nrf24l01_get_pipe_2_payload_number test.\n");

    value = rand() % 32;
    res = nrf24l01_set_pipe_2_payload_number(&gs_handle, value);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe 2 payload number failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: set pipe 2 payload number %d.\n", value);
    res = nrf24l01_get_pipe_2_payload_number(&gs_handle, (uint8_t *)&value_check);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get pipe 2 payload number failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check payload number %s.\n", value_check == value ? "ok" : "error");

    /* nrf24l01_set_pipe_3_payload_number/nrf24l01_get_pipe_3_payload_number test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_set_pipe_3_payload_number/nrf24l01_get_pipe_3_payload_number test.\n");

    value = rand() % 32;
    res = nrf24l01_set_pipe_3_payload_number(&gs_handle, value);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe 3 payload number failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: set pipe 3 payload number %d.\n", value);
    res = nrf24l01_get_pipe_3_payload_number(&gs_handle, (uint8_t *)&value_check);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get pipe 3 payload number failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check payload number %s.\n", value_check == value ? "ok" : "error");

    /* nrf24l01_set_pipe_4_payload_number/nrf24l01_get_pipe_4_payload_number test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_set_pipe_4_payload_number/nrf24l01_get_pipe_4_payload_number test.\n");

    value = rand() % 32;
    res = nrf24l01_set_pipe_4_payload_number(&gs_handle, value);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe 4 payload number failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: set pipe 4 payload number %d.\n", value);
    res = nrf24l01_get_pipe_4_payload_number(&gs_handle, (uint8_t *)&value_check);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get pipe 4 payload number failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check payload number %s.\n", value_check == value ? "ok" : "error");

    /* nrf24l01_set_pipe_5_payload_number/nrf24l01_get_pipe_5_payload_number test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_set_pipe_5_payload_number/nrf24l01_get_pipe_5_payload_number test.\n");

    value = rand() % 32;
    res = nrf24l01_set_pipe_5_payload_number(&gs_handle, value);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe 5 payload number failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: set pipe 5 payload number %d.\n", value);
    res = nrf24l01_get_pipe_5_payload_number(&gs_handle, (uint8_t *)&value_check);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get pipe 5 payload number failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check payload number %s.\n", value_check == value ? "ok" : "error");

    /* nrf24l01_set_pipe_dynamic_payload/nrf24l01_get_pipe_dynamic_payload test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_set_pipe_dynamic_payload/nrf24l01_get_pipe_dynamic_payload test.\n");

    /* disable pipe 0 */
    res = nrf24l01_set_pipe_dynamic_payload(&gs_handle, NRF24L01_PIPE_0, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: disable pipe 0 dynamic payload.\n");
    res = nrf24l01_get_pipe_dynamic_payload(&gs_handle, NRF24L01_PIPE_0, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get pipe dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check pipe dynamic payload %s.\n", enable == NRF24L01_BOOL_FALSE ? "ok" : "error");

    /* enable pipe 0 */
    res = nrf24l01_set_pipe_dynamic_payload(&gs_handle, NRF24L01_PIPE_0, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: enable pipe 0 dynamic payload.\n");
    res = nrf24l01_get_pipe_dynamic_payload(&gs_handle, NRF24L01_PIPE_0, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get pipe dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check pipe dynamic payload %s.\n", enable == NRF24L01_BOOL_TRUE ? "ok" : "error");

    /* disable pipe 1 */
    res = nrf24l01_set_pipe_dynamic_payload(&gs_handle, NRF24L01_PIPE_1, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: disable pipe 1 dynamic payload.\n");
    res = nrf24l01_get_pipe_dynamic_payload(&gs_handle, NRF24L01_PIPE_1, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get pipe dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check pipe dynamic payload %s.\n", enable == NRF24L01_BOOL_FALSE ? "ok" : "error");

    /* enable pipe 1 */
    res = nrf24l01_set_pipe_dynamic_payload(&gs_handle, NRF24L01_PIPE_1, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: enable pipe 1 dynamic payload.\n");
    res = nrf24l01_get_pipe_dynamic_payload(&gs_handle, NRF24L01_PIPE_1, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get pipe dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check pipe dynamic payload %s.\n", enable == NRF24L01_BOOL_TRUE ? "ok" : "error");

    /* disable pipe 2 */
    res = nrf24l01_set_pipe_dynamic_payload(&gs_handle, NRF24L01_PIPE_2, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: disable pipe 2 dynamic payload.\n");
    res = nrf24l01_get_pipe_dynamic_payload(&gs_handle, NRF24L01_PIPE_2, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get pipe dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check pipe dynamic payload %s.\n", enable == NRF24L01_BOOL_FALSE ? "ok" : "error");

    /* enable pipe 2 */
    res = nrf24l01_set_pipe_dynamic_payload(&gs_handle, NRF24L01_PIPE_2, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: enable pipe 2 dynamic payload.\n");
    res = nrf24l01_get_pipe_dynamic_payload(&gs_handle, NRF24L01_PIPE_2, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get pipe dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check pipe dynamic payload %s.\n", enable == NRF24L01_BOOL_TRUE ? "ok" : "error");

    /* disable pipe 3 */
    res = nrf24l01_set_pipe_dynamic_payload(&gs_handle, NRF24L01_PIPE_3, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: disable pipe 3 dynamic payload.\n");
    res = nrf24l01_get_pipe_dynamic_payload(&gs_handle, NRF24L01_PIPE_3, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get pipe dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check pipe dynamic payload %s.\n", enable == NRF24L01_BOOL_FALSE ? "ok" : "error");

    /* enable pipe 3 */
    res = nrf24l01_set_pipe_dynamic_payload(&gs_handle, NRF24L01_PIPE_3, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: enable pipe 3 dynamic payload.\n");
    res = nrf24l01_get_pipe_dynamic_payload(&gs_handle, NRF24L01_PIPE_3, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get pipe dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check pipe dynamic payload %s.\n", enable == NRF24L01_BOOL_TRUE ? "ok" : "error");

    /* disable pipe 4 */
    res = nrf24l01_set_pipe_dynamic_payload(&gs_handle, NRF24L01_PIPE_4, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: disable pipe 4 dynamic payload.\n");
    res = nrf24l01_get_pipe_dynamic_payload(&gs_handle, NRF24L01_PIPE_4, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get pipe dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check pipe dynamic payload %s.\n", enable == NRF24L01_BOOL_FALSE ? "ok" : "error");

    /* enable pipe 4 */
    res = nrf24l01_set_pipe_dynamic_payload(&gs_handle, NRF24L01_PIPE_4, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: enable pipe 4 dynamic payload.\n");
    res = nrf24l01_get_pipe_dynamic_payload(&gs_handle, NRF24L01_PIPE_4, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get pipe dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check pipe dynamic payload %s.\n", enable == NRF24L01_BOOL_TRUE ? "ok" : "error");

    /* disable pipe 5 */
    res = nrf24l01_set_pipe_dynamic_payload(&gs_handle, NRF24L01_PIPE_5, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: disable pipe 5 dynamic payload.\n");
    res = nrf24l01_get_pipe_dynamic_payload(&gs_handle, NRF24L01_PIPE_5, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get pipe dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check pipe dynamic payload %s.\n", enable == NRF24L01_BOOL_FALSE ? "ok" : "error");

    /* enable pipe 5 */
    res = nrf24l01_set_pipe_dynamic_payload(&gs_handle, NRF24L01_PIPE_5, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set pipe dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: enable pipe 5 dynamic payload.\n");
    res = nrf24l01_get_pipe_dynamic_payload(&gs_handle, NRF24L01_PIPE_5, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get pipe dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check pipe dynamic payload %s.\n", enable == NRF24L01_BOOL_TRUE ? "ok" : "error");

    /* nrf24l01_set_dynamic_payload/nrf24l01_get_dynamic_payload test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_set_dynamic_payload/nrf24l01_get_dynamic_payload test.\n");

    /* disable dynamic payload */
    res = nrf24l01_set_dynamic_payload(&gs_handle, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: disable dynamic payload.\n");
    res = nrf24l01_get_dynamic_payload(&gs_handle, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check dynamic payload %s.\n", enable == NRF24L01_BOOL_FALSE ? "ok" : "error");

    /* enable dynamic payload */
    res = nrf24l01_set_dynamic_payload(&gs_handle, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: enable dynamic payload.\n");
    res = nrf24l01_get_dynamic_payload(&gs_handle, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get dynamic payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check dynamic payload %s.\n", enable == NRF24L01_BOOL_TRUE ? "ok" : "error");

    /* nrf24l01_set_payload_with_ack/nrf24l01_get_payload_with_ack test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_set_payload_with_ack/nrf24l01_get_payload_with_ack test.\n");

    /* disable payload with ack */
    res = nrf24l01_set_payload_with_ack(&gs_handle, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set payload with ack failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: disable payload with ack.\n");
    res = nrf24l01_get_payload_with_ack(&gs_handle, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get payload with ack failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check payload with ack %s.\n", enable == NRF24L01_BOOL_FALSE ? "ok" : "error");

    /* enable payload with ack */
    res = nrf24l01_set_payload_with_ack(&gs_handle, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set payload with ack failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: enable payload with ack.\n");
    res = nrf24l01_get_payload_with_ack(&gs_handle, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get payload with ack failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check payload with ack %s.\n", enable == NRF24L01_BOOL_TRUE ? "ok" : "error");

    /* nrf24l01_set_tx_payload_with_no_ack/nrf24l01_get_tx_payload_with_no_ack test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_set_tx_payload_with_no_ack/nrf24l01_get_tx_payload_with_no_ack test.\n");

    /* disable tx payload with no ack */
    res = nrf24l01_set_tx_payload_with_no_ack(&gs_handle, NRF24L01_BOOL_FALSE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set tx payload with no ack failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: disable tx payload with no ack.\n");
    res = nrf24l01_get_tx_payload_with_no_ack(&gs_handle, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get tx payload with no ack failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check tx payload with no ack %s.\n", enable == NRF24L01_BOOL_FALSE ? "ok" : "error");

    /* enable tx payload with no ack */
    res = nrf24l01_set_tx_payload_with_no_ack(&gs_handle, NRF24L01_BOOL_TRUE);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set tx payload with no ack failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: enable tx payload with no ack.\n");
    res = nrf24l01_get_tx_payload_with_no_ack(&gs_handle, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get tx payload with no ack failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check tx payload with no ack %s.\n", enable == NRF24L01_BOOL_TRUE ? "ok" : "error");

    /* nrf24l01_read_rx_payload test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_read_rx_payload test.\n");

    /* read rx payload */
    res = nrf24l01_read_rx_payload(&gs_handle, (uint8_t *)buf, 32);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set rx payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check rx payload %s.\n", res == 0 ? "ok" : "error");

    /* nrf24l01 write tx payload test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_write_tx_payload test.\n");

    /* write tx payload */
    res = nrf24l01_write_tx_payload(&gs_handle, (uint8_t *)buf, 32);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: set tx payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check tx payload %s.\n", res == 0 ? "ok" : "error");

    /* nrf24l01 get interrupt test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_get_interrupt test.\n");

    /* rx_dr */
    res = nrf24l01_get_interrupt(&gs_handle, NRF24L01_INTERRUPT_RX_DR, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get interrupt failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check rx_dr %s.\n", enable == 0 ? "enable" : "disable");

    /* tx_ds */
    res = nrf24l01_get_interrupt(&gs_handle, NRF24L01_INTERRUPT_TX_DS, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get interrupt failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check tx_ds %s.\n", enable == 0 ? "enable" : "disable");

    /* max_rt */
    res = nrf24l01_get_interrupt(&gs_handle, NRF24L01_INTERRUPT_MAX_RT, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get interrupt failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check max_rt %s.\n", enable == 0 ? "enable" : "disable");

    /* tx_full */
    res = nrf24l01_get_interrupt(&gs_handle, NRF24L01_INTERRUPT_TX_FULL, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get interrupt failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check tx_full %s.\n", enable == 0 ? "enable" : "disable");

    /* nrf24l01 clear interrupt test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_clear_interrupt test.\n");

    /* rx_dr */
    res = nrf24l01_clear_interrupt(&gs_handle, NRF24L01_INTERRUPT_RX_DR);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: clear interrupt failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check rx_dr %s.\n", res == 0 ? "ok" : "error");

    /* tx_ds */
    res = nrf24l01_clear_interrupt(&gs_handle, NRF24L01_INTERRUPT_TX_DS);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: clear interrupt failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check tx_ds %s.\n", res == 0 ? "ok" : "error");

    /* max_rt */
    res = nrf24l01_clear_interrupt(&gs_handle, NRF24L01_INTERRUPT_MAX_RT);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: clear interrupt failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check max_rt %s.\n", res == 0 ? "ok" : "error");

    /* tx_full */
    res = nrf24l01_clear_interrupt(&gs_handle, NRF24L01_INTERRUPT_TX_FULL);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: clear interrupt failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: check tx_full %s.\n", res == 0 ? "ok" : "error");

    /* nrf24l01 get data pipe number test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_get_data_pipe_number test.\n");

    /* get data pipe number */
    res = nrf24l01_get_data_pipe_number(&gs_handle, (uint8_t *)&value);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get data pipe number failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: get data pipe number %d.\n", value);

    /* nrf24l01 get lost packet count test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_get_lost_packet_count test.\n");

    /* get lost packet count */
    res = nrf24l01_get_lost_packet_count(&gs_handle, (uint8_t *)&value);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get lost packet count failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: get lost packet count %d.\n", value);

    /* nrf24l01 get retransmitted packet count test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_get_retransmitted_packet_count test.\n");

    /* get retransmitted packet count */
    res = nrf24l01_get_retransmitted_packet_count(&gs_handle, (uint8_t *)&value);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get retransmitted packet count failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: get retransmitted packet count %d.\n", value);

    /* nrf24l01 get received power detector test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_get_received_power_detector test.\n");

    /* get received power detector */
    res = nrf24l01_get_received_power_detector(&gs_handle, &enable);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get received power detector failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: get received power detector %s.\n", enable == NRF24L01_BOOL_TRUE ? "enable" : "disable");

    /* nrf24l01 get fifo status test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_get_fifo_status test.\n");

    /* get fifo status */
    res = nrf24l01_get_fifo_status(&gs_handle, (uint8_t *)&value);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get fifo status failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: get fifo status 0x%02X.\n", value);

    /* nrf24l01 get rx payload width test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_get_rx_payload_width test.\n");

    /* get rx payload width */
    res = nrf24l01_get_rx_payload_width(&gs_handle, (uint8_t *)&value);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: get rx payload width failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: get rx payload width 0x%02X.\n", value);

    /* nrf24l01 flush tx test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_flush_tx test.\n");

    /* flush tx */
    res = nrf24l01_flush_tx(&gs_handle);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: flush tx failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: flush tx %s.\n", res == 0 ? "ok" : "error");

    /* nrf24l01 flush rx test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_flush_rx test.\n");

    /* flush rx */
    res = nrf24l01_flush_rx(&gs_handle);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: flush rx failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: flush rx %s.\n", res == 0 ? "ok" : "error");

    /* nrf24l01 reuse tx payload test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_reuse_tx_payload test.\n");

    /* reuse tx payload */
    res = nrf24l01_reuse_tx_payload(&gs_handle);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: reuse tx payload failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: reuse tx payload %s.\n", res == 0 ? "ok" : "error");

    /* nrf24l01 write payload with ack test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_write_payload_with_ack test.\n");

    /* pipe 0 write payload with ack */
    res = nrf24l01_write_payload_with_ack(&gs_handle, NRF24L01_PIPE_0, (uint8_t *)buf, 32);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: write payload with ack failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: write payload with ack %s.\n", res == 0 ? "ok" : "error");

    /* pipe 1 write payload with ack */
    res = nrf24l01_write_payload_with_ack(&gs_handle, NRF24L01_PIPE_1, (uint8_t *)buf, 32);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: write payload with ack failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: write payload with ack %s.\n", res == 0 ? "ok" : "error");

    /* pipe 2 write payload with ack */
    res = nrf24l01_write_payload_with_ack(&gs_handle, NRF24L01_PIPE_2, (uint8_t *)buf, 32);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: write payload with ack failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: write payload with ack %s.\n", res == 0 ? "ok" : "error");

    /* pipe 3 write payload with ack */
    res = nrf24l01_write_payload_with_ack(&gs_handle, NRF24L01_PIPE_3, (uint8_t *)buf, 32);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: write payload with ack failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: write payload with ack %s.\n", res == 0 ? "ok" : "error");

    /* pipe 4 write payload with ack */
    res = nrf24l01_write_payload_with_ack(&gs_handle, NRF24L01_PIPE_4, (uint8_t *)buf, 32);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: write payload with ack failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: write payload with ack %s.\n", res == 0 ? "ok" : "error");

    /* pipe 5 write payload with ack */
    res = nrf24l01_write_payload_with_ack(&gs_handle, NRF24L01_PIPE_5, (uint8_t *)buf, 32);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: write payload with ack failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: write payload with ack %s.\n", res == 0 ? "ok" : "error");

    /* nrf24l01 write payload with no ack test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_write_payload_with_no_ack test.\n");

    /* write payload with no ack */
    res = nrf24l01_write_payload_with_no_ack(&gs_handle, (uint8_t *)buf, 32);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: write payload with no ack failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: write payload with no ack %s.\n", res == 0 ? "ok" : "error");

    /* nrf24l01 nop test */
    nrf24l01_interface_debug_print("nrf24l01: nrf24l01_nop test.\n");

    /* nop */
    res = nrf24l01_nop(&gs_handle);
    if (res != 0)
    {
        nrf24l01_interface_debug_print("nrf24l01: nop failed.\n");
        (void)nrf24l01_deinit(&gs_handle);

        return 1;
    }
    nrf24l01_interface_debug_print("nrf24l01: nop %s.\n", res == 0 ? "ok" : "error");

    /* finish register test */
    nrf24l01_interface_debug_print("nrf24l01: finish register test.\n");
    (void)nrf24l01_deinit(&gs_handle);

    return 0;
}
