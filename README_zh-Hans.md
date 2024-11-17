[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver NRF24L01

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/nrf24l01/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

nRF24L01+ 是一款带有嵌入式基带协议引擎 (Enhanced ShockBurst™) 的单芯片 2.4GHz 收发器，适用于超低功耗无线应用。 nRF24L01+ 设计用于在 2.400 - 2.4835GHz 的全球 ISM 频段中运行。要使用 nRF24L01+ 设计无线电系统，您只需要一个 MCU（微控制器）和一些外部无源元件。您可以通过串行外设接口 (SPI)操作和配置 nRF24L01+。寄存器映射表可通过SPI访问，它包含nRF24L01+中的所有配置寄存器，可在芯片的所有工作模式下访问。 嵌入式基带协议引擎（Enhanced ShockBurst™）基于数据包通信，支持手动多种模式操作到高级自治协议操作。内部 FIFO 可确保无线电前端和系统 MCU 之间的数据流顺畅。增强型 Shock-Burst™ 通过处理所有高速链路层操作来降低系统成本。无线电前端使用 GFSK 调制。它具有用户可配置的参数，如通道、输出功率和空中数据速率。 nRF24L01+ 支持 250 kbps、1 Mbps 和 2Mbps 的空中数据速率。nRF24L01+ 与 nRF24L01 直接兼容，空中兼容 nRF2401A、nRF2402、nRF24E1 和 nRF24E2。与 nRF24L01 相比，nRF24L01+ 中的互调和宽带阻塞值有了很大改善，并且 nRF24L01+ 的内部滤波增加了满足 RF 监管标准的余量。内部稳压器确保高电源抑制比 (PSRR) 和宽功率供应范围。NRF24L01用于无线PC外设、鼠标、键盘、遥控器、游戏控制器等。

LibDriver NRF24L01是LibDriver推出的NRF24L01全功能驱动，提供无线发送、无线接收等功能并且它符合MISRA标准。

### 目录

  - [说明](#说明)
  - [安装](#安装)
  - [使用](#使用)
    - [example basic send](#example-basic-send)
    - [example basic receive](#example-basic-receive)
  - [文档](#文档)
  - [贡献](#贡献)
  - [版权](#版权)
  - [联系我们](#联系我们)

### 说明

/src目录包含了LibDriver NRF24L01的源文件。

/interface目录包含了LibDriver NRF24L01与平台无关的SPI总线模板。

/test目录包含了LibDriver NRF24L01驱动测试程序，该程序可以简单的测试芯片必要功能。

/example目录包含了LibDriver NRF24L01编程范例。

/doc目录包含了LibDriver NRF24L01离线文档。

/datasheet目录包含了NRF24L01数据手册。

/project目录包含了常用Linux与单片机开发板的工程样例。所有工程均采用shell脚本作为调试方法，详细内容可参考每个工程里面的README.md。

/misra目录包含了LibDriver MISRA代码扫描结果。

### 安装

参考/interface目录下与平台无关的SPI总线模板，完成指定平台的SPI总线驱动。

将/src目录，您使用平台的接口驱动和您开发的驱动加入工程，如果您想要使用默认的范例驱动，可以将/example目录加入您的工程。

### 使用

您可以参考/example目录下的编程范例完成适合您的驱动，如果您想要使用默认的编程范例，以下是它们的使用方法。

#### example basic send

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
if (nrf24l01_basic_send((uint8_t *)addr, (uint8_t *)"123", 3) != 0);
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

### 文档

在线文档: [https://www.libdriver.com/docs/nrf24l01/index.html](https://www.libdriver.com/docs/nrf24l01/index.html)。

离线文档: /doc/html/index.html。

### 贡献

请参考CONTRIBUTING.md。

### 版权

版权 (c) 2015 - 现在 LibDriver 版权所有

MIT 许可证（MIT）

特此免费授予任何获得本软件副本和相关文档文件（下称“软件”）的人不受限制地处置该软件的权利，包括不受限制地使用、复制、修改、合并、发布、分发、转授许可和/或出售该软件副本，以及再授权被配发了本软件的人如上的权利，须在下列条件下：

上述版权声明和本许可声明应包含在该软件的所有副本或实质成分中。

本软件是“如此”提供的，没有任何形式的明示或暗示的保证，包括但不限于对适销性、特定用途的适用性和不侵权的保证。在任何情况下，作者或版权持有人都不对任何索赔、损害或其他责任负责，无论这些追责来自合同、侵权或其它行为中，还是产生于、源于或有关于本软件以及本软件的使用或其它处置。

### 联系我们

请联系lishifenging@outlook.com。