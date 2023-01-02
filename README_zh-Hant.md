[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver NRF24L01

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/nrf24l01/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

nRF24L01+ 是一款帶有嵌入式基帶協議引擎 (Enhanced ShockBurst™) 的單芯片 2.4GHz 收發器，適用於超低功耗無線應用。 nRF24L01+ 設計用於在 2.400 - 2.4835GHz 的全球 ISM 頻段中運行。要使用 nRF24L01+ 設計無線電系統，您只需要一個 MCU（微控制器）和一些外部無源元件。您可以操作和配置 nRF24L01+通過串行外設接口 (SPI)。寄存器映射，可通過SPI訪問，包含nRF24L01+中的所有配置寄存器，可在芯片的所有工作模式下訪問。嵌入式基帶協議引擎（Enhanced ShockBurst™）基於數據包通信，支持手動多種模式操作到高級自治協議操作。內部 FIFO 可確保無線電前端和系統 MCU 之間的數據流順暢。增強型 Shock-Burst™ 通過處理所有高速鏈路層操作來降低系統成本。無線電前端使用 GFSK 調製。它具有用戶可配置的參數，如頻道、輸出功率和空中數據速率。 nRF24L01+ 支持 250 kbps、1 Mbps 和 2Mbps 的空中數據速率。高空中數據速率結合兩種省電模式nRF24L01+ 非常適合超低功耗設計。 nRF24L01+ 與 nRF24L01 直接兼容，空中兼容 nRF2401A、nRF2402、nRF24E1 和 nRF24E2。與 nRF24L01 相比，nRF24L01+ 中的互調和寬帶阻塞值有了很大改善，並且 nRF24L01+ 的內部濾波增加了滿足 RF 監管標準的餘量。內部穩壓器確保高電源抑制比 (PSRR) 和寬功率供應範圍。 NRF24L01用於無線PC外設、鼠標、鍵盤、遙控器、遊戲控制器等。

LibDriver NRF24L01是LibDriver推出的NRF24L01全功能驅動，提供無線發送、無線接收等功能並且它符合MISRA標準。

### 目錄

  - [說明](#說明)
  - [安裝](#安裝)
  - [使用](#使用)
    - [example basic sent](#example-basic-sent)
    - [example basic receive](#example-basic-receive)
  - [文檔](#文檔)
  - [貢獻](#貢獻)
  - [版權](#版權)
  - [聯繫我們](#聯繫我們)

### 說明

/src目錄包含了LibDriver NRF24L01的源文件。

/interface目錄包含了LibDriver NRF24L01與平台無關的SPI總線模板。

/test目錄包含了LibDriver NRF24L01驅動測試程序，該程序可以簡單的測試芯片必要功能。

/example目錄包含了LibDriver NRF24L01編程範例。

/doc目錄包含了LibDriver NRF24L01離線文檔。

/datasheet目錄包含了NRF24L01數據手冊。

/project目錄包含了常用Linux與單片機開發板的工程樣例。所有工程均採用shell腳本作為調試方法，詳細內容可參考每個工程裡面的README.md。

### 安裝

參考/interface目錄下與平台無關的SPI總線模板，完成指定平台的SPI總線驅動。

將/src目錄，/interface目錄和/example目錄加入工程。

### 使用

#### example basic sent

```C
#include "driver_nrf24l01_basic.h"

uint8_t (*g_gpio_irq)(void) = NULL;
uint8_t res;
uint8_t addr[5] = NRF24L01_BASIC_DEFAULT_RX_ADDR_0;

...

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
            nrf24l01_interface_debug_print("nrf24l01: irq sent ok.\n");
            
            break;
        }
        case NRF24L01_INTERRUPT_MAX_RT :
        {
            nrf24l01_interface_debug_print("nrf24l01: irq reach max retry times.\n");
            
            break;
        }
        case NRF24L01_INTERRUPT_TX_FULL :
        {
            break;
        }
        default :
        {
            break;
        }
    }
}

...

res = gpio_interrupt_init();
if (res != 0)
{
    return 1;
}
g_gpio_irq = nrf24l01_interrupt_irq_handler;
res = nrf24l01_basic_init(NRF24L01_TYPE_TX, a_callback);
if (res != 0)
{
    (void)gpio_interrupt_deinit();
    g_gpio_irq = NULL;

    return 1;
}
if (nrf24l01_basic_sent((uint8_t *)addr, (uint8_t *)"123", 3) != 0);
{
    (void)nrf24l01_basic_deinit();
    (void)gpio_interrupt_deinit();
    g_gpio_irq = NULL;
    
    return 1;
}
if (nrf24l01_basic_deinit() != 0)
{
    (void)gpio_interrupt_deinit();
    g_gpio_irq = NULL;
    
    return 1;
}

(void)gpio_interrupt_deinit();
g_gpio_irq = NULL;

return 0;
```

#### example basic receive

```C
#include "driver_nrf24l01_basic.h"

uint8_t (*g_gpio_irq)(void) = NULL;
uint8_t res;
uint32_t timeout;

...

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
            nrf24l01_interface_debug_print("nrf24l01: irq sent ok.\n");
            
            break;
        }
        case NRF24L01_INTERRUPT_MAX_RT :
        {
            nrf24l01_interface_debug_print("nrf24l01: irq reach max retry times.\n");
            
            break;
        }
        case NRF24L01_INTERRUPT_TX_FULL :
        {
            break;
        }
        default :
        {
            break;
        }
    }

}

...

timeout = 5000;
res = gpio_interrupt_init();
if (res != 0)
{
    return 1;
}
g_gpio_irq = nrf24l01_interrupt_irq_handler;
res = nrf24l01_basic_init(NRF24L01_TYPE_RX, a_callback);
if (res != 0)
{
    (void)gpio_interrupt_deinit();
    g_gpio_irq = NULL;

    return 1;
}
nrf24l01_interface_delay_ms(timeout);
if (nrf24l01_basic_deinit() != 0)
{
    (void)gpio_interrupt_deinit();
    g_gpio_irq = NULL;
    
    return 1;
}

(void)gpio_interrupt_deinit();
g_gpio_irq = NULL;

return 0;
```

### 文檔

在線文檔: [https://www.libdriver.com/docs/nrf24l01/index.html](https://www.libdriver.com/docs/nrf24l01/index.html)。

離線文檔: /doc/html/index.html。

### 貢獻

請參攷CONTRIBUTING.md。

### 版權

版權 (c) 2015 - 現在 LibDriver 版權所有

MIT 許可證（MIT）

特此免費授予任何獲得本軟件副本和相關文檔文件（下稱“軟件”）的人不受限制地處置該軟件的權利，包括不受限制地使用、複製、修改、合併、發布、分發、轉授許可和/或出售該軟件副本，以及再授權被配發了本軟件的人如上的權利，須在下列條件下：

上述版權聲明和本許可聲明應包含在該軟件的所有副本或實質成分中。

本軟件是“如此”提供的，沒有任何形式的明示或暗示的保證，包括但不限於對適銷性、特定用途的適用性和不侵權的保證。在任何情況下，作者或版權持有人都不對任何索賠、損害或其他責任負責，無論這些追責來自合同、侵權或其它行為中，還是產生於、源於或有關於本軟件以及本軟件的使用或其它處置。

### 聯繫我們

請聯繫lishifenging@outlook.com。